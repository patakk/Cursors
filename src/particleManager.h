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

	ofShader computeShader, renderShader;

	vector<Particle> particles;
	vector<ofVec4f> vpos;
	vector<ofVec4f> vvel;
	vector<ofVec4f> vacc;
	vector<float> vdrg;
	ofBufferObject particlesBuffer;

	ofVbo vbo;

public:
	ofTexture imgTexture;
	void setup(const int n);
	void initParticles();
	void update();
	void draw();
};