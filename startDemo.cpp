#include <vector>
#include "TwitterDemo/twitter_streamer.h"


int main(int argc, char **argv)
{
    const char *URL = "https://stream.twitter.com/1.1/statuses/filter.json?track=clinton%2Ctrump%2Csanders"; // NEEDDS question mark to work with GET
    
    // MANUALLY INPUT KEYS
    // FROM TWITTER DEV ACCOUNT AndyPandy60
    const char *CONSU_KEY = "8j4yV6Vk9rzw7J1kDuhoIPsZR";
    const char *CONSU_SEC = "JNloCz1F1U2UeFTcN4ZOnK1IEt53gwIVRRrdsDoC2TeBA9iMMW";
    const char *ACCTOK_KEY = "332151906-wl9kRcvzoji8zJ3ZJIeOqmv8SZBwz1eC5BA2glJp";
    const char *ACCTOK_SEC = "CJKsLPOq3nfEWyjXm9Y2mFXYPt1sTImvDewySX4wABCH7";

    // Instantiate new object
    twit_streamer objDemo(URL, CONSU_KEY, CONSU_SEC, ACCTOK_KEY, ACCTOK_SEC);

    objDemo.runDemo();

    return 0;

}
