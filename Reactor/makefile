
CC=g++
OBJ=Mutex_test.o TimeStamp.o TimeStamp_test.o CurrentThread.o CountDownLatch.o Thread.o \
	CountDownLatch_test.o ThreadPool.o LogStream.o Logger.o FileUtil.o LogFile.o AsyncLogging.o Timer.o EventLoop.o Channel.o Poll.o\
	Poller.o Epoll.o DefaultPoller.o Signal.o  \
	TimerQueue.o Socket.o SocketOps.o InetAddress.o Acceptor.o TcpServer.o \
	TcpConnection.o Buffer.o Connector.o TcpClient.o EventThreadLoop.o EventLoopThreadPool.o \
	CountDownLatch_test Mutex_test TimeStamp_test CurrentThread_test Thread_test ThreadPool_test \
	FixedBuffer_test LogStream_test Logger_test FileUtil_test LogFile_test  AsyncLogging_test \
	TimerQueue_test Acceptor_test TcpConnection_test TcpServer_test Buffer_test\
	Connector_test TcpClient_test EventLoop_test EventThreadLoop_test EventLoopThreadPool_test SignalFd_test \
	TcpClient1_test

LIBS=-lpthread
CFLAGS=-g -Wall
VPATH=src:include
HEADERS:=include/Connector.h
CINCLUDES=-I/include
all:$(OBJ)

Mutex_test.o:Mutex_test.cpp Mutex.h
	$(CC) $(CFLAGS) -c $^ -I  include/ 

TimeStamp.o:TimeStamp.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
TimeStamp_test.o:TimeStamp_test.cpp TimeStamp.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
CurrentThread.o:CurrentThread.cpp TimeStamp.cpp TimeStamp.h
	$(CC) $(CFLAGS) -c $^ -I  include/ 

CountDownLatch.o:CountDownLatch.cpp Mutex.h Condition.cpp
	$(CC) $(CFLAGS) -c $^ -I  include/ 

Thread.o:Thread.cpp CountDownLatch.cpp CurrentThread.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
CountDownLatch_test.o:CountDownLatch.cpp CountDownLatch_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 

CurrentThread_test.o:CurrentThread_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
ThreadPool.o:ThreadPool.cpp CountDownLatch.cpp Thread.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
LogStream.o:LogStream.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
Logger.o:Logger.cpp  TimeStamp.cpp CurrentThread.cpp LogStream.cpp  LogStream.h
	$(CC) $(CFLAGS) -c $^ -I  include/
FileUtil.o:FileUtil.cpp Logger.cpp LogStream.cpp
	$(CC) $(CFLAGS) -c $^ -I  include/
LogFile.o:FileUtil.cpp CurrentThread.cpp LogFile.cpp Logger.cpp LogStream.cpp
	$(CC) $(CFLAGS) -c $^ -I  include/
AsyncLogging.o:AsyncLogging.cpp Logger.cpp FileUtil.cpp LogStream.cpp Logger.cpp LogFile.cpp
	$(CC) $(CFLAGS) -c $^ -I  include/
Timer.o:Timer.cpp TimeStamp.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
EventLoop.o:Channel.cpp Poller.cpp DefaultPoller.cpp Poll.cpp Epoll.cpp  Timer.cpp TimerQueue.cpp TimeStamp.cpp EventLoop.cpp Mutex.h Logger.cpp LogStream.cpp Signal.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
Channel.o:Channel.cpp EventLoop.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
Poll.o:Poll.cpp Channel.cpp Poller.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
Poller.o:Poller.cpp Poll.cpp Epoll.cpp  
	$(CC) $(CFLAGS) -c $^ -I  include/
Epoll.o:Epoll.cpp Channel.cpp Poller.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
DefaultPoller.o:DefaultPoller.cpp Epoll.cpp Poll.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
Signal.o:Signal.cpp EventLoop.cpp Channel.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
TimerQueue.o:TimerQueue.cpp Timer.cpp Channel.cpp Logger.cpp LogStream.cpp TimeStamp.cpp  
	$(CC) $(CFLAGS) -c $^ -I  include/
Socket.o:Socket.cpp SocketOps.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
SocketOps.o:SocketOps.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
InetAddress.o:InetAddress.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
Acceptor.o:Acceptor.cpp InetAddress.cpp Socket.cpp Channel.cpp LogStream.cpp Logger.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
TcpServer.o:TcpServer.cpp Acceptor.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
TcpConnection.o:TcpConnection.cpp InetAddress.cpp EventLoop.cpp Channel.cpp Logger.cpp LogStream.cpp
	$(CC) $(CFLAGS) -c $^ -I  include/
Buffer.o:Buffer.cpp Logger.cpp LogStream.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
Connector.o:Connector.cpp  EventLoop.cpp Channel.cpp Socket.cpp SocketOps.cpp InetAddress.cpp ${HEADERS}
	$(CC) $(CFLAGS) -c $^  -I include/ 
TcpClient.o:TcpClient.cpp Connector.cpp EventLoop.cpp Channel.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
EventThreadLoop.o:EventLoop.cpp EventThreadLoop.cpp Condition.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
EventLoopThreadPool.o:EventLoopThreadPool.cpp EventThreadLoop.cpp EventLoop.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/

CountDownLatch_test:CountDownLatch_test.o CurrentThread.o CountDownLatch.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
TimeStamp_test:TimeStamp_test.o TimeStamp.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
Mutex_test:Mutex_test.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
CurrentThread_test:CurrentThread.o CurrentThread_test.o TimeStamp.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
Thread_test.o:Thread_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
Thread_test:Thread.o Thread_test.o CurrentThread.o TimeStamp.o  CountDownLatch.o TimeStamp.h
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
ThreadPool_test.o:ThreadPool_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
ThreadPool_test:ThreadPool_test.o  ThreadPool.o  CurrentThread.o Thread.o CountDownLatch.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
FixedBuffer_test.o:FixedBuffer_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
FixedBuffer_test:LogStream.h FixedBuffer_test.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
LogStream_test.o:LogStream_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
LogStream_test:LogStream.o LogStream_test.o  TimeStamp.o TimeStamp.h
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
Logger_test.o:Logger_test.cpp TimeStamp.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
Logger_test:Logger_test.o Logger.o TimeStamp.o CurrentThread.o LogStream.o TimeStamp.h  LogStream.h
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
FileUtil_test.o:FileUtil_test.cpp  Logger.cpp LogStream.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
FileUtil_test:FileUtil_test.o FileUtil.o  TimeStamp.o CurrentThread.o Logger.o LogStream.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
LogFile_test.o:LogFile_test.cpp Mutex.h  Logger.cpp LogStream.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
LogFile_test:LogFile_test.o LogFile.o Logger.o LogStream.o  TimeStamp.o CurrentThread.o FileUtil.o  
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
AsyncLogging_test.o:AsyncLogging_test.cpp Mutex.h Logger.cpp LogStream.cpp LogFile.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
AsyncLogging_test:AsyncLogging.o Thread.o LogFile.o CurrentThread.o FileUtil.o Condition.o AsyncLogging_test.o TimeStamp.o CountDownLatch.o  Logger.o LogStream.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

TimerQueue_test.o:TimerQueue_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
TimerQueue_test:TimerQueue_test.o  EventLoop.o CurrentThread.o TimeStamp.o TimerQueue.o Timer.o  Channel.o DefaultPoller.o Poller.o Poll.o Epoll.o Signal.o Logger.o LogStream.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
# Signal_test.o:Signal_test.cpp
	# $(CC) $(CFLAGS) -c $^ -I  include/
# Signal_test:Signal_test.o signal.o EventLoop.o DefaultPoller.o Poller.o Epoll.o Poll.o CurrentThread.o TimeStamp.o Channel.o TimerQueue.o Timer.o Logger.o LogStream.o
	# $(CC) $(CFLAGS) $^ -o $@ $(LIBS)
Acceptor_test.o:Acceptor_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
Acceptor_test:Acceptor_test.o Acceptor.o Channel.o Poller.o DefaultPoller.o Epoll.o Poll.o EventLoop.o TimerQueue.o Timer.o Signal.o  CurrentThread.o TimeStamp.o SocketOps.o InetAddress.o Socket.o Logger.o LogStream.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
TcpConnection_test.o:TcpConnection_test.cpp TcpConnection.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
TcpConnection_test:TcpConnection_test.o TcpConnection.o TcpServer.o Acceptor.o DefaultPoller.o Epoll.o Poller.o Poll.o Channel.o EventLoop.o Timer.o Signal.o CurrentThread.o TimeStamp.o TimerQueue.o SocketOps.o Socket.o InetAddress.o Buffer.o Logger.o LogStream.o EventLoopThreadPool.o EventThreadLoop.o Thread.o CountDownLatch.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
TcpServer_test.o:TcpServer_test.cpp  TcpServer.cpp Acceptor.cpp InetAddress.cpp EventLoop.cpp CurrentThread.cpp
	$(CC) $(CFLAGS) -c $^ -I  include/ 
TcpServer_test:TcpServer_test.o TcpConnection.o TcpServer.o Acceptor.o DefaultPoller.o Epoll.o Poller.o Poll.o Channel.o EventLoop.o Timer.o Signal.o CurrentThread.o TimeStamp.o TimerQueue.o SocketOps.o Socket.o InetAddress.o Buffer.o Logger.o LogStream.o Thread.o CountDownLatch.o EventLoopThreadPool.o EventThreadLoop.o CountDownLatch.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
Buffer_test.o:Buffer_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
Buffer_test:Buffer_test.o Buffer.o Logger.o LogStream.o TimeStamp.o CurrentThread.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
Connector_test.o:Connector_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
Connector_test:Connector_test.o Connector.o Poll.o EventLoop.o DefaultPoller.o Poll.o Poller.o Epoll.o Timer.o TimeStamp.o TimerQueue.o Signal.o Channel.o  Logger.o LogStream.o CurrentThread.o SocketOps.o Socket.o InetAddress.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
TcpClient_test.o:TcpClient_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
TcpClient_test:TcpClient_test.o TcpClient.o EventLoop.o Channel.o DefaultPoller.o Epoll.o Poller.o Poll.o Timer.o TimerQueue.o CurrentThread.o Signal.o TimeStamp.o SocketOps.o InetAddress.o Socket.o Connector.o Logger.o LogStream.o TcpConnection.o Buffer.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
EventLoop_test.o:EventLoop_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
EventLoop_test:EventLoop_test.o EventLoop.o Channel.o DefaultPoller.o Epoll.o Poller.o Poll.o TimerQueue.o Timer.o CurrentThread.o Signal.o TimeStamp.o Thread.o CountDownLatch.o Condition.o  Logger.o LogStream.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
EventThreadLoop_test.o:EventThreadLoop_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
EventThreadLoop_test:EventThreadLoop_test.o EventThreadLoop.o EventLoopThreadPool.o EventLoop.o Poller.o Poller.o DefaultPoller.o Poll.o Epoll.o Channel.o CurrentThread.o Signal.o Timer.o TimerQueue.o CountDownLatch.o Logger.o LogStream.o Condition.o TimeStamp.o Thread.o TcpServer.o Socket.o SocketOps.o InetAddress.o Buffer.o TcpConnection.o Acceptor.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
EventLoopThreadPool_test.o:EventLoopThreadPool_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
EventLoopThreadPool_test:EventLoopThreadPool_test.o EventLoopThreadPool.o EventLoop.o Poll.o Poller.o DefaultPoller.o Epoll.o EventThreadLoop.o  Timer.o TimerQueue.o Channel.o CurrentThread.o Logger.o LogStream.o Signal.o TimeStamp.o  Poll.o Thread.o CountDownLatch.o EventThreadLoop.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
SignalFd_test.o:SignalFd_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
SignalFd_test:SignalFd_test.o Signal.o Poll.o Poller.o EventLoop.o Epoll.o Timer.o TimeStamp.o TimerQueue.o LogStream.o Logger.o SocketOps.o Socket.o Channel.o CurrentThread.o CountDownLatch.o Thread.o DefaultPoller.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
TcpClient1_test.o:TcpClient1_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
TcpClient1_test:TcpClient1_test.o TcpClient.o TcpConnection.o Connector.o SocketOps.o Socket.o InetAddress.o LogStream.o Logger.o Channel.o Poll.o Poller.o Epoll.o EventLoop.o CurrentThread.o DefaultPoller.o Timer.o TimeStamp.o TimerQueue.o Signal.o Buffer.o CountDownLatch.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
.PHONY :clean 
clean:
	rm -rf *.o
