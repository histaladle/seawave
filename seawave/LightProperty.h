#ifndef LIGHTPROPERTY_H
#define LIGHTPROPERTY_H

#include <QVector3D>
#include <QOpenGLFunctions>

struct LightProperty {
    GLboolean isEnable;
    GLboolean isLocal;
    GLboolean isSpot;
    QVector3D ambient;
    QVector3D color;
    QVector3D position;
    QVector3D halfVector;
    QVector3D coneDirection;
    GLfloat spotCosCutoff;
    GLfloat spotExponent;
    GLfloat constantAttenuation;
    GLfloat linearAttenuation;
    GLfloat quadraticAttenuation;
    GLuint texLoc;
};

#endif // LIGHTPROPERTY_H
