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

/* USED FOR FUNCTIONS AND STRUCTURES THAT ARE USED OFTEN THROUGH OUT THE CODE */
#ifndef TOOLS_H
#define TOOLS_H

#define GLEW_STATIC
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <time.h>
#include <sstream>
#include <drawlib.h>
#include <lodepng.h>
#include <btBulletDynamicsCommon.h>
#include <duktape.h>

#include <stdio.h>
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

inline int bullet_btInfinityMask(){
   return btInfinityMask;
}

#define PI 3.141592653589793

typedef std::string String;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char byte;
typedef std::vector<String> StringArray;

struct Color{
     byte r, b, g, a;
};

struct Vertex{
public:
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texcoord[2];
	Vertex(glm::vec3 a, glm::vec3 b, glm::vec2 c){
		// Set Position
		position[0] = (GLfloat)a.x;
		position[1] = (GLfloat)a.y;
		position[2] = (GLfloat)a.z;
		// Set Normal
		normal[0] = (GLfloat)b.x;
		normal[1] = (GLfloat)b.y;
		normal[2] = (GLfloat)b.z;
		// Set Texcoords
		texcoord[0] = (GLfloat)c.x;
		texcoord[1] = (GLfloat)c.y;
	}
};

StringArray LoadFile(String fileLocation);

StringArray SplitString(String s, String delimiter);

String StringStrip(String s, String charsToRemove);

String RemoveCharToChar(String s, char start, char end);

bool FileExists(String filename);

void Dbug(int i);

void PrintMat4(glm::mat4 mat);

String CurrentWorkingDirectory();

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#endif /* TOOLS_H */
