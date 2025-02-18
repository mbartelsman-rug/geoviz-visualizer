#version 410
// Tessellation evaluation shader

layout (quads) in;

layout(location = 0) in vec3 vertexPosition_tcs[];
layout(location = 2) in vec2 vertexTexCoord_tcs[];

layout(location = 0) out vec3 vertexPosition_tes;
layout(location = 1) out vec3 vertexNormal_tes;
layout(location = 2) out vec2 vertexTexCoord_tes;
layout(location = 3) out float vertexHeight_tes;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 modelNormalMatrix;
uniform mat3 viewNormalMatrix;

uniform vec3 lightPos;

uniform sampler2D heightMap;  // the texture corresponding to our height map

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // Texture
    vec2 t00 = vertexTexCoord_tcs[0];
    vec2 t01 = vertexTexCoord_tcs[1];
    vec2 t10 = vertexTexCoord_tcs[2];
    vec2 t11 = vertexTexCoord_tcs[3];

    vec2 t0x = mix(t00, t01, u);
    vec2 t1x = mix(t10, t11, u);

    vertexTexCoord_tes = mix(t0x, t1x, v);

    vertexHeight_tes = texture(heightMap, vertexTexCoord_tes).y;

    // Position
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    vec4 vertical = normalize( vec4(cross(vVec.xyz, uVec.xyz), 0) );

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    p += vertical * vertexHeight_tes;

    vertexPosition_tes = vec3(viewMatrix * modelMatrix * p);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * p;

    // Normal
    float dx = 1.0 / gl_TessLevelInner[0];
    float dy = 1.0 / gl_TessLevelInner[1];
    float z00 = texture(heightMap, vertexTexCoord_tes + vec2(0, 0)).y;
    float z01 = texture(heightMap, vertexTexCoord_tes + vec2(0, dy)).y;
    float z10 = texture(heightMap, vertexTexCoord_tes + vec2(dx, 0)).y;
    float dzdx = z10 - z00;
    float dzdy = z01 - z00;

    vec3 normal = normalize(cross(vec3(dzdx, 0, dx), vec3(0, dzdy, dy)));

    vertexNormal_tes = viewNormalMatrix * modelNormalMatrix * normal;
}