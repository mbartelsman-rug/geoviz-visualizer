
#include "Light.h"

QVector3D Light::position() {
    return m_position;
}

QVector3D Light::color() {
    return m_color;
}

void Light::position(QVector3D value) {
    m_position = value;
}

void Light::color(QVector3D value) {
    m_color = value;
}
