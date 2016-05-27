#include "EventLoop.h"
#include <iostream>
#include <cassert>
#include <sys/eventfd.h>

#include "Channel.h"
#include "Epoll.h"
#include "TimeHeap.h"
#include "Timer.h"
#include "SocketOps.h"
#include "Log.h"


using namespace std;
__thread EventLoop *t_loopInThisThread = 0;

pthread_t GetPthreadId() {
	return pthread_self();
}

int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0)
	{
		LOG(INFO) << " eventfd create error " << std::endl;
		abort();
	}
	return evtfd;
}


EventLoop::EventLoop() 
	:m_bloop(false), 
	m_isquit(false),
	m_mutex(),
	m_threadId(GetPthreadId()),
	m_weakFd(createEventfd()),
	m_weakChannel(new Channel(this,m_weakFd)),
	m_activeChannels(),
	m_epoll(new Epoll(this)),
	m_timeheap(new TimeHeap(this))
{
	if (t_loopInThisThread) {
		LOG(INFO) << " Another EventLoop , exists in this thread " << endl;
		exit(-1);
	}
	else {
		t_loopInThisThread = this;
	}
	m_weakChannel->setReadCallBack(std::bind(&EventLoop::headRead,this));
	m_weakChannel->enableReading();
}

EventLoop::~EventLoop()
{
	assert(!m_bloop);
	t_loopInThisThread = NULL;
}


void EventLoop::loop()
{
	assert(!m_bloop);
	assertInLoopThread();
	m_bloop = true;
	LOG(INFO) << " loop is run  threadid is : "<< m_threadId << endl;
	while (!m_isquit) {
		m_activeChannels.clear();
		m_epoll->poll(-1, &m_activeChannels);
		for (auto it = m_activeChannels.begin(); it != m_activeChannels.end(); it++) 
		{
			(*it)->handleEvent();
		}
		runLoopTask();
	}
	m_bloop = false;
}

void EventLoop::updateChannel(Channel *channel)
{
	m_epoll->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
	m_epoll->removeChannel(channel);
}

PtrTimer EventLoop::runAt(TimeCallBack&& cb, TimeStamp time)
{
	PtrTimer ptrTimer(new Timer(cb, time, 0));
	m_timeheap->addTimer(ptrTimer);
}

PtrTimer EventLoop::runAfter(TimeCallBack&& cb, long usec)
{
	TimeStamp ts;
	ts.setNowAfter(usec);
	PtrTimer ptrTimer(new Timer(cb,ts,0));
	m_timeheap->addTimer(ptrTimer);
	return ptrTimer;
}

PtrTimer EventLoop::runEvery(TimeCallBack&& cb, long interval)
{
	TimeStamp ts;
	ts.setNowAfter(interval);
	PtrTimer ptrTimer(new Timer(cb, ts, interval));
	m_timeheap->addTimer(ptrTimer);
	return ptrTimer;
}

void EventLoop::CancalTimer(PtrTimer ptr)
{
	m_timeheap->cancelTimer(ptr);
}

void EventLoop::runInLoop(CallFun&& cb)
{
	if (isInLoopThread()) {
		cb();
	}
	else {
		MutexLockGuard lock(m_mutex);
		m_vecTasks.push_back(std::move(cb));
		WeakUp();
	}

}

void EventLoop::headRead()
{
	uint64_t one = 1;
	ssize_t n = hhp::net::read(m_weakFd, &one, sizeof one);
	if (n != sizeof one) {
		LOG(INFO) << " read weakFd error " << std::endl;
	}
}

void EventLoop::abortNotInLoopThread()
{
	LOG(INFO) << "EventLoop was exit loop is created in m_threadId : "<<m_threadId << endl;
	exit(-1);
}

void EventLoop::runLoopTask()
{
	std::vector<CallFun> vecTemp;
	{
		MutexLockGuard lock(m_mutex);
		vecTemp.swap(m_vecTasks);
	}
	for (auto it = vecTemp.begin(); it != vecTemp.end(); it++) {
		(*it)();
	}
}

void EventLoop::WeakUp()
{
	uint64_t one = 1;
	ssize_t n = hhp::net::write(m_weakFd, &one, sizeof one);
	if (n != sizeof one) {
		LOG(INFO) << " write weakFd error " << std::endl;
	}
}
