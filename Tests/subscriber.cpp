
#include "subscriber.h"

#include <iostream>
#include <chrono>

/**
 * Methods and variable declarations for the SUBSCRIBER_EVENT class
 **/


//copy constructor for subscriber_event
subscriber_event::subscriber_event(const subscriber_event& event){
    // copy type
    type = event.type;

    // copy correct variable from data
    switch(type){
        case message_type::BOOL:
            data.b = event.data.b;
            break;
        case message_type::INT:
            data.i = event.data.i;
            break;
        case message_type::FLOAT:
            data.f = event.data.f;
            break;
        case message_type::STR:
            data.s = event.data.s;
            break;
    }
}


/**
 * Methods and variable declarations for the SUBSCRIBER class
 **/

sub_id subscriber::usub_id;

void subscriber::simple_on_next(subscriber_event event){
    // TODO: define behavior for simple next handler
}

void subscriber::simple_on_error(std::exception e){
    // TODO: define behavior for simple error handler
}

void subscriber::simple_on_completed(){
    // TODO: define behavior for simple completed handler
    // behaviour should include removing the stream from the stream map
}

subscriber::subscriber(std::function<void(subscriber_event)> next,
                       std::function<void(std::exception)> error,
                       std::function<void()> completed) :
    on_next(next), 
    on_error(error), 
    on_completed(completed), 
    id(usub_id++) {}

subscriber::subscriber(const subscriber &sub) :
    on_next(sub.on_next), 
    on_error(sub.on_error), 
    on_completed(sub.on_completed) {}

/**
 * Methods and variable declarations for the SUBSCRIBER_POOL class
 */

// worker thread function for the subscriber_pool class
void subscriber_pool::handle_event(){
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

        // get event from queue and remove it from queue
        if(events.size() == 0)
            continue;

        struct event ev = events.front();
        events.pop_front();
        lk.unlock();

        // call the on_next function for the corresponding subscriber
        subscriber sub;
        while(!sub_lock.try_lock());
        if(subscribers.count(ev.s_id) == 1)
            sub = subscribers.at(ev.s_id);
        sub_lock.unlock();

        if(sub.on_error != nullptr)
            sub.on_next(ev.s_event);
    }
}


void subscriber_pool::notify(sub_id id, subscriber_event event){
    // create an event to add to the queue
    struct event ev(id, event);

    // lock the queue and add the event to it
    std::unique_lock<std::mutex> lk(e_lock);
    events.push_back(ev);
    lk.unlock();

    // wake up any worker threads
    e_cond.notify_all();
}

void subscriber_pool::notify_stream(stream_id id, subscriber_event event) {
    
}

void subscriber_pool::error(sub_id id, std::exception e){
    subscriber sub;
    while(!sub_lock.try_lock());
    if(subscribers.count(id) == 1)
        sub = subscribers.at(id);
    sub_lock.unlock();
    if(sub.on_error != nullptr)
        sub.on_error(e);
}


void subscriber_pool::complete(sub_id id){
    // lock the subscriber pool
    while(!e_lock.try_lock());
    while(!sub_lock.try_lock());

    // get and delete the subscriber
    subscriber sub = subscribers.at(id);
    subscribers.erase(id);

    // unlock the subscriber pool
    sub_lock.unlock();
    e_lock.unlock();
    // call the subscribers on_complete function
    sub.on_completed();
}


sub_id subscriber_pool::register_subscriber(subscriber sub) {
    
    // lock the subscriber pool
    while(!sub_lock.try_lock());
    
    // add the subscriber to the map
    subscribers.insert(std::pair<sub_id, subscriber&>(id, sub));
    
    // unlock the subscriber pool
    sub_lock.unlock();

    // increment and return the counter
    return id++;
}


subscriber_pool::subscriber_pool(int concurrency){
    // never make more than 16 threads
    int max = concurrency > 16 ? 16 : concurrency;

    // create threads based on the concurrency requested
    for(int i = 0; i < max; i++){
        pool.push_back(std::thread(&subscriber_pool::handle_event, this));
    }
}


subscriber_pool::~subscriber_pool(){
    // set the end flag
    std::unique_lock<std::mutex> lk(e_lock);
    end = true;
    lk.unlock();

    // join all worker threads
    for(auto& t : pool){
        e_cond.notify_all();
        t.join();
    }
}


/*
 * TESTING
 */
void func1(subscriber_event event){
    using namespace std;
    cout << "func 1" << endl;
}

void func2(subscriber_event event){
    using namespace std;
    cout << "func 2" << endl;
}

void func3(subscriber_event event){
    using namespace std;
    cout << "func 3" << endl;
}

void func4(subscriber_event event){
    using namespace std;
    cout << "func 4" << endl;
}

int main(void){
    using namespace std;
    subscriber_pool pool(2);
    subscriber s1(func1);
    subscriber s2(func2);
    subscriber s3(func3);
    subscriber s4(func4);
    pool.register_subscriber(subscriber(func1));
    pool.register_subscriber(s2);
    pool.register_subscriber(s3);
    pool.register_subscriber(s4);
    pool.notify(0,1);
    pool.notify(0,1);
    pool.notify(0,1);
    pool.notify(0,1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for(int i = 0; i < 4; i++){
        pool.complete(i);
    }
    return 0;
}