#include <unistd.h>
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


    shared_ptr<subscriber_pool<int>> pool(new subscriber_pool<int>);
    //subscriber_pool<int>* pool_ptr = &pool;

    std::function<void(stream<int> & my_stream)> my_on_start = [](stream<int> & my_stream) {
        for (int i = 1; i < 7; i ++) {
            //event<std::string> e = event<std::string>(std::to_string(i));
            event<int> e(i);
            my_stream.notify(e);
            usleep(100);

         }
    };

    //construction taking in a pool
    stream<int> int_stream1(pool, my_on_start);

    //construction taking only a on_start function
    stream<int> int_stream2(my_on_start);

    //int_stream.on_start = my_on_start;

    subscriber<int> s1(greater_than_3);
    subscriber<int> s2(you_entered_int);

    vector<subscriber<int>> slist = {s1, s2};   

    int_stream2.register_subscribers(slist); 
    int_stream1.start();
    int_stream2.start();

    return 0;
}
