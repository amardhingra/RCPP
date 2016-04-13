#ifndef INCLUDED_SUBSCRIBER
#define INCLUDED_SUBSCRIBER


#include <condition_variable>
#include <functional>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <iostream>

typedef unsigned long sub_id;

/*
 * Subscriber Event
 * Defines a class that allows a stream to pass an event
 * (usually a new data item) to a subscriber
 */
class subscriber_event {

public:
    // structure that indicates the type of data being passed to the subscriber
    enum message_type {
        BOOL  = 0,
        INT   = 1,
        FLOAT = 2,
        STR   = 3
    };

private:
    // structure to hold the data being passed to the subscriber
    union message_data {
        bool        b;
        int         i;
        float       f;
        std::string s;

    public:
        // constructors for supported types
        message_data() {};
        message_data(bool _b) : b(_b) {};
        message_data(int _i) : i(_i) {};
        message_data(float _f) : f(_f) {};
        message_data(std::string _s) : s(_s) {};
        ~message_data(){};

    }; // end message_data


private:
    // class variables to represent the type and data being passed
    message_type type;
    message_data data;

public:
    // constructors for supported types
    // set the correct member of the enum and assign the correct selector
    subscriber_event(bool _b) : type(message_type::BOOL), data(_b) {};
    subscriber_event(int _i) : type(message_type::INT), data(_i) {};
    subscriber_event(float _f) : type(message_type::FLOAT), data(_f) {};
    subscriber_event(std::string _s) : type(message_type::STR), data(_s) {};

    // copy constructor
    subscriber_event(const subscriber_event &event);

public:
    // accessors
    message_type get_type() { return type; }
    bool         get_b() { return data.b; }
    int          get_i() { return data.i; }
    float        get_f() { return data.f; }
    std::string  get_s() { return data.s; }

}; // end subscriber_event


/*
 * Subscriber
 * Defines a class that acts on events generated by a data stream,
 * deal with errors and handle stream termination
 */
class subscriber {

public:
    // function that is called when the stream generates new data
    std::function<void(subscriber_event)> on_next;

    // function that is called when the stream encounters an error
    std::function<void(std::exception)> on_error;

    // function that is called when the stream is done producing data
    std::function<void()> on_completed;


public:
    static void simple_on_next(subscriber_event n){
        // TODO: define behavior for simple next handler
    }

    static void simple_on_error(std::exception e){
        // TODO: define behavior for simple error handler
    }

    static void simple_on_completed(){
        // TODO: define behavior for simple completed handler
        // behaviour should include removing the stream from the stream map
    }

public:
    // constructor only requires an on_next function
    subscriber(
        std::function<void(subscriber_event)> next,
        std::function<void(std::exception)> error = simple_on_error,
        std::function<void()> completed = simple_on_completed) :
    on_next(next), on_error(error), on_completed(completed) {};

    // copy constructor
    subscriber(subscriber &sub) :
        on_next(sub.on_next), on_error(sub.on_error), on_completed(sub.on_completed) {};

}; // end subscriber


/*
 * Subscriber Pool
 * Defines a class that maintains a pool of threads that are used to handle
 * messages passed to subscribers that have been registered with this pool
 */
class subscriber_pool {

private:
    // wrapper class to store the subscriber_event and subscriber_id to 
    // which the event is related
    class event {
    public:
        sub_id           s_id;
        subscriber_event s_event;
        event(sub_id _id, subscriber_event _event) : s_id(_id), s_event(_event) {};
    };

    bool                                   end = false; // tells threads when to exit
    sub_id                                 id = 0; // used for sequentially registering subscribers with unique ids
    std::map<sub_id, subscriber&>          subscribers; // maintains a map of subscribers for fast lookup
    std::list<event>                       events; // list of events to be processed
    std::mutex                             lock; // used to syncronize access to list
    std::condition_variable                cond; // used to let worker threads sleep
    std::vector<std::thread>               pool; // maintains a list of runnning threads

    // function passed to threads to handle subscriber_events
    void handle_event();

public:

    // constructor with default concurrency level set to 1
    subscriber_pool(int concurrency = 1);
    ~subscriber_pool();

    // called to pass a new subscriber_event to subscriber given by sub_id
    void notify(sub_id id, subscriber_event event);

    // used to register a subscriber with the burrent subscriber_pool
    sub_id register_subscriber(subscriber& sub);
};

#endif