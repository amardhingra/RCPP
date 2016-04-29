/*

In order for this to work on Mac OS X you need to have oauth and jsoncpp installed. Run the following:

>ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" < /dev/null 2> /dev/null
>brew install oauth
>brew install jsoncpp

*/
#include "Tests/stream.h"
#include <vector>
#include "TwitterDemo/twitter_dataparser.h"
#include "TwitterDemo/twitter_streamer.h"


void callback(parse t)
{
	// check to make sure it wasn't a rate limit message
	if(t.m_id != -1)
	{
	    //t.print(parse::ID);
            	t.print(parse::ID | parse::ORIGINAL_ID | parse::TEXT | parse::FOLLOWERS | parse::RETWEETS | parse::IS_RETWEET | parse::LANGUAGE);
	}
}

int main(int argc, char **argv)
{
    const char *URL = "https://stream.twitter.com/1.1/statuses/filter.json?track=clinton%2Ctrump%2Csanders&stall_warnings=true"; // NEEDDS question mark to work with GET
    
    // MANUALLY INPUT KEYS
    // FROM TWITTER DEV ACCOUNT AndyPandy60
    const char *CONSU_KEY = "8j4yV6Vk9rzw7J1kDuhoIPsZR";
    const char *CONSU_SEC = "JNloCz1F1U2UeFTcN4ZOnK1IEt53gwIVRRrdsDoC2TeBA9iMMW";
    const char *ACCTOK_KEY = "332151906-wl9kRcvzoji8zJ3ZJIeOqmv8SZBwz1eC5BA2glJp";
    const char *ACCTOK_SEC = "CJKsLPOq3nfEWyjXm9Y2mFXYPt1sTImvDewySX4wABCH7";

    // Instantiate new object
   
    twit_streamer objDemo = twit_streamer(&callback, URL, CONSU_KEY, CONSU_SEC, ACCTOK_KEY, ACCTOK_SEC);
    // twit_streamer objDemo(&callback, URL, CONSU_KEY, CONSU_SEC, ACCTOK_KEY, ACCTOK_SEC);
    cout << "Starting the TwitterDemo...\n";
    objDemo.runDemo();
   
    return 0;

}
