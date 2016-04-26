CC  = g++
CXX = g++ -O0

#-fno-elide-constructors

INCLUDES =

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -std=c++14 -g -Wall $(INCLUDES) -o3

LDFLAGS = -lcurl -loauth
LDLIBS  =

default: demo
demo: demo.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) demo.cpp libs/twitcurl/urlencode.cpp -o demo

.PHONY: clean
clean:
	rm -f demo demo.o a.out core *.o

.PHONY: all
all: clean default
