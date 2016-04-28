CC = gcc
CXX = g++

CFLAGS   = -std=c++11 -g -Wall $(INCLUDES)
CXXFLAGS = -std=c++11 -g -Wall $(INCLUDES)

INCLUDES = -I/usr/local/include

LDFLAGS = -L/usr/local/lib -lboost_thread-mt -lboost_system
LDLIBS = 

objects = test \
		   test-map \
		   test-filter \
		   test-file-stream \
		   test-fast-events-failing \
		   test-stream-constructor-failing \
		   test-2-streams-share-pool-failing \
		   test-2-streams-share-pool-failing2 \
		   test-2-streams-diff-pools-failing \
		   test-2-streams-diff-pools-failing2 \
		   #test-merge \


executables: helloworld test test-2-streams-share-pool-failing2 test-2-streams-diff-pools-failing test-2-streams-diff-pools-failing2
$(objects): stream.h subscriber.h

debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: executables

helloworld: helloworld.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) helloworld.cpp -o helloworld

debugtest: CXXFLAGS += -DDEBUG -g
debugtest: CCFLAGS += -DDEBUG -g
debugtest: test

test: test.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test.cpp -o test

# success test
test-map: test-map.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-map.cpp -o test-map.out

test-filter: test-filter.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-filter.cpp -o test-filter.out

test-file-stream: test-file-stream.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-file-stream.cpp -o test-file-stream.out

test-merge: test-merge.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-merge.cpp -o test-merge.out

# failing test
test-fast-events-failing: test-fast-events-failing.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-fast-events-failing.cpp -o test-fast-events-failing.out

# failing test
test-stream-constructor-failing: test-stream-constructor-failing.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-stream-constructor-failing.cpp -o test-stream-constructor-failing.out

# failing test
test-2-streams-share-pool-failing: test-2-streams-share-pool-failing.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-2-streams-share-pool-failing.cpp -o test-2-streams-share-pool-failing.out

# failing test
test-2-streams-share-pool-failing2: test-2-streams-share-pool-failing2.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-2-streams-share-pool-failing2.cpp -o test-2-streams-share-pool-failing2.out

test-2-streams-diff-pools-failing: test-2-streams-diff-pools-failing.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-2-streams-diff-pools-failing.cpp -o test-2-streams-diff-pools-failing.out

test-2-streams-diff-pools-failing2: test-2-streams-diff-pools-failing2.cpp stream.h subscriber.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test-2-streams-diff-pools-failing2.cpp -o test-2-streams-diff-pools-failing2.out



.PHONY: clean
clean:
	rm -rf *~ *.o helloworld2 helloworld test-fast-events-failing a.out subscriber test *.out *.dSYM $(objects)

.PHONY: all
all: clean executables
