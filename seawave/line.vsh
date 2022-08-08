#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 qt_vertex;
attribute vec4 qt_color;
uniform mat4 qt_Model;
uniform mat4 qt_View;
uniform mat4 qt_Projection;
varying vec4 vColor;
void main(void)
{
    vColor=qt_color;
    gl_Position=qt_Projection*qt_View*qt_Model*vec4(qt_vertex,1.0);
}
