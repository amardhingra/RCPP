#include "stream.h"
#include "event.h"
#include "subscriber.h"

void func1(event<std::string> event){
    using namespace std;
    if (event.get_data().size() > 2) {
            cout << "The word you entered is longer than 2 characters." << endl;
    };
}

void func2(event<std::string> event){
    using namespace std;
    cout << "you entered " << event.get_data() << endl;
}

int main(void){
    using namespace std;

    subscriber_pool<string> pool;
    subscriber_pool<string>* pool_ptr = &pool;
    stream<string> keyboard_stream(pool_ptr);

    subscriber<string> s1(func1); 
    subscriber<string> s2(func2);

    vector<subscriber<string>> slist = {s1, s2};   
    keyboard_stream.register_subscriber(slist); 

    for (int i = 100; i < 105; i ++) {
        event<std::string> e = event<std::string>(std::to_string(i));
        keyboard_stream.change();
        keyboard_stream.notify_subscribers(e);
        
        // Description of bug: events get lost if I don't call sleep after each event is fired. 
        // Correct behavior: the numbers 100 - 104 should get printed to the screen, along with "The word you entered is longer than 2 characters".
        // Current(buggy) behavior: only 1 handler might get called.
        // Uncommenting below line results in correct behavior.
        // usleep(50);

     }

    return 0;
}
