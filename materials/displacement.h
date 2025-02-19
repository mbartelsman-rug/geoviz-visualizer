#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H

#include "materials/Material.h"
#include <QVector>
#include <qimage.h>
#include <qopenglshaderprogram.h>
#include <qvectornd.h>

class Displacement: public Material {
public:
    Displacement();
    ~Displacement();

    QImage toImage() const;
    void setData(QVector<float> &data, int numX, int numY);

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
    QVector<float> *data;
    int numX = 0;
    int numY = 0;
    QVector<float> *dst;

    void computeDistanceTransform();
    static QImage getQImage(QVector<float> &data, int numX, int numY, bool scale);

    QOpenGLShaderProgram m_program;

    GLuint textureID, dstID;
};



#endif //DISPLACEMENT_H
