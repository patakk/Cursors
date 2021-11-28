#include "particleManager.h"

void particleManager::setup(const int n) {
	particleCount = n;

	particles.resize(particleCount);

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
		float an = ofRandomf()*PI;
		particles[i].pos.x = ofGetWidth()/2;
		particles[i].pos.y = ofGetHeight()/2;
		particles[i].pos.z = 0.0;
		particles[i].pos.w = 1.0;
		particles[i].vel.x = 1*cos(anginc * i);
		particles[i].vel.y = 1*sin(anginc * i);
		particles[i].acc.x = particles[i].vel.x;
		particles[i].acc.y = particles[i].vel.y;
		particles[i].drag.x = ofRandom(0.9, 0.98);
	}
}

void particleManager::update() {

	computeShader.begin();
	computeShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	computeShader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());
	computeShader.setUniform1f("time", ofGetFrameNum());
	computeShader.dispatchCompute(particleCount / WORK_GROUP_SIZE, 1, 1);
	// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	computeShader.end();
}


void particleManager::reset() {

	computeShader.begin();
	computeShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	computeShader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());
	computeShader.setUniform1f("time", -1);
	computeShader.dispatchCompute(particleCount / WORK_GROUP_SIZE, 1, 1);
	// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	computeShader.end();
}


void particleManager::draw() {
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	//ofEnableAlphaBlending();
	//ofEnablePointSprites();
	//ofEnableArbTex();

	renderShader.begin();
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