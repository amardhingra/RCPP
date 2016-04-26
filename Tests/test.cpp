#include "stream.h"
#include "event.h"
#include "subscriber.h"

void greater_than_3(event<int> event){
    using namespace std;
    if (event.get_data() > 3) {
            cout << "The number you entered is greater than 3." << endl;
    };
}

void func1_you_entered_int(event<int> event){
    using namespace std;
    cout << "func1: you entered " << event.get_data() << endl;
}

void func2_you_entered_int(event<int> event){
    using namespace std;
    cout << "func2: you entered " << event.get_data() << endl;
}

void func3_you_entered_int(event<int> event){
    using namespace std;
    cout << "func3: you entered " << event.get_data() << endl;
}

void func3(event<int> event){
    using namespace std;
    cout << "func 3" << endl;
}

void func4(event<int> event){
    using namespace std;
    cout << "func 4" << endl;
}

void func5(stream<int>) {
    std::cout << "lol" << std::endl;
}


int main(void){

    using namespace std;

    subscriber_pool<int> pool;

    std::function<void(stream<int> & my_stream)> my_on_start = [](stream<int> & my_stream) {
        for (int i = 1; i < 2; i ++) {
           // std::cout << "1" << endl;
            event<int> e(i);
            my_stream.notify_children(e);
            usleep(100);
            my_stream.notify_subscribers(e);
            usleep(100);

         }
    };

    stream<int> int_stream(&pool, my_on_start);
   
    subscriber<int> s1(func1_you_entered_int);


    int_stream.register_subscriber(s1); 


    auto mapped_stream = int_stream.map(
        [](event<int> e) -> event<int> {
             return event<int>(2 * e.get_data());
         }
    );

    subscriber<int> s2(func2_you_entered_int);

    mapped_stream.register_subscriber(s2);

    auto mapped_stream2 = int_stream.map(
                [](event<int> e) -> event<int> {
             return event<int>(100+ e.get_data());
         }
    );

    subscriber<int> s3(func3_you_entered_int);

    mapped_stream2.register_subscriber(s3);


    int_stream.start();


	
    return 0;
}
