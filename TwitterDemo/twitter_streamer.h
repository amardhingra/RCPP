#include "libs/twitcurl/urlencode.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <oauth.h>
#include <curl/curl.h>
#include <sstream>
#include <chrono>
#include "twitter_dataparser.h"
#include "stream.h"

using namespace std;

size_t f_CALLBACK(char *data, size_t size, size_t n_mem, void *streams); // start twitter app

class twit_streamer
{
    const char* c_URL;
    const char* c_CONSUKEY;
    const char* c_CONSUSEC;
    const char* c_ACCTOKKEY;
    const char* c_ACCTOKSEC;
    CURL    *curl;
    char*   c_OAUTHURL;
    string  next_tweet;
    bool    has_next_tweet = false;
    

    public:
    void (*m_callback)(parse);
    twit_streamer(void (*callback)(parse),const char*, const char*, const char*, const char*, const char*);
    void operator()(stream<string> &stream); 
    // void operator()(stream<std::string>& stream)
    // {
    //     while(1){
    //         // get string from the twitter api
    //         std::string str = get_next_data();
    //         stream.notify(event<std::string>(str));

    //     }

    // };
};

