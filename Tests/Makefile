CC = gcc
CXX = g++
CFLAGS = -std=c++11 -I/usr/local/include
CXXFLAGS = -std=c++11 -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lboost_thread-mt -lboost_system

default: helloworld

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: clean helloworld

helloworld: helloworld.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) helloworld.cpp -o helloworld


rc.o: rc.h rc.cpp
	$(CC) $(CFLAGS) -c -o rc.o rc.cpp

subscriber: subscriber.cpp subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) subscriber.cpp -o subscriber


helloworld2: helloworld2.cpp rc.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) helloworld2.cpp rc.o -o helloworld2	

.PHONY:
clean:
	rm -f *~ *.o helloworld2 helloworld a.out

.PHONY:
all: clean default
