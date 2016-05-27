#include "ThreadPool.h"
#include "Log.h"

ThreadPool::ThreadPool(int threads) :
	m_mutex(),
	m_cond(m_mutex)
{
	for (int i = 0; i < threads; i++) {
		Thread *thread = new Thread(std::bind(&ThreadPool::run, this));
		thread->start();
		m_vecThreads.push_back(thread);
	}
}

void ThreadPool::addTaskLock(Task&& task)
{
	MutexLockGuard lock(m_mutex);
	m_queTask.push_front(std::move(task));
	m_cond.notify();
}

void ThreadPool::addTaskUnlock(Task&& task)
{
	m_queTask.push_front(std::move(task));
	m_cond.notify();
}

void ThreadPool::join()
{
	for (auto it = m_vecThreads.begin(); it != m_vecThreads.end(); it++) {
		(*it)->join();
	}
}

void ThreadPool::run()
{
	while (true) {
		MutexLockGuard lock(m_mutex);
		if (m_queTask.size() == 0) {
			m_cond.wait();
			if (m_queTask.size() == 0) {
				continue;
			}
		}
		Task oTesk = m_queTask.back();
		m_queTask.pop_back();
		try {
			oTesk();
		}
		catch (...) {
			LOG(INFO) << " ThreadPool::run() error to call oTesk()  " << std::endl;
		}
	}
}
