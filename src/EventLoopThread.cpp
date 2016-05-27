#include "EventLoopThread.h"

EventLoopThread::EventLoopThread() :
	m_loop(NULL),
	m_Thread(std::bind(&EventLoopThread::ThreadFun,this)),
	m_Mutex(),
	m_cond(m_Mutex),
	m_isExit(true)
{
}

EventLoopThread::~EventLoopThread()
{
	m_Thread.join();
}

EventLoop * EventLoopThread::startLoop()
{
	m_Thread.start();
	{
	MutexLockGuard lock(m_Mutex);
	while (m_loop == NULL) {
		m_cond.wait();
	}
	}
	return m_loop;
}

void EventLoopThread::ThreadFun()
{
	EventLoop loop;

	{
	MutexLockGuard lock(m_Mutex);
	m_loop = &loop;
	m_cond.notify();
	}
	
	loop.loop();

	m_isExit = true;
	m_loop = NULL;
	
}
