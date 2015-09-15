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

#include "Transform.h"

Transform::Transform(void){
    this->localModelMatrix = glm::mat4(1.0f);
    this->globalModelMatrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f);
    this->rotation = glm::quat(glm::vec3(0.0f));
    this->scale = glm::vec3(1.0f);
}

Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale){
    this->localModelMatrix = glm::mat4(1.0f);
    this->globalModelMatrix = glm::mat4(1.0f);
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

void Transform::TranslateLocal(float x, float y, float z){
    glm::mat4 mat(1.0f);
    mat *= glm::toMat4(this->rotation);
    mat = glm::translate(mat, glm::vec3(x, y, z));
    this->position.x += mat[3][0];
    this->position.y += mat[3][1];
    this->position.z += mat[3][2];
    //PrintMat4(mat);
    //std::cout << mat[3][0] << " : " << mat[3][1] << " : " << mat[3][1] << std::endl;
}

void Transform::RotateLocal(float x, float y, float z){
    this->rotation = glm::rotate(this->rotation, x, glm::vec3(1, 0, 0));
    this->rotation = glm::rotate(this->rotation, y, glm::vec3(0, 1, 0));
    this->rotation = glm::rotate(this->rotation, z, glm::vec3(0, 0, 1));
}

void Transform::ScaleLocal(float x, float y, float z){

}

void Transform::TranslateGlobal(float x, float y, float z){
    this->position += glm::vec3(x, y, z);
}

void Transform::RotateGlobal(float x, float y, float z){
    glm::quat qu;
    qu = glm::rotate(qu, x, glm::vec3(1, 0, 0));
    qu = glm::rotate(qu, y, glm::vec3(0, 1, 0));
    qu = glm::rotate(qu, z, glm::vec3(0, 0, 1));
    this->rotation = qu * this->rotation;
}

void Transform::ScaleGlobal(float x, float y, float z){
    this->scale += glm::vec3(x, y, z);
}

glm::mat4 Transform::calcLocalModelMatrix(){
    glm::mat4 finalMat(1.0f);
    finalMat = glm::translate(finalMat, this->position);
    finalMat *= glm::toMat4(this->rotation);
    finalMat = glm::scale(finalMat, this->scale);
    this->localModelMatrix = finalMat;
    return finalMat;
}
