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

#ifndef GUI_H
#define GUI_H
#pragma once

#include "Tools.h"
#include "Mesh.h"

namespace GUI{

    struct Attibutes{
        int x, y, width, height;
        Color backgroundColor;
    };

    class Element{
    /*private:
        static ulong nextID;
        static std::vector<Element*> elements;
    public:
        static Mesh* At(ulong id);
        static Mesh* At(String name);
        static Mesh* WithLocation(String location);
        static void Cleanup();*/
    public:
        ulong id;
        String name;
        Attibutes attributes;
        Element();
        ~Element();
    };

    /*
    class Layout{
    private:
        static ulong nextID;
        static std::vector<Element*> elements;
    public:
        static Mesh* At(ulong id);
        static Mesh* At(String name);
        static Mesh* WithLocation(String location);
        static void Cleanup();
    };*/

    class GUIManager{
    private:
        static DrawLib::Surface * surface;
        static Mesh * guiQuad;
        static Shader * guiShader;
        static GLuint glTexId;
        static ulong nextElementID;
        static std::vector<Element*> elements;
    public:
        static void Setup(uint width, uint height);
        static Element* At(ulong id);
        static void Cleanup();
        static void UploadSurface();
        static void Render();
        static void Resize(uint width, uint height);
    };

}

#endif /* GUI_H */
