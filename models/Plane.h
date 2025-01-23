
#ifndef PLANE_H
#define PLANE_H

#include <QOpenGLBuffer>

#include "Model.h"
#include <QOpenGLVertexArrayObject>

class Plane : public Model {
    Material * m_material { nullptr };
    GLuint m_vao { 0 };
    GLuint m_positions { 0 };
    GLuint m_normals { 0 };
    GLuint m_indices { 0 };

public:
    explicit Plane(Material * mat);

    Material * material() override;
    void material(Material * value) override;

    QMatrix4x4 modelMatrix() override;

    void init(QOpenGLFunctions_4_1_Core * gl) override;
    void update(QOpenGLFunctions_4_1_Core * gl) override;
    void render(QOpenGLFunctions_4_1_Core * gl) override;
};



#endif //PLANE_H
