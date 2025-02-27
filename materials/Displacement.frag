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

uniform ivec2 mapSize;
uniform int method;
uniform float vertical_scale;

uniform vec3 waterColor1;
uniform vec3 waterColor2;
uniform vec3 landColor1;
uniform vec3 landColor2;

// Waterlining
uniform float space;
uniform float exponent;
uniform float phase;
uniform float thickness;

// Stippling

// Contour hatching 
uniform int c_mult1;
uniform int c_mult2;
uniform float c_exponent;
uniform float c_density;


// Texture
uniform sampler2D txt;

const vec3 cameraPos = vec3(0.0);

float get_terrain(float u, float v){
    return texture(txt, vec2(u/2,v)).r;
}

float get_dmap(float u, float v){
    return texture(txt, vec2(0.5 + u/2,v)).r;
}

vec3 get_stipple(vec2 coords) {
    return pow(coords.x - 0.5, 2) + pow(coords.y - 0.5, 2) <= pow(0.4, 2)
        ? vec3(1)
        : vec3(0);
}

vec3 color_land(float value) {
    float max_value_color = 1000.0; // TODO make uniform
    vec3 color = mix(landColor1, landColor2, min(value / max_value_color, 1.0));
    return color;
}

float phi(float value) {
    return pow(floor(pow(space * value, exponent) + phase) - phase, 1.0 / exponent) / space;
}

vec3 waterFlat() {
    return waterColor1;
}

vec3 waterlining(float u, float v) {
    float length = get_dmap(u, v);

    vec3 color;
    if (length - phi(length) < thickness) {
        color = waterColor2;
    } else {
        color = waterColor1;
    }

    return color;
}

vec2 rand(vec2);

vec3 stippling(float u, float v) {
    float d = get_dmap(u, v);
    vec2 uv = vec2(u, v) * 100;
    vec2 cell = floor(uv);
    vec2 offset = uv - cell;

    float wlDist = d - phi(d); // [0,1]
    if (wlDist < thickness) {
        vec2 cellCenter = cell + vec2(0.5); // [0.5, 100.5]^2
        float centerD = get_dmap(cellCenter.x, cellCenter.y); // [0, 1]
        vec2 toNextWl = normalize(vec2(dFdx(centerD), dFdy(centerD)));
        vec2 toWl = toNextWl * wlDist * 100;
        vec3 samp = get_stipple(uv + toWl - cell);
        if (samp != vec3(0))
            return samp;
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
        return waterColor1;
    }
    return waterColor2;
}

vec3 color_water(float u, float v) {
    switch (method) {
        case 0:
            return waterFlat();
        case 1:
            return waterlining(u, v);
        case 2:
            return waterFlat();
        case 3:
            return contourHatching(u, v);
    }
}

void main() {
    float u = uv_fr.r;
    float v = uv_fr.g;

    float height = get_terrain(u,v);

    vec3 color;
    if (height == 0.0){
        color = color_water(u, v);
    } else {
        color = color_land(height);
    }

    color_out = vec4(color, 1);
}
