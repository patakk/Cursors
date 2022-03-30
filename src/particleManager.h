#pragma once

#include "ofMain.h"

#define WORK_GROUP_SIZE 256

struct Particle {
	ofVec4f pos;
	ofVec4f vel;
	ofVec4f acc;
	ofVec4f drag;
};

class particleManager {

	int particleCount;

	ofShader computeShader;
	ofShader renderShader;
	ofShader flowShader;

	vector<Particle> particles;
	ofBufferObject particlesBuffer;

	ofVbo vbo;
	ofFbo aaa;

public:
	ofTexture imgTexture;
	void setup(const int n);
	void initParticles();
	void update(float aliveCount, ofFbo aaa, float crazy);
	void draw(float aliveCount);
	void drawFlow(float aliveCount);
};