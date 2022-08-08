#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
uniform mat4 qt_Projection;
uniform mat4 qt_View;
uniform mat4 qt_Model;
varying vec3 vPos;
void main(void)
{
    vec4 v=qt_Projection*qt_View*qt_Model*vPos;
//    v=vec4(vec3(v.z/v.w),1.0);
    gl_FragColor = v;
}
