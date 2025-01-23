
#include "OrthographicCamera.h"

QMatrix4x4 OrthographicCamera::projectionMatrix(float nearPlane, float farPlane) {
    float angle = qAtan2(1.0f, aspectRatio());
    float width = qCos(angle * fov());
    float height = qSin(angle * fov());

    float left = -width / 2.0f;
    float right = width / 2.0f;
    float bottom = -height / 2.0f;
    float top = height / 2.0f;

    QMatrix4x4 matrix;
    matrix.ortho(left, right, bottom, top, nearPlane, farPlane);
    return matrix;
}
