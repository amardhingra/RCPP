#include "stream.h"

void func1(subscriber_event event){
    using namespace std;
    cout << "func 1" << endl;
}

void func2(subscriber_event event){
    using namespace std;
    cout << "func 2" << endl;
}

void func3(subscriber_event event){
    using namespace std;
    cout << "func 3" << endl;
}

void func4(subscriber_event event){
    using namespace std;
    cout << "func 4" << endl;
}

int main(void){
    using namespace std;
    stream *keyboard_stream = stream::stream_from_keyboard_input();

    subscriber s1(func1); //subscribers specify the streams they subscribe to, get assigned unique id in that stream 
    subscriber s2(func2);
    subscriber s3(func3);
    subscriber s4(func4);

    vector slist = {s1, s2, s3, s4};   
   new_stream.register_subscribers(slist); 


   //source.register_subscriber(new subscriber(function whatever))
	

    return 0;
}
