#include "stream.h"
#include "event.h"
#include "subscriber.h"

/* 

Expected output:

func1: you entered 1
func2: you entered 2
func3: you entered 1001
func4: you entered 102
func1: you entered 2
func2: you entered 4
func3: you entered 1002
func4: you entered 104

*/

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

int main(void){

    using namespace std;

    // create pool
    shared_ptr<subscriber_pool<int>> pool(new subscriber_pool<int>);

    // stream definition
    auto my_on_start = [](stream<int> & my_stream) {
        for (int i = 1; i < 3; i ++) {
            event<int> e(i);
            my_stream.notify(e);
            usleep(100);

        }
    };

    // parent stream
    stream<int> int_stream(pool, my_on_start);

    // parent stream's subscriber
    subscriber<int> s1(func1_you_entered_int);
    int_stream.register_subscriber(s1); 

    // mapped stream #1 from parent
    stream<int> mapped_stream = int_stream.map([](int input) {
        return 2 * input;
        }
    );

    subscriber<int> s2(func2_you_entered_int);
    mapped_stream.register_subscriber(s2);

    // mapped stream #2 from parent
    auto mapped_stream2 = int_stream.map(
        [](int input) {
           return 1000 + input;
       }
    );

    subscriber<int> s3(func3_you_entered_int);
    mapped_stream2.register_subscriber(s3);

    // mapped stream #3 from mapped stream #1
    auto mapped_stream3 = mapped_stream.map(
        [](int input) {
           return 100 + input;
       }
    );

    subscriber<int> s4(func4_you_entered_int);
    mapped_stream3.register_subscriber(s4);

    // start parent stream
    int_stream.start();

    return 0;
}
