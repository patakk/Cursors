#version 150
//#extension GL_ARB_texture_rectangle : enable  

uniform sampler2DRect tex0;

in float vPointSize;
out vec4 vFragColor;
in float vPointDrgY;
uniform float aliveCount;


void main () {
    float sca = vPointSize/32.;
    vec2 xy = gl_PointCoord;
    xy = vec2(xy.x, 1. - xy.y);
    xy /= sca;
    vec4 texcolor = texture(tex0, xy*vPointSize);

    float dist = length(xy*vPointSize);
    dist = float(dist < 6);

    //vFragColor = texcolor;

    if(vPointDrgY > aliveCount)
        vFragColor = vec4(1.0, 1.0, 1.0, 0);
    else{
        vFragColor = vec4(1.0, 1.0, 1.0, dist);
    }
	
}