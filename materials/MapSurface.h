
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
    void init(QOPENGLFUNCTIONS * gl) override;
    void update(QOPENGLFUNCTIONS * gl) override;
    void update(QOPENGLFUNCTIONS * gl, Camera & camera) override;
    void update(QOPENGLFUNCTIONS * gl, Light & light) override;
    void update(QOPENGLFUNCTIONS * gl, Model & model) override;
    QOpenGLShaderProgram * program() override;
};



#endif //GEOVIZVISUALIZER3_MATERIALS_MAPSURFACE_HPP
