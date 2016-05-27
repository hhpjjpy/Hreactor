#pragma once
#ifndef HREACTOR_TYPE_H
#define HREACTOR_TYPE_H
#include <functional>
#include <string>
#include <memory>
#include "TcpConnection.h"

typedef std::shared_ptr<TcpConnection> TcpConnPtr;
typedef std::function<int(TcpConnPtr conn, std::string&& buf)> CallReadBack;
typedef std::function<void(TcpConnPtr conn)> CallOtherBack;


#endif