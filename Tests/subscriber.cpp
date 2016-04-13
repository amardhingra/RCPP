#include "subscriber.h"

#include <iostream>
#include <chrono>


/*
 * copy constructor for subscriber_event
 */
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


// worker thread function
void subscriber_pool::handle_event(){
    while(true){
        // acquire lock
        std::unique_lock<std::mutex> lk(lock);
        
        // wait until told to exit or while queue is empty
        while(!end && events.size() == 0){
            cond.wait(lk);

            // exit on flag
            if(end){
                std::cout << "thread exiting" << std::endl;
                lk.unlock();
                return;
            }
        }

        // get event from queue and remove it from queue
        struct event ev = events.front();
        events.pop_front();
        lk.unlock();

        // call the on_next function for the corresponding subscriber
        subscribers.at(ev.s_id).on_next(ev.s_event);
    }
}


void subscriber_pool::notify(sub_id id, subscriber_event event){
    // create an event to add to the queue
    struct event ev(id, event);

    // lock the queue and add the event to it
    std::unique_lock<std::mutex> lk(lock);
    events.push_back(ev);
    lk.unlock();

    // wake up any worker threads
    cond.notify_all();
}


sub_id subscriber_pool::register_subscriber(subscriber &sub) {
    // add the subscriber to the map
    subscribers.insert(std::pair<sub_id, subscriber&>(id, sub));
    
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
    std::unique_lock<std::mutex> lk(lock);
    end = true;
    lk.unlock();

    // wake up all worker threads
    cond.notify_all();

    // join all worker threads
    for(auto& t : pool){
        t.join();
    }
}


/*
 * TESTING
 */
void func1(subscriber_event event){
    using namespace std;
    std::this_thread::sleep_for (std::chrono::seconds(1));
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
    subscriber_pool pool(4);
    subscriber s1(func1);
    subscriber s2(func2);
    subscriber s3(func3);
    subscriber s4(func4);
    pool.register_subscriber(s1);
    pool.register_subscriber(s2);
    pool.register_subscriber(s3);
    pool.register_subscriber(s4);
    pool.notify(0,1);
    pool.notify(0,1);
    pool.notify(0,1);
    pool.notify(0,1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}