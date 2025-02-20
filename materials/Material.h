
#ifndef MATERIAL_H
#define MATERIAL_H
#include "openglfunction.h"

class QOpenGLShaderProgram;
class Light;
class Camera;
class Model;

class Material {
public:
    explicit Material() = default;
    virtual ~Material() = default;

    virtual void init(QOPENGLFUNCTIONS * gl) = 0;
    virtual void update(QOPENGLFUNCTIONS * gl) = 0;
    virtual void update(QOPENGLFUNCTIONS * gl, Camera & camera) = 0;
    virtual void update(QOPENGLFUNCTIONS * gl, Light & light) = 0;
    virtual void update(QOPENGLFUNCTIONS * gl, Model & model) = 0;

    virtual QOpenGLShaderProgram * program() = 0;
};

#endif //MATERIAL_H
