#version 410
// Vertex shader

layout(location = 0) in vec3 vertexPosition_in;
layout(location = 2) in vec2 vertexTexCoord_in;

layout(location = 0) out vec3 vertexPosition_vs;
layout(location = 2) out vec2 vertexTexCoord_vs;


void main() {
    vertexPosition_vs = vertexPosition_in;
    vertexTexCoord_vs = vertexTexCoord_in;

    gl_Position = vec4(vertexPosition_in, 1);
}
