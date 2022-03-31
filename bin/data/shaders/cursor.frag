#version 440
//#extension GL_ARB_texture_rectangle : enable  

uniform sampler2DRect tex0;

in float vPointSize;
out vec4 vFragColor;
in float vPointDrgY;
in vec3 vCol;
uniform float aliveCount;

vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main () {
    vec2 xy = gl_PointCoord;
    xy = vec2(xy.x, 1. - xy.y);
    //xy /= sca;
    vec4 texcolor = texture(tex0, xy*128);

    float dist = length(xy*vec2(2., 1.)-.5);
    //float ddist = 1. - smoothstep(0.25, 0.45, dist);
    float ddist = float(dist < .5);
    float dddist = dist * float(ddist < .5);

    if(vPointDrgY > aliveCount)
        vFragColor = vec4(0.0, 0.0, 0.0, 0);
    else{
        vFragColor = vec4(0.0, 0.0, 0.0, ddist*.18);
        //vec3 rgb = hsv2rgb(vec3(vCol, 1., 0.9));
    }

    vFragColor.rgb = vCol.rgb;
    vFragColor.a = ddist;

}