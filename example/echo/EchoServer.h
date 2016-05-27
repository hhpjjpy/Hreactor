#pragma once
#ifndef ECHOSERVER_H
#define ECHOSERVER_H
#include "TcpServer.h"

class EchoServer {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnPtr;
	typedef std::function<int(TcpConnPtr conn, std::string&& buf)> CallReadBack;
	typedef std::function<void(TcpConnPtr conn)> CallOtherBack;

	EchoServer(EventLoop *loop, uint16_t port):
	m_cServer(loop,port,10)
	{
		m_cServer.setConnectCallBack(std::bind(&EchoServer::getConnection, this,std::placeholders::_1));
		m_cServer.setReadCallBack(std::bind(&EchoServer::getConnMsg,this,std::placeholders::_1,std::placeholders::_2));
		m_cServer.setClosetCallBack(std::bind(&EchoServer::getClose,this,std::placeholders::_1));
	}
	void start() {
		m_cServer.start();
	}
	void getConnection(TcpConnPtr conn) {
		std::cout << conn->GetTcpConnection() << std::endl;
	}
	int getConnMsg(TcpConnPtr conn, std::string &&buf) {
		conn->Write(buf);
	}
	void getClose(TcpConnPtr conn) {
		std::cout << "fd : " << conn->GetTcpConnection() << "is close " << std::endl;
		conn.reset();
	}
private:
	TcpServer m_cServer;
};


#endif // !ECHOSERVER_H

