#include "displacement.h"
#include <qdir.h>
#include <qopenglshaderprogram.h>
#include <qpixmap.h>

#include "cameras/Camera.h"
#include "lights/Light.h"
#include "models/Model.h"

Displacement::Displacement() {

}

void Displacement::setData(QVector<float> &data, int numX, int numY){
    this->data.clear();
    this->data.append(data);

    this->numX = numX;
    this->numY = numY;
}


void Displacement::init(QOpenGLFunctions_4_1_Core * gl) {
    program()->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/materials/Displacement.vert");
    program()->addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/materials/Displacement.tesc");
    program()->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/materials/Displacement.tese");
    program()->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/materials/Displacement.frag");
    program()->link();
    if (!program()->isLinked()) {
        qWarning() << "Displacement::init: program is not linked:\n" << program()->log();
    }
}

void Displacement::update(QOpenGLFunctions_4_1_Core * gl) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniform3f(gl->glGetUniformLocation(id, "diffuseColor"), diffuseColor[0], diffuseColor[1], diffuseColor[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "specularColor"), specularColor[0], specularColor[1], specularColor[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "ambientColor"), ambientColor[0], ambientColor[1], ambientColor[2]);
    gl->glUniform1f(gl->glGetUniformLocation(id, "diffuseCoefficient"), diffuseCoefficient);
    gl->glUniform1f(gl->glGetUniformLocation(id, "specularCoefficient"), specularCoefficient);
    gl->glUniform1f(gl->glGetUniformLocation(id, "ambientCoefficient"), ambientCoefficient);
    gl->glUniform1f(gl->glGetUniformLocation(id, "shininess"), shininess);


    gl->glDisable(GL_CULL_FACE);

    // Displacement
    QImage gray(numX, numY, QImage::Format_Grayscale16);

    float max_val = std::max_element(data.cbegin(), data.cend())[0];
    float min_val = std::min_element(data.cbegin(), data.cend())[0];
    qDebug() << "Max" << max_val << "min" << min_val;

    int i = 0;
    for (float elem : data){
        quint16 val = std::max(elem, 0.0f);

        //dst[i] = val;
        int x = i % numX;
        int y = i / numX;
        ((quint16 *)gray.scanLine(y))[x] = val;

        i++;
    }

    gl->glEnable(GL_TEXTURE_2D); // Enable texturing
    gl->glBindTexture(GL_TEXTURE_2D, texBufferID); // Set as the current texture
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    gl->glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        gray.width(),
        gray.height(),
        0,
        GL_RED,
        GL_UNSIGNED_SHORT,
        gray.bits()
        );

    gl->glDisable(GL_TEXTURE_2D);
    gl->glUseProgram(0);
}

void Displacement::update(QOpenGLFunctions_4_1_Core * gl, Camera &camera) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);

    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "viewMatrix"), 1, false, camera.viewMatrix().data());
    gl->glUniformMatrix3fv(gl->glGetUniformLocation(id, "viewNormalMatrix"), 1, false, camera.viewMatrix().normalMatrix().data());
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "projectionMatrix"), 1, false, camera.projectionMatrix(0.1f, 40.0f).data());
    gl->glUseProgram(0);
}

void Displacement::update(QOpenGLFunctions_4_1_Core * gl, Model &model) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "modelMatrix"), 1, false, model.modelMatrix().data());
    gl->glUniformMatrix3fv(gl->glGetUniformLocation(id, "modelNormalMatrix"), 1, false, model.modelMatrix().normalMatrix().data());
    gl->glUseProgram(0);
}

void Displacement::update(QOpenGLFunctions_4_1_Core * gl, Light &light) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniform3f(gl->glGetUniformLocation(id, "lightPos"), light.position()[0], light.position()[1], light.position()[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "lightColor"), light.color()[0], light.color()[1], light.color()[2]);
    gl->glUseProgram(0);
}

QOpenGLShaderProgram * Displacement::program() {
    return &m_program;
}
