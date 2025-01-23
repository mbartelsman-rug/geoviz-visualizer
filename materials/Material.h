
#ifndef MATERIAL_H
#define MATERIAL_H
#include <QOpenGLFunctions_4_1_Core>

class QOpenGLShaderProgram;
class Light;
class Camera;
class Model;

class Material {
public:
    explicit Material() = default;
    virtual ~Material() = default;

    virtual void init(QOpenGLFunctions_4_1_Core * gl) = 0;
    virtual void update(QOpenGLFunctions_4_1_Core * gl) = 0;
    virtual void update(QOpenGLFunctions_4_1_Core * gl, Camera & camera) = 0;
    virtual void update(QOpenGLFunctions_4_1_Core * gl, Light & light) = 0;
    virtual void update(QOpenGLFunctions_4_1_Core * gl, Model & model) = 0;

    virtual QOpenGLShaderProgram * program() = 0;
};

#endif //MATERIAL_H
