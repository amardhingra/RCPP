CC = g++
CXX = g++

#CFLAGS   = -std=c++11 -g -Wall $(INCLUDES)
CXXFLAGS = -std=c++11 -g -Wall $(INCLUDES)

INCLUDES = -I/usr/local/include

LDFLAGS = -L/usr/local/lib -lboost_thread-mt -lboost_system
LDLIBS = 

dependencies = stream.h subscriber.h event.h stream_factory.h

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


obj: $(objects)

$(objects):


.PHONY: clean
clean:
	rm -rf *~ *.o *.out *.dSYM $(objects)
