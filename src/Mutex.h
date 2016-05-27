#pragma once
#ifndef MUTEX_H
#define MUTEX_H
#include <iostream>
#include <pthread.h>
#include "Log.h"
class MutexLock {
public:
	MutexLock() {
		int ret = pthread_mutex_init(&m_mutex, NULL);//此处待修复，最好不成功时马上退出
		if (ret < 0) {
			LOG(INFO) << " mutex init error " << std::endl;
		}
	}
	~MutexLock() {
		int ret = pthread_mutex_destroy(&m_mutex);
		if (ret < 0) {
			LOG(INFO) << " mutex destroy error " << std::endl;
		}
	}
	void Lock() {
		int ret = pthread_mutex_lock(&m_mutex);
		if (ret < 0) {
			LOG(INFO) << " mutex lock error " << std::endl;
		}
	}
	void unLock() {
		int ret = pthread_mutex_unlock(&m_mutex);
		if (ret < 0) {
			LOG(INFO) << " mutex unlock error " << std::endl;
		}
	}
	pthread_mutex_t* getPthreadMutex() {
		return &m_mutex;
	}

private:
	pthread_mutex_t m_mutex;
};

class MutexLockGuard {
public:
explicit 
	MutexLockGuard(MutexLock& Mutex):
		m_MutexLock(Mutex)
	{
		m_MutexLock.Lock();
	}

	~MutexLockGuard() {
		m_MutexLock.unLock();
	}

private:
	MutexLock& m_MutexLock;
};




#define MutexLockGuard(x) static_assert(false,"missing mutex guard var name ");



#endif // !MUTEX_H
