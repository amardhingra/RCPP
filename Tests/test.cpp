//right now subscriber pool feels too naked. this shouldn’t be exposed to the user. instead, streams should own a single subscriber pool, which then gets assigned various subscribers. All the details of the subscriber pool should be hidden away in the stream class. 


int main(void){
    using namespace std;
    stream source( <con_lvl> 2);
    subscriber s1(func1, source); //subscribers specify the streams they subscribe to, get assigned unique id in that stream 
    subscriber s2(func2, source);
    subscriber s3(func3, source);
    subscriber s4(func4, source);
   // stream.register_subscriber(s1); // OR turn this into variable args
    //stream.register_subscriber(s2);
   // stream.register_subscriber(s3);
    //stream.register_subscriber(s4);
   stream.register_subscribers(s1, s2, s3, s4); //takes variadic args
   stream.register_subscribers(slist); //vector or list


   source.register_subscriber(new subscriber(function whatever))
	
    pool.notify(0,1); //we can get rid of this entirely in user space… obviously need to implement, but shouldn’t be accessible to user. 
    pool.notify(0,1); 
    pool.notify(0,1);
    pool.notify(0,1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for(int i = 0; i < 4; i++){
        pool.complete(i);
    }
    return 0;
}
