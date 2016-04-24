#include "stream.h"

template <typename T>
stream<T>::stream(subscriber_pool<T>* some_pool){
    thread_pool = some_pool; 
    id++;
}

//create a threadpool with concurrency 2 if none exists
template <typename T>
stream<T>::stream(){
    subscriber_pool<T> pool(2);
    thread_pool = &pool;
    id++;
}

template <typename T>
void stream<T>::change() {
    changed = true;
}

template <typename T>
void stream<T>::clear_changed() {
    changed = false;
}

template <typename T>
bool stream<T>::has_changed() {
    return changed;
}

// Add a single new subscriber
template <typename T> 
void stream<T>::register_subscriber(subscriber<T> new_subscriber) {
    thread_pool->register_subscriber(new_subscriber, id);
}

// Add a vector of new subscribers
template <typename T>
void stream<T>::register_subscriber(std::vector<subscriber<T>> new_subscribers){
    for (subscriber<T> new_subscriber : new_subscribers )
        thread_pool->register_subscriber(new_subscriber, id);
}

// If the stream has changed, then notify all its subscribers and clear the "changed" variable to indicate that the stream has no longer changed
template<class T> 
void stream<T>::notify_subscribers(event<T> new_event) {
    if (this->has_changed()) {
        // auto pool_subscribers = thread_pool->pool;
        // for (subscriber pool_subscriber : pool_subscribers)
        //     pool_subscribers.notify(new_event);
        thread_pool->notify_stream(id, new_event);
        clear_changed();
    }
}


//dont use this
// stream *stream::stream_from_keyboard_input(subscriber_pool* pool) {
//     using namespace std;
//     // Make a new stream
//     stream new_stream = stream(pool);
//     stream *new_stream_ptr = &new_stream;

//     // Sets get_events_from_source() to read in from keyboard and notify observers when a new line is entered from keyboard
//     new_stream.get_events_from_source = [new_stream]() {   
//         string keyinput;
//         cin >> keyinput; 
//         event<string> my_event(keyinput);
//         new_stream.change();
//         new_stream.notify_subscribers(my_event);
//     };
//     return new_stream_ptr;
// }

// Starts the stream; will continuously call get_events_from_source.
template <typename T>
void stream<T>::start() {
    while(true) {
        get_events_from_source();
    }
}

