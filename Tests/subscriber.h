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

#define MAX_SUB_THREADS 16

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

   // std::function<void(event<T>)> julie_on_next;



    // function that is called when the stream encounters an error
    std::function<void(std::exception)> on_error;

    // function that is called when the stream is done producing data
    std::function<void()> on_completed;


public:
    static void simple_on_next(event<T> n){};

    static void simple_on_error(std::exception e){};

    static void simple_on_completed(){};

public:
    // constructor only requires an on_next function
    subscriber(
        std::function<void(event<T>)> next,
        std::function<void(std::exception)> error = simple_on_error,
        std::function<void()> completed = simple_on_completed) :
        
        id(usub_id++),
        on_next(next),
        on_error(error), 
        on_completed(completed) 
        {};

    // copy constructor
    subscriber(const subscriber &sub) :
        id(sub.id),
        on_next(sub.on_next), 
        on_error(sub.on_error), 
        on_completed(sub.on_completed)
    {};

    subscriber& operator=(subscriber &sub){
        return sub;
    };

    // move constructor
    subscriber(subscriber &&sub) :
        id(sub.id),
        on_next(sub.on_next), 
        on_error(sub.on_error), 
        on_completed(sub.on_completed) {

            sub.on_next = nullptr;
            sub.on_error = nullptr;
            sub.on_completed = nullptr;
            sub.id = 0;
    };

}; // end subscriber

template <typename T>
sub_id subscriber<T>::usub_id;


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
    struct queue_event {
    public:
        sub_id           s_id;
        event<T>         s_event;
        queue_event(sub_id _id, event<T> _event) : s_id(_id), s_event(_event) {
            
        };
    };

    bool                                   end = false; // tells threads when to exit
    sub_id                                 id = 0; // used for sequentially registering subscribers with unique ids
    std::map<sub_id, subscriber<T>>       subscribers; // maintains a map of subscribers for fast lookup
    std::list<queue_event>                 events; // list of events to be processed
    std::mutex                             e_lock; // used to syncronize access to list
    std::condition_variable                e_cond; // used to let worker threads sleep
    std::mutex                             sub_lock; // used to synchronize access to the subscriber map
    std::vector<std::thread>               pool; // maintains a list of runnning threads

    std::map<stream_id, std::vector<sub_id>>  stream_subs;

    // function passed to threads to handle subscriber_events
    void handle_event(){
        while(true){
            // acquire lock
            std::unique_lock<std::mutex> lk(e_lock);

            // wait until told to exit or while queue is empty
            while(!end && events.size() == 0){

                e_cond.wait(lk);

                // exit on flag
                if(end){
                    lk.unlock();
                    return;
                }
            }

            if(end){
                lk.unlock();
                return;
            }

            // get event from queue and remove it from queue
            if(events.size() == 0)
                continue;

            struct queue_event ev = events.front();
            events.pop_front();
            lk.unlock();

            // call the on_next function for the corresponding subscriber
            while(!sub_lock.try_lock());
            if(subscribers.count(ev.s_id) == 0){
                sub_lock.unlock();
                continue;
            }

            subscriber<T> sub = subscribers.at(ev.s_id);
            sub_lock.unlock();
            sub.on_next(ev.s_event);
        }
    };

    void grow(){
        auto size = subscribers.size();
        if(pool.size() < MAX_SUB_THREADS && ((float) subscribers.size()) / pool.size() >= 4){
            pool.push_back(std::thread(&subscriber_pool<T>::handle_event, this));
        }
    };
public:

    // constructor with default concurrency level set to 1
    subscriber_pool(int concurrency = 1){
        // never make more than 16 threads
        int max = concurrency > 16 ? 16 : concurrency;

        // create threads based on the concurrency requested
        for(int i = 0; i < max; i++){
            pool.push_back(std::thread(&subscriber_pool<T>::handle_event, this));
        }
    };

    // destructor
    ~subscriber_pool(){
        // set the end flag
        std::unique_lock<std::mutex> lk(e_lock);
        end = true;
        lk.unlock();

        // join all worker threads
        for(auto& t : pool){
            e_cond.notify_all();
            t.join();
        }

    };

    // called to pass a new subscriber_event to subscriber given by sub_id
    void notify(sub_id id, event<T> event){

        // create an event to add to the queue
        struct queue_event ev(id, event);

        // lock the queue and add the event to it
        std::unique_lock<std::mutex> lk(e_lock);
        
        events.push_back(ev);

        lk.unlock();

        // wake up any worker threads
        e_cond.notify_all();
    };

    void notify_all(std::vector<sub_id>& ids, event<T> event){
        for(auto id : ids){
            notify(id, event);
        }
    };
    
    void notify_stream(stream_id str_id, event<T> event){
        for(auto id : stream_subs.at(str_id)){
            notify(id, event);
        }
    };

    // used to notify a subscriber when a stream encounters an exception
    void error(sub_id id, std::exception e){
        while(!sub_lock.try_lock());
        if(subscribers.count(id) == 0){
            sub_lock.unlock();
            return;
        }
        subscriber<T> sub = subscribers.at(id);
        sub_lock.unlock();
        sub.on_error(e);
    };

    // used to notify a subscriber that a stream has ended
    void complete(sub_id id){
        // lock the subscriber pool
        while(!e_lock.try_lock());
        while(!sub_lock.try_lock());

        // get and delete the subscriber
        subscriber<T> sub = subscribers.at(id);
        subscribers.erase(id);

        // unlock the subscriber pool
        sub_lock.unlock();
        e_lock.unlock();
        // call the subscribers on_complete function
        sub.on_completed();
    };

    // used to register a subscriber with the current subscriber_pool
    void register_subscriber(subscriber<T> sub){

        // lock the subscriber pool
        while(!sub_lock.try_lock());
        
        // OLD: add the subscriber to the map
        subscribers.insert(std::pair<sub_id, subscriber<T>>(id, sub));
        
        // unlock the subscriber pool
        sub_lock.unlock();
    };

    // used to register a subscriber and indicate the stream it is associated with
    void register_subscriber(subscriber<T> sub, stream_id id){

        //TODO: it'll hang here forever if you make the stream using stream's default constructor
        // lock the subscriber pool
        while(!sub_lock.try_lock());
           // std::cout << "trying to get lock" << std::endl;

       // std::cout << "got the lock" << std::endl;

        // add a stream_id mapping
        subscribers.insert(std::pair<sub_id, subscriber<T>>(sub.id, sub));
        stream_subs[id].push_back(sub.id);

        // unlock the subscriber pool
        sub_lock.unlock();
    };

};

#endif