#include <unistd.h>
#include "stream.h"

void greater_than_3(event<int> event){
    using namespace std;
    if (event.get_data() > 3) {
            cout << "The number you entered is greater than 3." << endl;
    };
}

void you_entered_int(event<int> event){
    using namespace std;
    cout << "you entered " << event.get_data() << endl;
}

int main(void){
    using namespace std;

    std::function<void(stream<int> & my_stream)> my_on_start = [](stream<int> & my_stream) {
        for (int i = 1; i < 7; i ++) {
            //event<std::string> e = event<std::string>(std::to_string(i));
            event<int> e(i);
            my_stream.notify(e);
            usleep(100);

         }
    };

    //move assignment taking only a on_start function
    stream<int> int_stream1 = stream<int>(my_on_start);

    //call copy constructor... generates error!
    stream<int> int_stream2 = int_stream1;


    subscriber<int> s1(greater_than_3);
    subscriber<int> s2(you_entered_int);

    vector<subscriber<int>> slist = {s1, s2};   

    int_stream2.register_subscribers(slist); 
    
    int_stream2.start();

    return 0;
}
