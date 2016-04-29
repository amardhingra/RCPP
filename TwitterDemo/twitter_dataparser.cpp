#include "twitter_dataparser.h"
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "stream.h"

void string_output(event<string> event){   
   std::cout << event.get_data() << '\n';

}

parse::parse() {}
parse::parse(const parse &tweet)
{		
	m_text = tweet.m_text;
	m_id = tweet.m_id;
	m_original_id = tweet.m_original_id;
	m_followers = tweet.m_followers;
	m_retweets = tweet.m_retweets;
	m_is_retweet = tweet.m_is_retweet;
	m_language = tweet.m_language;
}



parse::parse(const char *json_data)   // trying to parse incoming stream
{
	Json::Value root;
	Json::Reader reader;
        Json::StyledWriter styledWriter;
    std::string s = json_data;
          //std::cout << s;
/*	
        std::string delimiter = "\r\n";
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
    	token = s.substr(0, pos);
    	std::cout << token << std::endl;
    	s.erase(0, pos + delimiter.length());

        
        //
	//std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
	//s.erase(end_pos, s.end());
	//remove_if(s.begin(), s.end(), ::isspace);
	//std::cout << s << '\n';
*/
	//cout << json_data
	vector<string> results;        // STORES each tweet into the vector with proper syntax (No missing brackets)(how to get json from the vector)?
	boost::split(results, s, boost::is_any_of("\r\n"));
	// for (auto i: results){
	// 	std::cout << i << '\n';
	// }

	std::function<void(stream<string> & my_stream)> my_on_start = [s](stream<string> & my_stream) {
        
            
            event<string> e(s);
            my_stream.notify(e);


         
    };


/*

	
	
        bool parsedSUCCESS = reader.parse(json_data,root,false);
        if (!parsedSUCCESS)
        {
        cout << reader.getFormattedErrorMessages() << endl;
	return;
        }
	if(root.get("user", "").asString().length() > 0)
	{
	
		m_id = root.get("id", 0).asInt64();
		m_language = root["user"].get("lang", "").asString();
		m_followers = root["user"].get("followers_count", 0).asInt();
		m_retweets = root.get("retweet_count", 0).asInt();
	}



	

	if(!reader.parse(s, root))
	{
	        cout << "failed to decode tweet JSON!\n" << reader.getFormattedErrorMessages() << s << "\n\n";
		return;
	}
	else
	{
		if(root["warning"].get("code", "").asString() == "FALLING_BEHIND")
		{
			cout << "Experiencing slowdown on twitter... (" << root["warning"].get("percent_full", 0).asInt() << ")\n";
			cout << "Tweets are arriving too fast.";
			m_id = -1;
		}

	
		else
		{			m_id = root.get("id", 0).asInt64();
			m_text = filter(root.get("text", "").asString());
			if(root.get("created_at", "").asString().length() > 0)
			{
				m_language = root["user"].get("lang", "").asString();
				m_followers = root["user"].get("followers_count", 0).asInt();
				m_retweets = root.get("retweet_count", 0).asInt();
				m_is_retweet = !root["retweeted_status"].empty();
				if(m_is_retweet)
				{
					m_original_id = root["retweeted_status"].get("id", 0).asInt64();
				}
				else
				{
					m_original_id = 0;
				}
			}
			else
			{
				m_language = root.get("language", "").asString();
				m_followers = root.get("followers", 0).asInt();
				m_retweets = root.get("retweets", 0).asInt();
				
				m_is_retweet = root.get("is_retweet", false).asBool();
				m_original_id = root.get("original_id", 0).asInt64();
				
				//if (root.get("timestamp_ms", "").asString().length() > 0) {
				//return;
				}
			}
			
		}
	}

*/
}

string parse::filter(string text)
{
	for(int i = 0; i < text.length(); ++i)
	{
		text[i] = tolower(text[i]); // conversion to lower case
		if(text[i] == '\n') text[i] = ' '; // if newline then replace as a space
		if(text[i] == '"') text[i] = '\''; // if theres a single quote replace with double quotes
		if(text[i] == '\\') text[i] = '/';
	}

	return text;
}

void parse::print(unsigned int fields) // prints JSON version of a single tweet
{
/*
	cout << "{";

	cout << "\"id\":" << m_id;

	if(fields & TEXT)
		cout << ",\"text\":\"" << m_text << "\"";
	if(fields & FOLLOWERS)
		cout << ",\"followers\":" << m_followers;
	if(fields & RETWEETS)
		cout << ",\"retweets\":" << m_retweets;
	if(fields & IS_RETWEET)
		cout << ",\"is_retweet\":" << m_is_retweet;
	if(fields & ORIGINAL_ID)
		cout << ",\"original_id\":" << m_original_id;
	if(fields & LANGUAGE)
		cout << ",\"language\":\"" << m_language << "\"";

	cout << "}\n" << flush;
*/
}

