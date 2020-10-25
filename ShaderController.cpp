//  ShaderController.cpp
//  OpenGL
//
//  Created by 0 on 04/02/2020.
//  Copyright © 2020 0. All rights reserved.

#include "ShaderController.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

GLuint ShaderController::makeProgram()
{
    GLuint program = glCreateProgram();
    std::vector<GLuint> shaders;
    GLuint shader;
    
    shaders.push_back(compileShader("vs.txt", GL_VERTEX_SHADER));
    shaders.push_back(compileShader("tcs.txt", GL_TESS_CONTROL_SHADER));
    shaders.push_back(compileShader("tes.txt", GL_TESS_EVALUATION_SHADER));
    //shaders.push_back(compileShader("gs.txt", GL_GEOMETRY_SHADER));
    shaders.push_back(compileShader("fs.txt", GL_FRAGMENT_SHADER));

    while (!shaders.empty()) {
        shader = shaders.back();
        shaders.pop_back();
        if (shader > 0)
            glAttachShader(program, shader);
        glDeleteShader(shader);
    }

    glLinkProgram(program);
    int result;
    GLchar errorLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(program, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
        return false;
    }
    return program;
}


GLuint ShaderController::compileShader(const char *fileName, GLuint shaderType)
{
    GLuint shader;
    const char * code = readFile(fileName);
    std::cout << "shader \"" << fileName << "\"";
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    if (compileErrorCheck(shader))
        std::cout << "is compiled successfully." << std::endl;
    else
        std::cout << "is failed to compile." << std::endl;

    delete code;
    return shader;
}

char *ShaderController::readFile(const char *fileName)
{
    std::ifstream fin;
    long fileLength;
    char *data;
    char *cursor;

    fin.open(fileName, std::ios::in);
    fin.seekg(0, std::ios::end);
    fileLength = fin.tellg();
    fin.seekg(0, std::ios::beg);

    data = new char[fileLength + 1];
    cursor = data;

    while (fin.good())
    {
        fin.getline(cursor, fileLength);
        cursor += strnlen(cursor, fileLength);
        if ((int)(cursor - data) == fileLength) break;
        *(cursor++) = '\n';
    }
    *cursor = '\0';
    std::cout << "read " << (int)(cursor - data) << "/" << fileLength << " byte - ";
    fin.close();
    return data;
}

bool ShaderController::compileErrorCheck(GLuint shader)
{
    GLint isCompiled;
    GLint maxLength = 0;
    std::vector<GLchar> errorLog;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    return isCompiled == GL_TRUE;
}
