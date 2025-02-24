#version 460
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

// Waterlining
uniform int method;
uniform float vertical_scale;
uniform float waterline_space;
uniform float waterline_exponent;
uniform float waterline_thickness;

uniform sampler2D txt;

const vec3 cameraPos = vec3(0.0);

float get_terrain(float u, float v){
    return texture(txt, vec2(u/2,v)).r;
}

float get_dmap(float u, float v){
    return texture(txt, vec2(0.5 + u/2,v)).r;
}

vec3 color_land(float value) {
    float max_value_color = 1000.0; // TODO make uniform

    vec3 color = vec3(0, 0.3, 0);
    color.g += min(value / max_value_color, 1.0) * (1 - color.g);
    return color;
}

vec3 waterlining(float u, float v) {
    float length = get_dmap(u, v);

    float h = 0;
    float phi = pow(floor(pow(waterline_space * length, waterline_exponent) + h), 1.0 / waterline_exponent) / waterline_space;

    vec3 color;
    if (length - phi < waterline_thickness){
        color = vec3(0, 0, 0.8);
    } else { // normal water
        color = vec3(0.6, 0.6, 1);
    }

    return color;
}

vec3 stippling(float u, float v) {
    return vec3(1, 0, 1);
}

vec3 color_water(float u, float v) {
    if (method == 0) {
        return waterlining(u, v);
    }
    if (method == 1) {
        return stippling(u, v);
    }
}

void main() {
    float u = uv_fr.r;
    float v = uv_fr.g;

    float value = get_terrain(u,v);
    float height = value * vertical_scale;

    vec3 color;
    if (height == 0.0){
        color = color_water(u, v);
    } else {
        color = color_land(value);
    }

    color_out = vec4(color, 1);
}
