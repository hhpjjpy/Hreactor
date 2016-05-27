all:EchoClient EchoServer

EchoClient:EchoClient.cpp EchoClient.h
	g++ -std=c++11 -pthread -I ../../src/ EchoClient.cpp -lHreactor -lglog  -o EchoClient
EchoServer:EchoServer.cpp EchoServer.h
	g++ -std=c++11 -pthread -I ../../src/ EchoServer.cpp -lHreactor -lglog -o EchoServer 
clean:
	rm -rf EchoClient EchoServer
