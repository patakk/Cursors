#include "ofApp.h"


void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	ofSetVerticalSync(true);
	//ofHideCursor();

	particleNum = 131100;
	resetProject(particleNum);

	gui.setup();
	gui.setSize(200, 100);
	gui.add(particleNumField.setup("particle count", particleNum, 10000, 3000000));
	gui.add(crazy.setup("crazy", false));

}

void ofApp::update() {
	particleUpdate(aliveCount, threeDLayer, float(crazy));
}


void ofApp::resetProject(int numParticles) {
	aliveCount = numParticles;
	particleSetup(numParticles);
	blurShader.load("shaders/blur.vert", "shaders/blur.frag");

	particleLayer.allocate(ofGetWidth(), ofGetHeight());
	postLayer.allocate(ofGetWidth(), ofGetHeight());
	flowLayer.allocate(ofGetWidth(), ofGetHeight());

	threeDLayer.allocate(ofGetWidth(), ofGetHeight());


	particleLayer.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	//ofBackground(255, 255, 255);
	particleLayer.end();

	postLayer.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackground(255, 255, 255);
	postLayer.end();

	flowLayer.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackgroundGradient(ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 255));
	flowLayer.end();

	ofBackground(255);
}

void ofApp::draw() {
	ofClear(0);
	ofSetColor(255);

	threeDLayer.begin();
	cam.begin();
	ofEnableDepthTest();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	ofBackground(230, 90, 30);
	ofPushMatrix();
	ofTranslate(0, 0, 0);
	ofRotate(ofDegToRad(33), 1.0, 1.0, 1.0);
	ofSetColor(0,255,0);
	ofBox(444, 22, 444);
	ofPopMatrix();
	ofDisableDepthTest();
	cam.end();
	threeDLayer.end();

	particleLayer.begin();
	ofBackground(255);
	particleDraw(aliveCount);
	particleLayer.end();

	postLayer.begin();
	//blurShader.begin();
	blurShader.setUniform2f("res", glm::vec2(ofGetWidth(), ofGetHeight()));
	blurShader.setUniform1f("time", ofGetFrameNum());
	ofSetColor(255);
	//threeDLayer.draw(0, 0);
	particleLayer.draw(0, 0);
	//threeDLayer.getDepthTexture().draw(0, 0);
	blurShader.end();
	postLayer.end();

	postLayer.draw(0, 0);

	gui.draw();

	if (particleNum != particleNumField) {
		//cout << "aaaaa " << particleNumField << " " << particleNum << "\n";
		particleNum = particleNumField;
		resetProject(particleNum);
	}
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

		//threeDLayer.allocate(fbosettings);

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

	//setupProject(particleNum);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
