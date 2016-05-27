#pragma once
#ifndef CHANNEL_H
#define CHANNEL_H

#include <iostream>
#include <functional>

class EventLoop;

class Channel
{
public:
	typedef std::function<void()> EventCallBack;
	Channel(EventLoop* loop,int fd);

	void handleEvent();

	void setReadCallBack(const EventCallBack&& cb) {
		m_readCallBack = std::move(cb);
	}
	void setWriteCallBack(const EventCallBack&& cb) {
		m_writeCallBack = std::move(cb);
	}
	void setErrorCallBack(const EventCallBack&& cb) {
		m_errorCallBack = std::move(cb);
	}
	void setRevents(int revents) { m_revents = revents; }

	int fd()const { return m_fd; }
	int events()const { return m_events; }

	void enableReading() { m_events |= kReadEvent; update();}
	void enableWriting() { m_events |= kWriteEvent; update(); }
	void disableReading() { m_events &= ~kReadEvent; update();}
	void disableWriting() { m_events &= ~kWriteEvent; update(); }
	void disableAll() { m_events = kNoneEvent; update(); }

	void remove();

private:
	void update();
	
	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;

	EventLoop *m_loop;
	const int m_fd;
	int m_events;
	int m_revents;
	int m_index;

	EventCallBack m_readCallBack;
	EventCallBack m_writeCallBack;
	EventCallBack m_errorCallBack;

};

#endif // !CHANNEL_H

