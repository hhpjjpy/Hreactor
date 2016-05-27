#pragma once
#ifndef  EVENTLOOPTHREAD_H
#define	 EVENTLOOPTHREAD_H
#include <iostream>
#include <functional>
#include "EventLoop.h"
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

class EventLoopThread {
public:
	EventLoopThread();
	~EventLoopThread();
	EventLoop* startLoop();
private:
	void ThreadFun();
	EventLoop* m_loop;
	Thread m_Thread;
	MutexLock m_Mutex;
	Condition m_cond;
	bool m_isExit;
};


#endif // ! EVENTLOOPTHREAD_H
