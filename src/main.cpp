#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//--------------------------------------------------------------
int main(){
#ifdef USE_PROGRAMMABLE_PIPELINE
	ofGLFWWindowSettings settings;
	settings.setGLVersion(4, 3);
	settings.width = 1400;
	settings.height = 1050;
	//settings.monitor = 2;
	//settings.multiMonitorFullScreen = true;
	//settings.windowMode = OF_WINDOW;
	//settings.setPosition(ofVec2f(1920, 0));
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	//settings.setGLVersion(4, 3);
	//settings.width = 1024;
	//settings.height = 1024;
	//settings.monitor = 1;
	////settings.multiMonitorFullScreen = true;
	//settings.setPosition(ofVec2f(0, 0));
	//// uncomment next line to share main's OpenGL resources with gui
	////settings.shareContextWith = mainWindow;	
	//shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
	//guiWindow->setVerticalSync(false);

	shared_ptr<ofApp> mainApp(new ofApp);
	//mainApp->setupGui();
	//ofAddListener(guiWindow->events().draw, mainApp.get(), &ofApp::drawGui);

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

#else
	ofSetupOpenGL(1024, 768, OF_WINDOW);
#endif

	//ofRunApp(new ofApp());
}
