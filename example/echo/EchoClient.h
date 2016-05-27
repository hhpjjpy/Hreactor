#pragma once
#ifndef  ECHOCLIENT_H
#define  ECHOCLIENT_H
#include "TcpClient.h"

class EchoClient {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnPtr;
	typedef std::function<int(TcpConnPtr conn, std::string&& buf)> CallReadBack;
	typedef std::function<void(TcpConnPtr conn)> CallOtherBack;

	EchoClient(EventLoop *loop, hhp::net::InetAddr addr) :
		m_cClient(loop,addr),
		m_isConn(false)
	{
		m_cClient.setConnCallBack(std::bind(&EchoClient::Connection, this, std::placeholders::_1));
		m_cClient.setReadCallBack(std::bind(&EchoClient::getMsg, this, std::placeholders::_1, std::placeholders::_2));
		m_cClient.setCloseCallBack(std::bind(&EchoClient::Close, this, std::placeholders::_1));
	}
	void start() {
		m_cClient.start();
	}
	void Connection(TcpConnPtr conn) {
		std::cout << "is connection is ok " << conn->GetTcpConnection() << std::endl;
		m_isConn = true;
	}
	int getMsg(TcpConnPtr conn,std::string&& buf) {
		std::cout << buf << std::endl;
		std::string msg;
		std::cin >> msg;
		sendMsg(msg);
	}
	void sendMsg(std::string& buf) {
		m_cClient.Write(buf);
	}

	void Close(TcpConnPtr conn) {
		conn.reset();
		m_isConn = false;
	}

private:
	TcpClient m_cClient;
	bool m_isConn;
};


#endif // ! ECHOCLIENT_H
