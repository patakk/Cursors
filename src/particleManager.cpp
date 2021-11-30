#include "particleManager.h"

void particleManager::setup(const int n) {
	particleCount = n;

	particles.resize(particleCount);

	inputImage.load("curves.png");
	objectsLayer.allocate(ofGetWidth(), ofGetHeight());
	objectsLayer.begin();
	ofBackground(ofColor(0, 0, 0));
	ofSetColor(ofColor(255, 255, 255, 33));
	ofSetRectMode(OF_RECTMODE_CENTER);
	/*for (int k = 0; k < 5; k++) {
		float x = ofRandom(444, ofGetWidth() - 444);
		float y = ofRandom(444, ofGetHeight() - 444);
		float w = ofRandom(500, 700)*0.13;
		float h = ofRandom(500, 700);
		//ofDrawCircle(x, y, w, h);
		ofDrawRectangle(x, y, w, h);
	}*/
	inputImage.draw(ofGetWidth() / 2, ofGetHeight() / 2);
	objectsLayer.end();

	initParticles();

	ofLoadImage(imgTexture, "cursor.png");
	particlesBuffer.allocate(particles, GL_DYNAMIC_DRAW);
	vbo.setVertexBuffer(particlesBuffer, 4, sizeof(Particle));
	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	computeShader.setupShaderFromFile(GL_COMPUTE_SHADER, "shaders/cursor.comp");
	computeShader.linkProgram();

	renderShader.load("shaders/cursor.vert", "shaders/cursor.frag");
	flowShader.load("shaders/cursorflow.vert", "shaders/cursorflow.frag");
}

void particleManager::initParticles() {
	float anginc = 1. / particleCount * 2 * PI;
	for (int i = 0; i < particleCount; ++i) {
		//particles[i].pos.x = ofRandom(100, ofGetWidth() - 100);
		//particles[i].pos.y = ofRandom(100, ofGetHeight() - 100);
		float ang = anginc * i + anginc / .31141;
		particles[i].drg.x = 0;
		particles[i].drg.y = ofRandom(-23, 23);
		particles[i].drg.z = ofRandom(-23, 23);
		particles[i].col.x = 1;
		particles[i].col.y = 1;
		particles[i].col.z = 1;
		particles[i].pos.x = ofGetWidth()/2 + particles[i].drg.y;
		particles[i].pos.y = ofGetHeight()/2 + particles[i].drg.z;
		particles[i].pos.z = 0.0;
		particles[i].pos.w = 1.0;
		particles[i].vel.x = 1*cos(ang);
		particles[i].vel.y = 1*sin(ang);
		particles[i].acc.x = particles[i].vel.x;
		particles[i].acc.y = particles[i].vel.y;
	}
}

void particleManager::update() {

	computeShader.begin();
	computeShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	computeShader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY()-120);
	computeShader.setUniform1f("time", ofGetFrameNum());
	computeShader.setUniformTexture("tex0", objectsLayer, 0);
	computeShader.dispatchCompute(particleCount / WORK_GROUP_SIZE, 1, 1);
	// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	computeShader.end();
}


void particleManager::reset() {

	computeShader.begin();
	computeShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	computeShader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY()-120);
	computeShader.setUniform1f("time", -1);
	computeShader.dispatchCompute(particleCount / WORK_GROUP_SIZE, 1, 1);
	// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	computeShader.end();
}


void particleManager::draw(float progress) {
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	//ofEnableAlphaBlending();
	//ofEnablePointSprites();
	//ofEnableArbTex();

	renderShader.begin();
	renderShader.setUniform1f("progress", progress);
	renderShader.setUniform2f("screen", glm::vec2(ofGetWidth(), ofGetHeight()));
	renderShader.setUniformTexture("tex0", imgTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	vbo.draw(GL_POINTS, 0, particleCount);
	renderShader.end();
}


void particleManager::drawFlow() {
	//ofEnableBlendMode(OF_BLENDMODE_ADD);

	ofEnableAlphaBlending();
	ofEnablePointSprites();
	ofEnableArbTex();

	flowShader.begin();
	flowShader.setUniform2f("screen", glm::vec2(ofGetWidth(), ofGetHeight()));
	flowShader.setUniformTexture("tex0", imgTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	vbo.draw(GL_POINTS, 0, particleCount);
	flowShader.end();
}