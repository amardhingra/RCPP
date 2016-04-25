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

template <typename InputType, typename OutputType = InputType>
class stream {
    //initially set to 0
	static stream_id id; 

    bool changed = false;

    // The threadpool that is assigned to this stream. If none are specified then it gets initialized??
    //seems like there would be ownership problems with this model
    subscriber_pool<InputType> *thread_pool; 

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
        std::cout << "calling stream default constructor" << std::endl;
        subscriber_pool<InputType> pool(2);
        thread_pool = &pool;
        id++;
    }; 

    //you can specify a specific subscriber_pool
    stream(subscriber_pool<InputType>* some_pool) {
        thread_pool = some_pool; 
        id++;
    };  

	~stream(){}; 
    
    // Factory method: returns a stream from keyboard input
    //static stream *stream_from_keyboard_input(subscriber_pool* pool);

    // A function that gets events and notifies subscribers accordingly
    //std::function<void()> get_events_from_source;

    std::function<void(stream<InputType> my_stream)> on_subscribe;

    // Add a new subscriber to this stream
    void register_subscriber(subscriber<InputType> new_subscriber){
        thread_pool->register_subscriber(new_subscriber, id);
    };
    void register_subscriber(std::vector<subscriber<InputType>> new_subscribers){
        for (subscriber<InputType> new_subscriber : new_subscribers )
            thread_pool->register_subscriber(new_subscriber, id);
    };

    // If the stream has changed, then notify all subscribers and clear the "changed" variable to indicate that the stream has no longer changed
    //template<class T> 
    void notify_subscribers(event<InputType> new_event){
         if (this->has_changed()) {
           // std::cout << "1" << std::endl;
        // auto pool_subscribers = thread_pool->pool;
        // for (subscriber pool_subscriber : pool_subscribers)
        //     pool_subscribers.notify(new_event);
            thread_pool->notify_stream(id, new_event);
            clear_changed();
        }
    };

    // Starts the stream;
    void start(){
        on_subscribe(*this);

    };

    // create stream with pool and on_subscribe
    // TODO: create version of this w/o pool arg once stream's default constructor gets fixed
    /*
    static stream<InputType> *create(std::function<void(stream<InputType> my_stream)> on_subscribe, subscriber_pool<InputType>* some_pool) {
        stream *new_stream = new stream(some_pool);
        new_stream->on_subscribe = on_subscribe;
        return new_stream;
    }*/

    // TODO: returns by value, is that ok?
    static stream<InputType> create(std::function<void(stream<InputType> my_stream)> on_subscribe, subscriber_pool<InputType>* some_pool) {
        stream<InputType> new_stream (some_pool);
        new_stream.on_subscribe = on_subscribe;
        return new_stream;
    }

    //stream<InputType> 


    std::vector<stream<InputType>> children;

    stream<InputType> map(std::function<event<InputType>(event<InputType>)>);




};

template <typename InputType, typename OutputType>
stream_id stream<InputType, OutputType>::id;

#endif
