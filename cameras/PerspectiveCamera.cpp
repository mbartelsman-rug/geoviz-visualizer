
#include "PerspectiveCamera.h"


QMatrix4x4 PerspectiveCamera::projectionMatrix(float nearPlane, float farPlane) {
    QMatrix4x4 matrix;
    matrix.perspective(fov(), aspectRatio(), nearPlane, farPlane);
    return matrix;
}
