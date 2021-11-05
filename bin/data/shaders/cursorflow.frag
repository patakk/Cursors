#version 150
//#extension GL_ARB_texture_rectangle : enable  

uniform sampler2DRect tex0;

in float vPointSize;
in vec2 vPointVelocity;
out vec4 vFragColor;

vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main () {
    vec2 xy = gl_PointCoord;
    xy = vec2(xy.x, 1. - xy.y);
    vec4 texcolor = texture( tex0, xy*vPointSize);

    float a = texcolor.a;

    float vx = vPointVelocity.x;
    float vy = vPointVelocity.y;
    //vx = clamp(vx*0.1 + 0.5, 0., 1.);
    //vy = clamp(vy*0.1 + 0.5, 0., 1.);
    vx *= 0.1;
    vy *= 0.1;

    //float ang = atan(vy, vx);
    //float hue = (ang + 3.14159)/(2*3.14159);
    //float sat = 0.6;
    //float bri = 0.9;
    //vec4 dircol = vec4(hsv2rgb(vec3(hue, sat, bri)), 1.);
    
    //vx = max(0.0, min(vx, 1.0));
    //vy = max(0.0, min(vy, 1.0));

    //vFragColor = texcolor;
    float alpha = float(texcolor.a > 0.5);
    vFragColor = vec4(vx, vy, 0., 1.) * clamp(alpha, 0.3, 0.5);
}