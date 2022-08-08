#include "GLCube.h"
QOpenGLShaderProgram GLCube::program;
GLCube::GLCube()
{

}

void GLCube::draw(GLDrawConfig config)
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
    GLint pos_loc,texcoord_loc,normal_loc;
    pos_loc=program.attributeLocation("qt_vertex");
    program.setAttributeBuffer(pos_loc, GL_FLOAT, 0, 3, 0);
    program.enableAttributeArray(pos_loc);
    texcoord_loc = program.attributeLocation("qt_texCoord");
    program.setAttributeBuffer(texcoord_loc, GL_FLOAT,
                               6*4*3*sizeof(GLfloat), 2, 0);
    program.enableAttributeArray(texcoord_loc);
    normal_loc = program.attributeLocation("qt_normal");
    program.setAttributeBuffer(normal_loc, GL_FLOAT,
                               6*4*3*sizeof(GLfloat)
                               +6*4*2*sizeof(GLfloat), 3, 0);
    program.enableAttributeArray(normal_loc);
    program.setUniformValue("qt_Texture",0);
    texture->bind(0);
    for(int i=0; i<6; i++)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
    }
    texture->release();
    vBuffer.release();
    program.release();
}

void GLCube::updateBuffer()
{
    texture=new QOpenGLTexture(QImage(":/image/cube.png").mirrored());
    GLfloat vertices[6*4*3] =
    {
        //face 0
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        //face 1
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        //face 2
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        //face 3
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        //face 4
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        //face 5
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
    };
    GLfloat texcoords[6*4*2] =
    {
        //face 0
         0.0f, 0.0f,
        0.33f, 0.0f,
         0.0f, 0.5f,
        0.33f, 0.5f,
        //face 1
         0.0f, 0.5f,
        0.33f, 0.5f,
         0.0f, 1.0f,
        0.33f, 1.0f,
        //face 2
        0.66f, 0.5f,
         1.0f, 0.5f,
        0.66f, 1.0f,
         1.0f, 1.0f,
        //face 3
        0.66f, 0.0f,
         1.0f, 0.0f,
        0.66f, 0.5f,
         1.0f, 0.5f,
        //face 4
        0.33f, 0.0f,
        0.66f, 0.0f,
        0.33f, 0.5f,
        0.66f, 0.5f,
        //face 5
        0.33f, 0.5f,
        0.66f, 0.5f,
        0.33f, 1.0f,
        0.66f, 1.0f,
    };
    GLfloat normals[6*4*3] =
    {
        //face 0
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
        //face 1
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
        //face 2
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
        //face 3
        -1.0f,  0.0f, 0.0f,
        -1.0f,  0.0f, 0.0f,
        -1.0f,  0.0f, 0.0f,
        -1.0f,  0.0f, 0.0f,
        //face 4
         0.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 0.0f,
        //face 5
        0.0f,  1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    initializeOpenGLFunctions();
    if(!program.isLinked()) {
        QOpenGLShader *vsh=nullptr;
        vsh=new QOpenGLShader(QOpenGLShader::Vertex);
        qDebug() << ":/shader/texture.vsh"
                 << vsh->compileSourceFile(":/shader/texture.vsh");
        QOpenGLShader *fsh=nullptr;
        fsh=new QOpenGLShader(QOpenGLShader::Fragment);
        qDebug() << ":/shader/texture.fsh"
                 << fsh->compileSourceFile(":/shader/texture.fsh");
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
    vBuffer.allocate(6*4*3*sizeof(GLfloat)
                     +6*4*2*sizeof(GLfloat)
                     +6*4*3*sizeof(GLfloat));
    vBuffer.write(0, vertices, 6*4*3*sizeof(GLfloat));
    vBuffer.write(6*4*3*sizeof(GLfloat), texcoords,
                  6*4*2*sizeof(GLfloat));
    vBuffer.write(6*4*3*sizeof(GLfloat)+6*4*2*sizeof(GLfloat),
                  normals, 6*4*3*sizeof(GLfloat));

    vBuffer.release();
    inited=true;
}

void GLCube::setLight(int index, LightProperty light)
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
