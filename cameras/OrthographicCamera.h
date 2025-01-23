
#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H
#include "Camera.h"


class OrthographicCamera : Camera {
public:
    QMatrix4x4 projectionMatrix(float nearPlane, float farPlane) override;
};



#endif //ORTHOGRAPHICCAMERA_H
