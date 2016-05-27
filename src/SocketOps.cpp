#include "SocketOps.h"
#include "Log.h"

#include <iostream> 
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>  // snprintf
#include <strings.h>  // bzero
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>

#define LISTENQ 34 

namespace hhp{
	namespace net {

		void setNonBlockAndCloseOnExec(int sockfd)
		{
			// non-block
			int flags = ::fcntl(sockfd, F_GETFL, 0);
			flags |= O_NONBLOCK;
			int ret = ::fcntl(sockfd, F_SETFL, flags);
			if (ret < 0) {
				LOG(INFO) << "fcntl(" << sockfd << "," << "F_SETFL" << "," << flags << "): " << ret << std::endl;
			}

			// close-on-exec
			flags = ::fcntl(sockfd, F_GETFD, 0);
			flags |= FD_CLOEXEC;
			ret = ::fcntl(sockfd, F_SETFD, flags);
			// FIXME check

			(void)ret;
		}

		int createNonblockingOrDir()
		{
			int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sockfd < 0) {
				LOG(INFO) << "socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP): " << sockfd << std::endl;
			}
			
			setNonBlockAndCloseOnExec(sockfd);
			return sockfd;
		}

		int  Connect(int sockfd, const sockaddr * addr)
		{
			int ret = ::connect(sockfd, addr, sizeof(addr));
			if (ret <= 0) {
				LOG(INFO) << " Connect is error " << std::endl;

			}
			return ret;
		}

		int Connect_nonb(int sockfd, const struct sockaddr * addr,int timeouts) {
			struct timeval tval;
			int error;
			
			/////////
			/*
			struct sockaddr_in* test = (struct sockaddr_in*) addr;
			char IPdotdec[20];
			inet_ntop(AF_INET, (void *)&(test->sin_addr), IPdotdec, 16);
			LOG(INFO) << "inet_ntop ip: " << IPdotdec << std::endl;
			//////////

			int sockfd2 = socket(AF_INET, SOCK_STREAM, 0);

			
			//int flags = fcntl(sockfd2, F_GETFL, 0);
			//fcntl(sockfd2, F_SETFL, flags | O_NONBLOCK);
			struct sockaddr_in servaddr;
			bzero(&servaddr, sizeof(servaddr));
			servaddr.sin_family = AF_INET;
			servaddr.sin_port = htons(12345);
			servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
			//::inet_pton(AF_INET,"127.0.0.1", &servaddr.sin_addr);
			*/
			/*
			  错 误！！！！！

			const struct sockaddr * addr  结构体已指针的方式作为参数传入函数，退化为指针，在此使用sizeof()时只取得了
			指针本身的大小。类似的现象还要注意数组，对象等等，退化为指针的现象。当使用sizeof的时候尤其要注意
			int ret = ::connect(sockfd, (const struct sockaddr*)&addr,//sizeof(addr)///);
			
			*/
			int ret = ::connect(sockfd, addr, sizeof(struct sockaddr));
			if (ret < 0) {
				if (errno != EINPROGRESS) {
					LOG(INFO) << " errno : "<< errno << std::endl;
					return -1;
				}	
			}
			if (ret == 0) return 0;
			
			fd_set rset, wset;

			FD_ZERO(&rset);
			FD_SET(sockfd,&rset);
			wset = rset;
			tval.tv_sec = timeouts;
			int n = 0;
			if ((n = ::select(sockfd + 1, &rset, &wset, NULL, timeouts ? &tval : NULL))==0) {
				LOG(INFO) << "select( " << sockfd + 1 << " ,&rset, &wset, NULL ," << (timeouts ? "tval" : "NULL") << ": " << n << std::endl;
				close(sockfd);
				errno = ETIMEDOUT;
				return -1;
			}
			if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
				socklen_t len = sizeof(error);
				if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len)<0 ) {
					LOG(INFO) << "getsockout < 0 " << std::endl;
					return -1;
				}
				else {
					return 0;
				}
			}
			else {
				LOG(INFO) << "select  error : sockfd not set" << std::endl;
				return -1;
			}
		}


		void  BindOrDie(int sockfd, const sockaddr * addr)
		{
			int ret = ::bind(sockfd,addr,sizeof(struct sockaddr));
			if (ret < 0) {
				LOG(INFO) << " bind is error " << std::endl;
			}
		}

		void ListenOrDie(int sockfd) {
			int ret = ::listen(sockfd, LISTENQ);
			if (ret < 0) {
				LOG(INFO) << " listen is error " << std::endl;
			}
		}

		int  Accept(int sockfd, sockaddr * addr)
		{
			socklen_t len = sizeof(addr);
			int connfd = ::accept(sockfd, addr, &len);
			if (connfd <= 0) {
				LOG(INFO) << "accept(" << sockfd << "): " << connfd << std::endl;
			}
			
			setNonBlockAndCloseOnExec(connfd);
			return connfd;
		}

		ssize_t  read(int sockfd, void * buf, size_t count)
		{
			return ::read(sockfd, buf, count);
		}

		ssize_t  readv(int sockfd, const iovec * iov, int iovcnt)
		{
			int ret =  ::readv(sockfd, iov, iovcnt);
			LOG(INFO) << " readv : " << ret << std::endl;
			if (ret < 0) {
				LOG(INFO) << errno << std::endl;
			}

			return ret;
		}

		ssize_t  write(int sockfd, const void * buf, size_t count)
		{
			return ::write(sockfd,buf,count);
		}

		void shutdowmWrite(int sockfd)
		{
			if (::shutdown(sockfd, SHUT_WR) < 0)
			{
				LOG(INFO) << " shutdownWrite is error " << std::endl;
			}
		}

		void  close(int sockfd)
		{
			::close(sockfd);
		}

	}
}

