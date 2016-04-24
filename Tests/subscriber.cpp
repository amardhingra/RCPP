#include "subscriber.h"

#include <iostream>
#include <chrono>


/**
 * Methods and variable declarations for the SUBSCRIBER class
 **/
template <class T>
sub_id subscriber<T>::usub_id;

template <class T>
void subscriber<T>::simple_on_next(event<T> n) {
    // TODO: define behavior for simple next handler
}

template <class T>
void subscriber<T>::simple_on_error(std::exception e){
    // TODO: define behavior for simple error handler
}

template <class T>
void subscriber<T>::simple_on_completed(){
    // TODO: define behavior for simple completed handler
    // behaviour should include removing the stream from the stream map
}

template <class T>
subscriber<T>::subscriber(std::function<void(event<T>)> next,
                       std::function<void(std::exception)> error,
                       std::function<void()> completed) :
    on_next(next), 
    on_error(error), 
    on_completed(completed), 
    id(usub_id++) {}

template <class T>
subscriber<T>::subscriber(const subscriber<T> &sub) :
    on_next(sub.on_next), 
    on_error(sub.on_error), 
    on_completed(sub.on_completed),
    id(usub_id++) {}

template <class T>
subscriber<T>& subscriber<T>::operator=(subscriber<T> &sub){
    return sub;
}

// move constructor
template <class T>
subscriber<T>::subscriber(subscriber<T> &&sub) :
    on_next(sub.on_next), 
    on_error(sub.on_error), 
    on_completed(sub.on_completed),
    id(usub_id) {

        sub.on_next = nullptr;
        sub.on_error = nullptr;
        sub.on_completed = nullptr;
        sub.id = 0;
}


/**
 * Methods and variable declarations for the SUBSCRIBER_POOL class
 */

// worker thread function for the subscriber_pool class
template <class T>
void subscriber_pool<T>::handle_event(){
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
}

template <class T>
void subscriber_pool<T>::notify(sub_id id, event<T> event){

    // create an event to add to the queue
    struct queue_event ev(id, event);

    // lock the queue and add the event to it
    std::unique_lock<std::mutex> lk(e_lock);
    events.push_back(ev);
    lk.unlock();

    // wake up any worker threads
    e_cond.notify_all();
}

template <class T>
void subscriber_pool<T>::notify_all(std::vector<sub_id>& ids, event<T> event){
    for(auto id : ids){
        notify(id, event);
    }
}

template <class T>
void subscriber_pool<T>::notify_stream(stream_id str_id, event<T> event){
    for(auto id : stream_subs.at(str_id)){
        notify(id, event);
    }
}

template <class T>
void subscriber_pool<T>::error(sub_id id, std::exception e){
    while(!sub_lock.try_lock());
    if(subscribers.count(id) == 0){
        sub_lock.unlock();
        return;
    }
    subscriber<T> sub = subscribers.at(id);
    sub_lock.unlock();
    sub.on_error(e);
}

template <class T>
void subscriber_pool<T>::complete(sub_id id){
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

}

template <class T>
void subscriber_pool<T>::grow(){
    auto size = subscribers.size();
    if(pool.size() < MAX_SUB_THREADS && ((float) subscribers.size()) / pool.size() >= 4){
        pool.push_back(std::thread(&subscriber_pool<T>::handle_event, this));
    }
}

template <class T>
void subscriber_pool<T>::register_subscriber(subscriber<T> sub) {
    
    // lock the subscriber pool
    while(!sub_lock.try_lock());
    
    // OLD: add the subscriber to the map
    subscribers.insert(std::pair<sub_id, subscriber<T>&>(id, sub));
    
    // unlock the subscriber pool
    sub_lock.unlock();

    grow();
}

template <class T>
void subscriber_pool<T>::register_subscriber(subscriber<T> sub, stream_id id){

    // lock the subscriber pool
    while(!sub_lock.try_lock());
    
    // add a stream_id mapping
    subscribers.insert(std::pair<sub_id, subscriber<T>&>(sub.id, sub));
    stream_subs[id].push_back(sub.id);

    // unlock the subscriber pool
    sub_lock.unlock();

    grow();

}

template <class T>
subscriber_pool<T>::subscriber_pool(int concurrency){
    // never make more than 16 threads
    int max = concurrency > MAX_SUB_THREADS ? MAX_SUB_THREADS : concurrency;

    // create threads based on the concurrency requested
    for(int i = 0; i < max; i++){
        pool.push_back(std::thread(&subscriber_pool<T>::handle_event, this));
    }
}

template <class T>
subscriber_pool<T>::~subscriber_pool(){
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

void func1(event<int> event){
    using namespace std;
    cout << "func 1" << endl;
}

void func2(event<int> event){
    using namespace std;
    cout << "func 2" << endl;
}

void func3(event<int> event){
    using namespace std;
    cout << "func 3" << endl;
}

void func4(event<int> event){
    using namespace std;
    cout << "func 4" << endl;
}

#ifndef MAIN
int main(void){
    using namespace std;
    subscriber_pool<int> pool;
    pool.register_subscriber(subscriber<int>(func1), 0);
    pool.register_subscriber(subscriber<int>(func2), 0);
    pool.register_subscriber(subscriber<int>(func3), 1);
    pool.register_subscriber(subscriber<int>(func4), 0);
    pool.register_subscriber(subscriber<int>(func4), 0);
    std::vector<sub_id> subs{0, 1, 2};
    pool.notify_all(subs, event<int>::event(1));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for(int i = 0; i < 4; i++){
        pool.complete(i);
    }
    return 0;
}
#endif