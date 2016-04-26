#include "stream.h"
#include "event.h"
#include "subscriber.h"

/* BUG:
stream1 and stream2 are created using different pools. each stream gets a subscriber. then stream1 starts. 

Correct behavior: stream1's events should fire and be handled by stream1's subscriber. 

Correct output:

func1: you entered 1
func1: you entered 2
func1: you entered 3
func1: you entered 4
func1: you entered 5

Buggy (current) behavior: program crashes with below output:

libc++abi.dylib: terminating with uncaught exception of type std::out_of_range: map::at:  key not found
Abort trap: 6

*/

void func1_you_entered_int(event<int> event){
    using namespace std;
    cout << "func1: you entered " << event.get_data() << endl;
}

void func2_you_entered_int(event<int> event){
    using namespace std;
    cout << "func 2: you entered " << event.get_data() << endl;
}

int main(void){

    using namespace std;

    // stream1's pool
    subscriber_pool<int> pool1;

    // stream1's definition
    std::function<void(stream<int> & my_stream)> my_on_start = [](stream<int> & my_stream) {
        for (int i = 1; i < 6; i ++) {
            event<int> e(i);
            my_stream.notify_subscribers(e);
            usleep(100);
         }
    };

    // construct stream 1 using pool1
    stream<int> stream1(&pool1);
    stream1.on_start = my_on_start;

    // stream1's subscriber
    subscriber<int> s1(func1_you_entered_int); 
    stream1.register_subscriber(s1); 

    // constructing stream 2 with its own pool
    subscriber_pool<int> pool2;
    stream<int> stream2(&pool2);

    // stream2's subscriber
    subscriber<int> s2(func2_you_entered_int);
    stream2.st_register_subscriber(s2);

    // start stream 1
    stream1.start();

    return 0;
}
