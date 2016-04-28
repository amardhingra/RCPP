#ifndef TWITTER_DATAPARSER_H
#define TWITTER_DATAPARSER_H

#include <iostream>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include "libs/jsoncpp/json/json.h"

using namespace std;

class parse {

private: 
        string filter(string text);

public:
	enum fields { // fields to be printed out as json
		NONE			= 0,
		ID 		    	= (1u << 0),
		TEXT 			= (1u << 1),
		FOLLOWERS 		= (1u << 2),
		RETWEETS 		= (1u << 3),
		IS_RETWEET 		= (1u << 4),
		ORIGINAL_ID     	= (1u << 5),
		LANGUAGE		= (1u << 6)
	};

	string m_text;
	int64_t m_id;
	int64_t m_original_id;
	int m_followers;
	int m_retweets;
	bool m_is_retweet;
	string m_language;
        
	parse(const char *json_data);
	parse(const parse &tweet);
	parse();
	void print(unsigned int fields);
};

#endif
