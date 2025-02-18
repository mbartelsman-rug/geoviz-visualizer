#version 410
layout(quads, equal_spacing, ccw) in;

layout(location = 0) in vec3[] vertcoords_tc;
layout(location = 1) in vec3[] vertnormals_tc;

layout(location = 0) out vec3 vertcoords_te;
layout(location = 1) out vec3 vertnormals_te;
layout(location = 2) out vec3 lightPos_fs;
layout(location = 3) out vec3 in_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 modelNormalMatrix;
uniform mat3 viewNormalMatrix;
uniform vec3 lightPos;

uniform sampler2D texture;

void main() {
float u = gl_TessCoord.x;
float v = gl_TessCoord.y;

vec3 pos = mix(
    mix(vertcoords_tc[0], vertcoords_tc[1], u),
    mix(vertcoords_tc[3],  vertcoords_tc[2], u),
    v  
);

vec3 normal = mix(
    mix(vertnormals_tc[0], vertnormals_tc[1], u),
    mix(vertnormals_tc[3],  vertnormals_tc[2], u),
    v  
);

float scale = 100; // TODO make uniform
float value = texture2D(texture, vec2(u,v)).r;
float height = value * scale;

pos.y = pos.y + height;


vec4 coords = vec4(pos, 1.0);
mat4 viewModel = viewMatrix * modelMatrix;

vertcoords_te = vec3(viewMatrix * modelMatrix * coords);
vertnormals_te = normalize(viewNormalMatrix * modelNormalMatrix * normalize(normal));
lightPos_fs = vec3(viewMatrix * vec4(lightPos, 1.0));
gl_Position = projectionMatrix * viewModel * coords;


in_color = texture2D(texture, vec2(u,v)).rgb;

if (height == 0.0){
    in_color = vec3(0, 0, 0.8);
} else {
    float max_value_color = 512.0 / 65535; // TODO make uniform

    in_color = vec3(0, 0.3, 0);
    in_color.g += min(value / max_value_color, 1.0) * (1 - in_color.g);
}
}
