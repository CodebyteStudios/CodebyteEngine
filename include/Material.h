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

#ifndef MATERIAL_H
#define MATERIAL_H
#pragma once

#include "Tools.h"
#include "Shader.h"
#include "Texture.h"

#include "Manager.h"
#include "Object.h"

/*	A Material is a collection of settings used for rendering
	They will have a shader, and settings for this shader 
	which may include:
		-> Diffuse Color
		-> Specular Color
		-> Ambient Color
		-> Textures
		-> etc.
*/

struct GLDataTypeList{
public:
    std::map<String, GLfloat>    floats;
    std::map<String, glm::vec2>  vec2s;
    std::map<String, glm::vec3>  vec3s;
    std::map<String, glm::vec4>  vec4s;
    std::map<String, GLuint>     textures;
};

class Material{
public:
    static Manager<Material> manager;

// Instance stuff
public:
    Object object;
	Shader * shader;
    GLDataTypeList uniforms;
    Material(String name);
	~Material(void);
    void Load(String location);
    void Upload(Shader * shader);
};

#endif /* MATERIAL_H */
