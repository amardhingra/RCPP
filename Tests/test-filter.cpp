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
        for (int i = 1; i < 10; i ++) {
            event<int> e(i);
            my_stream.notify(e);
            usleep(100);
        }
    };

    // parent stream
    stream<int> int_stream(pool, my_on_start);

    // mapped stream #1 from parent
    stream<int> filtered_stream = int_stream.filter([](int input) {
            return input % 2 == 0;
        }
    );

    subscriber<int> s2(func2_you_entered_int);
    filtered_stream.register_subscriber(s2);

    // mapped stream #2 from parent
    auto filtered_stream2 = int_stream.filter(
        [](int input) {
           return input < 5;
       }
    );

    subscriber<int> s3(func3_you_entered_int);
    filtered_stream2.register_subscriber(s3);

    // mapped stream #3 from mapped stream #1
    auto filtered_stream3 = filtered_stream.filter(
        [](int input) {
           return input % 4 == 0;
       }
    );

    subscriber<int> s4(func4_you_entered_int);
    filtered_stream3.register_subscriber(s4);

    // start parent stream
    int_stream.start();

    return 0;
}
