#include "TcpServer.h"
#include <algorithm>

#include "Log.h"


TcpServer::TcpServer(EventLoop * loop, uint16_t port, int threadNums) :
	m_loop(loop),
	m_Acceptor(m_loop, port),
	m_isConnect(false),
	m_EventPool(loop,threadNums)
{
	m_Acceptor.setNewConnectCallBack(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}


void TcpServer::start()
{
	m_Acceptor.Listen();
}


int TcpServer::handRead(TcpConnPtr  conn, std::string&&  buf)
{
	m_readCallBack(conn,std::move(buf));

}

void TcpServer::newConnection(int fd, hhp::net::InetAddr addr)
{  
	EventLoop *ioLoop = m_EventPool.getNextLoop();
	TcpConnPtr oTcpConnPtr(new TcpConnection(ioLoop, fd));
	oTcpConnPtr->SetConnectionCallBack(std::bind(&TcpServer::handConnected, this, std::placeholders::_1));
	oTcpConnPtr->SetReadCallBack(std::bind(&TcpServer::handRead,this, std::placeholders::_1, std::placeholders::_2));
	oTcpConnPtr->SetCloseCallBack(std::bind(&TcpServer::handClose, this, std::placeholders::_1));
	ioLoop->runInLoop(std::bind(&TcpConnection::ConnectEstablished, oTcpConnPtr));
	map_TcpConn[fd] = oTcpConnPtr;
}

void TcpServer::handConnected(TcpConnPtr conn)
{
	m_connectCallBack(conn);
}

void TcpServer::handClose(TcpConnPtr conn)
{
	m_closeCallBack(conn);
	this->removeConnection(conn);
}

void TcpServer::removeConnection(TcpConnPtr conn)
{
	for (auto iter = map_TcpConn.begin(); iter != map_TcpConn.end(); iter++) {
		if (iter->second == conn) {
			EventLoop *ioLoop = conn->getLoop();
			ioLoop->runInLoop(std::bind(&TcpConnection::ConnectDestory,conn));
			map_TcpConn.erase(iter->first);
			return ;
		}
	}

}
