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
debug: clean test

helloworld: helloworld.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) helloworld.cpp -o helloworld

test: test.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test.cpp -o test

# failing test
test-fast-events-failing: test-fast-events-failing.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-fast-events-failing.cpp -o test-fast-events-failing.out

# failing test
test-stream-constructor-failing: test-stream-constructor-failing.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-stream-constructor-failing.cpp -o test-stream-constructor-failing.out

# failing test
test-2-streams-share-pool-failing: test-2-streams-share-pool-failing.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-2-streams-share-pool-failing.cpp -o test-2-streams-share-pool-failing.out

test-2-streams-diff-pools-failing: test-2-streams-diff-pools-failing.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-2-streams-diff-pools-failing.cpp -o test-2-streams-diff-pools-failing.out



.PHONY: clean
clean:
	rm -f *~ *.o helloworld2 helloworld a.out subscriber test *.out

.PHONY: all
all: clean executables
