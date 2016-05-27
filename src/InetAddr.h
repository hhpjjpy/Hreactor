#pragma once
#ifndef INETADDR_H
#define INETADDR_H

#include <string>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream> 
#include "Log.h"
namespace hhp {
	namespace net {
class InetAddr {
public:
	InetAddr() {
		::bzero(&sockaddr_, sizeof(sockaddr_));
	}

explicit  
	InetAddr(uint16_t port) {
			::bzero(&sockaddr_,sizeof(sockaddr_));
			sockaddr_.sin_family = AF_INET;
			sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
			sockaddr_.sin_port = htons(port);
		}

explicit 
	InetAddr(std::string ip, uint16_t port) {
	::bzero(&sockaddr_, sizeof(sockaddr_));
	sockaddr_.sin_family = AF_INET;
	sockaddr_.sin_port = htons(port);
	int ret = ::inet_pton(AF_INET, ip.c_str(), &sockaddr_.sin_addr);
	if (ret < 0) {
		LOG(INFO) << "inet_pton(AF_INET, " << ip.c_str() << " , " << &sockaddr_.sin_addr << std::endl;
	}
}

const struct sockaddr_in* getSockaddr() {
		return &sockaddr_;
	}

void setSockaddr(struct sockaddr_in saddr) {
		sockaddr_ = saddr;
	}

private:
	struct sockaddr_in sockaddr_;
		
};


	}
}
#endif // !INETADDR_H
