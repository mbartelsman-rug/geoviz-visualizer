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

uniform int method;
uniform float vertical_scale;

// Waterlining
uniform float space;
uniform float exponent;
uniform float phase;
uniform float thickness;

// Stippling
uniform float range;

// Contour hatching 
uniform int c_mult1;
uniform int c_mult2;
uniform float c_exponent;
uniform float c_density;


// Texture
uniform sampler2D txt;

const vec3 cameraPos = vec3(0.0);
const vec3 waterColor = vec3(0.6, 0.6, 1);
const vec3 waterLineColor = vec3(0, 0, 0.8);

float get_terrain(float u, float v){
    return texture(txt, vec2(u/2,v)).r;
}

float get_dmap(float u, float v){
    return texture(txt, vec2(0.5 + u/2,v)).r;
}

vec3 get_stipple(vec2 coords) {
    return pow(mod(coords.x, 1.0) - 0.5, 2) + pow(mod(coords.y, 1.0) - 0.5, 2) <= pow(0.4, 2)
        ? vec3(1)
        : vec3(0);
}

vec3 color_land(float value) {
    float max_value_color = 1000.0; // TODO make uniform

    vec3 color = vec3(0, 0.3, 0);
    color.g += min(value / max_value_color, 1.0) * (1 - color.g);
    return color;
}

float phi(float value) {
    return pow(floor(pow(space * value, exponent) + phase) - phase, 1.0 / exponent) / space;
}

vec3 waterFlat() {
    return waterColor;
}

vec3 waterlining(float u, float v) {
    float length = get_dmap(u, v);

    vec3 color;
    if (length - phi(length) < thickness) {
        color = waterLineColor;
    } else {
        color = waterColor;
    }

    return color;
}

vec3 stippling(float u, float v) {
    float length = get_dmap(u, v);

    if (length - phi(length) < thickness) {
        // in range of waterline

        float value = get_dmap(u, v);
        vec2 dD = vec2(dFdx(value), dFdy(value));
        return get_stipple((vec2(u, v) + dD) * 200);
    }

    return vec3(0);
}

// Defined in simplex.glsl
float snoise(vec2 v);
float snoise(vec3 v);

vec3 contourHatching(float u, float v) {
    float d = get_dmap(u, v);
    float d_i = 1-d;
    float r1 = snoise(vec3(c_mult1 * d, c_mult2 * vec2(u, v))) / 2 + 0.5;

    float calculated = pow(d_i, c_exponent) * r1;
    if (calculated < 1 - c_density) {
        return waterColor;
    }
    return vec3(0, 0, 0);
}

vec3 color_water(float u, float v) {
    switch (method) {
        case 0:
            return waterFlat();
        case 1:
            return waterlining(u, v);
        case 2:
            return stippling(u,v);
        case 3:
            return contourHatching(u, v);
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
