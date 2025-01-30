#version 410
// Tessellation control shader

layout(vertices=4) out;

layout(location = 0) in vec3 vertexPosition_vs[];
layout(location = 2) in vec2 vertexTexCoord_vs[];

layout(location = 0) out vec3 vertexPosition_tcs[];
layout(location = 2) out vec2 vertexTexCoord_tcs[];

void main() {
    vertexPosition_tcs[gl_InvocationID] = vertexPosition_vs[gl_InvocationID];
    vertexTexCoord_tcs[gl_InvocationID] = vertexTexCoord_vs[gl_InvocationID];

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = min(GL_MAX_TESS_GEN_LEVEL, 256);
        gl_TessLevelOuter[1] = min(GL_MAX_TESS_GEN_LEVEL, 256);
        gl_TessLevelOuter[2] = min(GL_MAX_TESS_GEN_LEVEL, 256);
        gl_TessLevelOuter[3] = min(GL_MAX_TESS_GEN_LEVEL, 256);

        gl_TessLevelInner[0] = min(GL_MAX_TESS_GEN_LEVEL, 256);
        gl_TessLevelInner[1] = min(GL_MAX_TESS_GEN_LEVEL, 256);
    }
}
