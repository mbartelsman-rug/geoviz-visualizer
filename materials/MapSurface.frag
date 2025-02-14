#version 410
// Fragment shader

layout(location = 0) out vec3 vertexPosition_tes;
layout(location = 1) out vec3 vertexNormal_tes;
layout(location = 2) out vec2 vertexTexCoord_tes;
layout(location = 3) out float vertexHeight_tes;

uniform sampler2D heightMap;  // the texture corresponding to our height map

void main() {
    float value = texture(heightMap, vertexTexCoord_tes).g;
    gl_FragColor = vec4(vec3(value), 1.0);
}
