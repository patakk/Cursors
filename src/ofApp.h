#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#define WORK_GROUP_SIZE 256

class ofApp : public ofBaseApp {

	struct Particle {
		ofVec4f pos;
		ofVec4f vel;
		ofVec4f acc;
		ofVec4f drag;
	};

	struct Tree {
		ofVec3f pos;
		ofVec3f col;
		glm::mat4 worldMatrix;
		float size;
		float height;
		ofCylinderPrimitive primitive;
	};


	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void resetProject(int numParticles);
		void particleSetup(const int n);
		void initParticles();
		void particleUpdate(float aliveCount, ofFbo threeD, ofFbo depthL, float crazy);
		void particleDraw(float aliveCount);

		void threeDraw(bool depth);
		void drawTrees();

		ofTexture imgTexture;
		int particleCount;

		ofShader computeShader;
		ofShader renderShader;

		vector<Particle> particles;
		ofBufferObject particlesBuffer;

		ofVbo vbo;
		ofLight light;
		ofEasyCam cam;
		ofShader blurShader;
		ofShader depthShader;

		ofFbo threeDLayer;
		ofFbo depthLayer;
		ofFbo particleLayer;
		ofFbo postLayer;
		ofFbo flowLayer;

		float aliveCount;
		float time = 0;

		ofxPanel gui;
		ofxToggle crazy;
		ofxIntField particleNumField;
		int particleNum;
		ofColor hsv2rgb(ofColor);
		ofColor bgColor;
		ofColor offColor;

		vector<Tree> trees;
		void treeSetup();

};
