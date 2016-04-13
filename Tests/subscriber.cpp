#include "subscriber.h"

#include <iostream>
#include <chrono>

subscriber_event::subscriber_event(const subscriber_event& event){
    type = event.type;
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

void
subscriber_pool::handle_event(){
    while(true){
        std::unique_lock<std::mutex> lk(lock);
        while(!end && events.size() == 0){
            cond.wait(lk);
            if(end){
                std::cout << "thread exiting" << std::endl;
                lk.unlock();
                return;
            }
        }
        struct event ev = events.front();
        events.pop_front();
        lk.unlock();

        subscribers.at(ev.s_id).on_next(ev.s_event);
    }
}

void
subscriber_pool::notify(sub_id id, subscriber_event event){
    struct event ev(id, event);
    std::unique_lock<std::mutex> lk(lock);
    events.push_back(ev);
    lk.unlock();
    cond.notify_all();
}

sub_id
subscriber_pool::register_subscriber(subscriber &sub)
{
    subscribers.insert(std::pair<sub_id, subscriber&>(id, sub));
    return id++;
}

subscriber_pool::subscriber_pool(int concurrency){
    for(int i = 0; i < concurrency; i++){
        pool.push_back(std::thread(&subscriber_pool::handle_event, this));
        //std::thread t(&subscriber_pool::handle_event, this);
        //t.detach();
    }
}

subscriber_pool::~subscriber_pool(){
    std::unique_lock<std::mutex> lk(lock);
    end = true;
    cond.notify_all();
    lk.unlock();
    for(auto& t : pool){
        t.join();
    }
}

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