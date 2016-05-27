#pragma once
#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
class TimeStamp {
public:
	TimeStamp() {
		struct timeval tv;
		::gettimeofday(&tv, NULL);
		m_udata = tv.tv_sec*1000*1000+tv.tv_usec;
	}
explicit TimeStamp(long usec) { 
			m_udata = usec;
	}
	TimeStamp addTimeUsec(long usec) {
		m_udata +=  usec;	
		return *this;
	}

	long getlongdata() {
		return m_udata;
	}

	void setNowAfter(long usec) {
		struct timeval tv;
		::gettimeofday(&tv, NULL);
		m_udata = tv.tv_sec * 1000 * 1000 + tv.tv_usec + usec;
	}

	TimeStamp operator+(TimeStamp timestamp) {
		long usec = timestamp.getlongdata();
		return TimeStamp(m_udata + usec);
	}

	bool operator<(TimeStamp timestamp) {
		long data = timestamp.getlongdata();
		return m_udata<data;
	}
	bool operator>(TimeStamp timestamp) {
		long data = timestamp.getlongdata();
		return m_udata>data;
	}
	void reset(long data) {
		m_udata = data;
	}
	void setAfterNow(long data) {
		m_udata += data;
	}

private:
	long m_udata;
};
#endif // !TIMESTAMP_H
