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
}

void particleManager::initParticles() {
	for (int i = 0; i < particleCount; ++i) {
		particles[i].pos.x = ofRandom(100, ofGetWidth()-100);
		particles[i].pos.y = ofRandom(100, ofGetHeight()-100);
		particles[i].pos.z = 0.0;
		particles[i].pos.w = 1.0;
		particles[i].vel.x = 0.0;
		particles[i].vel.y = 0.0;
		particles[i].acc.x = 0.0;
		particles[i].acc.y = 0.0;
	}
}

void particleManager::update() {

	computeShader.begin();
	computeShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	computeShader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());
	computeShader.dispatchCompute(particleCount / WORK_GROUP_SIZE, 1, 1);
	// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	computeShader.end();
}


void particleManager::draw() {
	//ofEnableBlendMode(OF_BLENDMODE_ADD);

	ofEnableAlphaBlending();
	ofEnablePointSprites();
	ofEnableArbTex();

	renderShader.begin();
	imgTexture.bind();
	vbo.draw(GL_POINTS, 0, particleCount);
	imgTexture.unbind();
	renderShader.end();
}