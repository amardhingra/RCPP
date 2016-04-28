#ifndef INCLUDED_STREAM
#define INCLUDED_STREAM

#include <unistd.h>
#include <boost/thread.hpp>
#include <thread>
#include <functional>
#include <memory>
#include <chrono>

#include "subscriber.h"
#include "event.h"


typedef unsigned long stream_id;


/*
 * A stream emits data for subscribers to the stream to handle.
 * A stream can have many subscribers.
 */
template <typename InputType, typename OutputType = InputType>
class stream {

/* --------------------- STREAM MEMBER VARIABLES -------------------*/
private:

    static stream_id                             unique_id;   // unique id generator for streams
    stream_id                                    id;          // specific id for instance of stream
    std::shared_ptr<subscriber_pool<InputType>>  thread_pool; // subscriber pool to use for handling events
    std::thread                                  t;           // thread on which to run the on_start function
    std::function<void(stream<InputType> & my_stream)> on_start; // function to be called to generate data
    bool                                         is_throttled_stream = false; //are we a throttled stream?
    std::chrono::milliseconds                    last_emitted_time;
    std::chrono::milliseconds                    throttle_rate;                   

/* ---------------- STREAM CONSTRUCTORS AND OPERATORS --------------*/

public:

    // constructor with shared thread pool
    stream(std::shared_ptr<subscriber_pool<InputType>> some_pool = std::shared_ptr<subscriber_pool<InputType>>(new subscriber_pool<InputType>),
        const std::function<void(stream<InputType> & my_stream)> & on_start = nullptr) :
        id(unique_id++),
        thread_pool(some_pool),
        on_start(on_start),
        last_emitted_time(0),
        throttle_rate(0)
        {
            //std::cout << "constructor 1 called" << std::endl;
        };

    // constructor with shared thread pool
    stream(const std::function<void(stream<InputType> & my_stream)> & on_start,
        std::shared_ptr<subscriber_pool<InputType>> some_pool = std::shared_ptr<subscriber_pool<InputType>>(new subscriber_pool<InputType>)) :
        id(unique_id++),
        thread_pool(some_pool),
        on_start(on_start),
        last_emitted_time(0),
        throttle_rate(0)
        {
            //std::cout << "constructor 2 called" << std::endl;
        };
        
    // 
    ~stream(){
        // wait for the stream to finish
        complete();
    };

    // disable copy constructor
    stream(const stream& stream_to_copy) = delete;

    // move constructor
    stream(stream &&other) : 
        id(other.id),
        thread_pool(other.thread_pool),
        on_start(other.on_start),
        last_emitted_time(0),
        throttle_rate(0){
        
        //#ifdef DEBUG
        //std::cout << "stream: move constructor called" << std::endl;
        //#endif
        other.on_start = nullptr;
        other.thread_pool = nullptr;
    };

/* ---------------- CREATING NEW STREAMS FROM EXISTING STREAMS --------------*/


    // assignment operator just assigns the reference
    stream& operator=(stream &my_stream){
        
        //#ifdef 
        //std::cout << "stream: operator= called" << std::endl;
        //#endif
        
        return my_stream;
    };

    // move assignment operator
    stream& operator=(stream&& other){
        if (this != &other) {
            // Free the existing resource.
            delete thread_pool;

            // Copy the thread_pool pointer and its id from the source object.
            thread_pool = other.thread_pool;
            id = other.id;
            on_start = other.on_start;
            last_emitted_time = 0;
            throttle_rate = 0;
            
            
            std::cout << &thread_pool << std::endl;

             // Release the thread_pool pointer from the source object so that
            // the destructor does not free the memory multiple times.
            other.thread_pool = nullptr;
            other.on_start = nullptr;
        }
        return *this;
    };

    friend stream<InputType, OutputType> operator+(
                                    stream<InputType, OutputType>& l_stream,
                                    stream<InputType, OutputType>& r_stream){

        stream<InputType, OutputType> merged_stream;

        l_stream.register_subscriber(subscriber<InputType>([&merged_stream](event<InputType> e){
            merged_stream.notify(e);
        }));

        r_stream.register_subscriber(subscriber<InputType>([&merged_stream](event<InputType> e){
            merged_stream.notify(e);
        }));
        return merged_stream;
    }

    void operator+=(stream<InputType, OutputType>& r_stream){

        r_stream.register_subscriber(subscriber<InputType>([this](event<InputType> e){
            this->notify(e);
        }));

    }


/* --------------------- SUBSCRIBER FUNCTIONS -------------------*/
public:        


    // Add a new subscriber to this stream
    void register_subscriber(subscriber<InputType> new_subscriber){
        thread_pool->register_subscriber(new_subscriber, id);
    };

    // Add a list of subscribers to this stream
    void register_subscribers(std::vector<subscriber<InputType>> new_subscribers){
        for (subscriber<InputType> new_subscriber : new_subscribers){
            thread_pool->register_subscriber(new_subscriber, id);
        }
    };


    /*
     * Function that is used to notify the stream of new incoming events
     * the stream will notify the appropriate subscribers and invoke the 
     * provided filter, map and reduce operations
     */
    void notify(event<InputType> new_event){
        using namespace std::chrono;

        if(!is_throttled_stream){
            thread_pool->notify_stream(id, new_event);
        }

        milliseconds now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        std::cout << now;
        std::cout << id << ' ' << (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_emitted_time)).count() << ' ' << throttle_rate.count() << std::endl;
        if(std::chrono::duration_cast<std::chrono::milliseconds>(now - last_emitted_time) >= throttle_rate){
            last_emitted_time = now;
            thread_pool->notify_stream(id, new_event);
        }
    };

    /*
     * Function that is called to notify the streams subscribers that the
     * stream has encountered an error that they need to process
     */
    void error(std::exception e){
        thread_pool->error_stream(id, e);
    }

    /*
     * Function that is called to notify the streams subscribers that the
     * stream has ended and for them to do any cleanup required
     */
    void end_stream(){
        thread_pool->complete_stream(id);
    }


/* ---------------- FUNCTIONS TO START AND STOP STREAM --------------*/

public:
    // Starts the stream
    void start(){
        if(on_start != nullptr){
            t = std::thread(std::bind(on_start, std::ref(*this)));
        }
    };

    void complete(){
        if(t.joinable()){
            t.join();
        }
    };

/* -------------------------- FILTER/MAP/REDUCE ------------------------*/

public:


    stream<InputType> filter(std::function<bool(InputType)> filter_func){
        stream<InputType> filtered_stream(thread_pool);
  
        subscriber<InputType> filtered_stream_feeder([&filtered_stream, filter_func](event<InputType> e)
        {
            if(filter_func(e.get_data())){
                filtered_stream.notify(e);
            }
        });
  
        this->register_subscriber(filtered_stream_feeder);
        return filtered_stream;      
    };

    stream<OutputType> map(std::function<OutputType(InputType)> map_func){
         stream<OutputType> mapped_stream(thread_pool);
  
         subscriber<InputType> mapped_stream_feeder([&mapped_stream, map_func](event<InputType> e)
         {
             mapped_stream.notify(event<OutputType>(map_func(e.get_data())));
         });
  
         this->register_subscriber(mapped_stream_feeder);
         return mapped_stream;
    };


    stream<InputType> reduce(std::function<event<InputType>(event<InputType>)> reduce_func){
        // TODO
    };

    stream<InputType> throttle(unsigned int milliseconds){
        std::chrono::milliseconds time(milliseconds);
        stream<OutputType> throttled_stream(thread_pool);
        throttled_stream.set_throttle_rate(time);
  
        subscriber<InputType> throttled_stream_feeder([&throttled_stream](event<InputType> e)
        {
            throttled_stream.notify(event<OutputType>(e));
        });
  
        this->register_subscriber(throttled_stream_feeder);
        return throttled_stream;
    };

/* ---------------------------------- HELPER -----------------------------------*/

public: 

    void set_throttle_rate(std::chrono::milliseconds time){
        is_throttled_stream = true;
        throttle_rate = time;
    }

// functions/members prefixed with "st" are part of a single-threaded system that bypasses thread pool. Use these functions for debugging.
/* -------------------------- SINGLE THREADED FUNCTIONS ------------------------*/

public:
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
    };

    void print_subscribers() {
        std::cout << "subscribers: ";
        for (auto my_subscriber : st_my_subscribers) {
            std::cout << my_subscriber.id << " ";
        }
        std::cout << std::endl;
    };

};

template <typename InputType, typename OutputType>
    stream_id stream<InputType, OutputType>::unique_id;

#endif
