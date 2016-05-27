#include "EchoClient.h"
#include "Log.h"

int main() {
	INITLOG("EchoClient","./log");
	EventLoop loop;
	EchoClient oEchoClient(&loop, hhp::net::InetAddr("127.0.0.1", 45678));
	oEchoClient.start();
	std::string buf;
	std::cin >> buf;
	oEchoClient.sendMsg(buf);
	loop.loop();
}
