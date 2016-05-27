#pragma once
#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <functional>
#include <string>
#include <memory>
#include <map>
#include "InetAddr.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
class TcpServer {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnPtr;
	typedef std::function<int(TcpConnPtr conn, std::string&& buf)> CallReadBack;
	typedef std::function<void(TcpConnPtr conn)> CallOtherBack;
	TcpServer(EventLoop* loop,uint16_t port,int threadNums);
	
	void setReadCallBack(CallReadBack&& cb) { m_readCallBack = std::move(cb); }
	void setConnectCallBack(CallOtherBack&& cb) { m_connectCallBack = std::move(cb); }
	void setClosetCallBack(CallOtherBack&& cb) { m_closeCallBack = std::move(cb); }
	void start();

private:
	int handRead(TcpConnPtr conn, std::string&& buf);
	void newConnection(int fd, hhp::net::InetAddr addr);
	void handConnected(TcpConnPtr conn);
	void handClose(TcpConnPtr conn);
	void removeConnection(TcpConnPtr conn);

	EventLoop *m_loop;
	Acceptor m_Acceptor;
	EventLoopThreadPool m_EventPool;
	std::map<int, TcpConnPtr> map_TcpConn;
	CallReadBack m_readCallBack;
	CallOtherBack m_connectCallBack;
	CallOtherBack m_closeCallBack;
	bool m_isConnect;
};



#endif // !TCPSERVER_H
