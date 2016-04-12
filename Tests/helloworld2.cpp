
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "rc.h"

//using namespace std;


// Factory method: returns a stream from keyboard input
stream *stream::stream_from_keyboard_input() {
    // Make a new stream
    stream *new_stream = new stream;

    // Sets get_events_from_source() to read in from keyboard and notify observers when a new line is entered from keyboard
    new_stream->get_events_from_source = [new_stream]() { 	
        string keyinput;
        cin >> keyinput; 
        new_stream->setChanged();
        new_stream->notifySubscribers(keyinput);
    };
    return new_stream;
}

// Sets up a stream from the keyboard input. Adds 2 subscribers: the first will echo your input back at you, the second will tell you if what you entered is longer than 2 characters.
int main() {

    // Create a new stream that reads in from keyboard
    stream *keyboard_stream = stream::stream_from_keyboard_input();

    // First subscriber will echo what you entered 
    subscriber sub1;
    sub1.on_next = [](event new_event) { 
        cout << "You entered: " << new_event.data << endl; 
    };

    // Second subscriber will tell you if what you entered is longer than 2 characters
    subscriber sub2;
    sub2.on_next = [](event new_event) { 
        if (new_event.data.size() > 2) {
            cout << "The word you entered is longer than 2 characters." << endl;
        };
    };

    // Add the 2 subscribers to the stream
    keyboard_stream->subscribe(sub1);
    keyboard_stream->subscribe(sub2);

    // Currently threading manually; in future we should probably do it automatically or via wrapper functions
    boost::thread t2(boost::bind(&stream::start, keyboard_stream));
    t2.join();

    return 0;
}
