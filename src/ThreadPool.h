#pragma once
#ifndef  THREADPOOL_H
#define  THREADPOOL_H
#include <iostream>
#include <functional>
#include <deque>
#include <vector>
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

class ThreadPool {
public:
	typedef std::function<void()> Task;
	explicit ThreadPool(int threads);//初始化线程数

	void addTaskLock(Task&& task);
	void addTaskUnlock(Task&& task);
	void join();
private:
	void run();
	std::vector<Thread*> m_vecThreads;
	std::deque<Task> m_queTask;
	MutexLock m_mutex;
	Condition m_cond;
};




#endif // ! THREADPOOL_H
