#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 qt_vertex;
attribute vec4 qt_color;
attribute vec3 qt_normal;
varying vec3 fNormal;
varying vec3 vPos;
uniform mat4 qt_Model;
uniform mat4 qt_View;
uniform mat4 qt_Projection;
varying vec2 texCoord;
attribute vec2 qt_texCoord;

void main(void)
{
    vec4 vertex=vec4(qt_vertex,1.0);
    fNormal=qt_normal;
    gl_Position=qt_Projection*qt_View*qt_Model*vertex;
    vPos=qt_vertex;
    texCoord=qt_texCoord;
}
