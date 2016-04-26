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

class Demo
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
    Demo(const char*, const char*, const char*, const char*, const char*);
    void runDemo();  // start twitter app
};

Demo::Demo(  // constr
    const char* c_URL,
    const char* c_CONSUKEY, const char* c_CONSUSEC,
    const char* c_ACCTOKKEY, const char* c_ACCTOKSEC)
{
    this->c_URL     = c_URL;
    this->c_CONSUKEY = c_CONSUKEY;
    this->c_CONSUSEC = c_CONSUSEC;
    this->c_ACCTOKKEY = c_ACCTOKKEY;
    this->c_ACCTOKSEC = c_ACCTOKSEC;
}

void Demo::runDemo()
{
    
    std::chrono::time_point<std::chrono::system_clock> start_twit, end_twit;        // TIMER Settings
    curl_global_init(CURL_GLOBAL_ALL);      // INIT CURL CODE
    CURLcode res;
    curl = curl_easy_init();
    struct curl_slist *list = NULL;
    
    if (!curl) {
        cout << "ERROR failed: curl_easy_init" << endl;
        curl_global_cleanup();
        return;
    }
    // ==== cURL Setting
    // - URL, POST parameters, OAuth signing method, HTTP method, OAuth keys
    c_OAUTHURL = oauth_sign_url2(
        c_URL, NULL, OA_HMAC, "GET",c_CONSUKEY, c_CONSUSEC, c_ACCTOKKEY, c_ACCTOKSEC // generates oauth signature. Args are url, post para (using GET here so it is NULL, Oauth signature encryption method, HTTP method (GET or POST), and the 4 Oauth Keys from Twitter Dev Account
    );
    
    list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded"); // needs this header data to not get authorization error
    string temp;
    temp = string(c_OAUTHURL);

    // CURL STUFF FOR GET HTTP METHOD ONLY
    curl_easy_setopt(curl, CURLOPT_URL, temp.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);                // will print the header
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "ReactCppDemo/0.1");    // sets user agent header
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);                 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, f_CALLBACK);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunks);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);                          // enable verbose mode for debug
    
    start_twit = std::chrono::system_clock::now();                      // start timing how long it takes
    res = curl_easy_perform(curl);  // execute
    end_twit = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds_twit = end_twit-start_twit;
    std::time_t end_time_twit = std::chrono::system_clock::to_time_t(end_twit);
    std::cout << "finished computation at " << std::ctime(&end_time_twit) << "elapsed time: " << elapsed_seconds_twit.count() << "s\n";

    // CLEANUP
    if (res != CURLE_OK)
    {
        cerr <<"\ncurl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

size_t f_CALLBACK(char* ptr, size_t size, size_t n_mem, string* streams) {  //function callback
    int int_SIZE=size * n_mem;
    streams -> append(ptr, int_SIZE);
    string str = *streams;
    cout << str << endl;
    return int_SIZE;
}


int main(int argc, const char *argv[])
{
    const char *URL = "https://stream.twitter.com/1.1/statuses/filter.json?track=clinton%2Ctrump%2Csanders"; // NEEDDS question mark to work with GET
    
    // MANUALLY INPUT KEYS
    // FROM TWITTER DEV ACCOUNT AndyPandy60
    const char *CONSU_KEY = "8j4yV6Vk9rzw7J1kDuhoIPsZR";
    const char *CONSU_SEC = "JNloCz1F1U2UeFTcN4ZOnK1IEt53gwIVRRrdsDoC2TeBA9iMMW";
    const char *ACCTOK_KEY = "332151906-wl9kRcvzoji8zJ3ZJIeOqmv8SZBwz1eC5BA2glJp";
    const char *ACCTOK_SEC = "CJKsLPOq3nfEWyjXm9Y2mFXYPt1sTImvDewySX4wABCH7";

    // Instantiate new object
    Demo objDemo(URL, CONSU_KEY, CONSU_SEC, ACCTOK_KEY, ACCTOK_SEC);

    objDemo.runDemo();

    return 0;
}
