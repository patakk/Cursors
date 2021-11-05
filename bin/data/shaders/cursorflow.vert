// cusor.vert
#version 440

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
out float vPointSize;
out vec2 vPointVelocity;

struct Particle{
	vec4 pos;
	vec4 vel;
	vec4 acc;
	vec4 drg;
};

layout(std140, binding=0) buffer particles{
	Particle p[];
};

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

void main(){
	uint gid = gl_VertexID;

	gl_Position = modelViewProjectionMatrix * position;
	gl_PointSize = 40.0;
	vPointSize = gl_PointSize; 
	vPointVelocity = p[gid].vel.xy;
}