#include "stream.h"

stream::stream(subscriber_pool& some_pool){
    thread_pool = some_pool; 
}

stream::stream(){
    thread_pool = subscriber_pool pool(2);
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
// right now the stream knows nothing about the ID's of its subscribers, but it probably should
void stream::register_subscribers(subscriber new_subscriber) {
    thread_pool.register_subscriber(new_subscriber)
}

// Add a vector of new subscribers
// right now the stream knows nothing about the ID's of its subscribers, but it probably should
void stream::register_subscriber(std::vector<subscriber> new_subscribers){
    for (subscriber new_subscriber : new_subscribers )
        thread_pool.register_subscriber(new_subscriber)

}

// If the stream has changed, then notify all its subscribers and clear the "changed" variable to indicate that the stream has no longer changed

//TODO: THIS IS WRONG. IT NOTIFIES ALL SUBSCRIBERS IN ITS POOL
void stream::notifySubscribers(event new_event) {
    if (this->has_changed()) {
        auto pool_subscribers = thread_pool->pool;
        for (subscriber pool_subscriber : pool_subscribers)
            pool_subscribers.notify(new_event);
        clear_changed();
    }
}

// Starts the stream; will continuously call get_events_from_source.
void stream::start() {
    while(true) {
        get_events_from_source();
    }
}

stream *stream::stream_from_keyboard_input() {
    // Make a new stream
    stream new_stream = stream();

    // Sets get_events_from_source() to read in from keyboard and notify observers when a new line is entered from keyboard
    new_stream->get_events_from_source = [new_stream]() {   
        string keyinput;
        cin >> keyinput; 
        new_stream->set_changed();
        new_stream->notifySubscribers(keyinput);
    };
    return new_stream;
}


