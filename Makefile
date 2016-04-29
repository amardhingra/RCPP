CC  = g++
CXX = g++ -O0

#-fno-elide-constructors

INCLUDES = -I/usr/local/include 

CFLAGS   = -std=c++11  -g -Wall $(INCLUDES)
#CXXFLAGS = -std=c++14 -g -Wall $(INCLUDES) -o3

LDFLAGS =  -L/usr/local/lib -lcurl -loauth -ljsoncpp -lboost_system
LDLIBS  = 

default: startDemo

startDemo: startDemo.o twitter_streamer.o twitter_dataparser.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ 

startDemo.o: startDemo.cpp
	$(CC) -c $(CFLAGS) $<

twitter_streamer.o: TwitterDemo/twitter_streamer.cpp TwitterDemo/libs/twitcurl/urlencode.cpp TwitterDemo/twitter_streamer.h
	$(CC) -c $(CFLAGS) $<

twitter_dataparser.o: TwitterDemo/twitter_dataparser.cpp TwitterDemo/twitter_dataparser.h
	$(CC) -c $(CFLAGS) $<


.PHONY: clean
clean:
	rm -f startDemo a.out core *.o

.PHONY: all
all: clean default
