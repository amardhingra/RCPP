#include "stream.h"
#include "event.h"
#include "subscriber.h"

void func1(event<std::string> event){
    using namespace std;
    if (event.get_data().size() > 2) {
            cout << "The word you entered is longer than 2 characters." << endl;
    };
}

void func2(event<std::string> event){
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

    subscriber_pool<string> pool;
    //stream *keyboard_stream = stream::stream_from_keyboard_input(pool);
    subscriber_pool<string>* pool_ptr = &pool;

    stream<string> keyboard_stream(pool_ptr);

    //stream<string> keyboard_stream;

    std::function<void(stream<std::string> my_stream)> my_on_subscribe = [](stream<std::string> my_stream) {
        for (int i = 101; i < 105; i ++) {
            event<std::string> e = event<std::string>(std::to_string(i));
            my_stream.change();
            my_stream.notify_subscribers(e);
            usleep(50);

         }
    };



    //stream<string> keyboard_stream = stream<string>::create(my_on_subscribe);
    keyboard_stream.on_subscribe = my_on_subscribe;

    //stream<string> *keyboard_stream = stream<string>::create(my_on_subscribe);


    subscriber<string> s1(func1); //subscribers specify the streams they subscribe to, get assigned unique id in that stream 
    subscriber<string> s2(func2);
    subscriber<string> s3(func3);
    subscriber<string> s4(func4);

    vector<subscriber<string>> slist = {s1, s2, s3, s4};   

      //  keyboard_stream->register_subscriber(s1); 

    keyboard_stream.register_subscriber(slist); 

    
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
    keyboard_stream.start();
/*} catch(std::exception &e){
    std::cout << e.what() << std::endl;
}*/
	

    return 0;
}
