#include "ofApp.h"

void ofApp::treeSetup() {
	int numTrees = 30;

	trees.resize(numTrees);
	for (int k = 0; k < numTrees; k++) {
		Tree* tree = &trees[k];
		float size = ofRandom(3, 9);
		float height = 900;
		ofVec3f col = ofVec3f(194 + 39 * ofRandom(-1, 1), 82 + 25 * ofRandom(-1, 1), 70 + 22 * ofRandom(-1, 1));
		ofVec3f pos = ofVec3f(ofRandom(-888, 888), height * .5, ofRandom(-244, 244));
		ofCylinderPrimitive primitive = ofCylinderPrimitive(size,  height, 32, 2);
		primitive.setPosition(pos.x, pos.y, pos.z);
		tree->pos = pos;
		tree->size = size;
		tree->height = height;
		tree->col = col;
		tree->primitive = primitive;
	}
}


void ofApp::threeDraw(bool depth) {
	if(depth){
		depthShader.begin();
		ofBackground(0, 0, 255);
	}
	cam.begin();
	ofEnableDepthTest();
	//ofBackgroundGradient(ofColor(0, 0, 255), ofColor(70, 100, 255));

	if (!depth)
		ofBackground(100,30,120);
	ofPushMatrix();
	ofSetColor(0, 255, 0);
	ofBox(2000, 5, 1000);
	ofPopMatrix();

	drawTrees();

	ofDisableDepthTest();
	cam.end();

	if (depth)
		depthShader.end();
}

void ofApp::drawTrees() {
	for (int k = 0; k < trees.size(); k++) {
		Tree* tree = &trees[k];
		ofSetColor(tree->col[0], tree->col[1], tree->col[2]);
		tree->primitive.draw();
	}
}
