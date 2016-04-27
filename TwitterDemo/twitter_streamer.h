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

using namespace std;

size_t f_CALLBACK(char* ptr, size_t size, size_t n_mem, string* streams);
//size_t f_WRITE(char *data, size_t size, size_t n_mem, void *usrdata);

class twit_streamer
{
    const char* c_URL;
    const char* c_CONSUKEY;
    const char* c_CONSUSEC;
    const char* c_ACCTOKKEY;
    const char* c_ACCTOKSEC;
    CURL    *curl;
    char*   c_OAUTHURL;
    string  chunks;

    public:
    twit_streamer(const char*, const char*, const char*, const char*, const char*);
    bool runDemo();  // start twitter app
};

