#version 430

uniform sampler2DRect tex0;
uniform sampler2DRect depthL;
uniform float blurAmnt;
uniform float time;
uniform vec2 res;

in vec2 texCoordVarying;
out vec4 outputColor;

// Gaussian weights from http://dev.theomader.com/gaussian-kernel-calculator/

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


vec4 blur13(sampler2DRect image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = texture(image, uv) * 0.18;
  resolution = vec2(1.);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(2.2941176470588234) * direction;
  vec2 off3 = vec2(3.2941176470588234) * direction;
  vec2 off4 = vec2(4.2941176470588234) * direction;
  vec2 off5 = vec2(5.176470588235294) * direction;
  color += texture(image, uv + (off1 / resolution)) * 0.15;
  color += texture(image, uv - (off1 / resolution)) * 0.15;
  color += texture(image, uv + (off2 / resolution)) * 0.12;
  color += texture(image, uv - (off2 / resolution)) * 0.12;
  color += texture(image, uv + (off3 / resolution)) * 0.08;
  color += texture(image, uv - (off3 / resolution)) * 0.08;
  color += texture(image, uv + (off4 / resolution)) * 0.04;
  color += texture(image, uv - (off4 / resolution)) * 0.04;
  color += texture(image, uv + (off5 / resolution)) * 0.02;
  color += texture(image, uv - (off5 / resolution)) * 0.02;
  return color;
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

void main()
{

    float amp = length(texCoordVarying.xy/res.xy - .5)/length(vec2(.5));
	amp = pow(amp, 3)*7 + .2;
    //amp = abs(texCoordVarying.x/res.x - .5)*2;
    //amp = pow(amp, 4)*12;

	vec3 ppp = vec3(texCoordVarying.x/res.x*2, texCoordVarying.y/res.y*2, time*0.01);
	//amp = clamp(power(simplex3d(ppp+31.31), 3)*8, 0, 8);

	vec2 dir = vec2(amp, 0.);
	vec2 head = texCoordVarying.xy/res.xy - .5;
	float ang = -atan(head.y/3, head.x);
	dir.x = amp*simplex3d(ppp);
	dir.y = amp*simplex3d(ppp+.5);
	dir.x = amp*sin(ang);
	dir.y = amp*cos(ang);
	//dir = vec2(.1, .1);

    vec4 pts = texture(tex0, texCoordVarying.xy);
    outputColor = blur13(tex0, texCoordVarying.xy, vec2(1.,1.), dir) + 0*vec4(0,0.2,.3,0);
	
    vec4 thiscolor = texture(tex0, texCoordVarying.xy);
	outputColor = outputColor;
    //outputColor.r = blur13(tex0, texCoordVarying.xy, vec2(1.,1.), vec2(amp, 0.)).r;
    //outputColor.g = blur13(tex0, texCoordVarying.xy, vec2(1.,1.), vec2(amp*2.1, 0.)).g;
    //outputColor.b = blur13(tex0, texCoordVarying.xy, vec2(1.,1.), vec2(amp*3.2, 0.)).b;
    //outputColor = vec4(rgb, 1.0);
    //outputColor = vec4(flow_dir, 0., 1.);
     
}
