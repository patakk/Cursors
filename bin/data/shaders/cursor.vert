// cusor.vert
#version 440

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
out float vPointSize;
out float vPointDrgY;
out vec4 vCol;
out vec2 vPos;
out float vAng;
uniform float time;
uniform vec3 offColor;
uniform vec2 screen;

struct Particle{
	vec4 pos;
	vec4 vel;
	vec4 acc;
	vec4 drag;
};

layout(std140, binding=0) buffer particles{
	Particle p[];
};

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

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


float power(float p, float g) {
    if (p < 0.5)
        return 0.5 * pow(2*p, g);
    else
        return 1 - 0.5 * pow(2*(1 - p), g);
}

vec4 random4(vec3 pos){
	vec3 rr1 = random3(vec3(pos+1.2134114));
	vec3 rr2 = random3(vec3(pos+.414114));
	return vec4(rr2, rr1.x);
}


void main(){
	uint gid = gl_VertexID;
	
	vec4 rpos = position;
    if(p[gid].drag.z == 1.0) // if background
		rpos.xy += 8*random3(vec3(rpos.x, rpos.y, p[gid].drag.y)).xy;
	else
		rpos.xy += 8*random3(vec3(rpos.x, rpos.y, p[gid].drag.y)).xy * (.015*(1-p[gid].drag.x) + .9);
	gl_Position = modelViewProjectionMatrix * rpos;
	float ps = clamp(1.*simplex3d_fractal(vec3(gid)), 0.0, 1.0);
	// float ps = 2.728;
	ps = 1.6 + 1*power(ps, 1);
	gl_PointSize = (p[gid].drag.x*0.25 + .34) * ps*(9 + 5*(random3(vec3(rpos.x, rpos.y, p[gid].drag.y*0)).x) + 2*(random3(vec3(rpos.x, rpos.y, time*.1)).x));
	if(gl_PointSize != 0.0)
		vPointSize = clamp(gl_PointSize, 3., 1000.); 
	vPointDrgY = p[gid].drag.y;

	float ggid = float(gid);
	vec4 skyclra = vec4(194, 82, 70, 255)/255. + vec4(29, 35, 22, 0)/255. * (random4(rpos.xyz+.3141+ggid*.01412));
	vec4 skyclrb = vec4(88, 77, 83, 88)/255. + vec4(11, 28, 17, 88)/255. * (random4(rpos.xyz+.2141+ggid*.01412));
	vec4 skyclrc = vec4(166, 115, 62, 255)/255. + vec4(39, 25, 22, 0)/255. * (random4(rpos.xyz+.6141+ggid*.01412));
	
	vec4 groundclra = vec4(233, 134, 69, 255)/255. + vec4(22, 25, 22, 0)/255. * (random4(rpos.xyz+.3141+ggid*.01412));
	vec4 groundclrb = vec4(166, 133, 122, 188)/255. + vec4(11, 28, 17, 55)/255. * (random4(rpos.xyz+.2141+ggid*.1412));
	vec4 groundclrc = vec4(144, 133, 211, 255)/255. + vec4(39, 25, 22, 0)/255. * (random4(rpos.xyz+.6141+ggid*.01412));

	float psfactor = clamp(.5 + .5*gl_Position.y/800., 0.5, 1.);
	float clfactor = clamp(.5 + .5*gl_Position.y/800.*2, 0.0, 1.);

	vAng = random4(rpos.xyz+.3141+ggid*.11412).y*3.14*12 + 3.14/2;

    if(p[gid].drag.z == 1.0){ // if background
		gl_PointSize = ps*(9 + 5*(-.5+random3(vec3(rpos.x, rpos.y, p[gid].drag.y*0)).x)
					 + 2*(-.5+random3(vec3(rpos.x, rpos.y, time*.1)).x));
		vCol = skyclrc + (clfactor + 1.8*random4(rpos.xyz+.5141+ggid*.0001412).r)*(skyclra - skyclrc);
		if(random4(rpos.xyz+.13141+ggid*.311412).r > 0.248){
		vCol = skyclrc + (2.8*random4(rpos.xyz+.5141+ggid*.0001412).r)*((skyclrb+skyclrc)/2 - skyclrc);
		}
	}
	else if(p[gid].drag.z == .5){ // if ground
		vCol = skyclra;
	}
	else if(p[gid].drag.z == 0.0){ // if trees
		vec3 color = p[gid].acc.rgb;
		vCol = vec4(color+offColor, 1.0) + 1*vec4(29, 35, 22, 0)/255. * (1*random4(rpos.xyz+.3141+ggid*.00001412));
		vAng = random4(rpos.xyz+.3141+ggid*.11412).y*.05 + 3.14159/2;
	}
	else if(p[gid].drag.z == -1.0){
		gl_PointSize = 0.0;
	}

	if(random4(rpos.xyz+.3141+ggid*.311412).r > 0.348){
		vCol = vec4(.66,.66,.66, .5);
	}
	gl_PointSize *= 1.7;
	vPos = position.xy / screen.xy;
}