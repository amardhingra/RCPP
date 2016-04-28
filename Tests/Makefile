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
		  test-stream-constructor \
		  test-stream-move \
		  test-stream-copy \
		  test-2-streams-share-pool \
		  test-2-streams-share-pool2 \
		  test-2-streams-diff-pools \
		  test-2-streams-diff-pools2 \
		  #test-merge \


obj: $(objects)

$(objects):


debug: CXXFLAGS += -DDEBUG -g
debug: CCFLAGS += -DDEBUG -g
debug: obj

.PHONY: clean
clean:
	rm -rf *~ *.o *.out *.dSYM $(objects)
