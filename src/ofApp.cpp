#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	ofSetVerticalSync(true);
	//ofHideCursor();

	particleNum = 500000;
	setupProject(particleNum);

	gui.setup();
	gui.setSize(200, 100);
	gui.add(particleNumField.setup("particle count", particleNum, 10000, 3000000));
	gui.add(crazy.setup("crazy", false));

}

void ofApp::update() {
	particleSystem.update(aliveCount, flowLayer, float(crazy));
}

void ofApp::setupProject(int numParticles) {
	aliveCount = numParticles;
	particleSystem.setup(numParticles);
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

	//flowLayer.begin();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));
	//ofBackgroundGradient(ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 255));
	//particleSystem.drawFlow(aliveCount);
	//flowLayer.end();

	/*blurShader.begin();
	blurShader.setUniform1f("blurAmnt", 3.);
	blurShader.setUniform2f("res", glm::vec2(ofGetWidth(), ofGetHeight()));
	blurShader.setUniformTexture("flow", flowLayer.getTexture(), 1);
	layer.draw(0, 0);
	blurShader.end();*/

	//ofSetColor(255, 5);
	//ofFill();
	//ofRect(0, 0, ofGetWidth(), ofGetHeight());
	//ofBackgroundGradient(ofColor(255, 255, 255, 3), ofColor(255, 255, 255, 3));
	//layer1.draw(0, 0);

	//layer0.begin();
	//ofClear(255);
	//layer0.clearColorBuffer(ofFloatColor(0, 0));
	//blurShader.begin();
	//blurShader.setUniform2f("res", glm::vec2(ofGetWidth(), ofGetHeight()));
	//blurShader.setUniformTexture("flow", flowLayer.getTexture(), 1);
	//blurShader.setUniform1f("time", ofGetFrameNum());
	//layer1.draw(0, 0);
	//particleSystem.draw(aliveCount);
	//blurShader.end();
	//layer0.end();

	//layer1.draw(0, 0);
	//layer.getTexture().draw(0, 0);
	layer1.begin();
	ofBackground(211, 255);
	particleSystem.draw(aliveCount);
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
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
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
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
	aliveCount++;
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

	//setupProject(particleNum);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
