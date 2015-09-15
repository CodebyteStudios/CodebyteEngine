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

#include "Mesh.h"

Manager<Mesh> Mesh::manager;

Mesh::Mesh(String name){
    this->object.name = name;
    this->object.id = manager.Push(this);
	/* Create the VAO and bind it*/
	glGenVertexArrays(1, &this->vertexArrayID);
	glBindVertexArray(this->vertexArrayID);

	/* Create a VBO for vertex data (vertices, normals, texcoords)*/
	/* Binding it stores it in the VAO (I think)*/
	/* (this stays empty till we call SetData)*/
	glGenBuffers(1, &this->dataBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, this->dataBufferID);

	/* Create a VBO for index data (one Vertex per indice)*/
	/* Binding it stores it in the VAO (I think)*/
	/* (this stays empty till we call SetData too)*/
	glGenBuffers(1, &this->indiceBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indiceBufferID);

	/* Setup the VAO */
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	/* Unbind the VAO */
	glBindVertexArray(0);
}

Mesh::~Mesh(void){
	/* Free GPU allocated memory */
	glDeleteBuffers(1, &this->dataBufferID);
	glDeleteBuffers(1, &this->indiceBufferID);
	glDeleteVertexArrays(1, &this->vertexArrayID);
}

/* Loads an obj (blender format) */
bool Mesh::Load(String location){
    this->object.additonalProperties["fileLocation"] = location;
	/* Open the file and get the data from it */
    StringArray data = LoadFile(location);

	/* Some vectors to hold data with */
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<Vertex> vertexs;
	std::vector<GLuint> indices;

    uint currentLine = 0;
	while(currentLine != (uint)data.size()){
		StringArray tokens = SplitString(data.at(currentLine), " ");

		if(tokens.at(0) == "v"){
			/* Make sure it has 3 values in it (x, y, z) */
			if((uint)tokens.size() - 1 == 3)
				/* Add it to the vertices vector */
                vertices.push_back(glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str())));
			else{
				std::cout << "v at line " << currentLine << " must have 3 values!" << std::endl;
				return false;
			}
		}else if(tokens.at(0) == "vn"){
			/* Make sure it has 3 values in it (x, y, z) */
			if((uint)tokens.size() - 1 == 3)
				/* Add it to the normals vector */
                normals.push_back(glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str())));
			else{
				std::cout << "vn at line " << currentLine << " must have 3 values!" << std::endl;
				return false;
			}
		}else if(tokens.at(0) == "vt"){
			/* Make sure it has 2 values in it (u, v) */
			if((uint)tokens.size() - 1 == 2)
				/* Add it to the normals vector */
                texcoords.push_back(glm::vec2(atof(tokens[1].c_str()), atof(tokens[2].c_str())));
			else{
				std::cout << "vt at line " << currentLine << " must have 3 values!" << std::endl;
				return false;
			}
		}else if(tokens.at(0) == "f"){
			/* Make sure it has 3 values in it (v, v, v) */
			if((uint)tokens.size() - 1 == 3){

				/* There should be three vertexs to process (one face)*/
				for(int i = 0; i < 3; i++){
					/* vertice, texcoord, normal */
					StringArray vtn = SplitString(tokens[i + 1], "/");

					/* Get the vertice, texcoord, and normal */
                    uint v = (vtn[0] == "") ? 0 : atoi(vtn[0].c_str()) - 1;
                    uint t = (vtn[1] == "") ? 0 : atoi(vtn[1].c_str()) - 1;
                    uint n = (vtn[2] == "") ? 0 : atoi(vtn[2].c_str()) - 1;

					/* Add the vertex to the vertexs vector */
					vertexs.push_back(Vertex(
						(v < (uint)vertices.size()) ? vertices.at(v) : glm::vec3(0.0f),
						(n < (uint)normals.size()) ? normals.at(n) : glm::vec3(0.0f),
						(t < (uint)texcoords.size()) ? texcoords.at(t) : glm::vec2(0.0f)
					));

					/* Add that indice to the indices vector */
					indices.push_back(indices.size());
				}

			}else{
				std::cout << "f at line " << currentLine << " must have 3 values!" << std::endl;
				return false;
			}
		}
		currentLine++;
	}

	this->SetData(vertexs, indices);
	return true;
}

bool Mesh::LoadTerrain(String location){
    this->object.additonalProperties["fileLocation"] = location;

    std::vector<Vertex> verts;
    std::vector<GLuint> indices;

    // Load the image
    std::vector<unsigned char> terrainImageData;
    uint width, height;
    uint error = lodepng::decode(terrainImageData, width, height, location.c_str());

    std::cout << "error code: " << error << std::endl;

    // Push all the vertices and indices
    for(uint y = 0; y < height; y++){
        for(uint x = 0; x < width; x++){
            verts.push_back(Vertex(glm::vec3(x, (int)terrainImageData.at((y * width + x) * 4) / 10.0f, y), glm::vec3(0, 0, 0), glm::vec2(x / 1000.0f, y / 1000.0f)));
            if(x < width - 1 && y < height - 1){
                indices.push_back(y * width + x);
                indices.push_back(y * width + x + 1);
                indices.push_back((y + 1) * width + (x + 1));
                indices.push_back((y + 1) * width + (x + 1));
                indices.push_back((y + 1) * width + x);
                indices.push_back(y * width + x);
            }
        }
    }

    this->SetData(verts, indices);
}

void Mesh::SetData(std::vector<Vertex> verts, std::vector<GLuint> indices){
	/* Set indice count so we can use it later */
	this->indiceCount = indices.size();

	/* Bind the VAO just in case */
	glBindVertexArray(this->vertexArrayID);

	// Bind and set the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, this->dataBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts.size(), &verts[0], GL_STATIC_DRAW);

	// Now bind and set the indice data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indiceBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	/* Unbind the VAO */
	glBindVertexArray(0);
}

void Mesh::Render(){
    // Render the object
    glBindVertexArray(this->vertexArrayID);
    glDrawElements(GL_TRIANGLES, this->indiceCount, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

