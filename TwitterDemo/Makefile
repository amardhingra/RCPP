CC  = g++
CXX = g++ -O0

#-fno-elide-constructors

INCLUDES =

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -std=c++14 -g -Wall $(INCLUDES) -o3

LDFLAGS = -lcurl -loauth
LDLIBS  =

default: twitter_streamer
demo: twitter_streamer.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) twitter_streamer.cpp libs/twitcurl/urlencode.cpp -o twitter_streamer

.PHONY: clean
clean:
	rm -f twitter_streamer twitter_streamer.o a.out core *.o

.PHONY: all
all: clean default
