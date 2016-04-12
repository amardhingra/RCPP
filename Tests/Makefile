CC = g++
CFLAGS = -std=c++11 -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lboost_thread-mt -lboost_system

default: helloworld

helloworld: helloworld.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) helloworld.cpp -o helloworld

.PHONY:
clean:
	rm -f *~ *.o helloworld a.out

.PHONY:
all: clean default