#pragma once

#include <string>
#include <GL/glew.h>

using namespace std;

class ShaderHelper {
    public:
        enum class ShaderType { VERTEX = GL_VERTEX_SHADER, PIXEL = GL_FRAGMENT_SHADER };

        static GLuint loadShader(string shaderPath, ShaderType shaderType);
        static GLuint loadProgram(string vertexShaderPath, string pixelShaderPath);
};
