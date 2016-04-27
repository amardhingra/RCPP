#include "stream.h"
#include "event.h"
#include "subscriber.h"

/* BUG:
stream1 defined before stream2, each with a subscriber. they share same pool. stream1 starts, then stream2 starts.

Expected output:

func1: you entered 1
func1: you entered 2
func1: you entered 3
func1: you entered 4
func1: you entered 5
func 2: you entered 100
func 2: you entered 101
func 2: you entered 102
func 2: you entered 103
func 2: you entered 104

Current (buggy) behavior:

stream1 and 2 both fire events, but only stream2's subscribers get called, even for stream1's events.

buggy output:

func 2: you entered 1
func 2: you entered 2
func 2: you entered 3
func 2: you entered 4
func 2: you entered 5
func 2: you entered 100
func 2: you entered 101
func 2: you entered 102
func 2: you entered 103
func 2: you entered 104

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

    // shared pool
    shared_ptr<subscriber_pool<int>> pool1(new subscriber_pool<int>);

    // stream1's definition
    std::function<void(stream<int> & my_stream)> my_on_start1 = [](stream<int> & my_stream) {
        for (int i = 1; i < 6; i ++) {
            event<int> e(i);
            my_stream.notify(e);
            usleep(100);
         }
    };

    // construct stream 1 using pool1
    stream<int> stream1(&pool1);
    stream1.on_start = my_on_start1;

    // stream1's subscriber
    subscriber<int> s1(func1_you_entered_int); 
    stream1.register_subscriber(s1); 

    // stream2's definition
    std::function<void(stream<int> & my_stream)> my_on_start2 = [](stream<int> & my_stream) {
        for (int i = 100; i < 105; i ++) {
            event<int> e(i);
            my_stream.notify(e);
            usleep(100);
         }
    };

    // construct stream 2 using pool1
    stream<int> stream2(&pool1);
    stream2.on_start = my_on_start2;

    // stream2's subscriber
    subscriber<int> s2(func2_you_entered_int); 
    stream2.register_subscriber(s2); 
    
    stream1.start();
  //  stream2.start();


    return 0;
}
