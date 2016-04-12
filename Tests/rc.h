using namespace std;


// Assumptions: streams are immutable, but can be copied
// Putting all classes in this one file for testing purposes only

/*
   A stream consists of events, which contain data. 
   For the purposes of testing, currently an event just has a string as its data.
   */
class event {
    public:
        string data;
        event(string);
};



/* 
   A subscriber listens to one or more streams.
   It has 3 handlers for dealing with data received from the stream: one for handling normal events, one for handling errors, and one for handling the completion event.
   */
class subscriber {
    public:
        // Provides the subscriber with a new event to handle. 
        // Called by the stream when it has a new event for the subscriber.
        std::function<void(event new_event)> on_next;

        // Notifies the subscriber that the stream has experienced an error. The error is passed by the stream to the subscriber to handle.
        std::function<void(exception& new_exception)> on_error;

        // Notifies the subscriber that the stream has finished sending events.
        std::function<void()> on_completed;
};

/*
   A stream emits data for subscribers to the stream to handle.
   A stream can have many subscribers.
   */
class stream {

    bool changed = false;
    // List of subscribers that are subscribed to this stream.
    vector<subscriber> my_subscribers;

    public:

    // Factory method: returns a stream from keyboard input
    static stream *stream_from_keyboard_input();

    // A function that gets events and notifies subscribers accordingly
    std::function<void()> get_events_from_source;

    void setChanged();

    void clearChanged();

    bool hasChanged();

    // Add a new subscriber to this stream
    void subscribe(subscriber new_subscriber);

    // If the stream has changed, then notify all subscribers and clear the "changed" variable to indicate that the stream has no longer changed
    void notifySubscribers(event new_event);

    // Starts the stream; will continuously call get_events_from_source.
    void start();

    // Test function that 
    void get_keyboard_input(); 

};

