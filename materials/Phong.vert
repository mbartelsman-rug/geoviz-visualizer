#version 410
// Vertex shader

layout(location = 0) in vec3 vertexPosition_in;
layout(location = 1) in vec3 vertexNormal_in;

layout(location = 0) out vec3 vertexPosition_fs;
layout(location = 1) out vec3 vertexNormal_fs;
layout(location = 2) out vec3 lightPos_fs;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 modelNormalMatrix;
uniform mat3 viewNormalMatrix;
uniform vec3 lightPos;

void main() {
    vec4 vertCoords = vec4(vertexPosition_in, 1.0);
    mat4 viewModel = viewMatrix * modelMatrix;

    vertexPosition_fs = vec3(viewMatrix * modelMatrix * vertCoords);
    vertexNormal_fs = normalize(viewNormalMatrix * modelNormalMatrix * normalize(vertexNormal_in));
    lightPos_fs = vec3(viewMatrix * vec4(lightPos, 1.0));
    gl_Position = projectionMatrix * viewModel * vertCoords;
}
