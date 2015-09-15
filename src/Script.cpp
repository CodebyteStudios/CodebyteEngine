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

#include "Script.h"

ScriptManager Script::manager;

void ScriptManager::Setup(){
    ctx = duk_create_heap_default();
    if (!ctx) { std::cout << "Failed to create duk_context!" << std::endl; }
    duk_push_global_object(ctx);
    JavascriptWrappers::InjectWrappers(ctx);
}

void ScriptManager::LoadScript(String fileLocation){
    if(ctx != NULL){
        if(FileExists(fileLocation)){
            duk_eval_file(ctx, fileLocation.c_str());
        }else{
            std::cout << "Script file at location '" << fileLocation << "' does not exist!" << std::endl;
        }
    }else{
        std::cout << "Script:LoadScript ctx is NULL, make sure to call Script::Setup!" << std::endl;
    }
}

void ScriptManager::Update(){
    for(uint i = 0; i < (uint)objects.size(); i++){
        Script* script = objects.at(i);
        if(script->script_obj_index > -1){
            if(duk_has_prop_string(ctx, script->script_obj_index, "update")){
                duk_push_string(ctx, "update");
                duk_call_prop(ctx, script->script_obj_index, 0);
            }
        }
    }
}

void ScriptManager::Cleanup(){
    for(uint i = 0; i < (uint)objects.size(); i++)
        delete objects.at(i);
    objects.clear();
    nextID = 0;
    duk_destroy_heap(ctx);
}

Script::Script(String name, String objectName){
    this->object.name = name;
    this->object.id = manager.Push(this);

    // make sure that object exist
    if(manager.ctx != NULL){
        duk_get_prop_string(manager.ctx, 0, objectName.c_str());
        if(duk_get_type(manager.ctx, -1) != DUK_TYPE_UNDEFINED){
            // now create it and store its position on the stack for later use
            duk_new(manager.ctx, 0);
            this->script_obj_index = duk_get_top_index(manager.ctx);
        }else{
            this->script_obj_index = -1;
            std::cout << "Javascript object '" << objectName << "' does not exist!" << std::endl;
        }
    }else{
        std::cout << "ctx is NULL, make sure to call Script::Setup!" << std::endl;
    }
}

Script::~Script(){

}
