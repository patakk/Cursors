// cusor.vert
#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
out float vPointSize;
out vec4 vPointVelocity;


void main(){
	gl_Position = modelViewProjectionMatrix * position;
	gl_PointSize = 60.0;
	vPointSize = gl_PointSize; 
}