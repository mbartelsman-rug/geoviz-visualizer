
#ifndef PLANE_H
#define PLANE_H

#include "Model.h"

class Plane : public Model {
    Material * m_material { nullptr };
    GLuint m_vao { 0 };
    GLuint m_positions { 0 };
    GLuint m_normals { 0 };
    GLuint m_indices { 0 };

    QMatrix4x4 m_transform { };

public:
    explicit Plane(Material * mat);

    Material * material() override;
    void material(Material * value) override;

    QMatrix4x4 & modelMatrix() override;

    void init(QOPENGLFUNCTIONS * gl) override;
    void update(QOPENGLFUNCTIONS * gl) override;
    void render(QOPENGLFUNCTIONS * gl) override;
};



#endif //PLANE_H
