#include "ofApp.h"


void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	ofSetVerticalSync(true);
	//ofHideCursor();

	particleNum = 333333;
	resetProject(particleNum);


	gui.setup();
	gui.setSize(200, 100);
	gui.add(particleNumField.setup("particle count", particleNum, 10000, 3000000));
	gui.add(crazy.setup("crazy", false));

}

void ofApp::update() {
	if (ofGetFrameNum() < 2) {
	}
}


void ofApp::draw() {
	ofClear(0);
	ofSetColor(255);

	// create 3D scene
	threeDLayer.begin();
	threeDraw(false);
	threeDLayer.end();

	// create depth scene
	depthLayer.begin();
	threeDraw(true);
	depthLayer.end();

	// draw particles
	particleLayer.begin();
	ofBackground(bgColor);
	particleDraw(aliveCount);
	particleLayer.end();

	// blurring
	ofSetColor(255);
	postLayer.begin();
	ofEnableArbTex();
	blurShader.begin();
	blurShader.setUniform2f("res", glm::vec2(ofGetWidth(), ofGetHeight()));
	blurShader.setUniformTexture("depthL", depthLayer.getTexture(), 1);
	blurShader.setUniform1f("time", time);
	//threeDLayer.draw(0, 0);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	particleLayer.draw(0, 0);
	//threeDLayer.getTexture().draw(0, 0);
	blurShader.end();
	postLayer.end();

	particleLayer.draw(0, 0);

	gui.draw();

	if (aliveCount != particleNumField)
		resetProject(particleNumField);
}

void ofApp::resetProject(int numParticles) {
	aliveCount = numParticles;
	particleSetup(numParticles);

	offColor = ofColor(ofRandom(-14 * 3, 14), ofRandom(-7, 4), ofRandom(-7, 7));

	treeSetup();
	blurShader.load("shaders/blur.vert", "shaders/blur.frag");
	depthShader.load("shaders/depth.vert", "shaders/depth.frag");

	bgColor = ofColor::fromHsb(ofRandom(255), ofRandom(50, 110), ofRandom(133, 233));

	particleLayer.allocate(ofGetWidth(), ofGetHeight());
	postLayer.allocate(ofGetWidth(), ofGetHeight());
	flowLayer.allocate(ofGetWidth(), ofGetHeight());
	depthLayer.allocate(ofGetWidth(), ofGetHeight());
	threeDLayer.allocate(ofGetWidth(), ofGetHeight());

	time++;
	particleUpdate(aliveCount, threeDLayer, depthLayer, float(crazy));
}


void ofApp::keyPressed(int key) {
	ofImageQualityType quality = OF_IMAGE_QUALITY_BEST;
	ofPixels pix;
	switch (key) {
	case 's':
		pix.allocate(threeDLayer.getWidth(), threeDLayer.getHeight(), OF_IMAGE_QUALITY_BEST);
		threeDLayer.readToPixels(pix);
		ofSaveImage(pix, "mySnapshot.png");
		break;
	case 'f':
		ofToggleFullscreen();

		particleLayer.allocate(ofGetWidth(), ofGetHeight());
		postLayer.allocate(ofGetWidth(), ofGetHeight());
		flowLayer.allocate(ofGetWidth(), ofGetHeight());
		depthLayer.allocate(ofGetWidth(), ofGetHeight());
		threeDLayer.allocate(ofGetWidth(), ofGetHeight());

		//threeDLayer.allocate(fbosettings);

		depthLayer.begin();
		//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
		ofBackgroundGradient(ofColor(255, 255, 255), ofColor(255, 255, 255));
		depthLayer.end();

		threeDLayer.begin();
		//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
		ofBackgroundGradient(ofColor(255, 255, 255), ofColor(255, 255, 255));
		threeDLayer.end();

		particleLayer.begin();
		//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
		ofBackground(255, 255, 255);
		particleLayer.end();

		postLayer.begin();
		//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
		ofBackground(255, 255, 255);
		postLayer.end();

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
	time++;
	particleUpdate(aliveCount, threeDLayer, depthLayer, float(crazy));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	//resetProject(particleNum);
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
	resetProject(particleNum);
	//setupProject(particleNum);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
