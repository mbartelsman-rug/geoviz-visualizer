#version 410
// Fragment shader

layout(location = 0) in vec3 vertexPosition_fs;
layout(location = 1) in vec3 vertexNormal_fs;
layout(location = 2) in vec3 lightPos_fs;

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

const vec3 cameraPos = vec3(0.0);

vec3 phong_shading(vec3 coords, vec3 normal) {
    vec3 surf2Light = normalize(lightPos_fs - coords);
    vec3 surf2Camera = normalize(cameraPos - coords);

    vec3 orientedDiffuseColor = diffuseColor;
    if (!gl_FrontFacing) {
        // Make the inside a different shade.
        normal *= -1;
        orientedDiffuseColor = vec3(1.0, 0, 0);
    }

    vec3 reflected = reflect(surf2Light, normal);

    float lightAngle = max(dot(surf2Light, normal), 0);
    float reflection_angle = max(dot(reflected, surf2Camera), 0);

    vec3 ambient_comp = ambientCoefficient * ambientColor;
    vec3 diffuse_comp = diffuseCoefficient * orientedDiffuseColor * lightColor * lightAngle;
    vec3 specular_comp = specularCoefficient * pow(reflection_angle, shininess) * specularColor;

    return min(ambient_comp + diffuse_comp + specular_comp, 1);
}

void main() {
    vec3 color = phong_shading(vertexPosition_fs, vertexNormal_fs);
    color_out = abs(viewMatrix[0]);
}