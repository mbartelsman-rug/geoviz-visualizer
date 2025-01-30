
#include "MapSurface.h"

#include <qimage.h>

#include "cameras/Camera.h"
#include "lights/Light.h"
#include "models/Model.h"
MapSurface::MapSurface(QString const & texture) : m_texture_filename { texture } {}


void MapSurface::init(QOpenGLFunctions_4_1_Core * gl)
{
    program()->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/materials/MapSurface.vert");
    program()->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/materials/MapSurface.frag");
    program()->link();

    if (!program()->isLinked()) {
        qWarning() << "MapSurface::init: program is not linked:\n" << program()->log();
    }

    gl->glActiveTexture(GL_TEXTURE0);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    gl->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

}

void MapSurface::update(QOpenGLFunctions_4_1_Core * gl) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);

    gl->glActiveTexture(GL_TEXTURE0);

    QImage image { m_texture_filename };
    QImage rgb_image = image.convertToFormat(QImage::Format_RGB888);
    gl->glGenTextures(1, &m_texture);
    gl->glBindTexture(GL_TEXTURE_2D, m_texture);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rgb_image.width(), rgb_image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_image.bits());
    gl->glGenerateMipmap(GL_TEXTURE_2D);

    gl->glUniform1i(gl->glGetUniformLocation(id, "heightMap"), 0);

    gl->glBindTexture(GL_TEXTURE_2D, 0);
    gl->glUseProgram(0);
}

void MapSurface::update(QOpenGLFunctions_4_1_Core * gl, Camera &camera) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "viewMatrix"), 1, false, camera.viewMatrix().data());
    gl->glUniformMatrix3fv(gl->glGetUniformLocation(id, "viewNormalMatrix"), 1, false, camera.viewMatrix().normalMatrix().data());
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "projectionMatrix"), 1, false, camera.projectionMatrix(0.1f, 40.0f).data());
    gl->glUseProgram(0);
}

void MapSurface::update(QOpenGLFunctions_4_1_Core * gl, Model &model) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "modelMatrix"), 1, false, model.modelMatrix().data());
    gl->glUniformMatrix3fv(gl->glGetUniformLocation(id, "modelNormalMatrix"), 1, false, model.modelMatrix().normalMatrix().data());
    gl->glUseProgram(0);
}

void MapSurface::update(QOpenGLFunctions_4_1_Core * gl, Light &light) {
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniform3f(gl->glGetUniformLocation(id, "lightPos"), light.position()[0], light.position()[1], light.position()[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "lightColor"), light.color()[0], light.color()[1], light.color()[2]);
    gl->glUseProgram(0);
}

QOpenGLShaderProgram * MapSurface::program() {
    return &m_program;
}