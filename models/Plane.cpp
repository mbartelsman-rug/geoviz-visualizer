
#include "Plane.h"
#include <QOpenGLShaderProgram>

#include "materials/Material.h"

Plane::Plane(Material *mat)
    : Model()
    , m_material(mat)
{
}

QMatrix4x4 Plane::modelMatrix() {
    return {};
}

Material * Plane::material() {
    return m_material;
}

void Plane::material(Material * value) {
    m_material = value;
}

void Plane::init(QOpenGLFunctions_4_1_Core *gl) {
    gl->glGenVertexArrays(1, &m_vao);
    gl->glBindVertexArray(m_vao);
    gl->glGenBuffers(1, &m_positions);
    gl->glGenBuffers(1, &m_normals);
    gl->glGenBuffers(1, &m_indices);
    gl->glBindVertexArray(0);
}

void Plane::update(QOpenGLFunctions_4_1_Core * gl) {
    QList<QVector3D> vertices = {
        { -0.5, 0, -0.5 },
        { +0.5, 0, -0.5 },
        { +0.5, 0, +0.5 },
        { -0.5, 0, +0.5 },
    };
    QList<QVector3D> normals = {
        { 0, 1, 0 },
        { 0, 1, 0 },
        { 0, 1, 0 },
        { 0, 1, 0 },
    };
    QList<GLuint> indices = {
        0, 1, 2,
        2, 3, 0,
    };

    gl->glBindVertexArray(m_vao);

    gl->glBindBuffer(GL_ARRAY_BUFFER, m_positions);
    gl->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), vertices.data(), GL_STATIC_DRAW);
    int positionAttribute = gl->glGetAttribLocation(material()->program()->programId(), "vertexPosition_in");
    gl->glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void *)0);
    gl->glEnableVertexAttribArray(positionAttribute);

    gl->glBindBuffer(GL_ARRAY_BUFFER, m_normals);
    gl->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(QVector3D), normals.data(), GL_STATIC_DRAW);
    int normalsAttribute = gl->glGetAttribLocation(material()->program()->programId(), "vertexNormal_in");
    gl->glVertexAttribPointer(normalsAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void *)0);
    gl->glEnableVertexAttribArray(normalsAttribute);

    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    gl->glBindVertexArray(0);
}

void Plane::render(QOpenGLFunctions_4_1_Core * gl) {
    gl->glPointSize(50);

    GLuint id = material()->program()->programId();
    gl->glUseProgram(id);
    gl->glBindVertexArray(m_vao);
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
    gl->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    gl->glBindVertexArray(0);
    gl->glUseProgram(0);
}
