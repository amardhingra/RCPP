#include "stream.h"
#include "event.h"
#include "subscriber.h"
#include "stream_factory.h"

#include <functional>
#include <string>
#include <memory>
#include <iostream>

void print(event<std::string> s){
    std::cout << s.get_data() << std::endl;
}

int main(void){
    using namespace std;

    shared_ptr<subscriber_pool<string>> pool_str(new subscriber_pool<string>);
    stream<std::string> file_streamer(pool_str, file_stream("test.txtt"));
    subscriber<string> s([](event<std::string> s){
        std::cout << s.get_data() << std::endl;
    });
    
    file_streamer.register_subscriber(s);

    file_streamer.start();
}