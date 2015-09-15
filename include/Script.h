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

#ifndef SCRIPT_H
#define SCRIPT_H
#pragma once

#include "Tools.h"
#include "JavascriptWrappers.h"

#include "Manager.h"
#include "Object.h"

class ScriptManager;

class Script{
public:
    static ScriptManager manager;

// Instance stuff
public:
    Object object;
    int script_obj_index;
    Script(String name, String objectName);
    ~Script(void);
};

// A custom manager class
class ScriptManager: public Manager<Script>{
public:
    duk_context * ctx;
    void Setup();
    void LoadScript(String fileLocation);
    void Update(); // Calls each scripts update function if it exist
    void Cleanup();
};

#endif /* SCRIPT_H */
