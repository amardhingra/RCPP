#ifndef INCLUDED_EVENT 
#define INCLUDED_EVENT

template <class T>

class event {

private:
    T _data;


public:
    // constructors for supported types
    // set the correct member of the enum and assign the correct selector
    event(T data): _data (data) {};

    event(const event &event): _data (event._data) {};

public:
    // accessors
   
    const T& get_data() { return _data; }
    // int          get_i() { return data.i; }
    // float        get_f() { return data.f; }
    // std::string  get_s() { return data.s; }

}; // end subscriber_event

#endif