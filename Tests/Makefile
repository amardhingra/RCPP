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

subscriber: subscriber.o
	$(CXX) $(LDFLAGS) subscriber.o -o subscriber

subscriber.o: subscriber.cpp subscriber.h
	$(CXX) $(CXXFLAGS) subscriber.cpp -c


helloworld2: helloworld2.cpp rc.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) helloworld2.cpp rc.o -o helloworld2	

stream.o: stream.cpp stream.h 
	$(CXX) $(CXXFLAGS) stream.cpp -c

test: test.o stream.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o test

test.o: test.cpp stream.h 
	$(CXX) $(CXXFLAGS) test.cpp -c


.PHONY:
clean:
	rm -f *~ *.o helloworld2 helloworld a.out subscriber

.PHONY:
all: clean default
