#include "twitter_streamer.h"

size_t f_CALLBACK(char *data, size_t size, size_t nmemb, void *streams)
{
        if(data != NULL)
	{
		twit_streamer *objDemo = (twit_streamer*)(streams);
		parse t = parse(data);
		objDemo->m_callback(t);
	}
	else
	{
		cerr << "ERROR: A NULL json string was returned from twitter.\n";
	}
	
	return size * nmemb;
}

twit_streamer::twit_streamer(           // constr
    void(*callback)(parse),const char* c_URL,
    const char* c_CONSUKEY, const char* c_CONSUSEC,
    const char* c_ACCTOKKEY, const char* c_ACCTOKSEC)
{
    this->m_callback = callback;
    this->c_URL     = c_URL;
    this->c_CONSUKEY = c_CONSUKEY;
    this->c_CONSUSEC = c_CONSUSEC;
    this->c_ACCTOKKEY = c_ACCTOKKEY;
    this->c_ACCTOKSEC = c_ACCTOKSEC;
}

bool twit_streamer::runDemo()
{
    
    std::chrono::time_point<std::chrono::system_clock> start_twit, end_twit;        // TIMER Settings
    curl_global_init(CURL_GLOBAL_ALL);      // INIT CURL CODE
    CURLcode res;
    curl = curl_easy_init();
    struct curl_slist *list = NULL;
    
    if (!curl) {
        cout << "ERROR failed: curl_easy_init" << endl;
        curl_global_cleanup();
        
        return false;
        }
        
        else if (curl)
        
        {
        // ==== cURL Setting
        // - URL, POST parameters, OAuth signing method, HTTP method, OAuth keys
        c_OAUTHURL = oauth_sign_url2(
        c_URL, NULL, OA_HMAC, "GET",c_CONSUKEY, c_CONSUSEC, c_ACCTOKKEY, c_ACCTOKSEC // generates oauth signature. Args are url, post para (using GET here so it is NULL, Oauth signature encryption method, HTTP method (GET or POST), and the 4 Oauth Keys from Twitter Dev Account
        );
    
        list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded"); // needs this header data to not get authorization error
        //string temp;
        //temp = string(c_OAUTHURL);

        // CURL STUFF FOR GET HTTP METHOD ONLY
        curl_easy_setopt(curl, CURLOPT_URL, c_OAUTHURL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);                // will print the header
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "ReactCppDemo/0.1");    // sets user agent header
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);                 
        //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, f_CALLBACK);
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &chunk);
        
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, f_CALLBACK);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);                          // enable verbose mode for debug
    
        start_twit = std::chrono::system_clock::now();                      // start timing how long it takes
        res = curl_easy_perform(curl);  // execute
        end_twit = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds_twit = end_twit-start_twit;
        std::time_t end_time_twit = std::chrono::system_clock::to_time_t(end_twit);
        std::cout << "finished computation at " << std::ctime(&end_time_twit) << "elapsed time: " << elapsed_seconds_twit.count() << "s\n";
    // CLEANUP
        if (res != CURLE_OK) {
            cerr <<"\ncurl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            return false;
    }
        curl_easy_cleanup(curl);
//        curl_global_cleanup();
    }
    else
    {
        return false;
    }
    return 0;
    }
