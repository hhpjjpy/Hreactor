#include "Thread.h"
#include "Log.h"

typedef std::function<void()> ThreadFun;

void* runInThread(void *obj) {
	ThreadFun *fun = static_cast<ThreadFun*> (obj);

	if (obj == NULL) {
		return NULL;
	}
	else {
		try {
			(*fun)();
		}
		catch (...) {
			LOG(INFO) << " ThreadFun error to call fun()  " << std::endl;
		}
		
	}
	return NULL;
}


Thread::~Thread()
{
	if (m_bstart&&!m_join) {
		pthread_detach(m_ttid);
	}
}

void Thread::start()
{
	m_bstart = true;
	int ret = pthread_create(&m_ttid,NULL,&runInThread,&m_Fun);
	if (ret != 0) {
		LOG(INFO) << " pthread create error " << std::endl;
		m_bstart = false;
	}
}

int  Thread::join()
{
	if (m_bstart == false) {
		return -1;
	}
	m_join = true;
	return pthread_join(m_ttid, NULL);
}


bool Thread::operator==(Thread &thread) {
	return pthread_equal(m_ttid, thread.getTheadId());
}
