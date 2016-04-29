#ifndef INCLUDED_STREAM_FACTORY
#define INCLUDED_STREAM_FACTORY

#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "stream.h"
#include "event.h"

class file_stream{

    std::ifstream file;
    char delim;
    std::string filepath;

public:
    file_stream(std::string filename, char _delim = '\n') :
    file(filename), delim(_delim), filepath(filename) {};

    file_stream(const file_stream &fs) :
    file(fs.filepath), delim(fs.delim), filepath(fs.filepath) {};

    void operator()(stream<std::string> &str);

};

void file_stream::operator()(stream<std::string> &str){
    bool flag = true;

    if(!file){
        flag = false;
        str.error();
    }

    std::string buf;
    while(flag && !file.eof()){
        std::getline(file, buf, delim);
        event<std::string> e(buf);
        str.notify(e);
    }

    str.end_stream();
}

#endif