
#ifndef CAMERA_H
#define CAMERA_H
#include <QMatrix4x4>


class Camera {
    float m_aspectRatio { 16.0 / 9.0 };
    float m_fov { 40 };
    float m_rollAngle { 0 };
    QVector3D m_eye { 10, 10, 10 };
    QVector3D m_target { 0, 0, 0 };
    QVector3D m_referenceUp { 0, 1, 0 };

public:
    virtual ~Camera() = default;

    virtual float aspectRatio();
    virtual float fov();
    virtual float rollAngle();
    virtual QVector3D eye();
    virtual QVector3D target();
    virtual QVector3D referenceUp();

    virtual void aspectRatio(float value);
    virtual void fov(float value);
    virtual void rollAngle(float value);
    virtual void eye(QVector3D value);
    virtual void target(QVector3D value);
    virtual void referenceUp(QVector3D value);

    virtual QVector3D eyeForward();
    virtual QVector3D eyeUp();
    virtual QVector3D eyeRight();

    virtual void displace(float truck, float boom, float dolly);
    virtual void track(float truck, float boom, float dolly);

    virtual void rotate(float yaw, float pitch, float roll);
    virtual void orbit(float yaw, float pitch, float roll);

    virtual void zoom(float factor);

    virtual QMatrix4x4 viewMatrix();
    virtual QMatrix4x4 projectionMatrix(float nearPlane, float farPlane) = 0;
};



#endif //CAMERA_H
