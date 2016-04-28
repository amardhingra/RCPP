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

void func4_you_entered_int(event<int> event){
    using namespace std;
    cout << "func4: you entered " << event.get_data() << endl;
}

void func5_you_entered_int(event<int> event){
    using namespace std;
    cout << "func5: you entered " << event.get_data() << endl;
}


int main(void){

    using namespace std;

    shared_ptr<subscriber_pool<int>> pool(new subscriber_pool<int>);


    auto my_on_start = [](stream<int> & my_stream) {
        for (int i = 1; i < 3; i ++) {
            event<int> e(i);
            my_stream.notify(e);
            usleep(100);

         }
    };

    stream<int> int_stream(pool, my_on_start);

    subscriber<int> s1(func1_you_entered_int);

    int_stream.register_subscriber(s1); 


    stream<int> mapped_stream = int_stream.map([](int input) {
        return 2 * input;
    });

    subscriber<int> s2(func2_you_entered_int);

    mapped_stream.register_subscriber(s2);

    
    auto mapped_stream2 = int_stream.map(
                [](int input) {
             return 1000+input;
         }
    );

    subscriber<int> s3(func3_you_entered_int);

    mapped_stream2.register_subscriber(s3);

    auto mapped_stream4 = mapped_stream.map(
                [](int input) {
             return 100 + input;
         }
    );

    subscriber<int> s5(func5_you_entered_int);
    mapped_stream4.register_subscriber(s5);


    int_stream.start();

    return 0;
}
