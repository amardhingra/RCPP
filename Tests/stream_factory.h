#ifndef INCLUDED_STREAM_FACTORY
#define INCLUDED_STREAM_FACTORY

#include <string>
#include <iostream>
#include <fstream>

#include "stream.h"
#include "event.h"

class file_stream{

    std::ifstream file;
    char delim;

public:
    file_stream(std::string filename, char _delim = '\n') :
    file(filename), delim(_delim) {};

    void operator(stream<std::string> str)(){
        if (!file) {
            return;
        }   
        std::string buf;
        while(!file.eof()){
            std::getline(file, buf, delim);
            std::cout << buf << std::endl;
            event<std::string> e(buf);
            str.notify_subscribers(e);
        }
    }

};

#endif