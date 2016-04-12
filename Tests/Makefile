CC = gcc
CXX = g++
CFLAGS = -std=c++11 -I/usr/local/include
CXXFLAGS = -std=c++11 -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lboost_thread-mt -lboost_system

default: helloworld

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: all

helloworld: helloworld.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) helloworld.cpp -o helloworld

.PHONY:
clean:
	rm -f *~ *.o helloworld a.out

.PHONY:
all: clean default