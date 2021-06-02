
CC=g++
OBJ=Mutex_test.o TimeStamp.o TimeStamp_test.o CurrentThread.o CountDownLatch.o Thread.o CountDownLatch_test.o \
	ThreadPool.o LogStream.o Logger.o \
	CountDownLatch_test Mutex_test TimeStamp_test CurrentThread_test Thread_test ThreadPool_test \
	FixedBuffer_test LogStream_test Logger_test

LIBS=-lpthread
CFLAGS=-g -Wall
VPATH=src:include

CINCLUDES=-I/include
all:$(OBJ)

Mutex_test.o:Mutex_test.cpp Mutex.h
	$(CC) $(CFLAGS) -c $^ -I  include/ 

TimeStamp.o:TimeStamp.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
TimeStamp_test.o:TimeStamp_test.cpp TimeStamp.o
	$(CC) $(CFLAGS) -c $^ -I  include/ 
CurrentThread.o:CurrentThread.cpp TimeStamp.cpp TimeStamp.h
	$(CC) $(CFLAGS) -c $^ -I  include/ 

CountDownLatch.o:CountDownLatch.cpp Mutex.h Condition.cpp
	$(CC) $(CFLAGS) -c $^ -I  include/ 

Thread.o:Thread.cpp CountDownLatch.cpp CurrentThread.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
CountDownLatch_test.o:CountDownLatch.o CountDownLatch_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 

CurrentThread_test.o:CurrentThread_test.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/ 
ThreadPool.o:ThreadPool.cpp CountDownLatch.cpp Thread.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
LogStream.o:LogStream.cpp 
	$(CC) $(CFLAGS) -c $^ -I  include/
Logger.o:Logger.cpp  TimeStamp.cpp CurrentThread.cpp LogStream.cpp  LogStream.h
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
