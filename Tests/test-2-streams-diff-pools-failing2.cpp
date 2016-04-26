#include "stream.h"
#include "event.h"
#include "subscriber.h"

/* BUG:
NOTE: this is the same bug as in test-2-streams-diff-pools-failing.cpp; just tests a more complicated case.

stream1 defined before stream2. they each have their own pool.

After streams 1 and 2 are defined, try to start the streams. different things happen --

1.  if you just try to start stream1: 

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

2. if you try to start stream2, then stream1;

Correct output:

func 2: you entered 1
func 2: you entered 2
func 2: you entered 3
func 2: you entered 4
func 2: you entered 5
func1: you entered 1
func1: you entered 2
func1: you entered 3
func1: you entered 4
func1: you entered 5

Buggy (current) output:

func 2: you entered 1
func 2: you entered 2
func 2: you entered 3
func 2: you entered 4
func 2: you entered 5
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
    std::function<void(stream<int> & my_stream)> my_on_start1 = [](stream<int> & my_stream) {
        for (int i = 1; i < 6; i ++) {
            event<int> e(i);
            my_stream.notify_subscribers(e);
            usleep(100);
         }
    };

    // construct stream 1 using pool1
    stream<int> stream1(&pool1);
    stream1.on_start = my_on_start1;

    // stream1's subscriber
    subscriber<int> s1(func1_you_entered_int); 
    stream1.register_subscriber(s1); 

    // stream2's pool
    subscriber_pool<int> pool2;

    // stream2's definition
    std::function<void(stream<int> & my_stream)> my_on_start2 = [](stream<int> & my_stream) {
        for (int i = 1; i < 6; i ++) {
            event<int> e(i);
            my_stream.notify_subscribers(e);
            usleep(100);
         }
    };

    // construct stream 2 using pool2
    stream<int> stream2(&pool2);
    stream2.on_start = my_on_start2;

    // stream2's subscriber
    subscriber<int> s2(func2_you_entered_int); 
    stream2.register_subscriber(s2); 
    
    // try doing these in different orders, different weird things happen
    
    stream2.start();
    stream1.start();


    return 0;
}
