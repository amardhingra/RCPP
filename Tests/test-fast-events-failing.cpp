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

    shared_ptr<subscriber_pool<string>> pool(new subscriber_pool<string>);
    //subscriber_pool<string>* pool_ptr = &pool;
    stream<string> keyboard_stream(pool, [](stream<std::string> & my_stream) {
        for (int i = 1; i < 100; i ++) {
            event<std::string> e = event<std::string>(std::to_string(i));
            my_stream.notify(e);
        
        // Description of bug: events get lost if I don't call sleep after each event is fired. 
        // Correct behavior: the numbers 100 - 104 should get printed to the screen, along with "The word you entered is longer than 2 characters".
        // Current(buggy) behavior: only 1 handler might get called.
        // Uncommenting below line results in correct behavior.
        usleep(50);
        }
     });

    subscriber<string> s1(func1); 
    subscriber<string> s2(func2);

    vector<subscriber<string>> slist = {s1, s2};   
    keyboard_stream.register_subscribers(slist); 


     keyboard_stream.start();
     //keyboard_stream.complete();

    return 0;
}
