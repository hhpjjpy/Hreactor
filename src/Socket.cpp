#include "Socket.h"
#include "SocketOps.h"
#include "InetAddr.h"
#include <iostream>
#include "Log.h"

#define	SA struct sockaddr
using namespace hhp::net;


void Socket::Bind(InetAddr & Addr)
{
	::BindOrDie(sockfd_,(SA*)Addr.getSockaddr());
}

void Socket::Listen()
{
	::ListenOrDie(sockfd_);
}

int Socket::Accept(InetAddr & Addr)
{
	struct sockaddr_in sa;

	int connfd = ::Accept(sockfd_,(SA*)&sa);
	if (connfd > 0) {
		Addr.setSockaddr(sa);
	}
	return connfd;
}

int hhp::net::Socket::Connect(InetAddr & Addr)
{
	return ::Connect(sockfd_, (const SA*)Addr.getSockaddr());
}

int hhp::net::Socket::Connect_nonb(InetAddr & Addr,int timeouts)
{

	int ret = ::Connect_nonb(sockfd_, (const SA*)Addr.getSockaddr(), timeouts);
	if (ret < 0) {
		LOG(INFO) << "Connect_nonb(" << sockfd_ << "," << (SA*)Addr.getSockaddr() << "," << timeouts << "): " << ret << std::endl;
	}
	
	return ret;
}


void Socket::shutdownWrite()
{
	::shutdowmWrite(sockfd_);
}

