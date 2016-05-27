#pragma once
#ifndef LOG_H
#define LOG_H

#include <glog/logging.h>
#include <string>
#include <pthread.h>

class Log {
public:
	static pthread_once_t once;
	static std::string LogName;
	static std::string sPath;

static void initLog() {
		google::InitGoogleLogging(LogName.c_str());
		FLAGS_log_dir = sPath.c_str();
		LOG(INFO) << "LOG INIT OK , Path:./log ";
	}

static void initOnceLog() {
		pthread_once(&once, &Log::initLog);
	}

};




#define INITLOG(name,path) do { \
							Log::LogName=name;\
							Log::sPath = path;\
							Log::initOnceLog();\
							} while (0)  \
			


#endif
