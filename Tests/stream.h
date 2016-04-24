#ifndef INCLUDED_STREAM
#define INCLUDED_STREAM

#include "subscriber.h"
#include "event.h"

typedef unsigned long stream_id;
/*
   A stream emits data for subscribers to the stream to handle.
   A stream can have many subscribers.
   */

//namespace {class subscriber_pool; }
//namespace {class subscriber; }

template <typename T>
class stream {
    //initially set to 0
	static stream_id id; 

    bool changed = false;

    // The threadpool that is assigned to this stream. If none are specified then it gets initialized??
    //seems like there would be ownership problems with this model
    subscriber_pool<T> *thread_pool; 

    public:
    void change(){
        changed = true;
    };

    void clear_changed(){
        changed = false;
    };

    bool has_changed(){
        return changed;
    };


    public:
    //default constructor, will eventually take from a source. 
    stream(){
        subscriber_pool<T> pool(2);
        thread_pool = &pool;
        id++;
    }; 

    //you can specify a specific subscriber_pool
    stream(subscriber_pool<T>* some_pool) {
        thread_pool = some_pool; 
        id++;
    };  

	~stream(){}; 
    
    // Factory method: returns a stream from keyboard input
    //static stream *stream_from_keyboard_input(subscriber_pool* pool);

    // A function that gets events and notifies subscribers accordingly
    std::function<void()> get_events_from_source;

    // Add a new subscriber to this stream
    void register_subscriber(subscriber<T> new_subscriber){
        thread_pool->register_subscriber(new_subscriber, id);
    };
    void register_subscriber(std::vector<subscriber<T>> subscribers){
        for (subscriber<T> new_subscriber : new_subscribers )
            thread_pool->register_subscriber(new_subscriber, id);
    };

    // If the stream has changed, then notify all subscribers and clear the "changed" variable to indicate that the stream has no longer changed
    //template<class T> 
    void notify_subscribers(event<T> new_event){
         if (this->has_changed()) {
        // auto pool_subscribers = thread_pool->pool;
        // for (subscriber pool_subscriber : pool_subscribers)
        //     pool_subscribers.notify(new_event);
            thread_pool->notify_stream(id, new_event);
            clear_changed();
        }
    };

    // Starts the stream;
    void start(){
        while(true) {
            get_events_from_source();
        }
    };

    // Test function that 
    void get_keyboard_input(); 

};

template <typename T>
stream_id stream<T>::id;

#endif
