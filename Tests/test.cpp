#include "stream.h"
#include "event.h"
#include "subscriber.h"

void func1(event<std::string> event){
    using namespace std;
    cout << "func 1" << endl;
}

void func2(event<std::string> event){
    using namespace std;
    cout << "func 2" << endl;
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
    try{
    subscriber_pool<string> pool(2);
    //stream *keyboard_stream = stream::stream_from_keyboard_input(pool);
    subscriber_pool<string>* pool_ptr = &pool;

    stream<string> keyboard_stream(pool_ptr);
    // keyboard_stream.get_events_from_source = [keyboard_stream]() {   
    //     string keyinput;
    //     cin >> keyinput; 
    //     event<string> my_event(keyinput);
    //     keyboard_stream.change();
    //     keyboard_stream.notify_subscribers(my_event);
    // };


    subscriber<string> s1(func1); //subscribers specify the streams they subscribe to, get assigned unique id in that stream 
    subscriber<string> s2(func2);
    subscriber<string> s3(func3);
    subscriber<string> s4(func4);

    vector<subscriber<string>> slist = {s1, s2, s3, s4};   
    keyboard_stream.register_subscriber(slist); 
    keyboard_stream.start();
} catch(std::exception &e){
    std::cout << e.what() << std::endl;
}


   //source.register_subscriber(new subscriber(function whatever))
	

    return 0;
}
