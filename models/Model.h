#ifndef MODEL_H
#define MODEL_H

#include <QMatrix4x4>
#include "openglfunction.h"

class Material;

class Model {
public:
    virtual ~Model() = default;

    virtual Material * material() = 0;
    virtual void material(Material * value) = 0;

    virtual QMatrix4x4 & modelMatrix() = 0;

    virtual void init(QOPENGLFUNCTIONS * gl) = 0;
    virtual void update(QOPENGLFUNCTIONS * gl) = 0;
    virtual void render(QOPENGLFUNCTIONS * gl) = 0;
};



#endif //MODEL_H
