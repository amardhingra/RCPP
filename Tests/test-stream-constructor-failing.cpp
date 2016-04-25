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

    // this works
    //stream<int> int_stream(pool_ptr);

    // Description of bug: Using default stream constructor results in hang when you try to register_subscriber on that stream. Commenting out the above line and uncommenting the below line should result in hang, along with "trying to get lock" printed to screen repeatedly.

    // this fails
    stream<int> int_stream;

    int_stream.on_subscribe = my_on_subscribe;

    subscriber<int> s1(greater_than_3);
    subscriber<int> s2(you_entered_int);

    vector<subscriber<int>> slist = {s1, s2};   

    int_stream.register_subscriber(slist); 
    int_stream.start();

    return 0;
}
