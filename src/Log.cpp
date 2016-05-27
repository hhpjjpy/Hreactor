#include "Log.h"


pthread_once_t Log::once = PTHREAD_ONCE_INIT;
std::string Log::LogName = "NETLIB";
std::string Log::sPath = "./log";