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


	public:
		ofTexture imgTexture;
		void particleSetup(const int n);
		void initParticles();
		void particleUpdate(float aliveCount, ofFbo aaa, float crazy);
		void particleDraw(float aliveCount);
		void drawFlow(float aliveCount);
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

		void setupProject(int numParticles);

		int particleCount;

		ofShader computeShader;
		ofShader renderShader;
		ofShader flowShader;

		vector<Particle> particles;
		ofBufferObject particlesBuffer;

		ofVbo vbo;
		ofFbo aaa;

		ofShader blurShader;

		ofFbo layer0;
		ofFbo layer1;
		ofFbo layer2;
		ofFbo flowLayer;

		float aliveCount;

		ofxPanel gui;
		ofxToggle crazy;
		ofxIntField particleNumField;
		int particleNum;


};
