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

#ifndef MESH_H
#define MESH_H
#pragma once

#include "Tools.h"
#include "Transform.h"
#include "Material.h"

#include "Manager.h"
#include "Object.h"

class Mesh{
public:
    static Manager<Mesh> manager;

// Instance stuff
private:
	uint indiceCount;
	GLuint vertexArrayID;	/* Describes the dataBuffer */
	GLuint dataBufferID;	/* Hold vertices, normals, and texcoords data */
	GLuint indiceBufferID;	/* Indices used for renderering */
public:
    Object object;
    Mesh(String name);
	~Mesh(void);
    bool Load(String location);
    bool LoadTerrain(String location);
	void SetData(std::vector<Vertex> verts, std::vector<GLuint> indices);
    void Render();
};

#endif /* MESH_H */
