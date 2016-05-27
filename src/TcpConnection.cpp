#include "TcpConnection.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "Log.h"
#include <sstream>

using namespace hhp::net;

TcpConnection::TcpConnection(EventLoop *loop, int fd) :
	m_loop(loop),
	m_ptrChannel(new Channel(loop, fd)),
	m_ptrSocket(new Socket(fd)),
	m_isConnect(false)
{
	m_ptrChannel->setReadCallBack(std::bind(&TcpConnection::headRead,this));
	m_ptrChannel->setWriteCallBack(std::bind(&TcpConnection::headWrite, this));
}

TcpConnection::~TcpConnection()
{
	m_isConnect = false;
	m_ptrChannel->remove();
}


void TcpConnection::ConnectEstablished()
{
	m_isConnect = true;
	m_ptrChannel->enableReading();
	//m_ptrChannel->enableWriting();
	m_connectionCallBack(shared_from_this());
}

void TcpConnection::Write(std::string &buf)
{
	if (m_loop->isInLoopThread()) {
		this->sendInLoop(buf);
	}
	else {
		m_loop->runInLoop(std::bind(&TcpConnection::sendInLoop,this,buf));
	}
}

std::string TcpConnection::GetTcpConnection()
{
	std::stringstream ss;
	ss << " connection  fd is : " << m_ptrChannel->fd();
	return ss.str();
}

void TcpConnection::ConnectDestory()
{
	m_isConnect = false;
	m_ptrChannel->remove();
}

void  TcpConnection::headRead()
{
	if (!m_isConnect) {
		LOG(INFO) << " error this TcpConnection is not connected " << std::endl;
		return ;
	}

	int readerror;
	size_t n = m_outputbuff.readFd(m_ptrChannel->fd(),&readerror);
	if (n > 0) {
		m_readCallBack(shared_from_this(),m_outputbuff.retrieveAllAsString());
	}
	else {
		this->headColse();
	}
	return ;

}

void TcpConnection::headWrite()
{
	if (!m_isConnect) {
		LOG(INFO) << " error this TcpConnection is not connected " << std::endl;
		return;
	}
	if (m_inputbuff.readableBytes() == 0) {
		m_ptrChannel->disableWriting();
	}
	else {
		size_t n = write(m_ptrChannel->fd(), m_inputbuff.peek(),m_inputbuff.readableBytes());
		if (n < 0) {
			LOG(INFO) << "write is error " << std::endl;
		}else {
			m_inputbuff.retrieve(n);
		}
	}
}

void TcpConnection::headColse()
{
	m_isConnect = false;
	m_closeCallBack(shared_from_this());
	LOG(INFO) << " TcpConnection handClose " << std::endl;
}

void TcpConnection::sendInLoop(std::string &buf)
{
	m_inputbuff.append(buf);
	m_ptrChannel->enableWriting();
}
