#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	ofSetVerticalSync(true);
	//ofHideCursor();

	particleNum = 131100;
	setupProject(particleNum);

	gui.setup();
	gui.setSize(200, 100);
	gui.add(particleNumField.setup("particle count", particleNum, 10000, 3000000));
	gui.add(crazy.setup("crazy", false));

}

void ofApp::update() {
	if (ofGetFrameNum() < 0)
		particleUpdate(aliveCount, flowLayer, float(crazy));
}

void ofApp::setupProject(int numParticles) {
	aliveCount = numParticles;
	particleSetup(numParticles);
	blurShader.load("shaders/blur.vert", "shaders/blur.frag");

	layer1.allocate(ofGetWidth(), ofGetHeight());
	layer2.allocate(ofGetWidth(), ofGetHeight());
	flowLayer.allocate(ofGetWidth(), ofGetHeight());

	layer0.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackgroundGradient(ofColor(255, 255, 255), ofColor(255, 255, 255));
	layer0.end();

	layer1.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackground(255, 255, 255);
	layer1.end();

	layer2.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackground(255, 255, 255);
	layer2.end();

	flowLayer.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackgroundGradient(ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 255));
	flowLayer.end();

	ofBackground(255);
}

void ofApp::draw() {
		layer1.begin();
	ofBackground(11, 255);
	particleDraw(aliveCount);
	layer1.end();

	layer2.begin();
	blurShader.begin();
	blurShader.setUniform2f("res", glm::vec2(ofGetWidth(), ofGetHeight()));
	blurShader.setUniformTexture("flow", flowLayer.getTexture(), 1);
	blurShader.setUniform1f("time", ofGetFrameNum());
	layer1.draw(0, 0);
	blurShader.end();
	layer2.end();

	layer2.draw(0, 0);

	gui.draw();

	if (particleNum != particleNumField) {
		//cout << "aaaaa " << particleNumField << " " << particleNum << "\n";
		particleNum = particleNumField;
		setupProject(particleNum);
	}
}


void ofApp::particleSetup(const int n) {
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

void ofApp::initParticles() {
	for (int i = 0; i < particleCount; ++i) {
		particles[i].pos.x = ofRandom(100, ofGetWidth() - 100);
		//particles[i].pos.y = ofRandom(100, ofGetHeight() - 100);
		particles[i].pos.y = ofMap(i, 0, particleCount - 1, 100, ofGetHeight() - 100);
		//particles[i].pos.x = ofGetWidth()/2;
		//particles[i].pos.y = ofGetHeight()/2;
		particles[i].pos.z = 0.0;
		particles[i].pos.w = 1.0;
		particles[i].vel.x = 0.0;
		particles[i].vel.y = 0.0;
		particles[i].acc.x = ofRandom(140, 220) / 255.;
		particles[i].acc.y = ofRandom(110, 170) / 255.;
		particles[i].acc.z = ofRandom(20, 200 * particles[i].pos.y / ofGetHeight()) / 255.;
		particles[i].drag.x = ofRandom(0.9, 0.98);
		particles[i].drag.y = float(i);
	}
}

void ofApp::particleUpdate(float aliveCount, ofFbo aaa, float crazy) {

	computeShader.begin();
	computeShader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	computeShader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());
	computeShader.setUniform1f("time", ofGetFrameNum());
	computeShader.setUniform1f("aliveCount", aliveCount);
	computeShader.setUniform1f("crazy", crazy);
	computeShader.setUniformTexture("aaa", aaa, 0);
	computeShader.dispatchCompute(particleCount / WORK_GROUP_SIZE, 1, 1);
	// glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	computeShader.end();
}


void ofApp::particleDraw(float aliveCount) {
	//ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

	ofEnableAlphaBlending();
	ofEnablePointSprites();
	ofEnableArbTex();

	renderShader.begin();
	renderShader.setUniform2f("screen", glm::vec2(ofGetWidth(), ofGetHeight()));
	renderShader.setUniform1f("aliveCount", glm::float32(aliveCount));
	renderShader.setUniformTexture("tex0", imgTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	vbo.draw(GL_POINTS, 0, particleCount);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
	renderShader.end();

}


void ofApp::keyPressed(int key) {
	ofImageQualityType quality = OF_IMAGE_QUALITY_BEST;
	ofPixels pix;
	switch (key) {
	case 's':
		pix.allocate(layer0.getWidth(), layer0.getHeight(), OF_IMAGE_QUALITY_BEST);
		layer0.readToPixels(pix);
		ofSaveImage(pix, "mySnapshot.png");
		break;
	case 'f':
		ofToggleFullscreen();

		layer1.allocate(ofGetWidth(), ofGetHeight());
		layer2.allocate(ofGetWidth(), ofGetHeight());
		flowLayer.allocate(ofGetWidth(), ofGetHeight());

		layer0.begin();
		//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
		ofBackgroundGradient(ofColor(255, 255, 255), ofColor(255, 255, 255));
		layer0.end();

		layer1.begin();
		//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
		ofBackground(255, 255, 255);
		layer1.end();

		layer2.begin();
		//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
		ofBackground(255, 255, 255);
		layer2.end();

		flowLayer.begin();
		//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
		ofBackgroundGradient(ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 255));
		flowLayer.end();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	setupProject(particleNum);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

	//setupProject(particleNum);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
