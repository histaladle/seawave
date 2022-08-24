#include "GLSphere.h"
QOpenGLShaderProgram GLSphere::program;
QOpenGLShaderProgram GLSphere::depthProgram;
GLSphere::GLSphere(int longitudeCount,int latitudeCount,float radius,QVector3D center)
    :longitudeCount(longitudeCount),latitudeCount(latitudeCount),radius(radius),center(center)
{

}

void GLSphere::draw(GLDrawConfig config)
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
//    qDebug() << "debug72" << program.bind();
//    qDebug() << "debug73" << vBuffer.bind();
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
    offset+=(longitudeCount*3*3+longitudeCount*3*3+(latitudeCount-1)*longitudeCount*2*3*3)*int(sizeof(GLfloat));

    normal_loc=program.attributeLocation("qt_normal");
    program.setAttributeBuffer(normal_loc,GL_FLOAT,offset,3,0);
    glEnableVertexAttribArray(GLuint(normal_loc));
    offset+=(longitudeCount*3*3+longitudeCount*3*3+(latitudeCount-1)*longitudeCount*2*3*3)*int(sizeof(GLfloat));

    //draw south
    glDrawArrays(GL_TRIANGLES,0,longitudeCount*3);
    //draw north
    glDrawArrays(GL_TRIANGLES,longitudeCount*3,longitudeCount*3);
    //draw side
    glDrawArrays(GL_TRIANGLES,longitudeCount*3+longitudeCount*3,(latitudeCount-1)*longitudeCount*2*3);

    vBuffer.release();
    program.release();
}

void GLSphere::updateBuffer()
{
    QVector3D north=QVector3D(center.x(),center.y()+radius,center.z());
    QVector3D south=QVector3D(center.x(),center.y()-radius,center.z());
    double longitudeStep=2*M_PI/longitudeCount;
    double latitudeStep=M_PI/(latitudeCount+1);
    GLfloat *southvertices=new GLfloat[longitudeCount*3*3]{0};
    GLfloat *northvertices=new GLfloat[longitudeCount*3*3]{0};
    GLfloat *southnormals=new GLfloat[longitudeCount*3*3]{0};
    GLfloat *northnormals=new GLfloat[longitudeCount*3*3]{0};
    GLfloat *sidevertices=new GLfloat[(latitudeCount-1)*longitudeCount*2*3*3]{0};
    GLfloat *sidenormals=new GLfloat[(latitudeCount-1)*longitudeCount*2*3*3]{0};
    QVector3D *grid=new QVector3D[latitudeCount*longitudeCount];
    for(int i=0;i<latitudeCount;i++) {
        for(int j=0;j<longitudeCount;j++) {
            grid[i*longitudeCount+j].setX(radius*float(sin((i+1)*latitudeStep))*float(cos(j*longitudeStep)));
            grid[i*longitudeCount+j].setY(center.y()-radius*float(cos((i+1)*latitudeStep)));
            grid[i*longitudeCount+j].setZ(radius*float(sin((i+1)*latitudeStep))*float(sin(j*longitudeStep)));
        }
    }
#if 1 //smooth sphere
    for(int i=0;i<longitudeCount*3*3;i+=9) {
        southvertices[i  ]=south.x();
        southvertices[i+1]=south.y();
        southvertices[i+2]=south.z();

        southvertices[i+3]=radius*float(sin(latitudeStep))*float(cos(i/9*longitudeStep));
        southvertices[i+4]=center.y()-radius*float(cos(latitudeStep));
        southvertices[i+5]=radius*float(sin(latitudeStep))*float(sin(i/9*longitudeStep));

        southvertices[i+6]=radius*float(sin(latitudeStep))*float(cos(i/9*longitudeStep+longitudeStep));
        southvertices[i+7]=center.y()-radius*float(cos(latitudeStep));
        southvertices[i+8]=radius*float(sin(latitudeStep))*float(sin(i/9*longitudeStep+longitudeStep));

        QVector3D normal;
        normal=(south-center).normalized();
        southnormals[i  ]=normal.x();
        southnormals[i+1]=normal.y();
        southnormals[i+2]=normal.z();

        normal=(QVector3D(southvertices[i+3],southvertices[i+4],southvertices[i+5])-center).normalized();
        southnormals[i+3]=normal.x();
        southnormals[i+4]=normal.y();
        southnormals[i+5]=normal.z();

        normal=(QVector3D(southvertices[i+6],southvertices[i+7],southvertices[i+8])-center).normalized();
        southnormals[i+6]=normal.x();
        southnormals[i+7]=normal.y();
        southnormals[i+8]=normal.z();
    }
    for(int i=0;i<longitudeCount*3*3;i+=9) {
        northvertices[i  ]=north.x();
        northvertices[i+1]=north.y();
        northvertices[i+2]=north.z();

        northvertices[i+3]=radius*float(sin(latitudeStep))*float(cos(i/9*longitudeStep));
        northvertices[i+4]=center.y()+radius*float(cos(latitudeStep));
        northvertices[i+5]=radius*float(sin(latitudeStep))*float(sin(i/9*longitudeStep));

        northvertices[i+6]=radius*float(sin(latitudeStep))*float(cos(i/9*longitudeStep+longitudeStep));
        northvertices[i+7]=center.y()+radius*float(cos(latitudeStep));
        northvertices[i+8]=radius*float(sin(latitudeStep))*float(sin(i/9*longitudeStep+longitudeStep));

        QVector3D normal;
        normal=-(north-center).normalized();
        northnormals[i  ]=normal.x();
        northnormals[i+1]=normal.y();
        northnormals[i+2]=normal.z();

        normal=-(QVector3D(northvertices[i+3],northvertices[i+4],northvertices[i+5])-center).normalized();
        northnormals[i+3]=normal.x();
        northnormals[i+4]=normal.y();
        northnormals[i+5]=normal.z();

        normal=-(QVector3D(northvertices[i+6],northvertices[i+7],northvertices[i+8])-center).normalized();
        northnormals[i+6]=normal.x();
        northnormals[i+7]=normal.y();
        northnormals[i+8]=normal.z();
    }
    for(int i=0;i<latitudeCount-1;i++) {
        for(int j=0;j<longitudeCount*2*3*3;j+=18) {
//            qDebug() << "debug82" << i << i*longitudeCount*2*3*3;
            QVector3D normal;
            //---------------------first face---------------------
            sidevertices[i*longitudeCount*2*3*3+j   ]=grid[i*longitudeCount+((j/18+1)%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+ 1]=grid[i*longitudeCount+((j/18+1)%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+ 2]=grid[i*longitudeCount+((j/18+1)%longitudeCount)].z();

            sidevertices[i*longitudeCount*2*3*3+j+ 3]=grid[i*longitudeCount+(j/18%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+ 4]=grid[i*longitudeCount+(j/18%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+ 5]=grid[i*longitudeCount+(j/18%longitudeCount)].z();

            sidevertices[i*longitudeCount*2*3*3+j+ 6]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+ 7]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+ 8]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].z();

            //---------------------second face---------------------
            sidevertices[i*longitudeCount*2*3*3+j+ 9]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+10]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+11]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].z();

            sidevertices[i*longitudeCount*2*3*3+j+12]=grid[i*longitudeCount+(j/18%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+13]=grid[i*longitudeCount+(j/18%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+14]=grid[i*longitudeCount+(j/18%longitudeCount)].z();

            sidevertices[i*longitudeCount*2*3*3+j+15]=grid[(i+1)*longitudeCount+(j/18%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+16]=grid[(i+1)*longitudeCount+(j/18%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+17]=grid[(i+1)*longitudeCount+(j/18%longitudeCount)].z();

            //---------------------first noramls---------------------
            normal=(grid[i*longitudeCount+((j/18+1)%longitudeCount)]-center).normalized();
            sidenormals[i*longitudeCount*2*3*3+j   ]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+ 1]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+ 2]=normal.z();

            normal=(grid[i*longitudeCount+(j/18%longitudeCount)]-center).normalized();
            sidenormals[i*longitudeCount*2*3*3+j+ 3]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+ 4]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+ 5]=normal.z();

            normal=(grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)]-center).normalized();
            sidenormals[i*longitudeCount*2*3*3+j+ 6]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+ 7]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+ 8]=normal.z();

            //---------------------second noramls---------------------
            normal=(grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)]-center).normalized();
            sidenormals[i*longitudeCount*2*3*3+j+ 9]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+10]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+11]=normal.z();

            normal=(grid[i*longitudeCount+(j/18%longitudeCount)]-center).normalized();
            sidenormals[i*longitudeCount*2*3*3+j+12]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+13]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+14]=normal.z();

            normal=(grid[(i+1)*longitudeCount+(j/18%longitudeCount)]-center).normalized();
            sidenormals[i*longitudeCount*2*3*3+j+15]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+16]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+17]=normal.z();
        }
    }
#endif
#if 0 //polygon sphere
    //sphere
    for(int i=0;i<longitudeCount*3*3;i+=9) {
        southvertices[i  ]=south.x();
        southvertices[i+1]=south.y();
        southvertices[i+2]=south.z();

        southvertices[i+3]=radius*float(sin(latitudeStep))*float(cos(i/9*longitudeStep));
        southvertices[i+4]=center.y()-radius*float(cos(latitudeStep));
        southvertices[i+5]=radius*float(sin(latitudeStep))*float(sin(i/9*longitudeStep));

        southvertices[i+6]=radius*float(sin(latitudeStep))*float(cos(i/9*longitudeStep+longitudeStep));
        southvertices[i+7]=center.y()-radius*float(cos(latitudeStep));
        southvertices[i+8]=radius*float(sin(latitudeStep))*float(sin(i/9*longitudeStep+longitudeStep));

    //        southvertices[i+3]=grid[i/9%longitudeCount].x();
    //        southvertices[i+4]=grid[i/9%longitudeCount].y();
    //        southvertices[i+5]=grid[i/9%longitudeCount].z();

    //        southvertices[i+6]=grid[(i/9+1)%longitudeCount].x();
    //        southvertices[i+7]=grid[(i/9+1)%longitudeCount].y();
    //        southvertices[i+8]=grid[(i/9+1)%longitudeCount].z();
        QVector3D normal;
        QVector3D v[3];
        v[0]=QVector3D(southvertices[i  ],southvertices[i+1],southvertices[i+2]);
        v[1]=QVector3D(southvertices[i+3],southvertices[i+4],southvertices[i+5]);
        v[2]=QVector3D(southvertices[i+6],southvertices[i+7],southvertices[i+8]);
        normal=QVector3D::normal(v[0],v[1],v[2]);
        southnormals[i  ]=normal.x();
        southnormals[i+1]=normal.y();
        southnormals[i+2]=normal.z();

        southnormals[i+3]=normal.x();
        southnormals[i+4]=normal.y();
        southnormals[i+5]=normal.z();

        southnormals[i+6]=normal.x();
        southnormals[i+7]=normal.y();
        southnormals[i+8]=normal.z();
    }
    for(int i=0;i<longitudeCount*3*3;i+=9) {
        northvertices[i  ]=north.x();
        northvertices[i+1]=north.y();
        northvertices[i+2]=north.z();

        northvertices[i+3]=radius*float(sin(latitudeStep))*float(cos(i/9*longitudeStep));
        northvertices[i+4]=center.y()+radius*float(cos(latitudeStep));
        northvertices[i+5]=radius*float(sin(latitudeStep))*float(sin(i/9*longitudeStep));

        northvertices[i+6]=radius*float(sin(latitudeStep))*float(cos(i/9*longitudeStep+longitudeStep));
        northvertices[i+7]=center.y()+radius*float(cos(latitudeStep));
        northvertices[i+8]=radius*float(sin(latitudeStep))*float(sin(i/9*longitudeStep+longitudeStep));

        QVector3D normal;
        QVector3D v[3];
        v[0]=QVector3D(northvertices[i  ],northvertices[i+1],northvertices[i+2]);
        v[1]=QVector3D(northvertices[i+3],northvertices[i+4],northvertices[i+5]);
        v[2]=QVector3D(northvertices[i+6],northvertices[i+7],northvertices[i+8]);
        normal=QVector3D::normal(v[0],v[1],v[2]);
        northnormals[i  ]=normal.x();
        northnormals[i+1]=normal.y();
        northnormals[i+2]=normal.z();

        northnormals[i+3]=normal.x();
        northnormals[i+4]=normal.y();
        northnormals[i+5]=normal.z();

        northnormals[i+6]=normal.x();
        northnormals[i+7]=normal.y();
        northnormals[i+8]=normal.z();
    }
    for(int i=0;i<latitudeCount-1;i++) {
        for(int j=0;j<longitudeCount*2*3*3;j+=18) {
    //            qDebug() << "debug82" << i << i*longitudeCount*2*3*3;
            QVector3D normal;
            QVector3D v[3];
            //---------------------first face---------------------
            sidevertices[i*longitudeCount*2*3*3+j   ]=grid[i*longitudeCount+((j/18+1)%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+ 1]=grid[i*longitudeCount+((j/18+1)%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+ 2]=grid[i*longitudeCount+((j/18+1)%longitudeCount)].z();

            sidevertices[i*longitudeCount*2*3*3+j+ 3]=grid[i*longitudeCount+(j/18%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+ 4]=grid[i*longitudeCount+(j/18%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+ 5]=grid[i*longitudeCount+(j/18%longitudeCount)].z();

            sidevertices[i*longitudeCount*2*3*3+j+ 6]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+ 7]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+ 8]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].z();

            //---------------------second face---------------------
            sidevertices[i*longitudeCount*2*3*3+j+ 9]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+10]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+11]=grid[(i+1)*longitudeCount+((j/18+1)%longitudeCount)].z();

            sidevertices[i*longitudeCount*2*3*3+j+12]=grid[i*longitudeCount+(j/18%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+13]=grid[i*longitudeCount+(j/18%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+14]=grid[i*longitudeCount+(j/18%longitudeCount)].z();

            sidevertices[i*longitudeCount*2*3*3+j+15]=grid[(i+1)*longitudeCount+(j/18%longitudeCount)].x();
            sidevertices[i*longitudeCount*2*3*3+j+16]=grid[(i+1)*longitudeCount+(j/18%longitudeCount)].y();
            sidevertices[i*longitudeCount*2*3*3+j+17]=grid[(i+1)*longitudeCount+(j/18%longitudeCount)].z();

            //---------------------first noramls---------------------
            v[0]=QVector3D(sidevertices[i*longitudeCount*2*3*3+j   ],sidevertices[i*longitudeCount*2*3*3+j+ 1],sidevertices[i*longitudeCount*2*3*3+j+ 2]);
            v[1]=QVector3D(sidevertices[i*longitudeCount*2*3*3+j+ 3],sidevertices[i*longitudeCount*2*3*3+j+ 4],sidevertices[i*longitudeCount*2*3*3+j+ 5]);
            v[2]=QVector3D(sidevertices[i*longitudeCount*2*3*3+j+ 6],sidevertices[i*longitudeCount*2*3*3+j+ 7],sidevertices[i*longitudeCount*2*3*3+j+ 8]);
            normal=QVector3D::normal(v[0],v[1],v[2]);

            sidenormals[i*longitudeCount*2*3*3+j   ]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+ 1]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+ 2]=normal.z();

            sidenormals[i*longitudeCount*2*3*3+j+ 3]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+ 4]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+ 5]=normal.z();

            sidenormals[i*longitudeCount*2*3*3+j+ 6]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+ 7]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+ 8]=normal.z();

            //---------------------second noramls---------------------
            v[0]=QVector3D(sidevertices[i*longitudeCount*2*3*3+j+ 9],sidevertices[i*longitudeCount*2*3*3+j+10],sidevertices[i*longitudeCount*2*3*3+j+11]);
            v[1]=QVector3D(sidevertices[i*longitudeCount*2*3*3+j+12],sidevertices[i*longitudeCount*2*3*3+j+13],sidevertices[i*longitudeCount*2*3*3+j+14]);
            v[2]=QVector3D(sidevertices[i*longitudeCount*2*3*3+j+15],sidevertices[i*longitudeCount*2*3*3+j+16],sidevertices[i*longitudeCount*2*3*3+j+17]);
            normal=QVector3D::normal(v[0],v[1],v[2]);

            sidenormals[i*longitudeCount*2*3*3+j+ 9]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+10]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+11]=normal.z();

            sidenormals[i*longitudeCount*2*3*3+j+12]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+13]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+14]=normal.z();

            sidenormals[i*longitudeCount*2*3*3+j+15]=normal.x();
            sidenormals[i*longitudeCount*2*3*3+j+16]=normal.y();
            sidenormals[i*longitudeCount*2*3*3+j+17]=normal.z();
        }
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
    material.ambient=QVector3D(1.0f,1.0f,1.0f)*0.2f;
    material.diffuse=QVector3D(1.0f,1.0f,1.0f)*0.5f;
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
    vBuffer.allocate((longitudeCount*3*3+longitudeCount*3*3+(latitudeCount-1)*longitudeCount*2*3*3)*2*int(sizeof(GLfloat)));
    int offset=0;

    vBuffer.write(offset,southvertices,longitudeCount*3*3*int(sizeof(GLfloat)));
    offset+=longitudeCount*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,northvertices,longitudeCount*3*3*int(sizeof(GLfloat)));
    offset+=longitudeCount*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,sidevertices,(latitudeCount-1)*longitudeCount*2*3*3*int(sizeof(GLfloat)));
    offset+=(latitudeCount-1)*longitudeCount*2*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,southnormals,longitudeCount*3*3*int(sizeof(GLfloat)));
    offset+=longitudeCount*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,northnormals,longitudeCount*3*3*int(sizeof(GLfloat)));
    offset+=longitudeCount*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,sidenormals,(latitudeCount-1)*longitudeCount*2*3*3*int(sizeof(GLfloat)));
    offset+=(latitudeCount-1)*longitudeCount*2*3*3*int(sizeof(GLfloat));

    vBuffer.release();
    inited=true;
}

void GLSphere::setLight(int index, LightProperty light)
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

void GLSphere::renderDepth(GLDrawConfig config)
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
    depthProgram.bind();
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
    offset+=(longitudeCount*3*3+longitudeCount*3*3+(latitudeCount-1)*longitudeCount*2*3*3)*int(sizeof(GLfloat));

    normal_loc=program.attributeLocation("qt_normal");
    program.setAttributeBuffer(normal_loc,GL_FLOAT,offset,3,0);
    glEnableVertexAttribArray(GLuint(normal_loc));
    offset+=(longitudeCount*3*3+longitudeCount*3*3+(latitudeCount-1)*longitudeCount*2*3*3)*int(sizeof(GLfloat));

    //draw south
//    glDrawArrays(GL_TRIANGLES,0,longitudeCount*3);
    //draw north
//    glDrawArrays(GL_TRIANGLES,longitudeCount*3,longitudeCount*3);
    //draw side
    glDrawArrays(GL_TRIANGLES,longitudeCount*3+longitudeCount*3,(latitudeCount-1)*longitudeCount*2*3);
}
