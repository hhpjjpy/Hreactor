#include "Epoll.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "Channel.h"
#include "EventLoop.h"
#include "Log.h"

Epoll::Epoll(EventLoop * loop)
	:m_epolllist(InitEventNums),
	m_epollfd(::epoll_create(EPOLL_CLOEXEC)),
	m_loop(loop)
{
}

Epoll::~Epoll()
{
	::close(m_epollfd);
}

void Epoll::poll(int timeoutms, ChannelList * activeChannels)
{
	int readyNums = ::epoll_wait(m_epollfd,&*m_epolllist.begin(),(int)m_epolllist.size(),timeoutms);
	if (readyNums < 0) {
		LOG(INFO) << "epoll_wait(" << m_epollfd << ", " << &*m_epolllist.begin() << ", " << m_epolllist.size() << ", " << timeoutms << "): " << readyNums << std::endl;
	}

	Channel *readyChannel;
	for (int i = 0; i < readyNums; i++)
	{
		if ((readyChannel = m_channels.at(m_epolllist[i].data.fd) )!= NULL) {
			readyChannel->setRevents(m_epolllist[i].events);
			activeChannels->push_back(readyChannel);
		}
	}
}

void Epoll::updateChannel(Channel *channel)
{
	struct epoll_event  event;
	int fd;
	fd = event.data.fd = channel->fd();
	event.events = channel->events();
	
	if (m_channels.find(fd)!=m_channels.end()) {
		int s = ::epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &event);
		if (s < 0) {
			LOG(INFO) << "epoll_ctl( " << m_epollfd << ", EPOLL_CTL_MOD, " << fd << ", " << event.events << "): " << s << std::endl;
		}
	}
	else {
		int s = ::epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event);
		if (s < 0) {
			LOG(INFO) << "epoll_ctl( " << m_epollfd << ", EPOLL_CTL_ADD, " << fd << ", " << event.events << "): " << s << std::endl;
		}
	}

	m_channels[fd] =  channel ;
}


void Epoll::removeChannel(Channel *channel)
{
	struct epoll_event  event;
	int fd;
	fd = event.data.fd = channel->fd();
	event.events = channel->events();
	int s = ::epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, &event);
	if (s < 0) {
		LOG(INFO) << "epoll_ctl( " << m_epollfd << ", EPOLL_CTL_DEL, " << fd << ", " << event.events << "): " << s << std::endl;
	}
	
	m_channels.erase(fd);
}
