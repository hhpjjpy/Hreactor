#include "EchoServer.h"

int main() {
	INITLOG("EchoServer", "./log");
	EventLoop loop;
	EchoServer oEchoserver(&loop,45678);
	oEchoserver.start();
	loop.loop();
}