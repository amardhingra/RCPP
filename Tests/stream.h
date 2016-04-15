#include "subscriber.h"

/*
   A stream emits data for subscribers to the stream to handle.
   A stream can have many subscribers.
   */
class stream {

    bool changed = false;
    // The threadpool that is assigned to this stream. If none are specified then it gets initialized??
    //seems like there would be ownership problems with this model
    subscriber_pool *threadpool; 

    private:
    void setChanged();

    void clearChanged();

    bool hasChanged();


    public:
    stream(); //default constructor, will eventually take from a source. 
    stream(subscriber_pool some_pool); //you can specify a specific subscriber_pool 

    // Factory method: returns a stream from keyboard input
    static stream *stream_from_keyboard_input();

    // A function that gets events and notifies subscribers accordingly
    std::function<void()> get_events_from_source;

    

    // Add a new subscriber to this stream
    void register_subscriber(subscriber new_subscriber);
    void register_subscriber(std::vector<subscriber> subscribers);

    // If the stream has changed, then notify all subscribers and clear the "changed" variable to indicate that the stream has no longer changed
    void notifySubscribers(event new_event);

    // Starts the stream;
    void start();

    // Test function that 
    void get_keyboard_input(); 

};