#version 420
// Fragment shader

layout(location = 0) in vec3 vertcoords_te;
layout(location = 1) in vec3 vertnormals_te;
layout(location = 2) in vec3 lightPos_fs;
layout(location = 3) in vec3 in_color;
layout(location = 4) in vec2 uv_fr;

out vec4 color_out;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform float ambientCoefficient;
uniform float diffuseCoefficient;
uniform float specularCoefficient;
uniform float shininess;

uniform vec3 lightColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 modelNormalMatrix;
uniform mat3 viewNormalMatrix;
uniform vec3 lightPos;

layout(binding=0) uniform sampler2D txt;
layout(binding=1) uniform sampler2D dst;

const vec3 cameraPos = vec3(0.0);

void main() {
    float scale = 0.0002; // TODO make uniform
    float value = texture(txt, uv_fr).r;
    float height = value * scale;

    vec3 color;

    if (height == 0.0){
        float length = texture(dst, uv_fr).r;

        length = uv_fr.r;

        float num = 0.05;
        float eps = 0.01;

        if (length - floor(length / num) * num < eps){
            color = vec3(0, 0, 0.8);
        } else {
            color = vec3(0.5, 0.5, 1);
        }
    } else {
        float max_value_color = 1000.0; // TODO make uniform

        color = vec3(0, 0.3, 0);
        color.g += min(value / max_value_color, 1.0) * (1 - color.g);
    }



/*
    // TODO remove
    color = texture(dst, uv_fr).rrr;
    if (texture(dst, uv_fr).r > 0) {
        color = vec3(0, 1, 0);
    }

*/



    color_out = vec4(color, 1);
}
