#include "stream.h"
#include "event.h"
#include "subscriber.h"

void greater_than_3(event<int> event){
    using namespace std;
    if (event.get_data() > 3) {
            cout << "The number you entered is greater than 3." << endl;
    };
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

void func3(event<int> event){
    using namespace std;
    cout << "func 3" << endl;
}

void func4(event<int> event){
    using namespace std;
    cout << "func 4" << endl;
}

void func5(stream<int>) {
    std::cout << "lol" << std::endl;
}


int main(void){

    using namespace std;

    shared_ptr<subscriber_pool<int>> pool(new subscriber_pool<int>);

    auto my_on_start = [&int_stream]() {
        for (int i = 1; i < 2; i ++) {
           // std::cout << "1" << endl;
            event<int> e(i);
            int_stream.notify_subscribers(e);
            usleep(100);

         }
    };





    stream<int> int_stream(pool);

    stream<int> mapped_stream(pool);

    subscriber<int> mapped_stream_feeder([&mapped_stream](event<int> event) {
        mapped_stream.notify_subscribers(event<int>(2 * e.get_data()));           
    });

    int_stream.register_subscriber(mapped_stream_feeder); 



    stream<string> mapped_stream = int_stream.map(std::int_to_string)

    map(std::function<OutputType(InputType)> func){
        stream<OutputType> mapped_stream(pool);

        subscriber<InputType> feeder([&mapped_stream](event<InputType> event)
        {
            mapped_stream.notify_subscribers(event<OutputType>(func(event.get_data())));
        });

        pool.register_subscriber(feeder);
        return mapped_stream;
    }


    subscriber<int> s2(func2_you_entered_int);

    mapped_stream.register_subscriber(s2);

/////
    auto mapped_stream2 = int_stream.map(
                [](event<int> e) -> event<int> {
             return event<int>(100+ e.get_data());
         }
    );

    subscriber<int> s3(func3_you_entered_int);

    mapped_stream2.register_subscriber(s3);


 auto mapped_stream3 = int_stream.map(
                [](event<int> e) -> event<int> {
             return event<int>(1000+ e.get_data());
         }
    );

    subscriber<int> s4(func4_you_entered_int);

    mapped_stream3.register_subscriber(s4);




    int_stream.start();


	
    return 0;
}
