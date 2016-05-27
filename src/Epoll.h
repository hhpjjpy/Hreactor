#pragma once
#ifndef EPOLL_H
#define EPOLL_H
#include <vector>
#include <map>
#include <sys/epoll.h>

class Channel;
class EventLoop;

class Epoll {
public:
	typedef std::vector<Channel*> ChannelList;
	Epoll(EventLoop *loop);
	~Epoll();
	void poll(int timeoutms,ChannelList* activeChannels);
	void updateChannel(Channel *);
	void removeChannel(Channel *);
private:
	static const int InitEventNums = 16;

	typedef std::map<int, Channel*> ChannelMap;
	typedef std::vector<struct epoll_event> EpollList;
	ChannelMap m_channels;
	EpollList m_epolllist;
	int m_epollfd;
	EventLoop *m_loop;

};


#endif // !EPOLL_H
