#include "stream.h"
#include "event.h"
#include "subscriber.h"

/* BUGS:
1. stream1 defined before stream2. they share same pool. stream2 starts, then stream1 starts. stream1 and 2 both fire events, but only stream2's subscribers get called, even for stream1's events.
2. stream1 defined before stream2. they have different pools. stream2 starts, then stream1 starts. stream2's events fire and get handled by its subscribers. when stream1 tries to start, program crashes w/ "libc++abi.dylib: terminating with uncaught exception of type std::out_of_range: map::at:  key not found
Abort trap: 6"
3. stream1 is defined and some subscribers are added to it, then stream2 is defined and some subscribers are added. they share a pool. then stream1 starts. it fires stream1's events, but calling stream2's handlers.
4. same as #3 but they use different pools. program will crash with "libc++abi.dylib: terminating with uncaught exception of type std::out_of_range: map::at:  key not found"
*/

void greater_than_3(event<int> event){
    using namespace std;
    if (event.get_data() > 3) {
            cout << "The number you entered is greater than 3." << endl;
    };
}

void you_entered_int(event<int> event){
    using namespace std;
    cout << "you entered " << event.get_data() << endl;
}

void you_entered_int2(event<int> event){
    using namespace std;
    cout << "2 you entered " << event.get_data() << endl;
}

void func3(event<int> event){
    using namespace std;
    cout << "func 3" << endl;
}

void func4(event<int> event){
    using namespace std;
    cout << "func 4" << endl;
}
/*
event<int> map_func1(event<int> event) {
    event<int> e = 
    return event<int>(3*event.get_data());
}*/

int main(void){
    using namespace std;

    subscriber_pool<int> pool;

    std::function<void(stream<int> my_stream)> my_on_subscribe = [](stream<int> my_stream) {
        for (int i = 1; i < 7; i ++) {
           // std::cout << "1" << endl;
            event<int> e(i);
            my_stream.change();
            my_stream.notify_children(e);
            my_stream.st_notify_subscribers(e);
            usleep(100);

         }
    };

    stream<int> int_stream = stream<int>::create(my_on_subscribe, &pool);
    int_stream.julie_id = 0;
    cout << "stream 1 id = " << int_stream.julie_id << endl;

    subscriber<int> s1(greater_than_3); 
    cout << "s1 id = " << s1.id << endl;

    subscriber<int> s2(you_entered_int);
    cout << "s2 id = " << s2.id << endl;


    int_stream.st_register_subscriber(s1); 
    int_stream.st_register_subscriber(s2); 


//    int_stream.start();


    /////////////////////

     //   subscriber_pool<int> pool3;


    stream<int> mapped_stream(&pool);
    mapped_stream.julie_id = 1;
    cout << "stream 2 id = " << mapped_stream.julie_id << endl;
    std::function<event<int>(event<int>)> map_func = [](event<int>) -> event<int> {
        std::cout << "calling map_func " << endl;
        return event<int>(1);

    };

    mapped_stream.map_func = map_func;

    int_stream.children.push_back(mapped_stream);

   // subscriber<int> s3(you_entered_int2);
    //subscriber<int> s4(func4);
    subscriber<int> s3(NULL);
    s3.julie_on_next = you_entered_int2;
    cout << "s3 id = " << s3.id << endl;


    mapped_stream.st_register_subscriber(s3);
//    mapped_stream.st_register_subscriber(s4);

    //s3.on_next(1);
    //s4.on_next(2);




    int_stream.start();


   // mapped_stream.



    //std::function<event<InputType>(event<InputType>)> 

    /*
    subscriber_pool<int> pool2;

    std::function<void(stream<int> my_stream)> my_on_subscribe2 = [](stream<int> my_stream) {
        for (int i = 2; i < 5; i ++) {
           // std::cout << "2" << endl;
            event<int> e(i);
            my_stream.change();
            my_stream.notify_subscribers(e);
            usleep(100);

         }
    };

    stream<int> int_stream2 = stream<int>::create(my_on_subscribe2, &pool2);

    subscriber<int> s3(func3);
    subscriber<int> s4(func4);

    int_stream2.register_subscriber({s3, s4});
    int_stream2.start();*/

    //////////////////////////////




/*
    while(true) {
        string keyinput;
        cin >> keyinput;
        event<string> my_event(keyinput);
        keyboard_stream.change();
        keyboard_stream.notify_subscribers(my_event);
    }*/

    //int_stream->start();

/*} catch(std::exception &e){
    std::cout << e.what() << std::endl;
}*/
	
    return 0;
}
