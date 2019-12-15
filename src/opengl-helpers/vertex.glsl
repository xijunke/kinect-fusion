#version 330 core

layout(location = 0) in vec3 vertexPositionWorld;

uniform mat4 ViewProjection;

void main() {
    gl_Position = ViewProjection * vec4(vertexPositionWorld, 1);
}