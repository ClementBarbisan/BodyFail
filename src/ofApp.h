#pragma once

#define USE_PROGRAMMABLE_PIPELINE 1
#define MANUAL false
#define NOMULTIPLESCREEN true
#define FULLSCREEN false

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxFontStash2.h"
#include "ofxOsc.h"
#include <random>
#include <algorithm>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void setupGui();
		void update();
		void draw();
		void drawGui(ofEventArgs & args);

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
		vector<string> coordinates = vector<string>(100);
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
		ofFbo framebuffer;
		float lookalikeTarget = 1.0f;
		float lookalike = 1.0f;
		float initialLookalikeTarget = 1.0f;
		float initialLookalike = 1.0f;
		ofxOscReceiver oscReceiver;
		ofxOscSender oscSender;
		ofxOscMessage oscMessage;
		//default_random_engine rng;
		//ofxFontStash2::Fonts font;
		ofTrueTypeFont trueTypeFont;
		bool savedPosture = false;
		float speedWeight = 1.0;
		float positionWeight = 0.7;
		int maximumValue = 500;
		int behaviour = 1;
		float globalWeight = 1.0;
		//float progression_speed = 0.0f;
};
