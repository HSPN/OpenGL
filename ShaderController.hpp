//
//  ShaderController.hpp
//  OpenGL
//
//  Created by 0 on 04/02/2020.
//  Copyright © 2020 0. All rights reserved.
//

#ifndef ShaderController_hpp
#define ShaderController_hpp

#include "Singleton.cpp"

#include <stdio.h>
#include "Graphics.h"

class ShaderController:public Singleton<ShaderController>{
public:
    GLuint makeProgram();
    
private:
    GLuint compileShader(const char *fileName, GLuint shaderType);
    char *readFile(const char *fileName);
    bool compileErrorCheck(GLuint shader);
};
#endif /* ShaderController_hpp */
