#pragma once
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include <string>
#include <functional>
#include <memory>

#include "Socket.h"
#include "Buffer.h"

class EventLoop;
class Channel;



class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
	typedef std::shared_ptr<TcpConnection> TcpConnPtr;
	typedef std::function<int(TcpConnPtr conn ,std::string&& buf)> CallReadBack;
	typedef std::function<void(TcpConnPtr conn)>  CallOtherBack;
	TcpConnection(EventLoop* loop,int fd);
	~TcpConnection();

	void ConnectEstablished();
	void Write(std::string&);
	std::string GetTcpConnection();
	void SetReadCallBack(CallReadBack&& cb) { m_readCallBack = std::move(cb); }
	void SetConnectionCallBack(CallOtherBack&& cb) { m_connectionCallBack = std::move(cb); }
	void SetCloseCallBack(CallOtherBack&& cb) { m_closeCallBack = std::move(cb); }
	EventLoop* getLoop() { return m_loop; }
	void ConnectDestory();
private:
	void headRead();
	void headWrite();
	void headColse();
	void sendInLoop(std::string& );
	EventLoop* m_loop;
	std::unique_ptr<Channel> m_ptrChannel;
	std::unique_ptr<hhp::net::Socket> m_ptrSocket;
	Buffer m_inputbuff;
	Buffer m_outputbuff;
	bool m_isConnect;

	CallReadBack m_readCallBack;
	CallOtherBack m_connectionCallBack;
	CallOtherBack m_closeCallBack;

};

#endif // !TCPCONNECTION_H
