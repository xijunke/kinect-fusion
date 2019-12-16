#version 330 core

uniform mat4 ViewProjection;

layout(location = 0) in vec3 vertexPositionWorld;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

void main() {
    gl_Position = ViewProjection * vec4(vertexPositionWorld, 1);
    fragmentColor = vertexColor;
}