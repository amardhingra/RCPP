// Compilation instructions:
// 1. Must have boost installed. With homebrew: brew install boost
// 2. cd into Test/ directory and type "make"
// 3. run "helloworld"
// 4. Type stuff into command line and see that it's echoed back at you

/* 
References ----------------------------------------------------------

Wikipedia for "Observer Pattern"
https://en.wikipedia.org/wiki/Observer_pattern

Java SE7 Observable API
https://docs.oracle.com/javase/7/docs/api/java/util/Observable.html

Rx Docs:
http://reactivex.io/documentation/operators.html#creating
http://reactivex.io/documentation/observable.html

RxJava API
http://reactivex.io/RxJava/javadoc/rx/Observable.html
http://reactivex.io/RxJava/javadoc/index.html?rx/Observer.html

RxJS
https://gist.github.com/staltz/868e7e9bc2a7b8c1f754
https://github.com/Reactive-Extensions/RxJS/tree/master/doc/api/core
https://github.com/Reactive-Extensions/RxJS/blob/master/doc/api/core/observable.md#rxobservableprototypesubscribeobserver--onnext-onerror-oncompleted

Microsoft's System.Reactive.Linq
https://msdn.microsoft.com/en-us/library/system.reactive.linq.observable(v=vs.103).aspx 

StackOverflow: How to implement IObservable<T>:
http://stackoverflow.com/questions/1768974/implementing-iobservablet-from-scratch

boost::signals2
http://www.boost.org/doc/libs/1_43_0/doc/html/signals2.html

boost::thread
http://www.boost.org/doc/libs/1_60_0/doc/html/thread/thread_management.html#thread.thread_management.tutorial

*/

// started with diff sources and streams but saw no need for the extra layer of abstraction, at least for this example

#include <iostream>
#include <vector>
#include <exception>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

// thread-safe signal handling, used for the notification system
// Not currently used but will probably be useful later
// #include <boost/signals2.hpp> 

using namespace std;

// Assumptions: streams are immutable, but can be mirrored
// Putting all classes in this one file for testing purposes only

/*
 A stream consists of events, which contain data. 
 For the purposes of testing, currently an event just has a string as its data.
 */
class event {
public:
	string data;
	event(string);
};

event::event(string new_data) {
	data = new_data;
}

/* 
A subscriber listens to one or more streams.
 It has 3 handlers for dealing with data received from the stream: one for handling normal events, one for handling errors, and one for handling the completion event.
 */
class subscriber {
public:
	// Provides the subscriber with a new event to handle. 
	// Called by the stream when it has a new event for the subscriber.
		std::function<void(event new_event)> on_next;

		// Notifies the subscriber that the stream has experienced an error. The error is passed by the stream to the subscriber to handle.
		std::function<void(exception& new_exception)> on_error;

// Notifies the subscriber that the stream has finished sending events.
				std::function<void()> on_completed;


};

/*
 A stream emits data for subscribers to the stream to handle.
 A stream can have many subscribers.
 */
class stream {
	// Indicates if the stream has changed
	bool changed = false;
	// List of subscribers that are subscribed to this stream.
	vector<subscriber> my_subscribers;

public:

	void setChanged() {
		changed = true;
	}

	void clearChanged() {
		changed = false;
	}

	bool hasChanged() {
		return changed;
	}

	// Add a new subscriber to this stream
	void subscribe(subscriber new_subscriber) {
		my_subscribers.push_back(new_subscriber);
	}

	// If the stream has changed, then notify all subscribers and clear the "changed" variable to indicate that the stream has no longer changed
	void notifySubscribers(event new_event) {
		if (this->hasChanged()) {
		for (subscriber my_subscriber : my_subscribers)
			my_subscriber.on_next(new_event);
		clearChanged();
	}
	}

	// 
	static void get_keyboard_input() {
		while(true) {
			
			string keyinput;
			cin >> keyinput;
			
			setChanged();
			notifySubscribers(keyinput);
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
	boost::thread t1(stream.get_keyboard_input);
/*
	boost::thread t2(boost::bind(&stream::get_keyboard_input, &keyboard_stream));
	t2.join();*/

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

