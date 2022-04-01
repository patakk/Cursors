#include "ofApp.h"

void ofApp::particleSetup(const int n) {
	particleCount = n;

	particles.resize(particleCount);

	initParticles();

	ofLoadImage(imgTexture, "blob.png");
	particlesBuffer.allocate(particles, GL_DYNAMIC_DRAW);
	vbo.setVertexBuffer(particlesBuffer, 4, sizeof(Particle));
	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	computeShader.setupShaderFromFile(GL_COMPUTE_SHADER, "shaders/cursor.comp");
	computeShader.linkProgram();

	renderShader.load("shaders/cursor.vert", "shaders/cursor.frag");
}

void ofApp::initParticles() {
	for (int i = 0; i < particleCount; ++i) {
		Particle* p = &particles[i];
		p->pos.x = ofRandom(33, ofGetWidth() - 33);
		p->pos.y = ofRandom(33, ofGetHeight() - 33);
		//p->pos.y = ofMap(i, 0, particleCount - 1, 100, ofGetHeight() - 100) + ofRandom(-4, 4);

		p->acc.x = ofRandom(140, 220) / 255.;
		p->acc.y = ofRandom(110, 170) / 255.;
		p->acc.z = ofRandom(20, 200 * p->pos.y / ofGetHeight()) / 255.;

		if (p->pos.x > ofGetWidth() * 0.4 && p->pos.x < ofGetWidth() * 0.6 && p->pos.y > ofGetHeight() * 0.14 && p->pos.y < ofGetHeight() * 0.86) {
			p->acc.x = ofRandom(220, 200 * (1. - p->pos.y / ofGetHeight())) / 255. * .3;
			p->acc.x = ofRandom(140, 220) / 255. * .3;
			p->acc.z = ofRandom(33, 170) / 255.;
		}

		//p->pos.x += ofRandom(-33, 33) * pow(1. - p->pos.y / ofGetHeight(), 2);
		//p->pos.y += ofRandom(-100, 100) * pow(1. - p->pos.y / ofGetHeight(), 2);


		//particles[i].pos.x = ofGetWidth()/2;
		//particles[i].pos.y = ofGetHeight()/2;
		p->pos.z = 0.0;
		p->pos.w = 1.0;
		p->vel.x = 0.0;
		p->vel.y = 0.0;
		p->drag.x = ofRandom(0.9, 0.98);
		p->drag.y = float(i);
	}
}

void ofApp::particleUpdate(float aliveCount, ofFbo threeD, ofFbo depthL, float crazy) {

	computeShader.begin();
	computeShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	computeShader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());
	computeShader.setUniform1f("time", time);
	computeShader.setUniform1f("aliveCount", aliveCount);
	computeShader.setUniform1f("crazy", crazy);
	computeShader.setUniformTexture("threeD", threeD, 0);
	computeShader.setUniformTexture("depthL", depthL, 1);
	computeShader.dispatchCompute(particleCount / WORK_GROUP_SIZE, 1, 1);
	// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	computeShader.end();
}


void ofApp::particleDraw(float aliveCount) {

	ofEnableAlphaBlending();
	ofEnablePointSprites();

	renderShader.begin();
	renderShader.setUniform2f("screen", glm::vec2(ofGetWidth(), ofGetHeight()));
	renderShader.setUniform2f("offColor", glm::vec3(offColor.r/255, offColor.g / 255, offColor.b / 255));
	renderShader.setUniform1f("aliveCount", glm::float32(aliveCount));
	renderShader.setUniform1f("time", time);
	renderShader.setUniformTexture("tex0", imgTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	vbo.draw(GL_POINTS, 0, particleCount);
	renderShader.end();

}