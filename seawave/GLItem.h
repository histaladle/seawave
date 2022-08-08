#ifndef GLITEM_H
#define GLITEM_H

#include <QPoint>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <qmath.h>
#include <QMatrix4x4>
#include "LightProperty.h"
#include "MaterialProperty.h"
#include "GLDrawConfig.h"

class GLItem : protected QOpenGLFunctions
{
public:
    GLItem();
    virtual ~GLItem();
public:
    virtual void updateBuffer();
    virtual void draw(GLDrawConfig config);
    static const int MAX_LIGHT=10;
    virtual void setLight(int index,LightProperty light);
    virtual void renderDepth(GLDrawConfig config);
    virtual void updateFrame();
protected:
    bool inited=false;
    QOpenGLBuffer vBuffer;
};

#endif // GLITEM_H
