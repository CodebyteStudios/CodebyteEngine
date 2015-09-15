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

#include "Tools.h"

StringArray LoadFile(String fileLocation){
	StringArray lines;
	std::ifstream file(fileLocation.c_str());
	if(file.is_open()){
		// Load the file into memory
		std::string line;
		while(std::getline(file, line)){
			int s = line.find_first_not_of(" \t");
			//std::cout << "Start[ " << s << " ]" << line.substr(s, line.npos) << std::endl;
			if(s != -1)
				lines.push_back(line.substr(s, line.npos));
		}
	}else{
        std::cout << "Unable to load file: '" << fileLocation << "'" << std::endl;
	}
	return lines;
}

StringArray SplitString(String s, String delimiter){
	StringArray tokens;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos){
		if(s.substr(0, 1) == "\""){
			size_t pos2 = s.substr(1).find("\"");
			if(pos2 != std::string::npos){
				tokens.push_back(s.substr(0, pos2 + 2));
				s.erase(0, pos2 + 2);
			}
		}else{
			tokens.push_back(s.substr(0, pos));
			s.erase(0, pos + delimiter.length());
		}
	}
	if(s != "")
		tokens.push_back(s);

	return tokens;
}

String StringStrip(String s, String charsToRemove){
    for(uint i = 0; i < (uint)charsToRemove.size(); i++){
        s.erase(std::remove(s.begin(), s.end(), charsToRemove.at(i)));
    }
    return s;
}

String RemoveCharToChar(String s, char start, char end){
    std::string::size_type first = s.find_first_of(start);
    std::string::size_type second;
    if(first != std::string::npos){
        second = s.find_first_of(end, first + 1);
        if(second != std::string::npos){
            s.erase(first, (second - first));
        }
    }
    return s;
}

bool FileExists(String filename){
  std::ifstream file(filename);
  return file;
}

void Dbug(int i){
    std::cout << "Dbug: " << i << std::endl;
}

void PrintMat4(glm::mat4 mat){
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << mat[0][0] << " : " << mat[1][0] << " : " << mat[2][0] << " : " << mat[3][0] << std::endl;
    std::cout << mat[0][1] << " : " << mat[1][1] << " : " << mat[2][1] << " : " << mat[3][1] << std::endl;
    std::cout << mat[0][2] << " : " << mat[1][2] << " : " << mat[2][2] << " : " << mat[3][2] << std::endl;
    std::cout << mat[0][3] << " : " << mat[1][3] << " : " << mat[2][3] << " : " << mat[3][3] << std::endl;
}

String CurrentWorkingDirectory(){
    char cCurrentPath[FILENAME_MAX];

    if(!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))){
        return "";
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    return String(cCurrentPath);
}
