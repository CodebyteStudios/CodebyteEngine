/*********************************************************************************
Codebyte Engine

Copyright (c) 2014 Cameron Kline

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*********************************************************************************/

#ifndef MANAGER_H
#define MANAGER_H
#pragma once

#include "Tools.h"

template <class type> class Manager{
public:
    ulong nextID;
    std::vector<type*> objects;
    Manager(void){
        this->nextID = 0;
    }

    ~Manager(void){
        this->Cleanup();
    }

    type* At(ulong id){
        for(uint i = 0; i < (uint)this->objects.size(); i++){
            if(this->objects.at(i)->object.id == id) return this->objects.at(i);
        }
        return NULL;
    }

    type* At(String name){
        for(uint i = 0; i < (uint)this->objects.size(); i++){
            if(this->objects.at(i)->object.name == name) return this->objects.at(i);
        }
        std::cout << "Object with name: " << name << " does not exist!" << std::endl;
        return NULL;
    }

    type* At(String prop, String value){
        for(uint i = 0; i < (uint)this->objects.size(); i++){
            if(this->objects.at(i)->object.GetProperty(prop) == value) return this->objects.at(i);
        }
        return NULL;
    }

    ulong Push(type* object){
        this->objects.push_back(object);
        this->nextID++;
        return this->nextID;
    }

    void Cleanup(){
        for(uint i = 0; i < (uint)this->objects.size(); i++)
            delete this->objects.at(i);
        this->objects.clear();
        this->nextID = 0;
    }
};

#endif /* MANAGER_H */
