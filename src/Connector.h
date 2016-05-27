#pragma once
#ifndef CONNECTOR_H
#define CONNECTOR_H
#include "Socket.h"
#include "InetAddr.h"
#include "EventLoop.h"
#include "Channel.h"
#include "TcpConnection.h"

class Connector {
public:
	Connector(hhp::net::InetAddr& addr);
	int getConnectFd();
	
private:
	std::unique_ptr<hhp::net::Socket> m_pSocket;
	int m_connfd;
	hhp::net::InetAddr m_addr;
	
};


#endif // !CONNECTOR_H

