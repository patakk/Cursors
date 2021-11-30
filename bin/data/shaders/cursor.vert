// cusor.vert
#version 440

uniform mat4 modelViewProjectionMatrix;
uniform float progress;
out float vPointSize;
out vec4 vPointColor;
in vec4 position;

struct Particle{
	vec4 pos;
	vec4 vel;
	vec4 acc;
	vec4 drg;
	vec4 col;
};

layout(std140, binding=0) buffer particles{
	Particle p[];
};

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

float unity_noise_randomValue (vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233)))*43758.5453);
}

void main(){
	uint gid = gl_VertexID;

	vec4 poo = modelViewProjectionMatrix * position;

	float r1 = unity_noise_randomValue(position.xy/1000.);
	float r2 = unity_noise_randomValue(position.xy/1000. + vec2(.5, .5));

	poo.xy += 0*(1 + 8*progress)*(vec2(r1, r2) - vec2(.5, .5));

	gl_Position = poo;
	gl_PointSize = 13.0;
	vPointSize = gl_PointSize; 
	//if(int(gid+progress*9381.31)%3 == 0)
	//	vPointColor = vec4(1,0,0,1);
	//if(int(gid+progress*9381.31)%3 == 1)
	//	vPointColor = vec4(0,1,0,1);
	//if(int(gid+progress*9381.31)%3 == 2)
	//	vPointColor = vec4(0,0,1,1);

	vec4 pc = p[gid].col;
	vPointColor = vec4(pc.r, pc.g, pc.b, 1);
}