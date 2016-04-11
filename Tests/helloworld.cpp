// must have boost installed
// with homebrew: brew install boost

// assumptions: streams are immutable, but can be mirrored
// refs: Java SE7 Observable, RxJava, RxJS, boost::signals2, boost::thread, wiki page for observer pattern, 
// started with diff sources and streams but saw no need for the extra layer of abstraction, at least for this example

#include <iostream>
#include <vector>
#include <thread>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

// thread-safe signal handling, used for the notification system
// Not currently used but will probably be useful later
//http://www.boost.org/doc/libs/1_43_0/doc/html/signals2.html
#include <boost/signals2.hpp> 

using namespace std;

// putting all classes in this one file for testing purposes only

// A stream consists of events
class event {
public:
	string data;
	event(string);
};

event::event(string new_data) {
	data = new_data;
}

// A source is the input to a stream
class source {
public:
	bool has_next = false;
	event get_next;

	void get_keyboard_input(void) {
		while(true) {
			/*
			string keyinput;
			cin >> keyinput;
			cout << "u entered " << keyinput << endl;*/
			cout << "rofl ";
		}
	}
};

// A subscriber listens to one or more streams (for testing, a subscriber has one stream)
// It has a handler for handling the stream's events
class subscriber {
	//stream my_stream;
public:
	// Notifies the subscriber of a new element in the sequence
	//std::function<void(string)> on_next;
		std::function<void(event new_event)> on_next;
};

// A stream accumulates sources (currently for testing, a stream only has one source)
class stream {
	// indicates if there's a new value ready for subscribers to handle
	bool changed = false;
	subscriber my_subscriber;


public:
	//source my_source;

	void setChanged() {
		changed = true;
	}

	void subscribe(subscriber new_subscriber) {
		my_subscriber = new_subscriber;
	}

	void notifySubscribers(event new_event) {
		my_subscriber.on_next(new_event);
		changed = false;
	}

	void get_keyboard_input() {
		while(true) {
			
			string keyinput;
			cin >> keyinput;
			
			setChanged();
			notifySubscribers(keyinput);
			//cout << "lmao ";
		}
	}

};

void printlol() {
	while (true) {
		cout << "lol ";
	}
}

struct Hello {
	void operator()() const {
		cout << "hello  ";
	}
};

struct World {
	void operator()() const {
		cout << "world ";
	}
};

int main() {
	//source keyboard_source;
	stream keyboard_stream;
	//keyboard_stream.my_source = keyboard_source;

	subscriber mysub;
	mysub.on_next = [](event new_event) { cout << new_event.data << endl; };

	keyboard_stream.subscribe(mysub);

	boost::thread t2(boost::bind(&stream::get_keyboard_input, &keyboard_stream));
	t2.join();

		//mysub.on_next = []() { cout << "lol " << endl; };
/*
	boost::signals2::signal<void ()> sig;
	sig.connect(mysub.on_next);
	sig.connect(World());
	sig();
	sig();
	sig();*/

/*
	boost::thread t1{printlol};
	//t.join();
	boost::thread t2(boost::bind(&source::get_keyboard_input, &keyboard_source));
	t2.join();
	t1.join();
*/
	//thread t1(keyboard_source.get_keyboard_input);

	//t1.join();

//	mysub.on_next("poo");
		mysub.on_next(event("poo"));
/*
	string keyinput;
	cin >> keyinput;
	cout << "u entered " << keyinput << endl;*/
	return 0;
}

