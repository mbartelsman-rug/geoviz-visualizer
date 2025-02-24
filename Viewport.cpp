#include "Viewport.h"

#include <QMouseEvent>
#include <QOpenGLVersionFunctionsFactory>
#include <QRandomGenerator>
#include <QTimer>

#include "Scene.h"
#include "initializer/demloader.h"
#include "materials/Material.h"
#include "openglfunction.h"

void GLAPIENTRY
MessageCallback( GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar* message,
                const void* userParam )
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}


Viewport::Viewport(QWidget * parent) : QOpenGLWidget(parent) {
}

Viewport::~Viewport() = default;

void Viewport::loadDem(QString &filename){
    DemLoader demLoader(filename);

    scene->displacement->setData(demLoader.getData(), demLoader.getNBlockXSize(), demLoader.getNBlockYSize());

    updateModels();
}

void Viewport::initializeGL() {
    QOpenGLWidget::initializeGL();
    makeCurrent();
    gl = QOpenGLVersionFunctionsFactory::get<QOPENGLFUNCTIONS>(context());

    // enable debug output
    gl->glEnable              ( GL_DEBUG_OUTPUT );
    gl->glDebugMessageCallback( MessageCallback, 0 );

    scene = new Scene(&settings);

    for (auto & material : scene->materials) {
        material->init(gl);
        material->update(gl);
    }

    for (auto & model : scene->models) {
        model->init(gl);
        model->update(gl);
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
    gl->glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    gl->glEnable(GL_DEPTH_TEST);

    if (settings.wireFrameMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    for (auto & model : scene->models) {
        model->render(gl);
    }
}

void Viewport::mouseReleaseEvent(QMouseEvent *event) {
    m_mouseButtons = event->buttons();
    m_mousePosition = event->pos();
}

void Viewport::mousePressEvent(QMouseEvent *event) {
    // In order to allow keyPressEvents:
    setFocus();

    m_mouseButtons = event->buttons();
    m_mousePosition = event->pos();
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
    QOpenGLWidget::mouseMoveEvent(event);
    bool cameraChanged = false;

    // LMB: Rotation
    if ((m_mouseButtons & Qt::LeftButton) && (event->buttons() & Qt::LeftButton)) {
        QPoint mouseDelta = event->pos() - m_mousePosition;

        float deltaX = (float)mouseDelta.x() / 1.5;
        float deltaY = (float)mouseDelta.y() / 1.5;

        scene->camera->orbit(deltaX, deltaY, 0);
        cameraChanged = true;
    }

    // RMB: Movement
    if ((m_mouseButtons & Qt::RightButton) && (event->buttons() & Qt::RightButton)) {
        QPoint mouseDelta = event->pos() - m_mousePosition;

        float deltaX = -(float)mouseDelta.x();
        float deltaY = (float)mouseDelta.y();
        float length = (scene->camera->target() - scene->camera->eye()).length();
        float factor = length / (scene->camera->scale() * (float)width()) / 10;

        scene->camera->displace(deltaX * factor, deltaY * factor, 0);
        cameraChanged = true;
    }

    if (cameraChanged) {
        updateCamera();
    }

    m_mouseButtons = event->buttons();
    m_mousePosition = event->pos();
}

void Viewport::wheelEvent(QWheelEvent *event) {
    float pixelDelta = ((float)event->pixelDelta().y()) / 120.0f;
    float angleDelta = ((float)event->angleDelta().y()) / (8.0f * 8.0f);

    float delta = pixelDelta == 0 ? angleDelta : pixelDelta;
    float factor = qPow(1.1f, -delta);

    scene->camera->zoom(factor);
    updateCamera();
}

void Viewport::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case 'Z':
        settings.wireFrameMode = !settings.wireFrameMode;
        update();
        break;
    }
}

void Viewport::updateCamera() {
    makeCurrent();
    for (auto & material : scene->materials) {
        material->update(gl, *scene->camera);
    }
    update();
}

void Viewport::updateLight() {
    makeCurrent();
    for (auto & material : scene->materials) {
        material->update(gl, *scene->light);
    }
    update();
}

void Viewport::updateModels() {
    makeCurrent(); 

    for (auto & model : scene->models) {
        model->update(gl);
        model->material()->update(gl, *model);
        model->material()->update(gl);
    }
    update();
}
