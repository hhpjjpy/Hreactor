#pragma once
#ifndef TIMEHEAP_H
#define TIMEHEAP_H
#include <memory> 
#include <map>
#include <vector>
#include <sys/timerfd.h>
#include "Timer.h"
#include "TimeStamp.h"

class EventLoop;
class Channel;
typedef std::shared_ptr<Timer> PtrTimer;
typedef std::map<PtrTimer, int> TimePositionMap;
typedef std::vector<PtrTimer> VecTime;
typedef std::function<void()> TimeCallBack;

class TimeHeap {
public:
	TimeHeap(EventLoop *loop);
	void addTimer(PtrTimer);
	void cancelTimer(PtrTimer);
private:
	void headRead();
	void setTimefdTime(PtrTimer);
	void readTimefd();
	PtrTimer getMinTimer();
	PtrTimer popMinTimer();
	int adjectUpHeap(int index);
	int adjectDownHeap(int index);
	void setActiveTimes();
	EventLoop *m_loop;
	int m_timefd;
	std::unique_ptr<Channel> m_timeChannel;
	TimePositionMap m_MapPosition;
	VecTime m_vecTimes;
	VecTime m_vecActiveTimes;
	int m_uHeadSize;
};

#endif // !TIMEHEAP_H
