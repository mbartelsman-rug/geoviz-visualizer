
#include "Phong.h"

#include "cameras/Camera.h"
#include "lights/Light.h"
#include "models/Model.h"

Phong::Phong() {
}

void Phong::init(QOPENGLFUNCTIONS * gl) {
    program()->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/materials/Phong.vert");
    program()->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/materials/Phong.frag");
    program()->link();
    if (!program()->isLinked()) {
        qWarning() << "Phong::init: program is not linked:\n" << program()->log();
    }
}

void Phong::update(QOPENGLFUNCTIONS * gl) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniform3f(gl->glGetUniformLocation(id, "diffuseColor"), diffuseColor[0], diffuseColor[1], diffuseColor[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "specularColor"), specularColor[0], specularColor[1], specularColor[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "ambientColor"), ambientColor[0], ambientColor[1], ambientColor[2]);
    gl->glUniform1f(gl->glGetUniformLocation(id, "diffuseCoefficient"), diffuseCoefficient);
    gl->glUniform1f(gl->glGetUniformLocation(id, "specularCoefficient"), specularCoefficient);
    gl->glUniform1f(gl->glGetUniformLocation(id, "ambientCoefficient"), ambientCoefficient);
    gl->glUniform1f(gl->glGetUniformLocation(id, "shininess"), shininess);
    gl->glUseProgram(0);
}

void Phong::update(QOPENGLFUNCTIONS * gl, Camera &camera) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "viewMatrix"), 1, false, camera.viewMatrix().data());
    gl->glUniformMatrix3fv(gl->glGetUniformLocation(id, "viewNormalMatrix"), 1, false, camera.viewMatrix().normalMatrix().data());
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "projectionMatrix"), 1, false, camera.projectionMatrix(0.1f, 40.0f).data());
    gl->glUseProgram(0);
}

void Phong::update(QOPENGLFUNCTIONS * gl, Model &model) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "modelMatrix"), 1, false, model.modelMatrix().data());
    gl->glUniformMatrix3fv(gl->glGetUniformLocation(id, "modelNormalMatrix"), 1, false, model.modelMatrix().normalMatrix().data());
    gl->glUseProgram(0);
}

void Phong::update(QOPENGLFUNCTIONS * gl, Light &light) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniform3f(gl->glGetUniformLocation(id, "lightPos"), light.position()[0], light.position()[1], light.position()[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "lightColor"), light.color()[0], light.color()[1], light.color()[2]);
    gl->glUseProgram(0);
}

QOpenGLShaderProgram * Phong::program() {
    return &m_program;
}