#include "stream.h"

stream::stream(subscriber_pool& some_pool){
    thread_pool = some_pool; 
}

stream::stream(){
    thread_pool = subscriber_pool pool(2);
}

void stream::setChanged() {
    changed = true;
}

void stream::clearChanged() {
    changed = false;
}

bool stream::hasChanged() {
    return changed;
}

// Add a single new subscriber 
void stream::register_subscribers(subscriber new_subscriber) {
    thread_pool.register_subscriber(new_subscriber)
}

// Add a vector of new subscribers
void stream::register_subscriber(std::vector<subscriber> new_subscribers){
    for (subscriber new_subscriber : new_subscribers )
        thread_pool.register_subscriber(new_subscriber)

}

// If the stream has changed, then notify all subscribers and clear the "changed" variable to indicate that the stream has no longer changed
void stream::notifySubscribers(event new_event) {
    if (this->hasChanged()) {
        auto pool_subscribers = thread_pool->pool;
        for (subscriber pool_subscriber : pool_subscribers)
            pool_subscribers.notify(new_event);
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


