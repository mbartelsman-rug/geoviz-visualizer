#version 460
// Vertex shader

layout(location = 0) in vec3 vertexPosition_in;
layout(location = 1) in vec3 vertexNormal_in;

layout(location = 0) out vec3 vertcoords_vs;
layout(location = 1) out vec3 vertnormals_vs;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 modelNormalMatrix;
uniform mat3 viewNormalMatrix;
uniform vec3 lightPos;

void main() {
vertcoords_vs = vertexPosition_in;
vertnormals_vs = vertexNormal_in;
}
