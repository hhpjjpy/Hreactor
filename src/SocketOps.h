#pragma once
#ifndef SOCKETOPS_H
#define SOCKETOPS_H

#include <arpa/inet.h>

namespace hhp {
	namespace net {

int createNonblockingOrDir();

int Connect(int sockfd, const struct sockaddr* addr);

int Connect_nonb(int sockfd, const struct sockaddr* addr,int timeouts);

void BindOrDie(int sockfd ,const struct sockaddr* addr);

void ListenOrDie(int sockfd);

int Accept(int sockfd, struct sockaddr* addr);

ssize_t read(int sockfd, void *buf, size_t count);

ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);

ssize_t write(int sockfd, const void *buf, size_t count);

void shutdowmWrite(int sockfd);

void close(int sockfd);




	}
}




#endif // !SOCKETOPS_H
