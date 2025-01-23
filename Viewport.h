#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>

#include "Scene.h"

class Renderer;
struct Scene;

class Viewport : public QOpenGLWidget
{
public:
    explicit Viewport(QWidget * parent);
    ~Viewport() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void updateCamera();
    void updateLight();
    void updateModels();

private:
    QOpenGLFunctions_4_1_Core * gl { nullptr };
    Scene * scene { nullptr };

    Qt::MouseButtons m_mouseButtons { Qt::NoButton };
    QPoint m_mousePosition;
};

#endif // VIEWPORT_H
