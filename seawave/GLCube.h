#ifndef GLCUBE_H
#define GLCUBE_H

#include "GLItem.h"
#include <QOpenGLTexture>
#include <QImage>
#include "MaterialProperty.h"
class GLCube : public GLItem
{
public:
    GLCube();
    void draw(GLDrawConfig config);
    void updateBuffer();
    QOpenGLTexture *texture=nullptr;
    void setLight(int index,LightProperty light);
protected:
    static QOpenGLShaderProgram program;
};

#endif // GLCUBE_H
