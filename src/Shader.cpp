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

#include "Shader.h"

Manager<Shader> Shader::manager;

Shader::Shader(String name){
    this->object.name = name;
    this->object.id = manager.Push(this);
	this->shaderID = 0;
}

Shader::~Shader(void){
	/* Make sure the default shader is binded */
	glUseProgram(0);

	if(this->shaderID != 0){
		glDeleteProgram(this->shaderID);
	}else{
		std::cout << "Shader never loaded!" << std::endl;
	}
}

void Shader::GetUniformLocations(){
	GLint numActiveUniforms = 0;
	glGetProgramInterfaceiv(this->shaderID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

	std::vector<GLchar> nameData(256);
	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_TYPE);
	properties.push_back(GL_ARRAY_SIZE);
	std::vector<GLint> values(properties.size());

	std::vector<String> uniformNames;

	std::cout << "Total active uniforms: " << numActiveUniforms << std::endl;
	for(int attrib = 0; attrib < numActiveUniforms; attrib++){
		glGetProgramResourceiv(this->shaderID, GL_UNIFORM, attrib, properties.size(),
			&properties[0], values.size(), NULL, &values[0]);
		GLsizei actualLen = 0;
		nameData.resize(properties[0]);
		/* Get its name */
		glGetProgramResourceName(this->shaderID, GL_UNIFORM, attrib, nameData.size(), &actualLen, &nameData[0]);
		String name((char*)&nameData[0], actualLen);
		/* Get its location */
		uniforms[name] = glGetProgramResourceLocation(this->shaderID, GL_UNIFORM, name.c_str());
	}

	std::map<String, GLuint>::iterator iter;
	for(iter = uniforms.begin(); iter != uniforms.end(); ++iter){
		std::cout << iter->first << " : " << iter->second << std::endl;
	}

}

/* Loads a shader (the entire shader is stored inside the same file) */
bool Shader::Load(String location){
    this->object.additonalProperties["fileLocation"] = location;
	// Has it already been loaded? (if it == 0 then no)
	if(this->shaderID == 0){
		String vertexShaderCode("");
		String fragmentShaderCode("");

        std::ifstream file(location.c_str(), std::ios::in);

		if(file.is_open()){
			String line;
			int codeToAddTo = -1; // -1 = none, 0 = vertex, 1 = fragment
			while(std::getline(file, line)){
				if(line.substr(0, 10) == "[(vertex)]")
					codeToAddTo = 0;
				else if(line.substr(0, 12) == "[(fragment)]")
					codeToAddTo = 1;
				else{
					if(codeToAddTo == 0) vertexShaderCode += '\n' + line;
					if(codeToAddTo == 1) fragmentShaderCode += '\n' + line;
				}
			}
			file.close();

            if(vertexShaderCode == "") std::cout << location << " missing vertex shader!" << std::endl;
            if(fragmentShaderCode == "") std::cout << location << " missing fragment shader!" << std::endl;

			// If both strings have something in them
			if(vertexShaderCode != "" && fragmentShaderCode != ""){
			
				// Compile both the shaders
				GLint result = GL_FALSE;
				int infoLogLen;
				char const* vertexSourcePtr = vertexShaderCode.c_str();
				char const* fragmentSourcePtr = fragmentShaderCode.c_str();

				// Compile the vertex shader and check it
				GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vertexShaderID, 1, &vertexSourcePtr, NULL);
				glCompileShader(vertexShaderID);
				glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
				glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLen);
				std::vector<char> vertShaderErrMsg(infoLogLen);
				glGetShaderInfoLog(vertexShaderID, infoLogLen, NULL, &vertShaderErrMsg[0]);
				if(vertShaderErrMsg.size() != 0)
					std::fprintf(stdout, "%s\n", &vertShaderErrMsg[0]);

				// Compile the fragment shader and check it
				GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fragmentShaderID, 1, &fragmentSourcePtr, NULL);
				glCompileShader(fragmentShaderID);
				glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
				glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLen);
				std::vector<char> fragmentShaderErrMsg(infoLogLen);
				glGetShaderInfoLog(fragmentShaderID, infoLogLen, NULL, &fragmentShaderErrMsg[0]);
				if(fragmentShaderErrMsg.size() != 0)
					std::fprintf(stdout, "%s\n", &fragmentShaderErrMsg[0]);

				// Link the program and check it
				this->shaderID = glCreateProgram();
				glAttachShader(this->shaderID, vertexShaderID);
				glAttachShader(this->shaderID, fragmentShaderID);
				glLinkProgram(this->shaderID);
				glGetProgramiv(this->shaderID, GL_LINK_STATUS, &result);
				glGetProgramiv(this->shaderID, GL_INFO_LOG_LENGTH, &infoLogLen);
				std::vector<char> programErrMsg(std::max(infoLogLen, int(1)));
				glGetProgramInfoLog(this->shaderID, infoLogLen, NULL, &programErrMsg[0]);
				if(programErrMsg.size() != 0)
					std::fprintf(stdout, "%s\n", &programErrMsg[0]);

				// Delete the shaders now (we dont need them anymore)
				glDeleteShader(vertexShaderID);
				glDeleteShader(fragmentShaderID);

				// Get all the uniforms from this shader
				this->GetUniformLocations();
				return true;
			}
		}else{
            std::cout << "Unable to load shader: " << location << std::endl;
		}
	}else{
        std::cout << "Shader aleady loaded!" << location << std::endl;
	}

	return false;
}

void Shader::Bind(){
	glUseProgram(this->shaderID);
}

GLuint Shader::GetUniform(String name){
	std::map<String, GLuint>::iterator it;
	it = this->uniforms.find(name);
	if(it == this->uniforms.end()){
        std::cout << "Uniform '" << name << "' does not exist in shader " << this->object.name << "!" << std::endl;
		return -1;
	}else{
		return it->second;
	}
}
