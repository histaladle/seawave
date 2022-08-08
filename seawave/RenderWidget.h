#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include "GLOrigin.h"
#include "GLCube.h"
#include "GLSeaSurface.h"
#include "GLCylinder.h"
#include "GLSphere.h"
#include "GLSeaSurface.h"
#include <QPainter>
#include <QTime>
class QOpenGLShaderProgram;
class RenderWidget : public QOpenGLWidget
        ,protected QOpenGLFunctions
{
public:
    RenderWidget(QWidget *parent=nullptr);
protected:
    void initializeGL();
    void paintGL();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    GLOrigin *origin=nullptr;
    /* glsl: m[col][row], m[2][3]: col:3,row:4 */
    QVector3D displace = QVector3D(0.0, 0.0, -3.0);
    QVector3D scale = QVector3D(1.0, 1.0, 1.0);
    QVector3D eye = QVector3D(0.0,0.0,0.0);
    /* eye focus on point */
    QVector3D focus = displace;
    QVector3D up = QVector3D(0.0,2.0,0.0);

    LightProperty lights[GLItem::MAX_LIGHT];
    QList<GLItem*> items;

    void setViewPort(int left, int right, int bottom, int top);
    float xRot = 0.0;
    float yRot = 0.0;
    float zRot = 0.0;

    QPoint mousePosLast;
    GLSphere *sphere;
    QTimer *watcher;

    QOpenGLFramebufferObject *fbos[GLItem::MAX_LIGHT]={nullptr};

    void renderDepthFromLight(int index);
    int frmintv=0;
    int clkintv=0;
    int lastclk=0;
    int fps=0;
    QTime fpsTime;
};

#endif // RENDERWIDGET_H
