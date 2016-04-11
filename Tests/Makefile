# g++ -std=c++11 -lboost_thread-mt -lboost_system helloworld.cpp -o helloworld

CC = g++
CFLAGS = -std=c++11
LDFLAGS = -lboost_thread-mt -lboost_system

helloworld: helloworld.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) helloworld.cpp -o helloworld

.PHONY:
clean:
	rm -f *~ *.o helloworld a.out


