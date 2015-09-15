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

#include "GUI.h"

using namespace GUI;

Element::Element(){

}

Element::~Element(){

}

DrawLib::Surface *      GUIManager::surface;
Mesh *                  GUIManager::guiQuad;
Shader *                GUIManager::guiShader;
GLuint                  GUIManager::glTexId;
ulong                   GUIManager::nextElementID;
std::vector<Element*>   GUIManager::elements;

void GUIManager::Setup(uint width, uint height){
    if(guiQuad == NULL){
        std::vector<Vertex> verts;
        std::vector<GLuint> indices;
        verts.push_back(Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
        verts.push_back(Vertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
        verts.push_back(Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
        verts.push_back(Vertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));

        indices.push_back(0); indices.push_back(1); indices.push_back(2);
        indices.push_back(2); indices.push_back(3); indices.push_back(0);

        guiQuad = new Mesh("GUI");
        guiQuad->SetData(verts, indices);
    }

    if(guiShader == NULL){
        guiShader = new Shader("GUI");
        guiShader->Load("Resources/Shaders/gui.glsl");
    }

    Resize(width, height);
}

void GUIManager::Cleanup(){
    if(surface != NULL) delete surface;
    glTexId = 0;
    nextElementID = 0;
    glDeleteTextures(1, &glTexId);
    for(uint i = 0; i < (uint)elements.size(); i++)
        delete elements.at(i);
    elements.clear();
}

void GUIManager::UploadSurface(){
    std::cout << surface->Width() << ":" << surface->Height() << std::endl;
    glBindTexture(GL_TEXTURE_2D, glTexId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->Width(), surface->Height(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, surface->Data());
}

void GUIManager::Render(){
    guiShader->Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexId);
    glUniform1i(guiShader->GetUniform("textureSampler"), 0);
    glm::mat4 modelMat(1.0f);
    modelMat = glm::scale(modelMat, glm::vec3(surface->Width(), surface->Height(), 1.0f));
    glm::mat4 final = glm::ortho(0.0f, (float)surface->Width(), (float)surface->Height(), 0.0f, 0.0f, 1000.0f) * glm::mat4(1.0f) * modelMat;
    glUniformMatrix4fv(guiShader->GetUniform("MVP"), 1, GL_FALSE, &final[0][0]);
    guiQuad->Render();
}

Element* GUIManager::At(ulong id){
    for(uint i = 0; i < (uint)elements.size(); i++){
        if(elements.at(i)->id == id) return elements.at(i);
    }
    return NULL;
}

void GUIManager::Resize(uint width, uint height){
    if(surface != NULL) delete surface;
    surface = new DrawLib::Surface(width, height);

    //DrawLib::COLOR color;
    //surface->GetPixel(0, 0, &color);

    surface->DrawRect(DrawLib::RECT((surface->Width() / 2), (surface->Height() / 2) - 7, 2, 16), DrawLib::COLOR(0, 0, 0, 255), DrawLib::COLOR(0, 0, 0, 0), 0);
    surface->DrawRect(DrawLib::RECT((surface->Width() / 2) - 7, (surface->Height() / 2), 16, 2), DrawLib::COLOR(0, 0, 0, 255), DrawLib::COLOR(0, 0, 0, 0), 0);

    //surface->DrawRoundedRect(
    //    DrawLib::RECT(600, 300, 100, 100),
    //    DrawLib::COLOR(255, 0, 0, 155),
    //    DrawLib::COLOR(0, 255, 0, 128), 0, 32);

    //std::vector<DrawLib::POINT> points;
    //points.push_back(DrawLib::POINT(100.0f, 100.0f));
    //points.push_back(DrawLib::POINT(200.0f, 200.0f));
    //points.push_back(DrawLib::POINT(0.0f, 200.0f));
    //surface->FillCircle(DrawLib::POINT(200, 100), 50, DrawLib::COLOR(255, 0, 0, 255), DrawLib::COLOR(255, 0, 255, 255));

    UploadSurface();
}
