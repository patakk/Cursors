#version 150
//#extension GL_ARB_texture_rectangle : enable  

uniform sampler2DRect tex0;
uniform float progress;

in float vPointSize;
in vec4 vPointColor;
out vec4 vFragColor;


void main () {
    float sca = vPointSize/32.;
    vec2 xy = gl_PointCoord;
    xy = vec2(xy.x, 1. - xy.y);
    xy /= sca;
    vec4 texcolor = texture(tex0, xy*vPointSize);

    float dist = length(xy*vPointSize);
    dist = dist < 5 ? (1.-dist/5) : 0.;

    //vFragColor = texcolor;
    //vFragColor = vec4(1.0, 1.0, 1.0, dist*0.3);
    //vFragColor = vec4(dist*0.5, dist*0.5, dist*0.5, dist*0.19);
    float aa = progress;
    vFragColor = vec4(vPointColor.r, vPointColor.g, vPointColor.b, (1-progress)*dist*.029);
}