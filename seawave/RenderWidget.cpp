#include "RenderWidget.h"
#include <QTimer>
RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget (parent)
{
    origin=new GLOrigin();
}

void RenderWidget::initializeGL()
{
    QVector3D lightPos,lightDirection,eyeDirection;
    lightPos=eye;
    lightDirection=(lightPos-focus).normalized();
    eyeDirection=(eye-focus).normalized();
    qDebug() << "debug16" << lightPos << lightDirection << eyeDirection;
    LightProperty light;
    light.isEnable=true;
    light.isLocal=true;
    light.isSpot=true;
    light.ambient=QVector3D(1.0,1.0,1.0);
    light.color=QVector3D(1.0,1.0,1.0);
    light.position=lightPos;
    light.halfVector=(lightDirection+eyeDirection).normalized();
    light.coneDirection=lightDirection;
    light.spotCosCutoff=0.5f;
    light.spotExponent=0.9f;
    light.constantAttenuation=1.0;//1.0
    light.linearAttenuation=0.05f;//0.09 0.05
    light.quadraticAttenuation=0.022f;//0.032 0.022
    lights[0]=light;
    initializeOpenGLFunctions();
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST);
    origin->updateBuffer();
//    GLCube *cube=new GLCube();
//    items.append(cube);
//    GLCylinder *cylinder=new GLCylinder(2.5,50,5,QVector3D(0,0,0));
//    items.append(cylinder);
    GLSeaSurface *seasurface=new GLSeaSurface();
    items.append(seasurface);
//    sphere=new GLSphere(100,100,1,QVector3D(0,0,0));
//    items.append(sphere);
    for(int i=0; i<items.size(); i++)
    {
        GLItem *item=nullptr;
        item = items[i];
        item->updateBuffer();
        item->setLight(0,light);
    }
    watcher=new QTimer(this);
    watcher->setSingleShot(true);
    watcher->setInterval(2);
    connect(watcher,&QTimer::timeout,this,[this](){
        for(int i=0;i<items.size();i++) {
            items[i]->updateFrame();
        }
        qDebug() << "debug28";
        update();
        int clk1=clock();
        ++frmintv; clkintv+=clk1-lastclk;
        if(clkintv>=1000) {fps=1000*frmintv/clkintv;}
        lastclk=clk1;
        watcher->start();
    });
    watcher->start();
}

void RenderWidget::paintGL()
{
    int w=width();
    int h=height();
    renderDepthFromLight(0);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setViewPort(0.0, 0.0, w, h);
    glEnable(GL_DEPTH_TEST);
    QMatrix4x4 model, view, projection;
    projection.perspective(45.0f, GLfloat(w)/GLfloat(h), 0.1f, 1000.0f);
//    projection.frustum(-w/200, w/200, -h/200, h/200, 1.0, 100.0);
//    projection.ortho(-w/200, w/200, -h/200, h/200, 0.1, 100.0);
    model.translate(displace.x(), displace.y(), displace.z());
    model.rotate(xRot, 1.0, 0.0, 0.0);
    model.rotate(yRot, 0.0, 1.0, 0.0);
    model.rotate(zRot, 0.0, 0.0, 1.0);

    model.scale(scale);
    view.lookAt(eye,focus,up);

    GLDrawConfig config;
    config.projection=projection;
    config.view=view;
    config.model=model;
    origin->draw(config);
    for(int i=0; i<items.size(); i++) {
        GLItem *item=nullptr;
        item = items[i];
        item->draw(config);
    }
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.drawText(10,40,"fps:"+QString::number(fps));
}

void RenderWidget::renderDepthFromLight(int index)
{
    if(index>=GLItem::MAX_LIGHT) {
        return;
    }
    if(!lights[index].isEnable) {
        return;
    }
    int w=width();
    int h=height();
    QMatrix4x4 model,view,projection;
    projection.perspective(45.0f, GLfloat(w)/GLfloat(h), 0.1f, 1000.0f);
    view.lookAt(lights[index].position,focus,up);

    model.translate(displace.x(),displace.y(),displace.z());
    model.rotate(xRot, 1.0, 0.0, 0.0);
    model.rotate(yRot, 0.0, 1.0, 0.0);
    model.rotate(zRot, 0.0, 0.0, 1.0);
    if(!fbos[index]||fbos[index]->width()!=w||fbos[index]->height()!=h) {
        delete fbos[index];
        QOpenGLFramebufferObjectFormat fbofmt;
        fbofmt.setTextureTarget(GL_TEXTURE_2D);
        fbofmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        fbos[index]=new QOpenGLFramebufferObject(w,h,fbofmt);
    }
    fbos[index]->bind();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    setViewPort(0.0, 0.0, w, h);
    glEnable(GL_DEPTH_TEST);
    GLDrawConfig config;
    config.projection=projection;
    config.view=view;
    config.model=model;
    for(int i=0; i<items.size(); i++)
    {
        GLItem *item=nullptr;
        item = items[i];
        item->renderDepth(config);
    }
    lights[index].texLoc=fbos[index]->texture();
    glDisable(GL_DEPTH_TEST);
    fbos[index]->release();
    QOpenGLFramebufferObject::bindDefault();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    float deltaX = 0.0;
    float deltaY = 0.0;
    deltaX = 1.0f*(event->pos().x() - mousePosLast.x())/width();
    deltaY = 1.0f*(event->pos().y() - mousePosLast.y())/height();
    if(event->buttons() == Qt::LeftButton)
    {
        deltaX *= 100;
        deltaY *= 100;

        if(event->modifiers() == Qt::ControlModifier)
        {
            zRot -= deltaY;
            while(zRot<0) zRot+=360;
        }
        else if(event->modifiers() == Qt::NoModifier)
        {
            xRot += deltaY;
            while(xRot>360) xRot-=360;

            yRot += deltaX;
            while(yRot>360) yRot-=360;
        }

        update();
    }
    else if(event->buttons() == Qt::RightButton)
    {
        deltaX *= -displace.z();
        deltaY *= -displace.z();
        displace+=QVector3D(deltaX, -deltaY, 0);
        update();
    }
    mousePosLast = event->pos();
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    mousePosLast = event->pos();
}

void RenderWidget::wheelEvent(QWheelEvent *event)
{
    QMatrix4x4 t;
    int d = event->delta();
//    t.translate(0.0, 0.0, d/12.0);
//    glMatrixMode(GL_MODELVIEW);
//    glScalef(d/12.0, d/12.0, d/12.0);
    QVector3D p = scale;
    if(d < 0)
    {
//        displace.setZ(displace.z()*1.1f);
        scale.setX(p.x()*0.9f);
        scale.setY(p.y()*0.9f);
        scale.setZ(p.z()*0.9f);
    }
    else if(d > 0)
    {
//        displace.setZ(displace.z()*0.9f);
        scale.setX(p.x()*1.1f);
        scale.setY(p.y()*1.1f);
        scale.setZ(p.z()*1.1f);
    }
    update();
}

void RenderWidget::
setViewPort(int left, int right, int bottom, int top)
{
//    float zNear = 1.0;
//    float zFar = 100.0;
    int h = top - bottom;
    int w = right - left;
    int side = qMin(w, h);
    glViewport((w-side)/2, (h-side)/2, side, side);
//    glViewport(0.0, 0.0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(left,right,bottom,top,zNear,zFar);

//    float zNear = 1.0;
//    float zFar = 10.0;
//    float h = (float)(top - bottom);
//    float w = (float)(right - left);
//    glViewport(0.0, 0.0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glFrustum(left,right,bottom,top,zNear,zFar);
}

