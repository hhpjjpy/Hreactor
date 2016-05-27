#include "Acceptor.h"
#include "Socket.h"
#include "InetAddr.h"
#include "Eventloop.h"
#include "Channel.h"
#include "SocketOps.h"
#include "Log.h"

#include <functional>
#include  <iostream>


using namespace hhp;
using namespace hhp::net;


Acceptor::Acceptor(EventLoop * loop, uint16_t port)
	:m_loop(loop),
	m_ptrAddr(new net::InetAddr(port)),
	m_ptrSock(new net::Socket(net::createNonblockingOrDir())),
	m_ptrChannel(new Channel(loop, m_ptrSock->getfd()))
{
	m_ptrSock->Bind(*m_ptrAddr);
	m_ptrChannel->setReadCallBack(std::bind(&Acceptor::headleread, this));
}

Acceptor::~Acceptor()
{
	m_ptrChannel->remove();
}


void Acceptor::Listen()
{
	m_ptrSock->Listen();
	m_ptrChannel->enableReading();
}

void Acceptor::headleread()
{
	net::InetAddr connAddr;
	int connfd = m_ptrSock->Accept(connAddr);
	if (connfd < 0) {
		LOG(INFO) << "Acceptor::headleread() accept is error  " << std::endl;
	}
	m_connectBack(connfd,connAddr);
}


