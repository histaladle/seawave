#ifndef GLSPHERE_H
#define GLSPHERE_H

#include "GLItem.h"
class GLSphere : public GLItem
{
public:
    GLSphere(int longitudeCount,int latitudeCount,float radius,QVector3D center);
    const int longitudeCount;
    const int latitudeCount;
    const float radius;
    QVector3D center;
public:
    virtual void updateBuffer();
    virtual void draw(GLDrawConfig config);
    static const int MAX_LIGHT=10;
    virtual void setLight(int index,LightProperty light);
    virtual void renderDepth(GLDrawConfig config);
protected:
    static QOpenGLShaderProgram program;
    static QOpenGLShaderProgram depthProgram;
};

#endif // GLSPHERE_H
