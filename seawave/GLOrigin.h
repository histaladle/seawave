#ifndef GLORIGIN_H
#define GLORIGIN_H

#include "GLItem.h"
#include <QOpenGLShaderProgram>
class GLOrigin : public GLItem
{
public:
    GLOrigin();
    void updateBuffer();
    void draw(GLDrawConfig config);
protected:
    static QOpenGLShaderProgram program;
};

#endif // GLORIGIN_H
