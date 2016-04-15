#include "stream.h"

stream::stream(subscriber_pool& some_pool){
    thread_pool = some_pool; 
}

stream::stream(){
    thread_pool = subscriber_pool pool(2);
    
}


