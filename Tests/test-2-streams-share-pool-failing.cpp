#include <unistd.h>

#include "stream.h"
#include "event.h"
#include "subscriber.h"

/* BUG:
stream1 is defined and a subscriber s1 is added to it, then stream2 is defined and subscriber s2 is added to it. the 2 streams share a pool. then stream1 starts. 

Correct behavior: stream1's events should fire and be handled by stream1's subscriber. 

Correct output:

func1: you entered 1
func1: you entered 2
func1: you entered 3
func1: you entered 4
func1: you entered 5

Buggy (current) behavior: program crashes with "libc++abi.dylib: terminating with uncaught exception of type std::out_of_range: map::at:  key not found
Abort trap: 6"

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

    shared_ptr<subscriber_pool<int>> pool(new subscriber_pool<int>);

    std::function<void(stream<int> & my_stream)> my_on_start = [](stream<int> & my_stream) {
        for (int i = 1; i < 6; i ++) {
            event<int> e(i);
            my_stream.notify(e);
            usleep(100);
         }
    };

    // define stream 1

    stream<int> stream1(&pool);
    stream1.on_start = my_on_start;

    subscriber<int> s1(func1_you_entered_int);
    stream1.register_subscriber(s1); 
    // defining stream 2

    stream<int> stream2(&pool);

    subscriber<int> s2(func2_you_entered_int);
    stream2.st_register_subscriber(s2);


    // start stream 1
    stream1.start();

    return 0;
}
