#include "GLOrigin.h"
QOpenGLShaderProgram GLOrigin::program;
GLOrigin::GLOrigin()
{

}

void GLOrigin::draw(GLDrawConfig config)
{
    if(!vBuffer.isCreated()) {
        return;
    }
    if(!inited) {
        return;
    }
    QMatrix4x4 projection;
    QMatrix4x4 model, view;
    projection=config.projection;
    model=config.model;
    view=config.view;
    program.bind();
    vBuffer.bind();
    program.setUniformValue("qt_Model",model);
    program.setUniformValue("qt_View",view);
    program.setUniformValue("qt_Projection",projection);
    int pos_loc,color_loc;
    pos_loc=program.attributeLocation("qt_vertex");
    program.setAttributeBuffer(pos_loc,GL_FLOAT,0,3,0);
    glEnableVertexAttribArray(GLuint(pos_loc));
    color_loc=program.attributeLocation("qt_color");
    program.setAttributeBuffer(color_loc,GL_FLOAT,18*sizeof(GLfloat),3,0);
    glEnableVertexAttribArray(GLuint(color_loc));
    glDrawArrays(GL_LINES,0,6);

    vBuffer.release();
    program.release();
}

void GLOrigin::updateBuffer()
{
    QVector3D vx(5.0, 0.0, 0.0);
    QVector3D vy(0.0, 5.0, 0.0);
    QVector3D vz(0.0, 0.0, 5.0);
    QVector3D o;
    o = QVector3D(0.0, 0.0, 0.0);

    GLfloat lcolors[3*2*3] =
    {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    GLfloat lvertices[3*2*3] =
    {
         o.x(),  o.y(),  o.z(),
        vx.x(), vx.y(), vx.z(),
         o.x(),  o.y(),  o.z(),
        vy.x(), vy.y(), vy.z(),
         o.x(),  o.y(),  o.z(),
        vz.x(), vz.y(), vz.z(),
    };
    initializeOpenGLFunctions();
    if(!program.isLinked()) {
        QOpenGLShader *vsh=nullptr;
        vsh=new QOpenGLShader(QOpenGLShader::Vertex);
        qDebug() << ":/shader/line.vsh"
                 << vsh->compileSourceFile(":/shader/line.vsh");
        QOpenGLShader *fsh=nullptr;
        fsh=new QOpenGLShader(QOpenGLShader::Fragment);
        qDebug() << ":/shader/line.fsh"
                 << fsh->compileSourceFile(":/shader/line.fsh");
        program.addShader(vsh);
        program.addShader(fsh);
        program.link();
    }
    program.bind();
    vBuffer.create();
    vBuffer.bind();
    vBuffer.allocate(36*sizeof(GLfloat));
    vBuffer.write(0,lvertices,18*sizeof(GLfloat));
    vBuffer.write(18*sizeof(GLfloat),lcolors,18*sizeof(GLfloat));
    vBuffer.release();
    inited=true;
    program.release();
}
