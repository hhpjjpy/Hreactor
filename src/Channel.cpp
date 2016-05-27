#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "Log.h"
#include <sys/epoll.h> 

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN|EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;


Channel::Channel(EventLoop *loop, int fd)
	:m_loop(loop),
	m_fd(fd),
	m_events(0),
	m_revents(0),
	m_index(0)
{
}


void Channel::handleEvent()
{
	if (m_revents&EPOLLERR) {
		if (m_errorCallBack) m_errorCallBack();
	}
	if (m_revents & (EPOLLIN | EPOLLPRI | EPOLLHUP)) {
		if (m_readCallBack) m_readCallBack();
	}
	if (m_revents & EPOLLOUT) {
		if (m_writeCallBack) m_writeCallBack();
	}
}

void Channel::remove()
{
	m_loop->removeChannel(this);
}

void Channel::update()
{
	m_loop->updateChannel(this);
}
