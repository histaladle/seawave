#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 qt_vertex;
attribute vec4 qt_color;
varying vec4 vColor;
attribute vec3 qt_normal;
varying vec3 fNormal;
varying vec3 vPos;
uniform mat4 qt_Model;
uniform mat4 qt_View;
uniform mat4 qt_Projection;
void main(void)
{
    vColor=vec4(1.0,0.0,0.0,1.0);//vec4(0.33,0.33,0.33,1.0) vec4(qt_normal,1.0)
    fNormal=qt_normal;
    gl_Position=qt_Projection*qt_View*qt_Model*vec4(qt_vertex,1.0);
    vPos=qt_vertex;
}
