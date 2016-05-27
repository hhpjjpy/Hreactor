# Hreactor
# 一个reactor模式的，多线程高性能TCP网络库，参考陈硕的muduo网络库设计。
#基于 1.C++11标准
#     2.pthread 线程，里面包含使用C++ RAII模式对mutex和condtion的简单封装，以及线程池实现
#     3.reactor底层由epoll驱动，水平触发，只支持Linux
#     4. 使用优先队列（堆）和timefd 实现定时器。
#     5.日志使用Google开源日志库glog ，使用时需要链接该库。(后续版本完善)
#使用方法：见example/echo/下简单的echo服务器程序和客户端程序示例。
#后续完成示例的扩充。 
