
#ifndef GEOVIZVISUALIZER3_MODELS_TERRAIN_HPP
#define GEOVIZVISUALIZER3_MODELS_TERRAIN_HPP
#pragma once

#include "Model.h"

class Material;

class Terrain : Model {
    Material * m_material { nullptr };
    GLuint m_vao { 0 };
    GLuint m_positions { 0 };
    GLuint m_texcoords { 0 };
    GLuint m_indices { 0 };

    QMatrix4x4 m_transform;

public:
    explicit Terrain(Material * mat);

    Material * material() override;
    void material(Material * value) override;

    QMatrix4x4 & modelMatrix() override;

    void init(QOpenGLFunctions_4_1_Core * gl) override;
    void update(QOpenGLFunctions_4_1_Core * gl) override;
    void render(QOpenGLFunctions_4_1_Core * gl) override;
};



#endif //GEOVIZVISUALIZER3_MODELS_TERRAIN_HPP
