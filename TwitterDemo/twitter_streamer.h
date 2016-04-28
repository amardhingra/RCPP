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

using namespace std;

size_t f_CALLBACK(char *data, size_t size, size_t n_mem, void *streams);

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
    void (*m_callback)(parse);
    twit_streamer(void (*callback)(parse),const char*, const char*, const char*, const char*, const char*);
    bool runDemo();  // start twitter app
};

