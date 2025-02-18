#ifndef MODEL_H
#define MODEL_H

#include <QMatrix4x4>
#include <QOpenGLFunctions_4_1_Core>

class Material;

class Model {
public:
    virtual ~Model() = default;

    virtual Material * material() = 0;
    virtual void material(Material * value) = 0;

    virtual QMatrix4x4 & modelMatrix() = 0;

    virtual void init(QOpenGLFunctions_4_1_Core * gl) = 0;
    virtual void update(QOpenGLFunctions_4_1_Core * gl) = 0;
    virtual void render(QOpenGLFunctions_4_1_Core * gl) = 0;
};



#endif //MODEL_H
