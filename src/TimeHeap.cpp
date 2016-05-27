#include "TimeHeap.h"
#include "Channel.h"
#include "Log.h"
#include <functional>
#include <iostream>
#include <string.h>
int CreateNonbTimefd() {
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
		TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerfd < 0)
	{
		LOG(INFO) << " timerfd_create (CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC): " << timerfd << std::endl;
	}
	return timerfd;
}

struct itimerspec howmanyFromNow(PtrTimer ptrtimer) {
	if (ptrtimer == NULL) {
		struct itimerspec howmany;
		bzero(&howmany, sizeof(howmany));
		return howmany;
	}
	TimeStamp now;
	long usec = ptrtimer->getExpiration().getlongdata() - now.getlongdata();
	if (usec < 1000*10) {
		usec = 1000*10;
	}
	struct itimerspec howmany;
	bzero(&howmany, sizeof(howmany));
	howmany.it_value.tv_sec = usec / 1000 / 1000;
	howmany.it_value.tv_nsec = (usec % 1000000) * 1000;

	return howmany;
}


TimeHeap::TimeHeap(EventLoop *loop) :
	m_loop(loop),
	m_timefd(CreateNonbTimefd()),
	m_timeChannel(new Channel(loop, m_timefd)),
	m_MapPosition(),
	m_vecTimes(),
	m_vecActiveTimes(),
	m_uHeadSize(0)
{
	m_timeChannel->setReadCallBack(std::bind(&TimeHeap::headRead,this));
	m_timeChannel->enableReading();
	LOG(INFO) << "  TimeHeap is create is Ok " << std::endl;
}

void TimeHeap::addTimer(PtrTimer ptrTimer)
{
	if (ptrTimer == NULL) {
		return ;
	}

	PtrTimer oldRoot = getMinTimer();
	m_vecTimes.push_back(ptrTimer);
	m_uHeadSize++;
	int lastIndex = adjectUpHeap(m_uHeadSize-1);
	m_MapPosition[ptrTimer] = lastIndex;
	
	if (oldRoot == NULL || oldRoot != getMinTimer()) {
		setTimefdTime(getMinTimer());
	}
	LOG(INFO) << "m_vecTimes : " << m_uHeadSize << std::endl;
}

void TimeHeap::cancelTimer(PtrTimer ptrTimer)
{
	LOG(INFO) << "  cancelTimer " << std::endl;
	if (ptrTimer == NULL) {
		return ;
	}

	int index = m_MapPosition.at(ptrTimer);
	m_vecTimes[index].swap(m_vecTimes[m_uHeadSize-1]);
	m_vecTimes.erase(m_vecTimes.end() - 1);
	m_MapPosition.erase(ptrTimer);
	adjectDownHeap(index);
	m_uHeadSize--;
}

void TimeHeap::headRead()
{
	readTimefd();
	setActiveTimes();
	for (auto it = m_vecActiveTimes.begin(); it != m_vecActiveTimes.end();it++) {
		(*it)->run();
		if ((*it)->isRepeat()) {
			(*it)->reset();
			addTimer(*(it));
		}
	}
	m_vecActiveTimes.clear();
}

void TimeHeap::setTimefdTime(PtrTimer ptrTimer)
{
	if (ptrTimer == NULL) {
		return;
	}
	struct itimerspec newValue;
	bzero(&newValue, sizeof(newValue));
	newValue = howmanyFromNow(ptrTimer);
	int iRet = ::timerfd_settime(m_timefd,0,&newValue,NULL);

	if (iRet < 0) {
		LOG(INFO) << " timerfd_settime(" << m_timefd << ",0," << newValue.it_value.tv_sec << ",NULL) : " << iRet << std::endl;
		LOG(INFO) << " errno is : " << errno << std::endl;
		LOG(INFO) << "timerfd_settime is error " << std::endl;
	}
}

void TimeHeap::readTimefd()
{
	uint64_t  howmany;
	int n = ::read(m_timefd,&howmany,sizeof(howmany));
	
	if (n != sizeof(howmany)) {
		LOG(INFO) << " read(" << m_timefd << "," << howmany << ", " << sizeof(howmany) << " ):" << n << std::endl;
	}
}

PtrTimer TimeHeap::getMinTimer()
{
	if (m_uHeadSize == 0) {
		LOG(INFO) << " m_uHeadSize :" << m_uHeadSize << std::endl;
		return NULL;
	}

	return m_vecTimes[0];
}

PtrTimer TimeHeap::popMinTimer()
{
	if (m_uHeadSize == 0) {
		return NULL;
	}
	PtrTimer minPtr = m_vecTimes[0];
	m_vecTimes[0].swap(m_vecTimes[m_uHeadSize - 1]);
	m_vecTimes.erase(m_vecTimes.end()-1);
	m_uHeadSize--;
	adjectDownHeap(0);
	m_MapPosition.erase(minPtr);
	
	return minPtr;
}

int TimeHeap::adjectDownHeap(int index)
{
	if ( (2 * index + 1) > m_uHeadSize-1) {
		return index;
	}
	else {
		if ((2 * index + 2) >= m_uHeadSize) {
			PtrTimer parentPtr = m_vecTimes[index];
			PtrTimer leftPtr = m_vecTimes[2 * index + 1];
			if(*parentPtr>*leftPtr){
				m_vecTimes[index].swap(m_vecTimes[2 * index + 1]);
				m_MapPosition[parentPtr] = 2 * index + 1;
				m_MapPosition[leftPtr] = index;
				index = 2 * index + 1;
			}
			else {
				return index;
			}
		}
		else {
			PtrTimer parentPtr = m_vecTimes[index];
			PtrTimer leftPtr = m_vecTimes[2 * index + 1];
			PtrTimer rightPtr = m_vecTimes[2 * index + 2];
			PtrTimer minChild = *leftPtr > *rightPtr ? rightPtr : leftPtr;
			int minChildIndex = *leftPtr > *rightPtr ? (2 * index + 2) : (2 * index + 1);
			if (*parentPtr > *minChild) {
				m_vecTimes[index].swap(m_vecTimes[minChildIndex]);
				m_MapPosition[parentPtr] = minChildIndex;
				m_MapPosition[minChild] = index;
				index = minChildIndex;
			}
			else {
				return index;
			}
		}
	}

	return adjectDownHeap(index);
	
}


int TimeHeap::adjectUpHeap(int index)
{
	if (index == 0) {
		return index;
	}
	int parentIndex = (index - 1) / 2;
	PtrTimer childPtr = m_vecTimes[index];
	PtrTimer parentPtr = m_vecTimes[parentIndex];
	if (*parentPtr > *childPtr) {
		m_vecTimes[index].swap(m_vecTimes[parentIndex]);
		m_MapPosition[childPtr] = parentIndex;
		m_MapPosition[parentPtr] = index;
		index = parentIndex;
	}
	else {
		return index;
	}

	return adjectUpHeap(index);
}

void TimeHeap::setActiveTimes()
{
	Timer now;
	bool isChange = false;
	while (getMinTimer()!=NULL&&now>*(getMinTimer())) {
		m_vecActiveTimes.push_back(popMinTimer());
		isChange = true;
	}
	if (isChange) {
		setTimefdTime(getMinTimer());
	}	
}

