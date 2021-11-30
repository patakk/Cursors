#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	ofSetVerticalSync(true);
	ofHideCursor();
	particleSystem.setup(22444);
	blurShader.load("shaders/blur.vert", "shaders/blur.frag");

	layer.allocate(ofGetWidth(), ofGetHeight());
	flowLayer.allocate(ofGetWidth(), ofGetHeight());
}

void ofApp::update() {
}

void ofApp::draw() {
	particleSystem.reset();
	layer.begin();
	ofDisableBlendMode();
	//particleSystem.objectsLayer.draw(0, 0);
	ofBackgroundGradient(ofColor(15, 15, 15), ofColor(15, 15, 15));
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	for (int k = 0; k < 2133; k++) {
		particleSystem.update();
		particleSystem.draw(1. * k / (particleSystem.particleCount - 1));
	}
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	//ofBackgroundGradient(ofColor(15, 15, 15), ofColor(15, 15, 15));

	layer.end();

	flowLayer.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackgroundGradient(ofColor(0,0,0), ofColor(0,0,0));
	particleSystem.drawFlow();
	flowLayer.end();

	//blurShader.begin();
	//blurShader.setUniform1f("blurAmnt", 3.);
	//blurShader.setUniform2f("res", glm::vec2(ofGetWidth(), ofGetHeight()));
	//blurShader.setUniformTexture("flow", flowLayer.getTexture(), 1);
	//layer.draw(0, 0);
	//blurShader.end();

	layer.draw(0, 0);
	if (ofGetFrameNum() % 100 == 0)
		std::cout << ofGetFrameRate() << std::endl;

}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'f':
		ofToggleFullscreen();
		layer.allocate(ofGetWidth(), ofGetHeight());
		flowLayer.allocate(ofGetWidth(), ofGetHeight());
		break;
	case 's':
		ofSaveFrame();
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