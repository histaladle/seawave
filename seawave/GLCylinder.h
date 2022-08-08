#ifndef GLCYLINDER_H
#define GLCYLINDER_H

#include "GLItem.h"
#include "MaterialProperty.h"
class GLCylinder : public GLItem
{
public:
    GLCylinder(float radius,int poly,float height,QVector3D center);
    const float radius;
    const int poly;
    const float height;
    const QVector3D center;
public:
    virtual void updateBuffer();
    virtual void draw(GLDrawConfig config);
    static const int MAX_LIGHT=10;
    virtual void setLight(int index,LightProperty light);
protected:
    static QOpenGLShaderProgram program;
};

#endif // GLCYLINDER_H
