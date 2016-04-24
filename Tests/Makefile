CC = gcc
CXX = g++

CFLAGS   = -std=c++11 -g -Wall $(INCLUDES)
CXXFLAGS = -std=c++11 -g -Wall $(INCLUDES)

INCLUDES = -I/usr/local/include

LDFLAGS = -L/usr/local/lib -lboost_thread-mt -lboost_system
LDLIBS = 

# executables: helloworld test

# objects: stream.o subscriber.o test.o helloworld.o

# .PHONY: default
# default: $(executables)

# $(executables): subscriber.o stream.o

# $(objects): stream.h event.h subscriber.h


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

stream.o: stream.cpp stream.h event.h
	$(CXX) $(CXXFLAGS) stream.cpp -c

test: CXXFLAGS += -DMAIN
test: CCFLAGS += -DMAIN
test: test.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test.o -o test

test.o: test.cpp stream.h 
	$(CXX) $(CXXFLAGS) test.cpp -c


.PHONY: clean
clean:
	rm -f *~ *.o helloworld2 helloworld a.out subscriber test

.PHONY: all
all: clean default
