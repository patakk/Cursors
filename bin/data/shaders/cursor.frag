#version 440
//#extension GL_ARB_texture_rectangle : enable  

uniform sampler2DRect tex0;

in float vPointSize;
out vec4 vFragColor;
in float vPointDrgY;
in vec4 vCol;
in vec2 vPos;
in float vAng;
uniform float aliveCount;

vec3 random3(vec3 c) {
	float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
	vec3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r-0.5;
}

/* skew constants for 3d simplex functions */
const float F3 =  0.3333333;
const float G3 =  0.1666667;

/* 3d simplex noise */
float simplex3d(vec3 p) {
	 /* 1. find current tetrahedron T and it's four vertices */
	 /* s, s+i1, s+i2, s+1.0 - absolute skewed (integer) coordinates of T vertices */
	 /* x, x1, x2, x3 - unskewed coordinates of p relative to each of T vertices*/
	 
	 /* calculate s and x */
	 vec3 s = floor(p + dot(p, vec3(F3)));
	 vec3 x = p - s + dot(s, vec3(G3));
	 
	 /* calculate i1 and i2 */
	 vec3 e = step(vec3(0.0), x - x.yzx);
	 vec3 i1 = e*(1.0 - e.zxy);
	 vec3 i2 = 1.0 - e.zxy*(1.0 - e);
	 	
	 /* x1, x2, x3 */
	 vec3 x1 = x - i1 + G3;
	 vec3 x2 = x - i2 + 2.0*G3;
	 vec3 x3 = x - 1.0 + 3.0*G3;
	 
	 /* 2. find four surflets and store them in d */
	 vec4 w, d;
	 
	 /* calculate surflet weights */
	 w.x = dot(x, x);
	 w.y = dot(x1, x1);
	 w.z = dot(x2, x2);
	 w.w = dot(x3, x3);
	 
	 /* w fades from 0.6 at the center of the surflet to 0.0 at the margin */
	 w = max(0.6 - w, 0.0);
	 
	 /* calculate surflet components */
	 d.x = dot(random3(s), x);
	 d.y = dot(random3(s + i1), x1);
	 d.z = dot(random3(s + i2), x2);
	 d.w = dot(random3(s + 1.0), x3);
	 
	 /* multiply d by w^4 */
	 w *= w;
	 w *= w;
	 d *= w;
	 
	 /* 3. return the sum of the four surflets */
	 return dot(d, vec4(52.0));
}

/* const matrices for 3d rotation */
const mat3 rot1 = mat3(-0.37, 0.36, 0.85,-0.14,-0.93, 0.34,0.92, 0.01,0.4);
const mat3 rot2 = mat3(-0.55,-0.39, 0.74, 0.33,-0.91,-0.24,0.77, 0.12,0.63);
const mat3 rot3 = mat3(-0.71, 0.52,-0.47,-0.08,-0.72,-0.68,-0.7,-0.45,0.56);

/* directional artifacts can be reduced by rotating each octave */
float simplex3d_fractal(vec3 m) {
    return   0.5333333*simplex3d(m*rot1)
			+0.2666667*simplex3d(2.0*m*rot2)
			+0.1333333*simplex3d(4.0*m*rot3)
			+0.0666667*simplex3d(8.0*m);
}


vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float power(float p, float g) {
    if (p < 0.5)
        return 0.5 * pow(2*p, g);
    else
        return 1 - 0.5 * pow(2*(1 - p), g);
}

float circle(vec2 xy){
    float dist = length(xy - .5);
	return 1. - smoothstep(0.45, 0.55, dist);
}

float rect(vec2 xy){
	float dist1 = smoothstep(0.2, 0.3, xy.x);
    float dist2 = smoothstep(0.2, 0.3, 1. - xy.x);
	float dist = dist1 * dist2;
	return dist;
}

void main () {
    vec2 xy = gl_PointCoord;
    xy = vec2(xy.x, 1. - xy.y);
    //xy /= sca;
    vec4 texcolor = texture(tex0, xy*128);

	float sx = 1 + .9*power(clamp(.5+.5*simplex3d(vec3(xy, 0)*.03),0,1),3);
	float sy = 1 + .9*power(clamp(.5+.5*simplex3d(vec3(xy, 31.31)*.03),0,1),3);
    float dist = length(xy - .5);
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
    //vFragColor.a = circle(xy);

	float ang = vPos.y*3.14;
	ang = vAng;

	xy = xy - .5;
	float x = xy.x*cos(ang) - xy.y*sin(ang);
	float y = xy.x*sin(ang) + xy.y*cos(ang);
	xy.x = x*1.4;
	xy.y = y*1.4;
	xy = xy + .5;

    vFragColor.a = rect(xy);

}