
#include "PerspectiveCamera.h"


QMatrix4x4 PerspectiveCamera::projectionMatrix(float nearPlane, float farPlane) {
    const float DEG2RAD = acosf(-1.0f) / 180.0f;

    const float tangent = tanf(fov() / 2 * DEG2RAD);
    const float right = nearPlane * tangent;
    const float top = right / aspectRatio();

    QMatrix4x4 projectionMat;

    projectionMat(0, 0) = nearPlane / right;
    projectionMat(1, 1) = nearPlane / top;
    projectionMat(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
    projectionMat(2, 3) = -(2 * farPlane * nearPlane) / (farPlane - nearPlane);
    projectionMat(3, 2) = -1;
    projectionMat(3, 3) = 0;

    return projectionMat;
}
