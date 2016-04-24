#include "subscriber.h"

#include <iostream>
#include <chrono>

/**
 * Methods and variable declarations for the SUBSCRIBER_EVENT class
 **/




/**
 * Methods and variable declarations for the SUBSCRIBER class
 **/
// template <class T>
// sub_id subscriber<T>::usub_id;

// template <class T>
// void subscriber<T>::simple_on_next(event<T> n) {
//     // TODO: define behavior for simple next handler
// }

// template <class T>
// void subscriber<T>::simple_on_error(std::exception e){
//     // TODO: define behavior for simple error handler
// }

// template <class T>
// void subscriber<T>::simple_on_completed(){
//     // TODO: define behavior for simple completed handler
//     // behaviour should include removing the stream from the stream map
// }

// template <class T>
// subscriber<T>::subscriber(std::function<void(event<T>)> next,
//                        std::function<void(std::exception)> error,
//                        std::function<void()> completed)

// template <class T>
// subscriber<T>::subscriber(const subscriber<T> &sub)

// template <class T>
// subscriber<T>& subscriber<T>::operator=(subscriber<T> &sub)

// // move constructor
// template <class T>
// subscriber<T>::subscriber(subscriber<T> &&sub)


// /**
//  * Methods and variable declarations for the SUBSCRIBER_POOL class
//  */

// // worker thread function for the subscriber_pool class
// template <class T>
// void subscriber_pool<T>::handle_event()

// template <class T>
// void subscriber_pool<T>::notify(sub_id id, event<T> event)

// template <class T>
// void subscriber_pool<T>::notify_all(std::vector<sub_id>& ids, event<T> event)

// template <class T>
// void subscriber_pool<T>::notify_stream(stream_id str_id, event<T> event)

// template <class T>
// void subscriber_pool<T>::error(sub_id id, std::exception e)

// template <class T>
// void subscriber_pool<T>::complete(sub_id id)

// template <class T>
// void subscriber_pool<T>::register_subscriber(subscriber<T> sub) 

// template <class T>
// void subscriber_pool<T>::register_subscriber(subscriber<T> sub, stream_id id)

// template <class T>
// subscriber_pool<T>::subscriber_pool(int concurrency)

// template <class T>
// subscriber_pool<T>::~subscriber_pool()

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
    subscriber_pool<int> pool(2);
    pool.register_subscriber(subscriber<int>(func1), 0);
    pool.register_subscriber(subscriber<int>(func2), 0);
    pool.register_subscriber(subscriber<int>(func3), 1);
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