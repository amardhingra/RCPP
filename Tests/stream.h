#ifndef INCLUDED_STREAM
#define INCLUDED_STREAM

#include <unistd.h>
#include <boost/thread.hpp>
#include <thread>
#include <functional>

#include "subscriber.h"
#include "event.h"

typedef unsigned long stream_id;
/*
 * A stream emits data for subscribers to the stream to handle.
 * A stream can have many subscribers.
 */


template <typename InputType, typename OutputType = InputType>
class stream {

private:

    static stream_id            unique_id;   // unique id generator for streams
    stream_id                   id;          // specific id for instance of stream
    subscriber_pool<InputType> *thread_pool; // subscriber pool to use for handling events
    bool                        owner;       // determines if the current object is the owner of the pool
    std::thread                 t;           // thread on which to run the on_start function
    //bool changed = false;


public:
    // default constructor
    stream() : 
        id(unique_id++),
        thread_pool(new subscriber_pool<InputType>),
        owner(true)
        {}; 

    // constructor with shared thread pool
    stream(subscriber_pool<InputType>* some_pool) :
        id(unique_id++),
        thread_pool(some_pool),
        owner(false)
        {};

    ~stream(){
        // wait for the stream to finish
        complete();

        // check if we are the owner of the thread and delete it
        if(owner && thread_pool != nullptr){
             delete thread_pool;
             thread_pool = nullptr;
        }
    }; 


    // Add a new subscriber to this stream
    void register_subscriber(subscriber<InputType> new_subscriber){
        thread_pool->register_subscriber(new_subscriber, id);
    };
    void register_subscriber(std::vector<subscriber<InputType>> new_subscribers){

        for (subscriber<InputType> new_subscriber : new_subscribers )
            thread_pool->register_subscriber(new_subscriber, id);
    };

    // If the stream has changed, then notify all subscribers
    //template<class T> 
        void notify_subscribers(event<InputType> new_event){
            thread_pool->notify_stream(id, new_event);
        };

        /* --------------JULIE'S MESS BELOW --------------- */

public:
    // Starts the stream;
        void start(){
            t = std::thread(std::bind(on_start, std::ref(*this)));
            //on_start(*this);
        };

        void complete(){
            t.join();
        }

    // create stream with pool and on_start
    // TODO: create version of this w/o pool arg once stream's default constructor gets fixed

        //you can specify a specific subscriber_pool
        stream(subscriber_pool<InputType>* some_pool, const std::function<void(stream<InputType> & my_stream)> & on_start) {
            owner = false;
            thread_pool = some_pool; 
            id++;
            this->on_start = on_start;
        };

        // disable copy constructor
        stream(const stream& stream_to_copy) = delete;

        // assignment operator just assigns the reference
        stream& operator=(stream &my_stream){
            std::cout << "stream: operator= called" << std::endl;
            return my_stream;
        };

        // move constructor
        stream(stream &&other) 
            : id(other.id)
            , thread_pool(other.thread_pool)
             
        {
            std::cout << "stream: move constructor called" << std::endl;
            //thread_pool = other.thread_pool;
            //id = other.id;
            other.thread_pool = nullptr;
            //other.id = 0;
        };

        std::function<void(stream<InputType> & my_stream)> on_start;

        // map 
        // TODO: should it return by reference instead of value?
        stream<InputType> map(std::function<event<InputType>(event<InputType>)> new_map_func) {
            stream<InputType> new_stream;
            new_stream.map_func = new_map_func;
            this->children.push_back(&new_stream);
            return new_stream;

        };



// functions/members prefixed with "st" are part of a single-threaded system that bypasses thread pool. Use these functions for debugging.

        std::vector<subscriber<InputType>> st_my_subscribers;

        void st_register_subscriber(const subscriber<InputType> & new_subscriber){
#ifdef DEBUG
            std::cout << "registering to stream " << this << ". # of subscribers = " << st_my_subscribers.size() << "\n";
#endif
            st_my_subscribers.push_back(new_subscriber);

#ifdef DEBUG
            std::cout << "finished registering to stream " << this << ". # of subscribers = " << st_my_subscribers.size() << "\n";

#endif
        };


        void st_notify_subscribers(event<InputType> new_event) {
#ifdef DEBUG

            std::cout << "stream " << this <<  " notifying " <<st_my_subscribers.size() << " subscribers" << std::endl;
#endif


            for (subscriber<InputType> my_subscriber : st_my_subscribers) {
#ifdef DEBUG

                std::cout << "stream " << this <<  " trying to notify subscriber " << my_subscriber.id << std::endl;
#endif

                my_subscriber.on_next(new_event);

            }

        }

        void print_subscribers() {
            std::cout << "subscribers: ";
            for (auto my_subscriber : st_my_subscribers) {
                std::cout << my_subscriber.id << " ";
            }
            std::cout << std::endl;
        }

        std::vector<stream<InputType> *> children;

        std::function<event<InputType>(event<InputType>)> map_func;


        void notify_children(event<InputType> e) {
            for (stream<InputType> *child_stream : children) {

                child_stream->on_receive_event_from_parent(e);
            }
        }

        void st_notify_children(event<InputType> e) {
            for (stream<InputType> *child_stream : children) {
#ifdef DEBUG

                std::cout << "notifying child stream " <<  child_stream << std::endl;
#endif
                child_stream->st_on_receive_event_from_parent(e);
            }
        }

        void on_receive_event_from_parent(event<InputType> e) {
#ifdef DEBUG
            std::cout << "stream " << this << " received event" << std::endl;
#endif
            if (map_func != NULL) {
                e = map_func(e);
            }
#ifdef DEBUG
            usleep(100);
#endif
            notify_subscribers(e);
        };

        void st_on_receive_event_from_parent(event<InputType> e) {
#ifdef DEBUG
            std::cout << "stream " << this << " received event" << std::endl;
#endif

            if (map_func != NULL) {
                event<InputType> new_e = map_func(e);
                st_notify_subscribers(new_e);
            }
            else {
                st_notify_subscribers(e);

            }

        };

    };

template <typename InputType, typename OutputType>
    stream_id stream<InputType, OutputType>::unique_id;

#endif
