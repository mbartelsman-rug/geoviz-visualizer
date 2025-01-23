
#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "Camera.h"

class PerspectiveCamera : public Camera {
public:
    QMatrix4x4 projectionMatrix(float nearPlane, float farPlane) override;
};



#endif //PERSPECTIVECAMERA_H
