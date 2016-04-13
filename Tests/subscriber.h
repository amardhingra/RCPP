#ifndef INCLUDED_SUBSCRIBER
#define INCLUDED_SUBSCRIBER

#include <functional>
#include <string>


class subscriber_pool {

public:
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
    
        message_type get_type() { return type; }
        bool         get_b() { return data.b; }
        int          get_i() { return data.i; }
        float        get_f() { return data.f; }
        std::string  get_s() { return data.s; }
    };

public:
    class subscriber {
    public:
        std::function<void(subscriber_pool::subscriber_event)> on_next;
        std::function<void(std::exception)> on_error;
        std::function<void()> on_completed;


    public:
        static void simple_on_error(std::exception e){
            // TODO: define behavior for simple error handler
        }

        static void simple_on_completed(){
            // TODO: define behavior for simple completed handler
        }

    public:
        subscriber(
            std::function<void(subscriber_pool::subscriber_event)> next,
            std::function<void(std::exception)> error = simple_on_error,
            std::function<void()> completed = simple_on_completed) :
        on_next(next), on_error(error), on_completed(completed) {};

    };
};

#endif