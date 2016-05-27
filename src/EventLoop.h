#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <vector>
#include <memory> 
#include "Mutex.h"
#include "TimeStamp.h"

class Channel;
class Epoll;
class TimeHeap;
class Timer;

class EventLoop 
{
public:
	typedef std::vector<Channel*> ChannelList;
	typedef std::function<void()> TimeCallBack;
	typedef std::function<void()> CallFun;
	typedef std::shared_ptr<Timer> PtrTimer;
	EventLoop();
	~EventLoop();
	void loop();
	void updateChannel(Channel*);
	void removeChannel(Channel*);
	PtrTimer runAt(TimeCallBack&& cb,TimeStamp time);
	PtrTimer runAfter(TimeCallBack&& cb, long usec);//微秒后
	PtrTimer runEvery(TimeCallBack&& cb, long interval);//每隔多少微秒
	void CancalTimer(PtrTimer);
	void assertInLoopThread()
	{
		if (!isInLoopThread()) 
		{
			abortNotInLoopThread();
		}
	}
	bool isInLoopThread() const { return m_threadId == pthread_self(); }
	void qiut() { m_isquit = true; }
	void runInLoop(CallFun&& cb);
	void WeakUp();
	pthread_t getLoopThreadId() { return m_threadId; }
private:
	void headRead();
	void abortNotInLoopThread();
	void runLoopTask();
	bool m_bloop;
	bool m_isquit;
	MutexLock m_mutex;
	const pthread_t  m_threadId;
	int m_weakFd;
	std::unique_ptr<Channel> m_weakChannel;
	ChannelList m_activeChannels;
	std::unique_ptr<Epoll> m_epoll;
	std::unique_ptr<TimeHeap> m_timeheap;
	std::vector<CallFun> m_vecTasks;
};



#endif // !EVENTLOOP_H
