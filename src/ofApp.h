#pragma once

#define USE_PROGRAMMABLE_PIPELINE 1

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
//#include "ofxGrt.h"
#include "ofxFontStash2.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		//void setupGui();
		void update();
		void draw();
		//void drawGui(ofEventArgs & args);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void audioOut(float * input, int bufferSize, int nChannels);

		ofxKFW2::Device kinect;
		ofEasyCam cam;
		ofShader shader;
		ofShader raytracing;
		//ofTrueTypeFont largeFont;
		vector<string> coordinates = vector<string>(75);
		//float timeToUpdate = 0;
		//RegressionData trainingData;      		//This will store our training data
		//GestureRecognitionPipeline pipeline;        //This is a wrapper for our regression and any pre/post processing modules 
		//GRT::VectorFloat predictVector;
		ofSoundStream soundStream;
		float* buffer;
		vector<float> bufferStorage = vector<float>(75);
		int errorIndex = 0;
		float maxBuffer = 0;
		string lookalikeString = "1.0";
		//ofFbo framebuffer[2];
		//int framebufferIndex = 0;
		//ofFbo framebufferEnd[2];
		float lookalikeTarget = 0.75f;
		float lookalike = 0.75f;
		float initialLookalikeTarget = 0.75f;
		float initialLookalike = 0.75f;
		ofxFontStash2::Fonts font;
};
