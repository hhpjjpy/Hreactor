#pragma once
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>
#include <memory>

#include "Socket.h"
#include "InetAddr.h"

class EventLoop;
class Channel;


class Acceptor {
public:
	typedef std::function<void(int sockfd, hhp::net::InetAddr addr)> newConnectCallBack;
	Acceptor(EventLoop *loop, uint16_t port);
	~Acceptor();
	void Listen();

	void setNewConnectCallBack(newConnectCallBack&& cb) { m_connectBack = std::move(cb); }

private:
	void headleread();
	EventLoop *m_loop;
	std::shared_ptr<hhp::net::InetAddr> m_ptrAddr;
	std::shared_ptr<hhp::net::Socket> m_ptrSock;
	std::shared_ptr<Channel> m_ptrChannel;
	newConnectCallBack  m_connectBack;

};


#endif // !ACCEPT_H

