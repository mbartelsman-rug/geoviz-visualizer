#include "displacement.h"
#include <qdir.h>
#include <qopenglshaderprogram.h>
#include <qpixmap.h>

#include "cameras/Camera.h"
#include "lights/Light.h"
#include "models/Model.h"

QImage Displacement::getQImage(QVector<float> &data, int numX, int numY, bool scale = false)
{
    QImage image(numX, numY, QImage::Format_Grayscale16);

    float max_val = std::max_element(data.cbegin(), data.cend())[0];
    float min_val = std::min_element(data.cbegin(), data.cend())[0];
    qDebug() << "Max" << max_val << "min" << min_val;

    int i = 0;
    for (float elem : data)
    {
        quint16 val = std::max(elem, 0.0f);

        if (scale)
        {
            val = val * 65535 / max_val;
        }

        // dst[i] = val;
        int x = i % numX;
        int y = i / numX;
        ((quint16 *)image.scanLine(y))[x] = val;

        i++;
    }
    return image;
}

Displacement::Displacement()
{
}

void Displacement::setData(QVector<float> &data, int numX, int numY)
{
    this->numX = numX;
    this->numY = numY;

    // Set image data
    this->data = data;
    this->image = getQImage(data, numX, numY);

    computeDistanceTransform();
}

void Displacement::computeDistanceTransform()
{
    // MRH Distance transform by Meijster et al.
    // Based on Image Processing slides

    qDebug() << "Calculating distance transform...";

    QVector<float> g(numX * numY);
    QVector<float> distance(numX * numY);

    const float inf = numX + numY;

    auto sq = [](float num)
    {
        return num * num;
    };

    auto getPxl = [&](QVector<float> &img, int x, int y)
    {
        return img[y * numX + x];
    };

    auto setPxl = [&](QVector<float> &img, int x, int y, float value)
    {
        img[y * numX + x] = value;
    };

    auto f = [&](int x, int i, int y)
    {
        return sq(x - i) + sq(getPxl(g, i, y));
    };

    auto sep = [&](int i, int u, int y)
    {
        return static_cast<int>((sq(u) - sq(i) +
                                 sq(getPxl(g, u, y)) -
                                 sq(getPxl(g, i, y))) /
                                (2 * (u - i)));
    };

    // First phase
    for (int x = 0; x < numX; x++)
    {
        // Scan 1
        if (getPxl(data, x, 0) > 0)
        {
            setPxl(g, x, 0, 0);
        }
        else
        {
            setPxl(g, x, 0, inf);
        }

        for (int y = 1; y < numY; y++)
        {
            if (getPxl(data, x, y) > 0)
            {
                setPxl(g, x, y, 0);
            }
            else
            {
                setPxl(g, x, y, 1 + getPxl(g, x, y - 1));
            }
        }

        // Scan 2
        for (int y = numY - 2; y >= 0; y--)
        {
            if (getPxl(g, x, y + 1) < getPxl(g, x, y))
            {
                setPxl(g, x, y, 1 + getPxl(g, x, y + 1));
            }
        }
    }

    // Second phase
    for (int y = 0; y < numY; y++)
    {
        int q = 0;
        QVector<float> s = {0};
        s.resize(numX);
        QVector<float> t = {0};
        t.resize(numX);

        // Scan 3
        for (int u = 1; u < numX; u++)
        {
            while (q >= 0 && f(t[q], s[q], y) > f(t[q], u, y))
            {
                q = q - 1;
            }
            if (q < 0)
            {
                q = 0;
                s[0] = u;
            }
            else
            {
                float w = 1 + sep(s[q], u, y);

                if (w < numX)
                {
                    q = q + 1;
                    s[q] = u;
                    t[q] = w;
                }
            }
        }

        // Scan 4
        for (int u = numX - 1; u >= 0; u--)
        {
            float pixelValue = sqrt(f(u, s[q], y));
            setPxl(distance, u, y, pixelValue);
            if (u == t[q])
            {
                q = q - 1;
            }
        }
    }
    qDebug() << "...Done";

    edt = getQImage(distance, numX, numY, false);
}

void Displacement::init(QOpenGLFunctions_4_1_Core *gl)
{
    program()->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/materials/Displacement.vert");
    program()->addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/materials/Displacement.tesc");
    program()->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/materials/Displacement.tese");
    program()->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/materials/Displacement.frag");
    program()->link();
    if (!program()->isLinked())
    {
        qWarning() << "Displacement::init: program is not linked:\n"
                   << program()->log();
    }
}

void Displacement::update(QOpenGLFunctions_4_1_Core *gl)
{
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniform3f(gl->glGetUniformLocation(id, "diffuseColor"), diffuseColor[0], diffuseColor[1], diffuseColor[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "specularColor"), specularColor[0], specularColor[1], specularColor[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "ambientColor"), ambientColor[0], ambientColor[1], ambientColor[2]);
    gl->glUniform1f(gl->glGetUniformLocation(id, "diffuseCoefficient"), diffuseCoefficient);
    gl->glUniform1f(gl->glGetUniformLocation(id, "specularCoefficient"), specularCoefficient);
    gl->glUniform1f(gl->glGetUniformLocation(id, "ambientCoefficient"), ambientCoefficient);
    gl->glUniform1f(gl->glGetUniformLocation(id, "shininess"), shininess);

    // Displacement
    gl->glEnable(GL_TEXTURE_2D);                   // Enable texturing
    gl->glBindTexture(GL_TEXTURE_2D, texBufferID); // Set as the current texture
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    gl->glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        image.width(),
        image.height(),
        0,
        GL_RED,
        GL_UNSIGNED_SHORT,
        image.bits());

    gl->glDisable(GL_TEXTURE_2D);
    gl->glUseProgram(0);
}

void Displacement::update(QOpenGLFunctions_4_1_Core *gl, Camera &camera)
{
    GLuint id = program()->programId();
    gl->glUseProgram(id);

    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "viewMatrix"), 1, false, camera.viewMatrix().data());
    gl->glUniformMatrix3fv(gl->glGetUniformLocation(id, "viewNormalMatrix"), 1, false, camera.viewMatrix().normalMatrix().data());
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "projectionMatrix"), 1, false, camera.projectionMatrix(0.1f, 40.0f).data());
    gl->glUseProgram(0);
}

void Displacement::update(QOpenGLFunctions_4_1_Core *gl, Model &model)
{
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniformMatrix4fv(gl->glGetUniformLocation(id, "modelMatrix"), 1, false, model.modelMatrix().data());
    gl->glUniformMatrix3fv(gl->glGetUniformLocation(id, "modelNormalMatrix"), 1, false, model.modelMatrix().normalMatrix().data());
    gl->glUseProgram(0);
}

void Displacement::update(QOpenGLFunctions_4_1_Core *gl, Light &light)
{
    GLuint id = program()->programId();
    gl->glUseProgram(id);
    gl->glUniform3f(gl->glGetUniformLocation(id, "lightPos"), light.position()[0], light.position()[1], light.position()[2]);
    gl->glUniform3f(gl->glGetUniformLocation(id, "lightColor"), light.color()[0], light.color()[1], light.color()[2]);
    gl->glUseProgram(0);
}

QOpenGLShaderProgram *Displacement::program()
{
    return &m_program;
}
