#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>
#include <string>
#include <iostream>
#include "Log.h"

namespace hhp {
	namespace net {
		
class InetAddr;

class Socket
{
public:
	Socket(int sockfd):sockfd_(sockfd){
	}
	~Socket() {
		::close(sockfd_);
	}
	void Bind(InetAddr& Addr);
	void Listen();
	int Accept(InetAddr & Addr);

	int Connect(InetAddr &Addr);
	int Connect_nonb(InetAddr &Addr,int timeous);
	void shutdownWrite();
	int getfd() { return sockfd_; }

private:
	const int sockfd_;
};


	}
}

#endif // !SOCKET_H
