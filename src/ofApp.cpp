#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	ofSetVerticalSync(true);

	particleSystem.setup(11000);
	blurShader.load("shaders/blur.vert", "shaders/blur.frag");

	layer.allocate(ofGetWidth(), ofGetHeight());
}

void ofApp::update() {
	particleSystem.update();
}

void ofApp::draw() {
	layer.begin();
	ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	particleSystem.draw();
	layer.end();

	ofEnableArbTex();
	blurShader.begin();
	blurShader.setUniform1f("blurAmnt", 3.);
	blurShader.setUniform2f("res", glm::vec2(ofGetWidth(), ofGetHeight()));
	ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofSetColor(ofColor(0, 255, 0));
	ofDrawCircle(400, 100, 30);
	layer.draw(0, 0);
	blurShader.end();
	//layer.getTexture().draw(0, 0);

}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'f':
		ofToggleFullscreen();
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