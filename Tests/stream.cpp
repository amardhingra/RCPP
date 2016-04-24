#include "stream.h"

stream::stream(subscriber_pool* some_pool){
    thread_pool = some_pool; 
    id++;
}

//create a threadpool with concurrency 2 if none exists
stream::stream(){
    subscriber_pool pool(2);
    thread_pool = &pool;
    id++;
}

void stream::set_changed() {
    changed = true;
}

void stream::clear_changed() {
    changed = false;
}

bool stream::has_changed() {
    return changed;
}

// Add a single new subscriber 
void stream::register_subscriber(subscriber new_subscriber) {
    thread_pool->register_subscriber(new_subscriber, id);
}

// Add a vector of new subscribers
void stream::register_subscriber(std::vector<subscriber> new_subscribers){
    for (subscriber new_subscriber : new_subscribers )
        thread_pool->register_subscriber(new_subscriber, id);
}

// If the stream has changed, then notify all its subscribers and clear the "changed" variable to indicate that the stream has no longer changed
void stream::notify_subscribers(event new_event) {
    if (this->has_changed()) {
        // auto pool_subscribers = thread_pool->pool;
        // for (subscriber pool_subscriber : pool_subscribers)
        //     pool_subscribers.notify(new_event);
        thread_pool->notify_stream(id, new_event);
        clear_changed();
    }
}


stream *stream::stream_from_keyboard_input(subscriber_pool* pool) {
    using namespace std;
    // Make a new stream
    stream new_stream = stream(pool);
    stream *new_stream_ptr = &new_stream;

    // Sets get_events_from_source() to read in from keyboard and notify observers when a new line is entered from keyboard
    new_stream.get_events_from_source = [new_stream]() {   
        string keyinput;
        cin >> keyinput; 
        new_stream.set_changed();
        new_stream.notify_subscribers(keyinput);
    };
    return new_stream_ptr;
}

// Starts the stream; will continuously call get_events_from_source.
void stream::start() {
    while(true) {
        get_events_from_source();
    }
}

