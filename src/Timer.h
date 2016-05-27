#pragma once
#include <functional>
#include "TimeStamp.h"
#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
	typedef std::function<void()> TimeCallBack;
	Timer(TimeCallBack cb, TimeStamp when, long interval) :
		m_CallBack(cb),
		m_texpiration(when),
		m_interval(interval),
		m_repeat(interval>0)
	{
	}
	Timer():
	m_CallBack(NULL),
	m_texpiration(),
	m_interval(0),
	m_repeat(false)
	{

	}
	void reset() {
		if (m_repeat) {
			m_texpiration.setNowAfter(m_interval);
		}
		else {
			m_texpiration = TimeStamp();
		}
	}

	void run() {
		if (m_CallBack != NULL) {
			m_CallBack();
		}
	}

	void setCallBack(TimeCallBack cb) { m_CallBack = cb; }

	TimeStamp getExpiration() {
		return m_texpiration;
	}
	
	void setExpiration(TimeStamp ts) {
		m_texpiration = ts;
	}

	bool isRepeat() {
		return m_repeat;
	}

	bool operator<(Timer timer) {
		return m_texpiration < timer.getExpiration();
	}
	
	bool operator>(Timer timer) {
		return m_texpiration > timer.getExpiration();
	}

private:
	TimeCallBack m_CallBack;
	TimeStamp m_texpiration;
	long m_interval;
	bool m_repeat;
};

#endif // !TIMER_H
