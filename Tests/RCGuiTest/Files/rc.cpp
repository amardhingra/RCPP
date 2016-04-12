
#include <iostream>
#include <vector>
#include <exception>
#include "rc.h"


// thread-safe signal handling, used for the notification system
// Not currently used but will probably be useful later
// #include <boost/signals2.hpp> 

using namespace std;

// Assumptions: streams are immutable, but can be copied
// Putting all classes in this one file for testing purposes only

/*
   A stream consists of events, which contain data. 
   For the purposes of testing, currently an event just has a string as its data.
   */
// class event {
//     public:
//         string data;
//         event(string);
// };

event::event(string new_data) {
    data = new_data;
}

// /* 
//    A subscriber listens to one or more streams.
//    It has 3 handlers for dealing with data received from the stream: one for handling normal events, one for handling errors, and one for handling the completion event.
//    */
// class subscriber {
//     public:
//         // Provides the subscriber with a new event to handle. 
//         // Called by the stream when it has a new event for the subscriber.
//         std::function<void(event new_event)> on_next;

//         // Notifies the subscriber that the stream has experienced an error. The error is passed by the stream to the subscriber to handle.
//         std::function<void(exception& new_exception)> on_error;

//         // Notifies the subscriber that the stream has finished sending events.
//         std::function<void()> on_completed;
// };

/*
   A stream emits data for subscribers to the stream to handle.
   A stream can have many subscribers.
   */
// stream::stream(){
//     changed = false;
// }
    
    // A function that gets events and notifies subscribers accordingly
    //std::function<void()> get_events_from_source;

    void stream::setChanged() {
        changed = true;
    }

    void stream::clearChanged() {
        changed = false;
    }

    bool stream::hasChanged() {
        return changed;
    }

    // Add a new subscriber to this stream
    void stream::subscribe(subscriber new_subscriber) {
        my_subscribers.push_back(new_subscriber);
    }

    // If the stream has changed, then notify all subscribers and clear the "changed" variable to indicate that the stream has no longer changed
    void stream::notifySubscribers(event new_event) {
        if (this->hasChanged()) {
            for (subscriber my_subscriber : my_subscribers)
                my_subscriber.on_next(new_event);
            clearChanged();
        }
    }

    // Starts the stream; will continuously call get_events_from_source.
    void stream::start() {
        while(true) {
            get_events_from_source();
        }
    }

    // Test function that 
    void stream::get_keyboard_input() {
        while(true) {

            string keyinput;
            cin >> keyinput;

            setChanged();
            notifySubscribers(keyinput);
        }
    }



