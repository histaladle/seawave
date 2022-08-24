#include "GLCylinder.h"
QOpenGLShaderProgram GLCylinder::program;
GLCylinder::GLCylinder(float radius,int poly,float height, QVector3D center)
    :radius(radius),poly(poly),height(height),center(center)
{

}

void GLCylinder::draw(GLDrawConfig config)
{
    if(!vBuffer.isCreated()) {
        return;
    }
    if(!inited) {
        return;
    }
    QMatrix4x4 projection;
    QMatrix4x4 view,model;
    QVector3D eyeDirection;
    projection=config.projection;
    view=config.view;
    model=config.model;
    program.bind();
    vBuffer.bind();
    QMatrix4x4 normalMat;
    QMatrix4x4 mvMat=view*model;
    normalMat=mvMat.inverted().transposed();
    program.setUniformValue("qt_Model",model);
    program.setUniformValue("qt_View",view);
    program.setUniformValue("qt_Projection",projection);
    program.setUniformValue("qt_NormalMatrix",normalMat);
    eyeDirection=QVector3D(view(2,0),view(2,1),view(2,2));
    program.setUniformValue("qt_EyeDirection",eyeDirection);
    int offset=0;
    int pos_loc,normal_loc;
    pos_loc=program.attributeLocation("qt_vertex");
    program.setAttributeBuffer(pos_loc,GL_FLOAT,offset,3,0);
    glEnableVertexAttribArray(GLuint(pos_loc));
    offset+=(poly*3*3+poly*3*3+poly*3*3*2)*int(sizeof(GLfloat));
    normal_loc=program.attributeLocation("qt_normal");
    program.setAttributeBuffer(normal_loc,GL_FLOAT,offset,3,0);
    glEnableVertexAttribArray(GLuint(normal_loc));

    glDrawArrays(GL_TRIANGLES,0,poly*3);
    glDrawArrays(GL_TRIANGLES,poly*3,poly*3);
    glDrawArrays(GL_TRIANGLES,poly*3*2,poly*6);
//    glDrawArrays(GL_TRIANGLES,poly*3*2,6);

    vBuffer.release();
    program.release();
}

void GLCylinder::updateBuffer()
{
    QVector3D bottomcenter=QVector3D(center.x(),center.y()-height/2,center.z());
    QVector3D topcenter=QVector3D(center.x(),center.y()+height/2,center.z());
    double stepangle=2*M_PI/poly;
    GLfloat *bottomvertices=new GLfloat[3*3*poly];
    GLfloat *bottomnormals=new GLfloat[3*3*poly];
#if 1 //smooth side
    for(int i=0;i<poly*3*3;i+=9) {
        bottomvertices[i  ]=bottomcenter.x();
        bottomvertices[i+1]=bottomcenter.y();
        bottomvertices[i+2]=bottomcenter.z();
        bottomnormals [i  ]= 0.0f;
        bottomnormals [i+1]=-1.0f;
        bottomnormals [i+2]= 0.0f;

        bottomvertices[i+3]=bottomcenter.x()+float(double(radius)*cos(i/9*stepangle));
        bottomvertices[i+4]=bottomcenter.y();
        bottomvertices[i+5]=bottomcenter.z()+float(double(radius)*sin(i/9*stepangle));
        bottomnormals [i+3]= 0.0f;
        bottomnormals [i+4]=-1.0f;
        bottomnormals [i+5]= 0.0f;

        bottomvertices[i+6]=bottomcenter.x()+float(double(radius)*cos(i/9*stepangle+stepangle));
        bottomvertices[i+7]=bottomcenter.y();
        bottomvertices[i+8]=bottomcenter.z()+float(double(radius)*sin(i/9*stepangle+stepangle));
        bottomnormals [i+6]= 0.0f;
        bottomnormals [i+7]=-1.0f;
        bottomnormals [i+8]= 0.0f;
    }
    GLfloat *topvertices=new GLfloat[3*3*poly];
    GLfloat *topnormals=new GLfloat[3*3*poly];
    for(int i=0;i<poly*3*3;i+=9) {
        topvertices[i  ]=topcenter.x();
        topvertices[i+1]=topcenter.y();
        topvertices[i+2]=topcenter.z();
        topnormals [i  ]=0.0f;
        topnormals [i+1]=1.0f;
        topnormals [i+2]=0.0f;

        topvertices[i+3]=topcenter.x()+float(double(radius)*cos(i/9*stepangle+stepangle));
        topvertices[i+4]=topcenter.y();
        topvertices[i+5]=topcenter.z()+float(double(radius)*sin(i/9*stepangle+stepangle));
        topnormals [i+3]=0.0f;
        topnormals [i+4]=1.0f;
        topnormals [i+5]=0.0f;

        topvertices[i+6]=topcenter.x()+float(double(radius)*cos(i/9*stepangle));
        topvertices[i+7]=topcenter.y();
        topvertices[i+8]=topcenter.z()+float(double(radius)*sin(i/9*stepangle));
        topnormals [i+6]=0.0f;
        topnormals [i+7]=1.0f;
        topnormals [i+8]=0.0f;
    }
    GLfloat *sidevertices=new GLfloat[poly*3*3*2];
    GLfloat *sidenormals=new GLfloat[poly*3*3*2];
    for(int i=0;i<poly*3*3*2;i+=18) {
        //--------------------------first triangle--------------------------
        sidevertices[i   ]=bottomcenter.x()+float(double(radius)*cos(i/18*stepangle));
        sidevertices[i+ 1]=bottomcenter.y();
        sidevertices[i+ 2]=bottomcenter.z()+float(double(radius)*sin(i/18*stepangle));
        sidenormals [i   ]=float(cos(i/18*stepangle));
        sidenormals [i+ 1]=0.0;
        sidenormals [i+ 2]=float(sin(i/18*stepangle));

        sidevertices[i+ 3]=topcenter.x()+float(double(radius)*cos(i/18*stepangle));
        sidevertices[i+ 4]=topcenter.y();
        sidevertices[i+ 5]=topcenter.z()+float(double(radius)*sin(i/18*stepangle));
        sidenormals [i+ 3]=float(cos(i/18*stepangle));
        sidenormals [i+ 4]=0.0;
        sidenormals [i+ 5]=float(sin(i/18*stepangle));

        sidevertices[i+ 6]=bottomcenter.x()+float(double(radius)*cos(i/18*stepangle+stepangle));
        sidevertices[i+ 7]=bottomcenter.y();
        sidevertices[i+ 8]=bottomcenter.z()+float(double(radius)*sin(i/18*stepangle+stepangle));
        sidenormals [i+ 6]=float(cos(i/18*stepangle+stepangle));
        sidenormals [i+ 7]=0.0;
        sidenormals [i+ 8]=float(sin(i/18*stepangle+stepangle));

        //--------------------------second triangle--------------------------
        sidevertices[i+ 9]=bottomcenter.x()+float(double(radius)*cos(i/18*stepangle+stepangle));
        sidevertices[i+10]=bottomcenter.y();
        sidevertices[i+11]=bottomcenter.z()+float(double(radius)*sin(i/18*stepangle+stepangle));
        sidenormals [i+ 9]=float(cos(i/18*stepangle+stepangle));
        sidenormals [i+10]=0.0;
        sidenormals [i+11]=float(sin(i/18*stepangle+stepangle));

        sidevertices[i+12]=topcenter.x()+float(double(radius)*cos(i/18*stepangle));
        sidevertices[i+13]=topcenter.y();
        sidevertices[i+14]=topcenter.z()+float(double(radius)*sin(i/18*stepangle));
        sidenormals [i+12]=float(cos(i/18*stepangle));
        sidenormals [i+13]=0.0;
        sidenormals [i+14]=float(sin(i/18*stepangle));

        sidevertices[i+15]=topcenter.x()+float(double(radius)*cos(i/18*stepangle+stepangle));
        sidevertices[i+16]=topcenter.y();
        sidevertices[i+17]=topcenter.z()+float(double(radius)*sin(i/18*stepangle+stepangle));
        sidenormals [i+15]=float(cos(i/18*stepangle+stepangle));
        sidenormals [i+16]=0.0;
        sidenormals [i+17]=float(sin(i/18*stepangle+stepangle));
    }
#endif
#if 0 //polygon side
    //cylinder
    for(int i=0;i<poly*3*3*2;i+=18) {
        //--------------------------first triangle--------------------------
        sidevertices[i   ]=bottomcenter.x()+float(double(radius)*cos(i/18*stepangle));
        sidevertices[i+ 1]=bottomcenter.y();
        sidevertices[i+ 2]=bottomcenter.z()+float(double(radius)*sin(i/18*stepangle));
        sidenormals [i   ]=float(cos(i/18*stepangle+stepangle/2));
        sidenormals [i+ 1]=0.0;
        sidenormals [i+ 2]=float(sin(i/18*stepangle+stepangle/2));

        sidevertices[i+ 3]=topcenter.x()+float(double(radius)*cos(i/18*stepangle));
        sidevertices[i+ 4]=topcenter.y();
        sidevertices[i+ 5]=topcenter.z()+float(double(radius)*sin(i/18*stepangle));
        sidenormals [i+ 3]=float(cos(i/18*stepangle+stepangle/2));
        sidenormals [i+ 4]=0.0;
        sidenormals [i+ 5]=float(sin(i/18*stepangle+stepangle/2));

        sidevertices[i+ 6]=bottomcenter.x()+float(double(radius)*cos(i/18*stepangle+stepangle));
        sidevertices[i+ 7]=bottomcenter.y();
        sidevertices[i+ 8]=bottomcenter.z()+float(double(radius)*sin(i/18*stepangle+stepangle));
        sidenormals [i+ 6]=float(cos(i/18*stepangle+stepangle/2));
        sidenormals [i+ 7]=0.0;
        sidenormals [i+ 8]=float(sin(i/18*stepangle+stepangle/2));

        //--------------------------second triangle--------------------------
        sidevertices[i+ 9]=bottomcenter.x()+float(double(radius)*cos(i/18*stepangle+stepangle));
        sidevertices[i+10]=bottomcenter.y();
        sidevertices[i+11]=bottomcenter.z()+float(double(radius)*sin(i/18*stepangle+stepangle));
        sidenormals [i+ 9]=float(cos(i/18*stepangle+stepangle/2));
        sidenormals [i+10]=0.0;
        sidenormals [i+11]=float(sin(i/18*stepangle+stepangle/2));

        sidevertices[i+12]=topcenter.x()+float(double(radius)*cos(i/18*stepangle));
        sidevertices[i+13]=topcenter.y();
        sidevertices[i+14]=topcenter.z()+float(double(radius)*sin(i/18*stepangle));
        sidenormals [i+12]=float(cos(i/18*stepangle+stepangle/2));
        sidenormals [i+13]=0.0;
        sidenormals [i+14]=float(sin(i/18*stepangle+stepangle/2));

        sidevertices[i+15]=topcenter.x()+float(double(radius)*cos(i/18*stepangle+stepangle));
        sidevertices[i+16]=topcenter.y();
        sidevertices[i+17]=topcenter.z()+float(double(radius)*sin(i/18*stepangle+stepangle));
        sidenormals [i+15]=float(cos(i/18*stepangle+stepangle/2));
        sidenormals [i+16]=0.0;
        sidenormals [i+17]=float(sin(i/18*stepangle+stepangle/2));
    }
#endif
    initializeOpenGLFunctions();
    if(!program.isLinked()) {
        QOpenGLShader *vsh=nullptr;
        vsh=new QOpenGLShader(QOpenGLShader::Vertex);
        qDebug() << ":/shader/facet.vsh"
                 << vsh->compileSourceFile(":/shader/facet.vsh");
        QOpenGLShader *fsh=nullptr;
        fsh=new QOpenGLShader(QOpenGLShader::Fragment);
        qDebug() << ":/shader/facet.fsh"
                 << fsh->compileSourceFile(":/shader/facet.fsh");
        program.addShader(vsh);
        program.addShader(fsh);
        program.link();
    }
    program.bind();
    MaterialProperty material;
    material.emission=QVector3D(0.0,0.0,0.0);
    material.ambient=QVector3D(1.0f,1.0f,1.0f)*0.5f;
    material.diffuse=QVector3D(1.0f,1.0f,1.0f)*0.20f;
    material.specular=QVector3D(1.0,1.0,1.0)*1.0f;
    material.shininess=100.0;
    program.setUniformValue("qt_Material.emission", material.emission);
    program.setUniformValue("qt_Material.ambient", material.ambient);
    program.setUniformValue("qt_Material.diffuse", material.diffuse);
    program.setUniformValue("qt_Material.specular", material.specular);
    program.setUniformValue("qt_Material.shininess", material.shininess);

    program.setUniformValue("qt_Strength",GLfloat(0.8));
    program.release();
    vBuffer.create();
    vBuffer.bind();
    qDebug() << "debug52" << (poly*3*3+poly*3*3+poly*3*3*2)*2*int(sizeof(GLfloat));
    vBuffer.allocate((poly*3*3+poly*3*3+poly*3*3*2)*2*int(sizeof(GLfloat)));
    qDebug() << "debug53" << vBuffer.size();
    int offset=0;

    vBuffer.write(offset,bottomvertices,poly*3*3*int(sizeof(GLfloat)));
    offset+=poly*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,topvertices,poly*3*3*int(sizeof(GLfloat)));
    offset+=poly*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,sidevertices,(poly*3*3*2)*int(sizeof(GLfloat)));
    offset+=(poly*3*3*2)*int(sizeof(GLfloat));

    vBuffer.write(offset,bottomnormals,poly*3*3*int(sizeof(GLfloat)));
    offset+=poly*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,topnormals,poly*3*3*int(sizeof(GLfloat)));
    offset+=poly*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,sidenormals,(poly*3*3*2)*int(sizeof(GLfloat)));

    vBuffer.release();
    inited=true;

    delete [] bottomvertices;
    delete [] bottomnormals;
    delete [] topvertices;
    delete [] topnormals;
    delete [] sidevertices;
    delete [] sidenormals;
}

void GLCylinder::setLight(int index, LightProperty light)
{
    if(index>=MAX_LIGHT)
    {
        return;
    }
    QString si=QString::number(index);
    int loc;
    program.bind();
    loc=program.uniformLocation("qt_Lights["+si+"].isEnable");
    program.setUniformValue(loc,light.isEnable);

    loc=program.uniformLocation("qt_Lights["+si+"].isLocal");
    program.setUniformValue(loc,light.isLocal);

    loc=program.uniformLocation("qt_Lights["+si+"].isSpot");
    program.setUniformValue(loc,light.isSpot);

    loc=program.uniformLocation("qt_Lights["+si+"].ambient");
    program.setUniformValue(loc,light.ambient);

    loc=program.uniformLocation("qt_Lights["+si+"].color");
    program.setUniformValue(loc,light.color);

    loc=program.uniformLocation("qt_Lights["+si+"].position");
    program.setUniformValue(loc,light.position);

    loc=program.uniformLocation("qt_Lights["+si+"].halfVector");
    program.setUniformValue(loc,light.halfVector);

    loc=program.uniformLocation("qt_Lights["+si+"].coneDirection");
    program.setUniformValue(loc,light.coneDirection);

    loc=program.uniformLocation("qt_Lights["+si+"].spotCosCutoff");
    program.setUniformValue(loc,light.spotCosCutoff);

    loc=program.uniformLocation("qt_Lights["+si+"].spotExponent");
    program.setUniformValue(loc,light.spotExponent);

    loc=program.uniformLocation("qt_Lights["+si+"].constantAttenuation");
    program.setUniformValue(loc,light.constantAttenuation);

    loc=program.uniformLocation("qt_Lights["+si+"].linearAttenuation");
    program.setUniformValue(loc,light.linearAttenuation);

    loc=program.uniformLocation("qt_Lights["+si+"].quadraticAttenuation");
    program.setUniformValue(loc,light.quadraticAttenuation);

    program.release();
}
