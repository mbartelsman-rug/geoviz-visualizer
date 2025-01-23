
#ifndef PHONG_H
#define PHONG_H

#include "Material.h"
#include <QOpenGLShaderProgram>

class Model;
class Camera;

class Phong : public Material {
public:
    explicit Phong();

    void init(QOpenGLFunctions_4_1_Core * gl) override;

    void update(QOpenGLFunctions_4_1_Core * gl) override;
    void update(QOpenGLFunctions_4_1_Core * gl, Camera & camera) override;
    void update(QOpenGLFunctions_4_1_Core * gl, Model & model) override;
    void update(QOpenGLFunctions_4_1_Core * gl, Light & light) override;

    QOpenGLShaderProgram * program() override;

    QVector3D diffuseColor { 0.03, 0.80, 0.87 };
    QVector3D specularColor { 1.0, 1.0, 1.0 };
    QVector3D ambientColor { 1.0, 1.0, 1.0 };

    float diffuseCoefficient { 0.8 };
    float specularCoefficient { 0.2 };
    float ambientCoefficient { 0.1 };
    float shininess { 0.8 };

private:
    QOpenGLShaderProgram m_program;
};



#endif //PHONG_H
