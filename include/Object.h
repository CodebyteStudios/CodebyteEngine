#ifndef OBJECT_H
#define OBJECT_H
#pragma once

#include "Tools.h"

class Object{
public:
    ulong id;
    String name;
    std::map<String, String> additonalProperties;
    Object(){

    }
    ~Object(){

    }
    String GetProperty(String propName){
        if(this->additonalProperties.find(propName) != this->additonalProperties.end()){
            return this->additonalProperties.at(propName);
        }
        return "";
    }
};

#endif /* OBJECT_H */
