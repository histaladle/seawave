#include "GLSeaSurface.h"
QOpenGLShaderProgram GLSeaSurface::program;
GLSeaSurface::GLSeaSurface()
{

}

void GLSeaSurface::draw(GLDrawConfig config)
{
    if(!vBuffer.isCreated()) {
        return;
    }
    if(!inited) {
        return;
    }
    QVector3D eyeDirection;
    QMatrix4x4 model,view,projection;
    model=config.model;
    view=config.view;
    projection=config.projection;
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
    GLint pos_loc,normal_loc;
    int offset=0;
    pos_loc=program.attributeLocation("qt_vertex");
    program.setAttributeBuffer(pos_loc, GL_FLOAT, offset, 3, 0);
    offset+=(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat));
    program.enableAttributeArray(pos_loc);

    normal_loc = program.attributeLocation("qt_normal");
    program.setAttributeBuffer(normal_loc,GL_FLOAT,offset,3,0);
    offset+=(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat));
    program.enableAttributeArray(normal_loc);

    glDrawArrays(GL_TRIANGLES,0,(row-1)*(column-1)*2*3);
    vBuffer.release();
    program.release();
}

void GLSeaSurface::updateFrame()
{
    if(!vBuffer.isCreated()) {
        return;
    }
    if(!inited) {
        return;
    }
    phillipSepctrum->run();
    for(int i=0;i<row-1;i++) {
        for(int j=0;j<(column-1)*2*3*3;j+=18) {
            QVector3D n[3];
            //[first triangle]
            vertice_buffer[i*(column-1)*2*3*3+j   ]=position_vecs[ i   *column+j/18  ].x()+float(phillipSepctrum->delta_width_array[ i   *column+j/18  ].re);
            vertice_buffer[i*(column-1)*2*3*3+j+ 3]=position_vecs[(i+1)*column+j/18  ].x()+float(phillipSepctrum->delta_width_array[(i+1)*column+j/18  ].re);
            vertice_buffer[i*(column-1)*2*3*3+j+ 6]=position_vecs[(i+1)*column+j/18+1].x()+float(phillipSepctrum->delta_width_array[(i+1)*column+j/18+1].re);
            vertice_buffer[i*(column-1)*2*3*3+j+ 1]=position_vecs[ i   *column+j/18  ].y()+float(phillipSepctrum->surface[ i   *column+j/18  ].re);
            vertice_buffer[i*(column-1)*2*3*3+j+ 4]=position_vecs[(i+1)*column+j/18  ].y()+float(phillipSepctrum->surface[(i+1)*column+j/18  ].re);
            vertice_buffer[i*(column-1)*2*3*3+j+ 7]=position_vecs[(i+1)*column+j/18+1].y()+float(phillipSepctrum->surface[(i+1)*column+j/18+1].re);
            vertice_buffer[i*(column-1)*2*3*3+j+ 2]=position_vecs[ i   *column+j/18  ].z()+float(phillipSepctrum->delta_height_array[ i   *column+j/18  ].re);
            vertice_buffer[i*(column-1)*2*3*3+j+ 5]=position_vecs[(i+1)*column+j/18  ].z()+float(phillipSepctrum->delta_height_array[(i+1)*column+j/18  ].re);
            vertice_buffer[i*(column-1)*2*3*3+j+ 8]=position_vecs[(i+1)*column+j/18+1].z()+float(phillipSepctrum->delta_height_array[(i+1)*column+j/18+1].re);
            n[0].setX(float(-phillipSepctrum->grad_width_array [ i   *column+j/18  ].re));
            n[0].setY(float(1));
            n[0].setZ(float(-phillipSepctrum->grad_height_array[ i   *column+j/18  ].re));
            n[0].normalize();
            n[0]=-n[0];
            normal_buffer[i*(column-1)*2*3*3+j   ]=n[0].x();
            normal_buffer[i*(column-1)*2*3*3+j+ 1]=n[0].y();
            normal_buffer[i*(column-1)*2*3*3+j+ 2]=n[0].z();
            n[1].setX(float(-phillipSepctrum->grad_width_array [(i+1)*column+j/18  ].re));
            n[1].setY(float(1));
            n[1].setZ(float(-phillipSepctrum->grad_height_array[(i+1)*column+j/18  ].re));
            n[1].normalize();
            n[1]=-n[1];
            normal_buffer[i*(column-1)*2*3*3+j+ 3]=n[1].x();
            normal_buffer[i*(column-1)*2*3*3+j+ 4]=n[1].y();
            normal_buffer[i*(column-1)*2*3*3+j+ 5]=n[1].z();
            n[2].setX(float(-phillipSepctrum->grad_width_array [(i+1)*column+j/18+1].re));
            n[2].setY(float(1));
            n[2].setZ(float(-phillipSepctrum->grad_height_array[(i+1)*column+j/18+1].re));
            n[2].normalize();
            n[2]=-n[2];
            normal_buffer[i*(column-1)*2*3*3+j+ 6]=n[2].x();
            normal_buffer[i*(column-1)*2*3*3+j+ 7]=n[2].y();
            normal_buffer[i*(column-1)*2*3*3+j+ 8]=n[2].z();
            //[second triangle]
            vertice_buffer[i*(column-1)*2*3*3+j+ 9]=position_vecs[ i   *column+j/18  ].x()+float(phillipSepctrum->delta_width_array[ i   *column+j/18  ].re);
            vertice_buffer[i*(column-1)*2*3*3+j+12]=position_vecs[(i+1)*column+j/18+1].x()+float(phillipSepctrum->delta_width_array[(i+1)*column+j/18+1].re);
            vertice_buffer[i*(column-1)*2*3*3+j+15]=position_vecs[ i   *column+j/18+1].x()+float(phillipSepctrum->delta_width_array[ i   *column+j/18+1].re);
            vertice_buffer[i*(column-1)*2*3*3+j+10]=position_vecs[ i   *column+j/18  ].y()+float(phillipSepctrum->surface[ i   *column+j/18  ].re);
            vertice_buffer[i*(column-1)*2*3*3+j+13]=position_vecs[(i+1)*column+j/18+1].y()+float(phillipSepctrum->surface[(i+1)*column+j/18+1].re);
            vertice_buffer[i*(column-1)*2*3*3+j+16]=position_vecs[ i   *column+j/18+1].y()+float(phillipSepctrum->surface[ i   *column+j/18+1].re);
            vertice_buffer[i*(column-1)*2*3*3+j+11]=position_vecs[ i   *column+j/18  ].z()+float(phillipSepctrum->delta_height_array[ i   *column+j/18  ].re);
            vertice_buffer[i*(column-1)*2*3*3+j+14]=position_vecs[(i+1)*column+j/18+1].z()+float(phillipSepctrum->delta_height_array[(i+1)*column+j/18+1].re);
            vertice_buffer[i*(column-1)*2*3*3+j+17]=position_vecs[ i   *column+j/18+1].z()+float(phillipSepctrum->delta_height_array[ i   *column+j/18+1].re);
            n[0].setX(float(-phillipSepctrum->grad_width_array [ i   *column+j/18  ].re));
            n[0].setY(float(1));
            n[0].setZ(float(-phillipSepctrum->grad_height_array[ i   *column+j/18  ].re));
            n[0].normalize();
            n[0]=-n[0];
            normal_buffer[i*(column-1)*2*3*3+j+ 9]=n[0].x();
            normal_buffer[i*(column-1)*2*3*3+j+10]=n[0].y();
            normal_buffer[i*(column-1)*2*3*3+j+11]=n[0].z();
            n[1].setX(float(-phillipSepctrum->grad_width_array [(i+1)*column+j/18+1].re));
            n[1].setY(float(1));
            n[1].setZ(float(-phillipSepctrum->grad_height_array[(i+1)*column+j/18+1].re));
            n[1].normalize();
            n[1]=-n[1];
            normal_buffer[i*(column-1)*2*3*3+j+12]=n[1].x();
            normal_buffer[i*(column-1)*2*3*3+j+13]=n[1].y();
            normal_buffer[i*(column-1)*2*3*3+j+14]=n[1].z();
            n[2].setX(float(-phillipSepctrum->grad_width_array [ i   *column+j/18+1].re));
            n[2].setY(float(1));
            n[2].setZ(float(-phillipSepctrum->grad_height_array[ i   *column+j/18+1].re));
            n[2].normalize();
            n[2]=-n[2];
            normal_buffer[i*(column-1)*2*3*3+j+15]=n[2].x();
            normal_buffer[i*(column-1)*2*3*3+j+16]=n[2].y();
            normal_buffer[i*(column-1)*2*3*3+j+17]=n[2].z();
        }
    }
    vBuffer.bind();
    int offset=0;

    vBuffer.write(offset,vertice_buffer,(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat)));
    offset+=(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,normal_buffer,(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat)));
    offset+=(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat));

    vBuffer.release();
}

void GLSeaSurface::updateBuffer()
{
    if(row<=0||column<=0) return;
    float wstep=width/(row-1),hstep=height/(column-1);
    QVector3D *pos_vecs=new QVector3D[row*column];
    for(int r=0;r<row;r++) {
        for(int c=0;c<column;c++) {
            pos_vecs[r*column+c]=QVector3D(center.x()-width/2+r*hstep,center.y(),center.z()-height/2+c*wstep);
        }
    }
    GLfloat *vertices=new GLfloat[(row-1)*(column-1)*2*3*3];
    for(int i=0;i<row-1;i++) {
        for(int j=0;j<(column-1)*2*3*3;j+=18) {
            //[first triangle]
            vertices[i*(column-1)*2*3*3+j   ]=pos_vecs[i*column+j/18].x();
            vertices[i*(column-1)*2*3*3+j+ 1]=pos_vecs[i*column+j/18].y();
            vertices[i*(column-1)*2*3*3+j+ 2]=pos_vecs[i*column+j/18].z();
            vertices[i*(column-1)*2*3*3+j+ 3]=pos_vecs[(i+1)*column+j/18].x();
            vertices[i*(column-1)*2*3*3+j+ 4]=pos_vecs[(i+1)*column+j/18].y();
            vertices[i*(column-1)*2*3*3+j+ 5]=pos_vecs[(i+1)*column+j/18].z();
            vertices[i*(column-1)*2*3*3+j+ 6]=pos_vecs[(i+1)*column+j/18+1].x();
            vertices[i*(column-1)*2*3*3+j+ 7]=pos_vecs[(i+1)*column+j/18+1].y();
            vertices[i*(column-1)*2*3*3+j+ 8]=pos_vecs[(i+1)*column+j/18+1].z();
            //[second triangle]
            vertices[i*(column-1)*2*3*3+j+ 9]=pos_vecs[i*column+j/18].x();
            vertices[i*(column-1)*2*3*3+j+10]=pos_vecs[i*column+j/18].y();
            vertices[i*(column-1)*2*3*3+j+11]=pos_vecs[i*column+j/18].z();
            vertices[i*(column-1)*2*3*3+j+12]=pos_vecs[(i+1)*column+j/18+1].x();
            vertices[i*(column-1)*2*3*3+j+13]=pos_vecs[(i+1)*column+j/18+1].y();
            vertices[i*(column-1)*2*3*3+j+14]=pos_vecs[(i+1)*column+j/18+1].z();
            vertices[i*(column-1)*2*3*3+j+15]=pos_vecs[i*column+j/18+1].x();
            vertices[i*(column-1)*2*3*3+j+16]=pos_vecs[i*column+j/18+1].y();
            vertices[i*(column-1)*2*3*3+j+17]=pos_vecs[i*column+j/18+1].z();
        }
    }
    GLfloat *normals=new GLfloat[(row-1)*(column-1)*2*3*3];
    for(int i=0;i<row-1;i++) {
        for(int j=0;j<(column-1)*2*3*3;j+=18) {
            QVector3D normal_vec,v[3];
            //[first triangle]
            v[0]=QVector3D(vertices[i*(column-1)*2*3*3+j   ],vertices[i*(column-1)*2*3*3+j+ 1],vertices[i*(column-1)*2*3*3+j+ 2]);
            v[1]=QVector3D(vertices[i*(column-1)*2*3*3+j+ 3],vertices[i*(column-1)*2*3*3+j+ 4],vertices[i*(column-1)*2*3*3+j+ 5]);
            v[2]=QVector3D(vertices[i*(column-1)*2*3*3+j+ 6],vertices[i*(column-1)*2*3*3+j+ 7],vertices[i*(column-1)*2*3*3+j+ 8]);
            normal_vec=QVector3D::normal(v[0],v[1],v[2]);
            normals[i*(column-1)*2*3*3+j   ]=normal_vec.x();
            normals[i*(column-1)*2*3*3+j+ 1]=normal_vec.y();
            normals[i*(column-1)*2*3*3+j+ 2]=normal_vec.z();
            normals[i*(column-1)*2*3*3+j+ 3]=normal_vec.x();
            normals[i*(column-1)*2*3*3+j+ 4]=normal_vec.y();
            normals[i*(column-1)*2*3*3+j+ 5]=normal_vec.z();
            normals[i*(column-1)*2*3*3+j+ 6]=normal_vec.x();
            normals[i*(column-1)*2*3*3+j+ 7]=normal_vec.y();
            normals[i*(column-1)*2*3*3+j+ 8]=normal_vec.z();
            //[second triangle]
            v[0]=QVector3D(vertices[i*(column-1)*2*3*3+j+ 9],vertices[i*(column-1)*2*3*3+j+10],vertices[i*(column-1)*2*3*3+j+11]);
            v[1]=QVector3D(vertices[i*(column-1)*2*3*3+j+12],vertices[i*(column-1)*2*3*3+j+13],vertices[i*(column-1)*2*3*3+j+14]);
            v[2]=QVector3D(vertices[i*(column-1)*2*3*3+j+15],vertices[i*(column-1)*2*3*3+j+16],vertices[i*(column-1)*2*3*3+j+17]);
            normal_vec=QVector3D::normal(v[0],v[1],v[2]);
            normals[i*(column-1)*2*3*3+j+ 9]=normal_vec.x();
            normals[i*(column-1)*2*3*3+j+10]=normal_vec.y();
            normals[i*(column-1)*2*3*3+j+11]=normal_vec.z();
            normals[i*(column-1)*2*3*3+j+12]=normal_vec.x();
            normals[i*(column-1)*2*3*3+j+13]=normal_vec.y();
            normals[i*(column-1)*2*3*3+j+14]=normal_vec.z();
            normals[i*(column-1)*2*3*3+j+15]=normal_vec.x();
            normals[i*(column-1)*2*3*3+j+16]=normal_vec.y();
            normals[i*(column-1)*2*3*3+j+17]=normal_vec.z();
        }
    }
    this->vertice_buffer=vertices;
    this->normal_buffer=normals;
    this->position_vecs=pos_vecs;
    initializeOpenGLFunctions();
    if(!program.isLinked()) {
        QOpenGLShader *vsh=nullptr;
        vsh=new QOpenGLShader(QOpenGLShader::Vertex);
        qDebug() << ":/shader/seasurface.vsh"
                 << vsh->compileSourceFile(":/shader/seasurface.vsh");
        QOpenGLShader *fsh=nullptr;
        fsh=new QOpenGLShader(QOpenGLShader::Fragment);
        qDebug() << ":/shader/seasurface.fsh"
                 << fsh->compileSourceFile(":/shader/seasurface.fsh");
        program.addShader(vsh);
        program.addShader(fsh);
        program.link();
    }
    program.bind();
    MaterialProperty material;
    material.emission=QVector3D(0.0,0.0,0.0);
    material.ambient=QVector3D(1.0f,1.0f,1.0f)*0.5f;
    material.diffuse=QVector3D(1.0f,1.0f,1.0f)*0.20f;
    material.specular=QVector3D(1.0,1.0,1.0)*0.6f;
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
    int offset=0;
    vBuffer.allocate((row-1)*(column-1)*2*3*3*int(sizeof(GLfloat))*2);

    vBuffer.write(offset,vertices,(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat)));
    offset+=(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat));

    vBuffer.write(offset,normals,(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat)));
    offset+=(row-1)*(column-1)*2*3*3*int(sizeof(GLfloat));

    vBuffer.release();
    inited=true;
}

void GLSeaSurface::setLight(int index, LightProperty light)
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
