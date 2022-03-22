#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	ofSetVerticalSync(true);
	//ofHideCursor();

	aliveCount = -0.5;
	particleSystem.setup(5555);
	blurShader.load("shaders/blur.vert", "shaders/blur.frag");

	layer.allocate(ofGetWidth(), ofGetHeight());
	flowLayer.allocate(ofGetWidth(), ofGetHeight());

	flowLayer.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackgroundGradient(ofColor(255,255,255), ofColor(255, 255, 255));
	flowLayer.end();
}

void ofApp::update() {
	particleSystem.update(aliveCount, flowLayer);
}

void ofApp::draw() {
	layer.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackgroundGradient(ofColor(15, 15, 15), ofColor(15, 15, 15));
	particleSystem.draw(aliveCount);
	layer.end();

	flowLayer.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackgroundGradient(ofColor(255, 255, 255, 12), ofColor(255, 255, 255, 12));
	particleSystem.drawFlow(aliveCount);
	flowLayer.end();

	/*blurShader.begin();
	blurShader.setUniform1f("blurAmnt", 3.);
	blurShader.setUniform2f("res", glm::vec2(ofGetWidth(), ofGetHeight()));
	blurShader.setUniformTexture("flow", flowLayer.getTexture(), 1);
	layer.draw(0, 0);
	blurShader.end();*/

	flowLayer.draw(0, 0);

	//layer.getTexture().draw(0, 0);

}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'f':
		ofToggleFullscreen();
		layer.allocate(ofGetWidth(), ofGetHeight());
		flowLayer.allocate(ofGetWidth(), ofGetHeight());
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
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
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
	layer.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}