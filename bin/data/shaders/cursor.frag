#version 150
//#extension GL_ARB_texture_rectangle : enable  

uniform sampler2DRect tex0;

in float vPointSize;
in vec4 vPointVelocity;
out vec4 vFragColor;

void main () {
    vec2 xy = gl_PointCoord;
    xy = vec2(xy.x, 1. - xy.y);
    vec4 texcolor = texture( tex0, xy*vPointSize);

    float a = texcolor.a;

    float vx = vPointVelocity.x;
    float vy = vPointVelocity.y;

    vFragColor = texcolor;
}