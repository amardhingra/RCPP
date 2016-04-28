#include "stream.h"
#include "event.h"
#include "subscriber.h"
#include "stream_factory.h"

#include <functional>
#include <string>
#include <memory>
#include <iostream>
#include <chrono>

void print(event<std::string> s){
    std::cout << s.get_data() << std::endl;
}

int main(void){
    using namespace std;

    shared_ptr<subscriber_pool<int>> pool_str(new subscriber_pool<int>(2));
    
    stream<int> str1(pool_str, [](stream<int>& str){
        for(int i = 0; i < 100; i += 2){
            str.notify(event<int>(i));
        }
    });

    stream<int> str2(pool_str, [](stream<int>& str){
        for(int i = 1; i < 100; i += 2){
            str.notify(event<int>(i));
        }
    });

    stream<int> str3 = str1 + str2;
    str3.register_subscriber(subscriber<int>([](event<int> e){
        std::cout << e.get_data() << ' ';
    }));

    str1.start();
    str2.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << std::endl;

    stream<int> str4(pool_str, [](stream<int>& str){
        for(int i = 0; i < 100; i += 2){
            str.notify(event<int>(i));
        }
    });

    stream<int> str5(pool_str, [](stream<int>& str){
        for(int i = 1; i < 100; i += 2){
            str.notify(event<int>(i));
        }
    });

    str4 += str5;

    str4.register_subscriber(subscriber<int>([](event<int> e){
        std::cout << e.get_data() << ' ';
    }));

    str4.start();
    str5.start();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << std::endl;
}