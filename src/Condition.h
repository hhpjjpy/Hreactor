#pragma once
#ifndef CONDITION_H
#define CONDITION_H
#include <iostream>
#include <pthread.h>
#include "Mutex.h"
#include "Log.h"

class Condition {
public:
explicit 
	Condition(MutexLock& mutex) :
		m_mutex(mutex)
	{
		int ret = pthread_cond_init(&m_pcond, NULL);
		if (ret < 0) {
			LOG(INFO) << " cond init error " << std::endl;
		}
	}
	~Condition() {
		int ret = pthread_cond_destroy(&m_pcond);
		if (ret < 0) {
			LOG(INFO) << " cond destory error " << std::endl;
		}
	}
	void wait() {
		int ret = pthread_cond_wait(&m_pcond, m_mutex.getPthreadMutex());
		if (ret < 0) {
			LOG(INFO) << " cond wait error " << std::endl;
		}
	}
	void notify() {
		int ret = pthread_cond_signal(&m_pcond);
		if (ret < 0) {
			LOG(INFO) << " cond signal  error " << std::endl;
		}
	}
	void notifyAll() {
		int ret = pthread_cond_broadcast(&m_pcond);
		if (ret < 0) {
			LOG(INFO) << " cond broadcase error " << std::endl;
		}
	}

private:
	MutexLock& m_mutex;
	pthread_cond_t m_pcond;
};

#endif // !CONDITION_H
