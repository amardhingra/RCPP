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

#include "event.h"
// #include "stream.h"

typedef unsigned long sub_id;
typedef unsigned long stream_id;


/*
 * Subscriber
 * Defines a class that acts on events generated by a data stream,
 * deal with errors and handle stream termination
 */
template <class T>
class subscriber {
private:
    static sub_id usub_id;

public:
    // unique identifier for subscriber
    sub_id id;
    
    // function that is called when the stream generates new data
    std::function<void(event<T>)> on_next;

    // function that is called when the stream encounters an error
    std::function<void(std::exception)> on_error;

    // function that is called when the stream is done producing data
    std::function<void()> on_completed;


public:
    static void simple_on_next(event<T> n);

    static void simple_on_error(std::exception e);

    static void simple_on_completed();

public:
    // constructor only requires an on_next function
    subscriber(
        std::function<void(event<T>)> next,
        std::function<void(std::exception)> error = simple_on_error,
        std::function<void()> completed = simple_on_completed);

    // copy constructor
    subscriber(const subscriber &sub);
    subscriber& operator=(subscriber &sub);

    // move constructor
    subscriber(subscriber &&sub);

}; // end subscriber


/*
 * Subscriber Pool
 * Defines a class that maintains a pool of threads that are used to handle
 * messages passed to subscribers that have been registered with this pool
 */
template <class T>
class subscriber_pool {

private:
    // wrapper class to store the subscriber_event and subscriber_id to 
    // which the event is related
    class queue_event {
    public:
        sub_id           s_id;
        event<T>         s_event;
        queue_event(sub_id _id, event<T> _event) : s_id(_id), s_event(_event) {};
    };

    bool                                   end = false; // tells threads when to exit
    sub_id                                 id = 0; // used for sequentially registering subscribers with unique ids
    std::map<sub_id, subscriber<T>&>       subscribers; // maintains a map of subscribers for fast lookup
    std::list<queue_event>                 events; // list of events to be processed
    std::mutex                             e_lock; // used to syncronize access to list
    std::condition_variable                e_cond; // used to let worker threads sleep
    std::mutex                             sub_lock; // used to synchronize access to the subscriber map
    std::vector<std::thread>               pool; // maintains a list of runnning threads

    std::map<stream_id, std::vector<sub_id>>  stream_subs;

    // function passed to threads to handle subscriber_events
    void handle_event();
public:

    // constructor with default concurrency level set to 1
    subscriber_pool(int concurrency = 1);
    ~subscriber_pool();

    // called to pass a new subscriber_event to subscriber given by sub_id
    void notify(sub_id id, event<T> event);
    void notify_all(std::vector<sub_id>& ids, event<T> event);
    void notify_stream(stream_id str_id, event<T> event);

    // used to notify a subscriber when a stream encounters an exception
    void error(sub_id id, std::exception e);

    // used to notify a subscriber that a stream has ended
    void complete(sub_id id);

    // used to register a subscriber with the current subscriber_pool
    void register_subscriber(subscriber<T> sub);
    void register_subscriber(subscriber<T> sub, stream_id id);

};

#endif