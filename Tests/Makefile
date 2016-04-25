CC = gcc
CXX = g++

CFLAGS   = -std=c++11 -g -Wall $(INCLUDES)
CXXFLAGS = -std=c++11 -g -Wall $(INCLUDES)

INCLUDES = -I/usr/local/include

LDFLAGS = -L/usr/local/lib -lboost_thread-mt -lboost_system
LDLIBS = 


executables: helloworld test

$(objects): stream.h subscriber.h

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: clean helloworld

helloworld: helloworld.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) helloworld.cpp -o helloworld

test: test.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test.cpp -o test


.PHONY: clean
clean:
	rm -f *~ *.o helloworld2 helloworld a.out subscriber test

.PHONY: all
all: clean executables
