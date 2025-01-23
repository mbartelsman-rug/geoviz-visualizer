
#ifndef LIGHT_H
#define LIGHT_H
#include <QVector3D>


class Light {
     QVector3D m_position { 0, 0, 0 };
     QVector3D m_color { 1, 1, 1 };
public:
     QVector3D position();
     QVector3D color();
     void position(QVector3D);
     void color(QVector3D);
};



#endif //LIGHT_H
