// must have boost installed
// with homebrew: brew install boost
// this does not work yet!

#include <iostream>
#include <vector>
//#include <functional>
#include <thread>
#include <boost/thread.hpp>

using namespace std;

// putting all classes in this one file for testing purposes only

// A stream consists of events
class event {
public:
	string data;
};

// A source is the input to a stream
class source {
public:
	bool has_next = 0;
	event get_next;

	void get_keyboard_input(void) {
		while(true) {
			string keyinput;
			cin >> keyinput;
			cout << "u entered " << keyinput << endl;
		}
	}
};

// A stream accumulates sources (currenty for testing, a stream only has one source)
class stream {
public:
	source my_source;

};

// A subscriber listens to a stream
// It has a handler for handling the stream's events
class subscriber {
	stream my_stream;
public:
	std::function<void()> handler;
};


// Don't try to make too much sense of this main, just testing out stuff
int main() {
	source keyboard_source;
	stream keyboard_stream;
	keyboard_stream.my_source = keyboard_source;

	subscriber mysub;

	mysub.handler = []() { cout << "hello" << endl; };

	thread t1(keyboard_source.get_keyboard_input);
	t1.join();

	mysub.handler();
	string keyinput;
	cin >> keyinput;
	cout << "u entered " << keyinput << endl;
	return 0;
}

