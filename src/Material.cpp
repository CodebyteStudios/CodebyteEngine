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

#include "Material.h"

Manager<Material> Material::manager;

Material::Material(String name){
    this->shader = NULL;
    this->object.name = name;
    this->object.id = manager.Push(this);
}

Material::~Material(void){
	this->shader = NULL;
}

void Material::Load(String location){
    this->object.additonalProperties["fileLocation"] = location;

    StringArray contents = LoadFile(location);

    for(uint i = 0; i < (uint)contents.size(); i++){
        String line = contents.at(i);
        // Remove comment from that line (if one exist)
        line = RemoveCharToChar(line, '#', '\n');
        // The first line should always be the shader name
        if(i == 0){
            if(line.substr(0, 1) == "["){
                this->shader = Shader::manager.At(StringStrip(line, "[]"));
                if(this->shader == NULL){
                    std::cout << "Unable to load material at '" << location << "': shader does not exist!" << std::endl;
                    return;
                }
            }
        }else{
            StringArray tokens = SplitString(line, ":");
            if((uint)tokens.size() == 3){
                // tokens.at(0) = type, tokens.at(1) = name, tokens.at(2) = value
                if(tokens.at(0) == "float"){
                    this->uniforms.floats[tokens.at(1)] = std::atof(tokens.at(2).c_str());
                }else if(tokens.at(0) == "vec2"){
                    StringArray parts = SplitString(tokens.at(2), " ");
                    this->uniforms.vec2s[tokens.at(1)] = glm::vec2(std::atof(parts.at(0).c_str()), std::atof(parts.at(1).c_str()));
                }else if(tokens.at(0) == "vec3"){
                    StringArray parts = SplitString(tokens.at(2), " ");
                    this->uniforms.vec3s[tokens.at(1)] = glm::vec3(std::atof(parts.at(0).c_str()), std::atof(parts.at(1).c_str()), std::atof(parts.at(2).c_str()));
                }else if(tokens.at(0) == "vec4"){
                    StringArray parts = SplitString(tokens.at(2), " ");
                    this->uniforms.vec4s[tokens.at(1)] = glm::vec4(std::atof(parts.at(0).c_str()), std::atof(parts.at(1).c_str()), std::atof(parts.at(2).c_str()), std::atof(parts.at(3).c_str()));
                }else if(tokens.at(0) == "texture"){
                    Texture * texture = Texture::manager.At("fileLocation", CurrentWorkingDirectory() + "/" + tokens.at(2));
                    if(texture != NULL)
                        this->uniforms.textures[tokens.at(1)] = texture->textureId;
                    else{
                        this->uniforms.textures[tokens.at(1)] = (GLuint)0;
                    }
                }
            }else{
                std::cout << "Material file '" << location << "' syntax error, on line " << (i + 1) << " : there must be 3 args here!" << std::endl;
            }
        }
    }
}

void Material::Upload(Shader * shader){
    if(this->shader != NULL){

        for(std::map<String, GLfloat>::iterator floats_iter = this->uniforms.floats.begin(); floats_iter != this->uniforms.floats.end(); ++floats_iter){
            glUniform1f(shader->GetUniform(floats_iter->first), floats_iter->second);
        }

        for(std::map<String, glm::vec2>::iterator vec2s_iter = this->uniforms.vec2s.begin(); vec2s_iter != this->uniforms.vec2s.end(); ++vec2s_iter){
            glUniform2f(shader->GetUniform(vec2s_iter->first), vec2s_iter->second.x, vec2s_iter->second.y);
        }

        for(std::map<String, glm::vec3>::iterator vec3s_iter = this->uniforms.vec3s.begin(); vec3s_iter != this->uniforms.vec3s.end(); ++vec3s_iter){
            glUniform3f(shader->GetUniform(vec3s_iter->first), vec3s_iter->second.x, vec3s_iter->second.y, vec3s_iter->second.z);
        }

        for(std::map<String, glm::vec4>::iterator vec4s_iter = this->uniforms.vec4s.begin(); vec4s_iter != this->uniforms.vec4s.end(); ++vec4s_iter){
            glUniform4f(shader->GetUniform(vec4s_iter->first), vec4s_iter->second.x, vec4s_iter->second.y, vec4s_iter->second.z, vec4s_iter->second.w);
        }

        int i = 0;
        for(std::map<String, GLuint>::iterator textures_iter = this->uniforms.textures.begin(); textures_iter != this->uniforms.textures.end(); ++textures_iter){
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures_iter->second);
            glUniform1i(shader->GetUniform(textures_iter->first), i);
            i++;
        }

    }else{
        std::cout << "Error! Material->UploadSettings: shader is null" << std::endl;
    }
}
