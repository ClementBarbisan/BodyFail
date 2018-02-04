#include "ofApp.h"
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <Psapi.h>

int clipCustom(int n, int lower, int upper) {
	return std::max(lower, std::min(n, upper));
}

void ofApp::killProcess()
{
	int retval = ::_tsystem(_T("taskkill /F /T /IM eyw_mobile_runtime.exe"));
	retval = ::_tsystem(_T("taskkill /F /T /IM Launcher.exe"));
}

void ofApp::startProcess()
{
	//ofLog() << "startProcess";
	//// additional information
	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;

	//// set the size of the structures
	//ZeroMemory(&si, sizeof(si));
	//si.cb = sizeof(si);
	//ZeroMemory(&pi, sizeof(pi));
	//string app = "D:\Program Files (x86)\EyesWeb XMI 5.7.2.0\body_fail\Launcher.exe";
	//std::wstring app_w(app.length(), L' '); // Make room for characters
	//std::copy(app.begin(), app.end(), app_w.begin());
	//const wchar_t* app_const = app_w.c_str();
	//CreateProcessW(app_const,   // the path
	//	NULL,        // Command line
	//	NULL,           // Process handle not inheritable
	//	NULL,           // Thread handle not inheritable
	//	FALSE,          // Set handle inheritance to FALSE
	//	0,              // No creation flags
	//	NULL,           // Use parent's environment block
	//	NULL,           // Use parent's starting directory 
	//	&si,            // Pointer to STARTUPINFO structure
	//	&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	//);
	//// Close process and thread handles. 
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
	system("E:\\of_v0.9.8_vs_release\\apps\\myApps\\BodyFail\\bin\\data\\Restart.bat");
}

//--------------------------------------------------------------
void ofApp::setupGui()
{
	ofBackground(0);
	ofHideCursor();
	ofSetWindowTitle("BodyFail");
	ofDirectory dir("");
	stringstream filePath;
	filePath << "start cmd.exe \"" << dir.getAbsolutePath() << "/data/AutoFocus.bat\"";
	system(filePath.str().c_str());
	//ofSetFullscreen(true);
}

//--------------------------------------------------------------
void ofApp::setup(){
	if (!MACHINELEARNING)
		startProcess();
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();
	shader.load("bodyIndex.vert", "bodyIndex.frag", "bodyIndex.geom");
	raytracing.load("raytracer.vert", "raytracer.frag", "raytracer.geom");
	contours.load("contours");
	rng = default_random_engine{};
	trueTypeFont.loadFont("backFont.ttf", 12);
	
	/*font.setup(false);
	font.addFont("backFont", "backFont.ttf");
	ofxFontStash2::Style style;
	style.color = ofColor::white;
	style.fontID = "backFont";
	style.fontSize = 12;
	font.addStyle("backFontStyle", style);
	font.setDefaultStyle("backFontStyle");*/
	if (!MACHINELEARNING)
	{
		oscReceiver.setup(8532);
		oscSender.setup("127.0.0.1", 8534);
		oscMessage.clear();
		oscMessage.setAddress("/modality");
		oscMessage.addIntArg(behaviour);
		oscSender.sendMessage(oscMessage);
		oscMessage.clear();
		oscMessage.setAddress("/parameters");
		oscMessage.addFloatArg(speedWeight);
		oscMessage.addFloatArg(positionWeight);
		oscMessage.addFloatArg(globalWeight);
		oscMessage.addFloatArg(globalWeight / 2);
		oscMessage.addFloatArg(globalWeight / 4);
		oscMessage.addFloatArg(maximumValue);
		oscSender.sendMessage(oscMessage);
	}
	else
	{
		//pipeline.load("pipeline_data.grt"); //The MLP algorithm directly supports multi-dimensional outputs, so MDRegression is not required here

		unsigned int numInputNeurons = 75;
		unsigned int numHiddenNeurons = 128; //For more complex problems, increase the number of hidden units
		unsigned int numOutputNeurons = 4;
		MLP mlp;
		mlp.init(numInputNeurons, numHiddenNeurons, numOutputNeurons, Neuron::LINEAR, Neuron::TANH, Neuron::TANH);

		//Set the training settings
		mlp.setMaxNumEpochs(1000); //This sets the maximum number of epochs (1 epoch is 1 complete iteration of the training data) that are allowed
		mlp.setMinChange(1.0e-10); //This sets the minimum change allowed in training error between any two epochs
		mlp.setLearningRate(0.2); //This sets the rate at which the learning algorithm updates the weights of the neural network
		mlp.setMomentum(0.5);
		mlp.setNumRandomTrainingIterations(2); //This sets the number of times the MLP will be trained, each training iteration starts with new random values
		mlp.setUseValidationSet(true); //This sets aside a small portiion of the training data to be used as a validation set to mitigate overfitting
		mlp.setValidationSetSize(40); //Use 20% of the training data for validation during the training phase
		mlp.setRandomiseTrainingOrder(true); //Randomize the order of the training data so that the training algorithm does not bias the training
		mlp.enableScaling(true); //Learning works much better if the training and prediction data is first scaled to a common range (i.e. [0.0 1.0])

		pipeline << mlp; //The MLP algorithm directly supports multi-dimensional outputs, so MDRegression is not required here
		pipeline.load(ofToDataPath("pipeline_data.grt"));
	}
	soundStream.setup(this, 2, 0, 44100, 512, 4);
	buffer = new float[75];
	for (int i = 0; i < 75; i++)
		buffer[i] = 1;
	//ofSetFullscreen(true);
	ofSetColor(255);
	ofBackground(0);
	initialLookalike = lookalike;
	initialLookalikeTarget = lookalikeTarget;
	framebuffer.allocate(1280, 1024);
	framebufferMask.allocate(1920, 1080);
	light.setup();
	light.enable();
	//light.setAreaLight(100, 100);
	ofSetGlobalAmbientColor(ofColor::white);
	light.setAmbientColor(ofColor::blueSteel);
	light.setDiffuseColor(ofColor::ghostWhite);
	light.setSpecularColor(ofColor::aqua);
	light.setPosition(ofVec3f(0.0, 0.0, 0.0));
	materialPlane.setAmbientColor(ofFloatColor(0.5, 0.5, 0.5, 1.0));
	materialPlane.setDiffuseColor(ofFloatColor(0.8, 0.8, 0.8, 1.0));
	materialPlane.setSpecularColor(ofFloatColor(0.8, 0.8, 0.8, 1.0));
	plane.set(20000, 20000, 2, 2);
	plane.move(ofVec3f(0, 0, -10.0));
	materialPlane.setShininess(10);
	//ofEnableDepthTest();
	ofEnableLighting();
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	ofHideCursor();
	//timeToUpdate += ofGetLastFrameTime();
	if (MANUAL)
	{
		if (abs(lookalikeTarget - lookalike) > 0.001)
		{
			if (lookalikeTarget > lookalike)
				lookalike += 0.001;
			else
				lookalike -= 0.001;
		}
	}
	if (!MACHINELEARNING)
	{
		if (resetDelayer > 5.0f)
		{
			oscMessage.clear();
			oscMessage.setAddress("/reset");
			oscMessage.addIntArg(1);
			oscSender.sendMessage(oscMessage);
		}
		else if (lookalike < lookalikeMin)
			return;
		resetDelayer = 0;
		while (oscReceiver.hasWaitingMessages())
		{
			oscReceiver.getNextMessage(oscMessage);
			if (oscMessage.getAddress() == "/progress_speed")
			{
				progression_speed = oscMessage.getArgAsFloat(0);
			}
			if (oscMessage.getAddress() == "/progress")
			{
				lookalike = 1.0 - oscMessage.getArgAsFloat(0);
				originalLookalike = oscMessage.getArgAsFloat(0);
			}
			//else if (oscMessage.getAddress() == "/progress_speed")
				//progression_speed = oscMessage.getArgAsFloat(0);
			//lookalike = 1.0 -;
			//ofLog() << lookalike;
			//ofLog() << "Treated = " << ((lookalike)*(((3 - (lookalike*(2 - lookalike))) - (lookalike))));
		}
		if (lookalike > 0.75 || lookalike < 0.1)
		{
			oscMessage.clear();
			oscMessage.setAddress("/parameters");
			oscMessage.addFloatArg(speedWeight * 1.5);
			oscMessage.addFloatArg(positionWeight * 1.5);
			oscMessage.addFloatArg(globalWeight);
			oscMessage.addFloatArg(globalWeight / 4);
			oscMessage.addFloatArg(globalWeight / 2);
			oscMessage.addFloatArg(maximumValue);
			oscSender.sendMessage(oscMessage);
		}
		else
		{
			oscMessage.clear();
			oscMessage.setAddress("/parameters");
			oscMessage.addFloatArg(speedWeight);
			oscMessage.addFloatArg(positionWeight);
			oscMessage.addFloatArg(globalWeight);
			oscMessage.addFloatArg(globalWeight / 2);
			oscMessage.addFloatArg(globalWeight / 4);
			oscMessage.addFloatArg(maximumValue);
			oscSender.sendMessage(oscMessage);
		}
		if (lookalike > lookalikeMin)
		{
			if (oldLookalike != lookalike)
				lookalike += clipCustom(progression_speed, 0, 1) / (3 + (1 - lookalike) * 5);
			oldLookalike = lookalike;
		}
	}
	else if (kinect.isFrameNew() && pipeline.getTrained())
	{
		if (resetDelayer > 5.0f)
		{
			lookalike = 1.0f;
		}
		else if (lookalike < lookalikeMin)
			return;
		resetDelayer = 0;
			/*if (predictVector.size() >= 375)
			{
			std::rotate(predictVector.begin(), predictVector.begin() + 75, predictVector.end());
			for (int i = 0; i < 75; i++)
			predictVector.pop_back();
			}*/
			predictVector.clear();
			auto bodies = kinect.getBodySource()->getBodies();
			for (auto body : bodies) {
				if (body.joints.size() == 25)
				{
					for (auto joint : body.joints) {
						predictVector.push_back(joint.second.getPosition().x);
						predictVector.push_back(joint.second.getPosition().y);
						predictVector.push_back(joint.second.getPosition().z);
						//ofLog() << joint.first << " " << joint.second.getPosition();
						//now do something with the joints
					}
					break;
				}
			}
			if (predictVector.size() == 75)
			{
				VectorFloat dataResult;
				if (pipeline.predict(predictVector))
				{
					dataResult = pipeline.getRegressionData();
					if (lookalike < 1)
						lookalike += ofGetLastFrameTime() * (CLAMP(dataResult[0], 0, 1) / TOTALTIME);
					if (lookalike > 0)
					{
						lookalike -= ofGetLastFrameTime() * (CLAMP(dataResult[1], 0, 1) / TOTALTIME / 2);
					}
					lookalike = CLAMP(lookalike, 0, 1);
				}
			}
	}
}

//-------------------------------------------------------------
void ofApp::drawGui(ofEventArgs &args)
{
	if (lookalike < lookalikeMin)
	{
		for (int i = 0; i < 200; i++)
		{
			if ((i + errorIndex) % 2 == 0)
				//font.drawFormatted("..yrteR", 200, 10 * i + 10);
				//trueTypeFont.drawString("..yrteR", 200, 10 * i + 10);
				ofDrawBitmapString("Retry...", 200, 10 * i + 10);
			else
				//font.drawFormatted("detpurroc eroC : tluaF noitatnemgeS", 200, 10 * i + 10);
				//trueTypeFont.drawString("detpurroc eroC : tluaF noitatnemgeS", 200, 10 * i + 10);
				ofDrawBitmapString("Segmentation Fault : Core corrupted", 200, 10 * i + 10);
		}
		errorIndex++;
		if (errorIndex == 200)
			errorIndex = 0;
		return;
	}
	rotate(coordinates.begin(), coordinates.begin() + 25, coordinates.end());
	auto bodies = kinect.getBodySource()->getBodies();
	
	for (auto body : bodies)
	{
		if (body.joints.size() == 25)
		{
			
			int index = 0;
			for (auto joint : body.joints)
			{
				stringstream stream;
				stream << joint.first << " " << joint.second.getPosition();
				if (index <= 25)
				{
					coordinates[174 + index] = stream.str();
					//reverse(coordinates[174 + index].begin(), coordinates[174 + index].end());
				}
				maxBuffer = MAX(abs(joint.second.getPosition().x), abs(joint.second.getPosition().y));
				maxBuffer = MAX(maxBuffer, abs(joint.second.getPosition().z));
				buffer[index * 3] = joint.second.getPosition().x;
				buffer[index * 3 + 1] = joint.second.getPosition().y;
				buffer[index * 3 + 2] = joint.second.getPosition().z;
				index++;
			}
		}
	}
	for (int i = 0; i < 100; i++)
	{
		//font.drawFormatted(coordinates[i], 200, 10 * i + 10);
		//trueTypeFont.drawString(coordinates[i], 200, 10 * i + 10);
		ofDrawBitmapString(coordinates[i], 200, 10 * i + 10);
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	stringstream ss;
	if (lookalike <= lookalikeMin)
	{
		resetDelayer += ofGetLastFrameTime();
		if (!savedPosture && !insideSavedPosture)
		{
			insideSavedPosture = true;
			auto bodies = kinect.getBodySource()->getBodies();
			for (auto body : bodies) {
				if (body.joints.size() >= 25)
				{
					int index = 0;
					stringstream url;
					url << "https://bodyfail.com/addSample?";
					for (auto joint : body.joints) 
					{
						if (index < 24)
							url << "n" << index << "x=" << joint.second.getPosition().x << "&n" << index << "y=" << joint.second.getPosition().y << "&n" << index << "z=" << joint.second.getPosition().z << "&";
						else
							url << "n" << index << "x=" << joint.second.getPosition().x << "&n" << index << "y=" << joint.second.getPosition().y << "&n" << index << "z=" << joint.second.getPosition().z;
						//ofLog() << joint.first << " " << joint.second.getPosition();
						//now do something with the joints
						index++;
					}
					url << "&place=Louvre";
					if (index >= 25)
					{
						ofLoadURL(url.str());
						savedPosture = true;
						break;
					}
				}
			}
			insideSavedPosture = false;
		}
		maxBuffer = 1;
		for (int i = 0; i < 75; i++)
		{
			buffer[i] = sin(PI);
		}
		
		ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofRotateY(180);
		ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);
		kinect.getBodySource()->drawProjected(0, 0, ofGetWidth(), ofGetHeight(), ofxKinectForWindows2::ColorCamera);
		ofPopMatrix();
		if (NOMULTIPLESCREEN)
		{
			for (int i = 0; i < 200; i++)
			{
				if (i < 100)
				{
					if ((i + errorIndex) % 2 == 0)
						//font.drawFormatted("..yrteR", 200, 10 * i + 10);
						//trueTypeFont.drawString("..yrteR", 200, 10 * i + 10);
						ofDrawBitmapString("Retry...", 1400, 10 * i + 10);
					else
						//font.drawFormatted("detpurroc eroC : tluaF noitatnemgeS", 200, 10 * i + 10);
						//trueTypeFont.drawString("detpurroc eroC : tluaF noitatnemgeS", 200, 10 * i + 10);
						ofDrawBitmapString("Segmentation Fault : Core corrupted", 1400, 10 * i + 10);
				}
				else
				{
					if ((i + errorIndex) % 2 == 0)
						//font.drawFormatted("..yrteR", 200, 10 * i + 10);
						//trueTypeFont.drawString("..yrteR", 200, 10 * i + 10);
						ofDrawBitmapString("Retry...", 1400 + ofGetWidth() / 2, 10 * (i - 100) + 10);
					else
						//font.drawFormatted("detpurroc eroC : tluaF noitatnemgeS", 200, 10 * i + 10);
						//trueTypeFont.drawString("detpurroc eroC : tluaF noitatnemgeS", 200, 10 * i + 10);
						ofDrawBitmapString("Segmentation Fault : Core corrupted", 1400 + ofGetWidth() / 2, 10 * (i - 100) + 10);
				}
			}
			errorIndex++;
			if (errorIndex == 200)
				errorIndex = 0;
		}
		return;
	}
	savedPosture = false;
	ofDisableDepthTest();
	ofTexture & texture = kinect.getBodyIndexSource()->getTexture();
	framebufferMask.begin();
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 5);
	texture.draw(0, 0);
	ofDisableAlphaBlending();
	framebufferMask.end();
	framebuffer.begin();	
	cam.begin();
	cam.setPosition(0.0, 0.0, 0.0);
	
	ofPushMatrix();
	//materialPlane.begin();
	//
	//plane.draw();
	//light.draw();

	ofScale(100, 100, 100);
	ofRotateY(180);
	ofMesh mesh = kinect.getDepthSource()->getMesh(false, ofxKFW2::Source::Depth::PointCloudOptions::ColorCamera);
	ofMesh meshWireframe = kinect.getDepthSource()->getMesh(true, ofxKFW2::Source::Depth::PointCloudOptions::ColorCamera);
	ofTexture & colorTexture = kinect.getColorSource()->getTexture();
	ofTexture & depthTexture = kinect.getDepthSource()->getTexture();
	vector<size_t> & indices = meshWireframe.getIndices();
	//shuffle(meshWireframe.getIndices().begin(), meshWireframe.getIndices().end(), rng);
	for (int i = 0; i < indices.size(); i += 4 + 100 * lookalike)
	{
		int random = rand() % indices.size();
		swap(indices[i], indices[random]);
	}
	meshWireframe.setMode(OF_PRIMITIVE_LINES);
	raytracing.begin();

	raytracing.setUniform1i("uWidth", kinect.getBodyIndexSource()->getWidth());
	raytracing.setUniformTexture("uBodyIndexTex", texture, 1);
	raytracing.setUniform1f("time", ofGetElapsedTimef());
	raytracing.setUniformTexture("uColorTex", colorTexture, 2);
	raytracing.setUniformTexture("depthTex", depthTexture, 3);
	raytracing.setUniform1f("lookalike", lookalike);
	meshWireframe.drawWireframe();
	raytracing.end();
	shader.begin();
	shader.setUniform1i("uWidth", kinect.getBodyIndexSource()->getWidth());
	shader.setUniform1i("uHeight", kinect.getBodyIndexSource()->getHeight());
	shader.setUniformTexture("uBodyIndexTex", framebufferMask.getTextureReference(), 1);
	shader.setUniformTexture("depthTex", depthTexture, 3);
	shader.setUniform1f("time", ofGetElapsedTimef());
	shader.setUniformTexture("uColorTex", colorTexture, 2);
	shader.setUniform1f("lookalike", lookalike);
	mesh.draw();
	
	shader.end();
	//materialPlane.end();
	
	ofPopMatrix();
	cam.end();
	
	framebuffer.end();
	//ofDisableLighting();
	framebuffer.draw(0, 0);
	contours.begin();
	ofPushMatrix();
	ofTranslate(1280, 0);
	contours.setUniform1f("lookalike", lookalike);
	texture.draw(0, 0, -1280, 1024);
	ofPopMatrix();
	contours.end();
	framebuffer.draw(1280, 0);
	
	framebuffer.begin();
	ofFill();
	ofSetColor(0, 0, 0, 10 + 20 * lookalike);
	ofDrawRectangle(0, 0, 1280, 1024);
	framebuffer.end();
	maxBuffer = 0;
	if (NOMULTIPLESCREEN)
	{
		rotate(coordinates.begin(), coordinates.begin() + 25, coordinates.end());
		auto bodies = kinect.getBodySource()->getBodies();
		if (bodies.size() == 0)
		{
			killProcess();
			startProcess();
			timeElapsedSinceBug = 0;
		}
		else
		{
			timeElapsedSinceBug += ofGetLastFrameTime();
			if (timeElapsedSinceBug > TIMETORESET)
			{
				killProcess();
				startProcess();
				timeElapsedSinceBug = 0;
			}
		}
		
		for (auto body : bodies)
		{
			if (body.joints.size() == 25)
			{
				if (originalLookalike == oldOriginalLookalike)
				{
					indexStaySame++;
					if (indexStaySame > 1000)
					{
						killProcess();
						startProcess();
						indexStaySame = 0;
					}
				}
				else
					indexStaySame = 0;
				oldOriginalLookalike = originalLookalike;
				int index = 0;
				for (auto joint : body.joints)
				{
					stringstream stream;
					stream << joint.first << " " << joint.second.getPosition();
					if (index <= 25)
					{
						coordinates[174 + index] = stream.str();
						//reverse(coordinates[174 + index].begin(), coordinates[174 + index].end());
					}
					maxBuffer = MAX(abs(joint.second.getPosition().x), abs(joint.second.getPosition().y));
					maxBuffer = MAX(maxBuffer, abs(joint.second.getPosition().z));
					buffer[index * 3] = joint.second.getPosition().x;
					buffer[index * 3 + 1] = joint.second.getPosition().y;
					buffer[index * 3 + 2] = joint.second.getPosition().z;
					index++;
				}
			}
		}
		for (int i = 0; i < 200; i++)
		{
			//font.drawFormatted(coordinates[i], 200, 10 * i + 10);
			if (i < 101)
				ofDrawBitmapString(coordinates[i], 1400, 10 * i + 10);
			else
				ofDrawBitmapString(coordinates[i], 1400 + ofGetWidth() / 2, 10 * (i - 100) + 10);
		}
	}
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
}


//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{
	float phase = ofRandom(TWO_PI);
	for (int i = 0; i < bufferSize; i += 2)
	{
		if (phase > TWO_PI)
			phase = 0;
		output[i * nChannels] = buffer[i % 75] / maxBuffer * sin(phase * lookalike);
		output[i * nChannels + 1] = buffer[i % 75] / maxBuffer * sin(phase * lookalike);
		phase += 0.05;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case 'm':
			if (lookalike < 1.0f && abs(lookalikeTarget - lookalike) <= 0.001)
			{
				lookalikeTarget *= 2;
				lookalikeString = ofToString(lookalike);
				//ofLog() << "lookalike = " << lookalike;
			}
		break;
		case 'p':
			if (lookalike > 0.01f && abs(lookalikeTarget - lookalike) <= 0.001);
			{
				lookalikeTarget /= 2;
				lookalikeString = ofToString(lookalike);
				//ofLog() << "lookalike = " << lookalike;
			}
		break;
		case 'r':
			killProcess();
			startProcess();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
