
#ifndef GEOVIZVISUALIZER3_MATERIALS_MAPSURFACE_HPP
#define GEOVIZVISUALIZER3_MATERIALS_MAPSURFACE_HPP
#pragma once
#include "Material.h"
#include <QOpenGLShaderProgram>


class MapSurface : public Material {
    QString m_texture_filename;
    GLuint m_texture;

    QOpenGLShaderProgram m_program;

public:
    explicit MapSurface(QString const & texture);
    void init(QOpenGLFunctions_4_1_Core * gl) override;
    void update(QOpenGLFunctions_4_1_Core * gl) override;
    void update(QOpenGLFunctions_4_1_Core * gl, Camera & camera) override;
    void update(QOpenGLFunctions_4_1_Core * gl, Light & light) override;
    void update(QOpenGLFunctions_4_1_Core * gl, Model & model) override;
    QOpenGLShaderProgram * program() override;
};



#endif //GEOVIZVISUALIZER3_MATERIALS_MAPSURFACE_HPP
