#pragma once
#ifndef  TCPCLIENT_H
#define  TCPCLIENT_H
#include "InetAddr.h"
#include "Connector.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Log.h"

class TcpClient {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnPtr;
	typedef std::function<int(TcpConnPtr conn, std::string&& buf)> CallReadBack;
	typedef std::function<void(TcpConnPtr conn)> CallOtherBack;
	TcpClient(EventLoop *loop,hhp::net::InetAddr& addr);
	int Write(std::string& buf) {
		if (!m_isConn) {
			LOG(INFO) << "Write is error ,connection is close " << std::endl;
			return -1;
		}
	   m_pTcpConn->Write(buf); 
	   return buf.size();
	}
	void start() {
		m_pTcpConn->ConnectEstablished();
	}
	bool ClientIsOK() { return m_isConn; }
	void setReadCallBack(CallReadBack&& cb) { m_funReadCallBack = std::move(cb); }
	void setConnCallBack(CallOtherBack&& cb) { m_funConnCallBack = std::move(cb); }
	void setCloseCallBack(CallOtherBack&& cb) { m_closeCallBack = std::move(cb); }
private:
	int handRead(TcpConnPtr conn, std::string&& buf);
	void handConnect(TcpConnPtr conn);
	void handClose(TcpConnPtr conn);
	EventLoop *m_loop;
	Connector m_Connector;
	TcpConnPtr m_pTcpConn;
	bool m_isConn;
	CallReadBack  m_funReadCallBack;
	CallOtherBack m_funConnCallBack;
	CallOtherBack m_closeCallBack;
};


#endif
