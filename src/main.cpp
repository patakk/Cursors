#include "ofMain.h"
#include "ofApp.h"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

//========================================================================
int main( ){

	ofGLWindowSettings settings;
	settings.setGLVersion(4, 6);
	//settings.windowMode = OF_FULLSCREEN;
	settings.setSize(1400, 1400);

	ofCreateWindow(settings);		// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
