
#include "Camera.h"

float Camera::aspectRatio() {
    return m_aspectRatio;
}

float Camera::fov() {
    return m_fov;
}


float Camera::scale()
{
    return m_scale;
}


QVector3D Camera::eye() {
    return m_eye;
}

QVector3D Camera::target() {
    return m_target;
}

float Camera::rollAngle() {
    return m_rollAngle;
}

QVector3D Camera::referenceUp() {
    return m_referenceUp;
}

void Camera::aspectRatio(float value) {
    m_aspectRatio = value;
}

void Camera::fov(float value) {
    m_fov = value;
}


void Camera::scale(float value)
{
    m_scale = value;
}


void Camera::eye(QVector3D value) {
    m_eye = value;
}

void Camera::target(QVector3D value) {
    m_target = value;
}

void Camera::rollAngle(float value) {
    m_rollAngle = value;
}

void Camera::referenceUp(QVector3D value) {
    m_referenceUp = value;
}

QVector3D Camera::eyeForward() {
    return (eye() - target()).normalized();
}

QVector3D Camera::eyeUp() {
    return QVector3D::crossProduct(eyeRight(), eyeForward()).normalized();
}

QVector3D Camera::eyeRight() {
    QQuaternion rollRotation = QQuaternion::fromAxisAndAngle(eyeForward(), rollAngle());
    return rollRotation.rotatedVector(QVector3D::crossProduct(eyeForward(), referenceUp())).normalized();
}

void Camera::displace(float truck, float boom, float dolly) {
    QVector3D dirX = eyeRight();
    QVector3D dirY = eyeUp();
    QVector3D dirZ = eyeForward();

    eye(eye() + dirX * truck + dirY * boom + dirZ * dolly);
    target(target() + dirX * truck + dirY * boom + dirZ * dolly);
}

void Camera::track(float truck, float boom, float dolly) {
    QVector3D dirX = eyeRight();
    QVector3D dirY = eyeUp();
    QVector3D dirZ = eyeForward();

    target(target() + dirX * truck + dirY * boom + dirZ * dolly);
}

void Camera::orbit(float yaw, float pitch, float roll) {
    QQuaternion yawRotation = QQuaternion::fromAxisAndAngle(referenceUp(), yaw);
    eye(yawRotation.rotatedVector(eye() - target()) + target());

    const float RAD2DEG = 180.0f / acosf(-1.0f);
    float angle2Down = RAD2DEG * qAcos(QVector3D::dotProduct(-referenceUp(), eyeForward()));
    float angle2Up = RAD2DEG * qAcos(QVector3D::dotProduct(referenceUp(), eyeForward()));

    if (-pitch < angle2Down - 1 && pitch < angle2Up - 1)
    {
        QQuaternion pitchRotation = QQuaternion::fromAxisAndAngle(eyeRight(), pitch);
        eye(pitchRotation.rotatedVector(eye() - target()) + target());
    }

    rollAngle(rollAngle() + roll);
}

void Camera::rotate(float yaw, float pitch, float roll) {
    QQuaternion yawRotation = QQuaternion::fromAxisAndAngle(eyeUp(), yaw);
    target(yawRotation.rotatedVector(target() - eye()) + eye());

    QQuaternion pitchRotation = QQuaternion::fromAxisAndAngle(eyeRight(), pitch);
    target(pitchRotation.rotatedVector(target() - eye()) + eye());

    rollAngle(rollAngle() + roll);
}

void Camera::zoom(float factor) {
    fov(fov() * factor);
}

QMatrix4x4 Camera::viewMatrix() {
    QVector3D z = eyeForward();
    QVector3D y = eyeUp();
    QVector3D x = eyeRight();
    QVector3D position = eye();

    QMatrix4x4 view {
        x.x(), x.y(), x.z(), -QVector3D::dotProduct(x, position),
        y.x(), y.y(), y.z(), -QVector3D::dotProduct(y, position),
        z.x(), z.y(), z.z(), -QVector3D::dotProduct(z, position),
        0, 0, 0, 1,
    };

    view.scale(scale());

    return view;
}
