#include "stream.h"
#include "event.h"
#include "subscriber.h"

/* BUGS:
1. stream1 defined before stream2. they share same pool. stream2 starts, then stream1 starts. stream1 and 2 both fire events, but only stream2's subscribers get called, even for stream1's events.
2. stream1 defined before stream2. they have different pools. stream2 starts, then stream1 starts. stream2's events fire and get handled by its subscribers. when stream1 tries to start, program crashes w/ "libc++abi.dylib: terminating with uncaught exception of type std::out_of_range: map::at:  key not found
Abort trap: 6"
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

void func3(event<int> event){
    using namespace std;
    cout << "func 3" << endl;
}

void func4(event<int> event){
    using namespace std;
    cout << "func 4" << endl;
}

int main(void){
    using namespace std;

    subscriber_pool<int> pool;

    std::function<void(stream<int> my_stream)> my_on_subscribe = [](stream<int> my_stream) {
        for (int i = 1; i < 7; i ++) {
           // std::cout << "1" << endl;
            event<int> e(i);
            my_stream.change();
            my_stream.notify_subscribers(e);
            usleep(100);

         }
    };

    stream<int> int_stream = stream<int>::create(my_on_subscribe, &pool);

    subscriber<int> s1(greater_than_3); 
    subscriber<int> s2(you_entered_int);

    int_stream.register_subscriber({s1, s2}); 

    int_stream.start();


    /////////////////////

    stream<int> mapped_stream(&pool);

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
