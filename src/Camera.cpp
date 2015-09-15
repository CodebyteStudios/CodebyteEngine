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

#include "Camera.h"

Manager<Camera> Camera::manager;

Camera::Camera(String name, float fovy, float aspect){
    this->object.name = name;
    this->object.id = manager.Push(this);

    this->projNeedsUpload = false;
	this->fovy = fovy;
	this->aspect = aspect;
	this->RecalcProjMatrix();
}

Camera::~Camera(void){

}

void Camera::RecalcProjMatrix(){
	this->projectionMatrix = glm::perspective(this->fovy, this->aspect, 0.1f, 1000.0f);
}

void Camera::SetFovy(float value){
	this->fovy = value;
	this->RecalcProjMatrix();
}

void Camera::SetAspect(float value){
	this->aspect = value;
	this->RecalcProjMatrix();
}

glm::mat4 Camera::GetProjectionMatrix(){
	return this->projectionMatrix;
}
