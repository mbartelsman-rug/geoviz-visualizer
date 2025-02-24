#version 460
layout(quads, equal_spacing, ccw) in;

layout(location = 0) in vec3[] vertcoords_tc;
layout(location = 1) in vec3[] vertnormals_tc;

layout(location = 0) out vec3 vertcoords_te;
layout(location = 1) out vec3 vertnormals_te;
layout(location = 2) out vec3 lightPos_fs;
layout(location = 3) out vec3 in_color;
layout(location = 4) out vec2 uv_fr;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 modelNormalMatrix;
uniform mat3 viewNormalMatrix;
uniform vec3 lightPos;

uniform float vertical_scale;
uniform sampler2D txt;

float get_terrain(float u, float v){
    return texture(txt, vec2(u/2,v)).r;
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    uv_fr = vec2(u, v);

    vec3 pos = mix(
        mix(vertcoords_tc[1], vertcoords_tc[0], u),
        mix(vertcoords_tc[2],  vertcoords_tc[3], u),
        v
    );

    vec3 normal = mix(
        mix(vertnormals_tc[1], vertnormals_tc[0], u),
        mix(vertnormals_tc[2],  vertnormals_tc[3], u),
        v
    );

    float value = get_terrain(u, v);
    float height = value * vertical_scale / 20000;

    pos.y = pos.y + height;


    vec4 coords = vec4(pos, 1.0);
    mat4 viewModel = viewMatrix * modelMatrix;

    vertcoords_te = vec3(viewMatrix * modelMatrix * coords);
    vertnormals_te = normalize(viewNormalMatrix * modelNormalMatrix * normalize(normal));
    lightPos_fs = vec3(viewMatrix * vec4(lightPos, 1.0));
    gl_Position = projectionMatrix * viewModel * coords;
}
