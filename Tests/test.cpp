#include "stream.h"
#include "event.h"
#include "subscriber.h"

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

void func3(event<std::string> event){
    using namespace std;
    cout << "func 3" << endl;
}

void func4(event<std::string> event){
    using namespace std;
    cout << "func 4" << endl;
}

int main(void){
    using namespace std;

    subscriber_pool<int> pool;
    subscriber_pool<int>* pool_ptr = &pool;

    std::function<void(stream<int> my_stream)> my_on_subscribe = [](stream<int> my_stream) {
        for (int i = 1; i < 7; i ++) {
            //event<std::string> e = event<std::string>(std::to_string(i));
            event<int> e(i);
            my_stream.change();
            my_stream.notify_subscribers(e);
            usleep(100);

         }
    };

    stream<int> *int_stream = stream<int>::create(my_on_subscribe, pool_ptr);
    //keyboard_stream.on_subscribe = my_on_subscribe;

    subscriber<int> s1(greater_than_3); //subscribers specify the streams they subscribe to, get assigned unique id in that stream 
    subscriber<int> s2(you_entered_int);
   /* subscriber<string> s3(func3);
    subscriber<string> s4(func4);*/

    vector<subscriber<int>> slist = {s1, s2};   

      //  keyboard_stream->register_subscriber(s1); 

    int_stream->register_subscriber(slist); 

    
    //keyboard_stream.register_subscriber(slist); 

/*
    while(true) {
        string keyinput;
        cin >> keyinput;
        event<string> my_event(keyinput);
        keyboard_stream.change();
        keyboard_stream.notify_subscribers(my_event);
    }*/
/*
    for (int i = 101; i < 105; i ++) {
        event<std::string> e = event<std::string>(std::to_string(i));
        keyboard_stream.change();
        keyboard_stream.notify_subscribers(e);
        usleep(100);

     }*/
    int_stream->start();
/*} catch(std::exception &e){
    std::cout << e.what() << std::endl;
}*/
	

    return 0;
}
