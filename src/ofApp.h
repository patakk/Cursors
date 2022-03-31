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
		void particleUpdate(float aliveCount, ofFbo aaa, float crazy);
		void particleDraw(float aliveCount);
		void drawFlow(float aliveCount);

		ofTexture imgTexture;
		int particleCount;

		ofShader computeShader;
		ofShader renderShader;
		ofShader flowShader;

		vector<Particle> particles;
		ofBufferObject particlesBuffer;

		ofVbo vbo;
		ofFbo aaa;
		ofLight light;
		ofEasyCam cam;
		ofShader blurShader;

		ofFbo threeDLayer;
		ofFbo particleLayer;
		ofFbo postLayer;
		ofFbo flowLayer;

		float aliveCount;

		ofxPanel gui;
		ofxToggle crazy;
		ofxIntField particleNumField;
		int particleNum;


};
