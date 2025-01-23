#include "Viewport.h"

#include <QMouseEvent>
#include <QOpenGLVersionFunctionsFactory>
#include <QRandomGenerator>

#include "Scene.h"
#include "materials/Material.h"

Viewport::Viewport(QWidget * parent) : QOpenGLWidget(parent) {
}

Viewport::~Viewport() = default;

void Viewport::initializeGL() {
    QOpenGLWidget::initializeGL();
    makeCurrent();
    gl = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_1_Core>(context());
    scene = new Scene();

    for (auto & model : scene->models) {
        model->init(gl);
        model->update(gl);
    }
    for (auto & material : scene->materials) {
        material->init(gl);
        material->update(gl);
    }

    updateCamera();
    updateLight();
    updateModels();
}

void Viewport::resizeGL(const int w, const int h) {
    QOpenGLWidget::resizeGL(w, h);
    makeCurrent();

    scene->camera->aspectRatio((float) w / (float) h);
    for (auto & material : scene->materials) {
        material->update(gl, *scene->camera);
    }
    updateCamera();
    update();
}

void Viewport::paintGL() {
    QOpenGLWidget::paintGL();
    makeCurrent();

    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float rand = (float) QRandomGenerator::global()->bounded(0.1);
    gl->glClearColor(rand, rand, rand, 1.0f);

    for (auto & model : scene->models) {
        model->render(gl);
    }
}

void Viewport::mouseReleaseEvent(QMouseEvent *event) {
    m_mouseButtons = event->buttons();
    m_mousePosition = event->pos();
}

void Viewport::mousePressEvent(QMouseEvent *event) {
    m_mouseButtons = event->buttons();
    m_mousePosition = event->pos();
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
    QOpenGLWidget::mouseMoveEvent(event);

    bool cameraChanged = false;

    // LMB: Rotation
    if ((m_mouseButtons & Qt::LeftButton) && (event->buttons() & Qt::LeftButton)) {
        QPoint mouseDelta = event->pos() - m_mousePosition;
        scene->camera->orbit(mouseDelta.x(), mouseDelta.y(), 0);
        cameraChanged = true;
    }

    // RMB: Movement
    if ((m_mouseButtons & Qt::RightButton) && (event->buttons() & Qt::RightButton)) {
        QPoint mouseDelta = event->pos() - m_mousePosition;
        scene->camera->displace(mouseDelta.x(), mouseDelta.y(), 0);
        cameraChanged = true;
    }

    if (cameraChanged) {
        updateCamera();
    }

    m_mouseButtons = event->buttons();
    m_mousePosition = event->pos();
}

void Viewport::wheelEvent(QWheelEvent *event) {
    float delta = (float)event->pixelDelta().y();
    float factor = qPow(1.1f, -delta / 120.0f);

    QVector3D viewVector = scene->camera->eye() - scene->camera->target();
    float targetDistance = viewVector.length();
    float newTargetDistance = targetDistance * factor;
    scene->camera->eye(viewVector.normalized() * newTargetDistance + scene->camera->target());
}

void Viewport::updateCamera() {
    for (auto & material : scene->materials) {
        material->update(gl, *scene->camera);
    }
    update();
}

void Viewport::updateLight() {
    for (auto & material : scene->materials) {
        material->update(gl, *scene->light);
    }
    update();
}

void Viewport::updateModels() {
    for (auto & model : scene->models) {
        model->update(gl);
        model->material()->update(gl, *model);
    }
    update();
}
