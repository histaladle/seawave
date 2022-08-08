#ifndef GLSEASURFACE_H
#define GLSEASURFACE_H

#include "GLItem.h"
#include "PhillipSpectrum.h"
class GLSeaSurface : public GLItem
{
public:
    GLSeaSurface();
    void draw(GLDrawConfig config);
    void updateBuffer();
    float width=1.0,height=1.0;
    int row=64,column=64;
    QVector3D center=QVector3D(0.0,0.0,0.0);
    GLfloat *vertice_buffer=nullptr;
    GLfloat *normal_buffer=nullptr;
    QVector3D *position_vecs=nullptr;
    void setLight(int index,LightProperty light);
    void updateFrame();
    PhillipSpectrum *phillipSepctrum=new PhillipSpectrum(row,column,double(width),double(height));
protected:
    static QOpenGLShaderProgram program;
};

#endif // GLSEASURFACE_H
