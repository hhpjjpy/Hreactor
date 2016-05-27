option = -g -std=c++11 -pthread

OUTPUT = ./lib/libHreactor.a

OBJS =./obj/Connector.o \
      ./obj/Acceptor.o \
      ./obj/Channel.o \
      ./obj/Epoll.o \
      ./obj/EventLoop.o \
      ./obj/Socket.o \
      ./obj/SocketOps.o \
      ./obj/Buffer.o \
      ./obj/TcpConnection.o \
      ./obj/TcpServer.o \
      ./obj/TcpClient.o \
      ./obj/TimeHeap.o \
      ./obj/Thread.o \
      ./obj/EventLoopThread.o \
      ./obj/EventLoopThreadPool.o \
      ./obj/Log.o

all: $(OUTPUT)

VPATH=./src

obj/%.o: %.cpp
	$(CXX) $(CFLAGS) $(option)  -c $^ -o $@

$(OUTPUT): $(OBJS)
	ar crv $(OUTPUT) $(OBJS) 
install:
	cp ./lib/libHreactor.a /usr/lib/ 
clean:
	@rm -f ./test/*
	@rm -f ./obj/*
