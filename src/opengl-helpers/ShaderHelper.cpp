#include <stdexcept>

#include "ShaderHelper.h"
#include "../misc/Utils.h"

#include <iostream>
GLuint ShaderHelper::loadShader(string shaderPath, ShaderType shaderType){
    GLuint shaderId = glCreateShader((GLenum) shaderType);
    string shaderCode = Utils::readTextFile(shaderPath).c_str();
    char const* shaderCodeAsCString = shaderCode.c_str();

    glShaderSource(shaderId, 1, &shaderCodeAsCString, NULL);
    glCompileShader(shaderId);

    GLint logLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength != 0){
        GLchar errorMessage[logLength + 1];
        glGetShaderInfoLog(shaderId, logLength, NULL, errorMessage);
        throw runtime_error("Cannot compile shader, message: " + string(errorMessage));
    }

    return shaderId;
}

GLuint ShaderHelper::loadProgram(string vertexShaderPath, string pixelShaderPath){
    GLuint vertexShaderId = loadShader(vertexShaderPath, ShaderType::VERTEX);
    GLuint pixelShaderId = loadShader(pixelShaderPath, ShaderType::PIXEL);

    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, pixelShaderId);

    glLinkProgram(programId);

    GLint logLength;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength != 0){
        GLchar errorMessage[logLength + 1];
        glGetProgramInfoLog(programId, logLength, NULL, errorMessage);
        throw runtime_error("Cannot link GPU program, message: " + string(errorMessage));
    }

    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, pixelShaderId);
    
    glDeleteShader(vertexShaderId);
    glDeleteShader(pixelShaderId);

    return programId;
}