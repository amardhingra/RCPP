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

class subscriber_event {
// structures for storing the event
public:
    enum message_type {
        BOOL  = 0,
        INT   = 1,
        FLOAT = 2,
        STR   = 3
    };
private:
    union message_data {
        bool        b;
        int         i;
        float       f;
        std::string s;
    public:
        message_data() {};
        message_data(bool _b) : b(_b) {};
        message_data(int _i) : i(_i) {};
        message_data(float _f) : f(_f) {};
        message_data(std::string _s) : s(_s) {};
        ~message_data(){};
    };

// data in the message
private:
    message_type type;
    message_data data;

// contructors and data accessor methods
public:
    subscriber_event(bool _b) : type(message_type::BOOL), data(_b) {};
    subscriber_event(int _i) : type(message_type::INT), data(_i) {};
    subscriber_event(float _f) : type(message_type::FLOAT), data(_f) {};
    subscriber_event(std::string _s) : type(message_type::STR), data(_s) {};
    subscriber_event(const subscriber_event &event);
public:
    message_type get_type() { return type; }
    bool         get_b() { return data.b; }
    int          get_i() { return data.i; }
    float        get_f() { return data.f; }
    std::string  get_s() { return data.s; }
};


class subscriber {
public:
    std::function<void(subscriber_event)> on_next;
    std::function<void(std::exception)> on_error;
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
    }

public:
    subscriber(
        std::function<void(subscriber_event)> next,
        std::function<void(std::exception)> error = simple_on_error,
        std::function<void()> completed = simple_on_completed) :
    on_next(next), on_error(error), on_completed(completed) {};

    subscriber(subscriber &sub) :
        on_next(sub.on_next), on_error(sub.on_error), on_completed(sub.on_completed) {};
};


class subscriber_pool {

private:
    class event {
    public:
        sub_id           s_id;
        subscriber_event s_event;
        event(sub_id _id, subscriber_event _event) : s_id(_id), s_event(_event) {};
    };
    bool                                   end = false;
    sub_id                                 id = 0;
    std::map<sub_id, subscriber&>          subscribers;
    std::list<event>                       events;
    std::mutex                             lock;
    std::condition_variable                cond;
    std::vector<std::thread>               pool;

    void handle_event();

public:
    subscriber_pool(int concurrency = 1);
    ~subscriber_pool();
    void notify(sub_id id, subscriber_event);
    sub_id register_subscriber(subscriber& sub);

};

#endif