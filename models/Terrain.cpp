
#include "Terrain.h"

#include <QOpenGLShaderProgram>

#include "materials/Material.h"

Material * Terrain::material() { return m_material; }
void Terrain::material(Material * value) { m_material = value; }


QMatrix4x4 & Terrain::modelMatrix() { return m_transform; }


void Terrain::init(QOpenGLFunctions_4_1_Core * gl)
{
    gl->glGenVertexArrays(1, &m_vao);
    gl->glBindVertexArray(m_vao);

    gl->glGenBuffers(1, &m_positions);
    gl->glBindBuffer(GL_ARRAY_BUFFER, m_positions);
    int positionAttribute = gl->glGetAttribLocation(material()->program()->programId(), "vertexPosition_in");
    gl->glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void *)0);
    gl->glEnableVertexAttribArray(positionAttribute);

    gl->glGenBuffers(1, &m_texcoords);
    gl->glBindBuffer(GL_ARRAY_BUFFER, m_texcoords);
    int texcoordAttribute = gl->glGetAttribLocation(material()->program()->programId(), "vertexTexCoord_in");
    gl->glVertexAttribPointer(texcoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), (void *)0);
    gl->glEnableVertexAttribArray(texcoordAttribute);

    gl->glBindVertexArray(0);
}

void Terrain::update(QOpenGLFunctions_4_1_Core * gl)
{
    QList<QVector3D> vertices = {
        { -1, 0, -1 },
        { +1, 0, -1 },
        { +1, 0, +1 },
        { -1, 0, +1 },
    };
    QList<QVector2D> texcoords = {
        { 0, 0 },
        { 1, 0 },
        { 1, 1 },
        { 0, 1 },
    };
    QList<GLuint> indices = {
        0, 1, 2, 3
    };

    gl->glBindVertexArray(m_vao);

    gl->glBindBuffer(GL_ARRAY_BUFFER, m_positions);
    gl->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), vertices.data(), GL_STATIC_DRAW);

    gl->glBindBuffer(GL_ARRAY_BUFFER, m_texcoords);
    gl->glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(QVector3D), texcoords.data(), GL_STATIC_DRAW);

    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    gl->glBindVertexArray(0);
}

void Terrain::render(QOpenGLFunctions_4_1_Core * gl)
{
    gl->glPatchParameteri(GL_PATCH_VERTICES, 4);

    GLuint id = material()->program()->programId();
    gl->glUseProgram(id);
    gl->glBindVertexArray(m_vao);
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
    gl->glDrawElements(GL_PATCHES, 4, GL_UNSIGNED_INT, nullptr);
    gl->glBindVertexArray(0);
    gl->glUseProgram(0);

}
