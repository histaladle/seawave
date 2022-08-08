#ifndef MATERIALPROPERTY_H
#define MATERIALPROPERTY_H

#include <QVector3D>
#include <QOpenGLFunctions>

struct MaterialProperty {
    QVector3D emission;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    GLfloat shininess; // 0 ~ 1000
};

#endif // MATERIALPROPERTY_H
