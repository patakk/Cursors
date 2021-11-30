#pragma once

#include "ofMain.h"

#define WORK_GROUP_SIZE 256

// using acc as vel0
// using drag as prev texture buffer and 

struct Particle {
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
	ofVec4f drg;
	ofVec4f col;
};

class particleManager {


	ofShader computeShader;
	ofShader renderShader;
	ofShader flowShader;

	vector<Particle> particles;
	ofBufferObject particlesBuffer;

	ofVbo vbo;

public:
	int particleCount;
	ofFbo objectsLayer;
	ofTexture imgTexture;
	void setup(const int n);
	void initParticles();
	void update();
	void draw(float progress);
	void reset();
	void drawFlow();
};