#pragma once
#ifndef THREAD_H
#define THREAD_H
#include <iostream>
#include <functional>
#include <pthread.h>
class Thread {
public:
	typedef std::function<void()> ThreadFun;
	Thread(ThreadFun&& fun) :
		m_bstart(false),
		m_join(false),
		m_ttid(0),
		m_Fun(std::move(fun))
	{
	}
	~Thread();
	void start();
	int join();
	bool operator==(Thread &thread);
	pthread_t getTheadId() { return m_ttid; }
	bool isStart() { return m_bstart; }
	bool isjoin() { return m_join; }
private:
	bool m_bstart;
	bool m_join;
	pthread_t m_ttid;
	ThreadFun m_Fun;
};

#endif // !
