#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop * baseloop, int threads) :
	m_baseloop(baseloop),
	m_uThreads(threads),
	m_unext(0)
{
	if (m_uThreads > 1) {
		for (int i = 1; i < m_uThreads; i++) {
			std::unique_ptr<EventLoopThread>  t(new EventLoopThread());
			m_vecloops.push_back(t->startLoop());
			m_vecEventLoopThreads.push_back(std::move(t));
		}
	}
}

EventLoop * EventLoopThreadPool::getNextLoop()
{
	EventLoop *loop;
	loop = m_baseloop;
	if (!m_vecloops.empty()) {
		loop = m_vecloops[m_unext];
		m_unext++;
		if (m_unext >= m_vecloops.size()) {
			m_unext = 0;
		}
	}
	return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoop()
{
	if (m_vecloops.empty())
	{
		return std::vector<EventLoop*>(1,m_baseloop);
	}

	return m_vecloops;
}


