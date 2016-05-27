#pragma once
#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H
#include <iostream>
#include <vector>
#include <memory> 
#include "EventLoop.h"
#include "EventLoopThread.h"

class EventLoopThreadPool {
public:
	explicit EventLoopThreadPool(EventLoop* baseloop,int threads);
	EventLoop* getNextLoop();
	std::vector<EventLoop*>  getAllLoop(); 
	int getThreadNums() { return m_uThreads; }
private:
	EventLoop *m_baseloop;
	int m_uThreads;
	int m_unext;
	std::vector<std::unique_ptr<EventLoopThread>> m_vecEventLoopThreads;
	std::vector<EventLoop*> m_vecloops;

};


#endif // !EVENTLOOPTHREADPOOL_H

