#include "ofApp.h"

//--------------------------------------------------------------
//void ofApp::setupGui()
//{
//	ofHideCursor();
//	ofSetFullscreen(true);
//}

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();
	shader.load("bodyIndex.vert", "bodyIndex.frag", "bodyIndex.geom");
	raytracing.load("raytracer.vert", "raytracer.frag", "raytracer.geom");
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
	soundStream.setup(this, 2, 0, 44100, 512, 4);
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
	oscMessage.addFloatArg(globalWeight);
	oscMessage.addFloatArg(globalWeight - 0.3);
	oscMessage.addFloatArg(maximumValue);
	oscSender.sendMessage(oscMessage);
	//pipeline.load("pipeline_data.grt"); //The MLP algorithm directly supports multi-dimensional outputs, so MDRegression is not required here
	buffer = new float[75];
	for (int i = 0; i < 75; i++)
		buffer[i] = 1;
	//ofSetFullscreen(true);
	ofHideCursor();
	ofSetColor(255);
	ofBackground(0);
	initialLookalike = lookalike;
	initialLookalikeTarget = lookalikeTarget;
	framebuffer.allocate(1400, 1050);
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
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
	while (oscReceiver.hasWaitingMessages())
	{
		oscReceiver.getNextMessage(oscMessage);
		if (lookalike < 0.01)
		{
			if (oscMessage.getAddress() == "/progress_speed")
			{
				if (oscMessage.getArgAsFloat(0) < 0.05)
				{
					oscMessage.clear();
					oscMessage.setAddress("/reset");
					oscMessage.addIntArg(1);
					oscSender.sendMessage(oscMessage);
				}
					
			}
		}
		if (oscMessage.getAddress() == "/progress")
		{
			lookalike = 1.0 - oscMessage.getArgAsFloat(0);
			break;
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
		oscMessage.addFloatArg(speedWeight * 2);
		oscMessage.addFloatArg(positionWeight * 2);
		oscMessage.addFloatArg(globalWeight);
		oscMessage.addFloatArg(globalWeight / 2);
		oscMessage.addFloatArg(globalWeight / 4);
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
	//lookalike += progression_speed / (10 / progression_speed);
	//if (kinect.isFrameNew() && timeToUpdate >= 0.25f && pipeline.getTrained())
	//{
	//	timeToUpdate = 0;
	//	if (predictVector.size() >= 375)
	//	{
	//		std::rotate(predictVector.begin(), predictVector.begin() + 75, predictVector.end());
	//		for (int i = 0; i < 75; i++)
	//			predictVector.pop_back();
	//	}
	//	auto bodies = kinect.getBodySource()->getBodies();
	//	for (auto body : bodies) {
	//		for (auto joint : body.joints) {
	//			predictVector.push_back(joint.second.getPosition().x);
	//			predictVector.push_back(joint.second.getPosition().y);
	//			predictVector.push_back(joint.second.getPosition().z);
	//			//ofLog() << joint.first << " " << joint.second.getPosition();
	//			//now do something with the joints
	//		}
	//	}
	//	if (predictVector.size() >= 375)
	//	{
	//		pipeline.predict(predictVector);
	//	}
	//}
}

//-------------------------------------------------------------
//void ofApp::drawGui(ofEventArgs &args)
//{
//	
//	
//}
int clip(int n, int lower, int upper) {
	return std::max(lower, std::min(n, upper));
}
//--------------------------------------------------------------
void ofApp::draw()
{
	//framebuffer[0].begin();
	stringstream ss;
	if (lookalike <= 0.01)
	{
		if (!savedPosture)
		{
			auto bodies = kinect.getBodySource()->getBodies();
			int index = 0;
			for (auto body : bodies) {
				if (body.joints.size() == 25)
				{
					savedPosture = true;
					stringstream url;
					url << "http://127.0.0.1:8080/addSample?";
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
					ofLoadURL(url.str());
				}
			}
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
		for (int i = 0; i < 75; i++)
		{
			if ((i + errorIndex) % 2 == 0)
				//font.drawFormatted("..yrteR", 200, 10 * i + 10);
				trueTypeFont.drawString("..yrteR", 200, 10 * i + 10);
			else
				//font.drawFormatted("detpurroc eroC : tluaF noitatnemgeS", 200, 10 * i + 10);
				trueTypeFont.drawString("detpurroc eroC : tluaF noitatnemgeS", 200, 10 * i + 10);
		}
		errorIndex++;
		if (errorIndex == 50)
			errorIndex = 0;
		return;
	}
	savedPosture = false;
	//ofDisableDepthTest();
	//ofBackgroundGradient(ofColor(50), ofColor::black, OF_GRADIENT_BAR);
	framebuffer.begin();	
	cam.begin();
	cam.setPosition(0.0, 0.0, 0.0);
	ofPushMatrix();
	ofScale(100, 100, 100);
	ofRotateY(180);
	ofMesh mesh = kinect.getDepthSource()->getMesh(false, ofxKFW2::Source::Depth::PointCloudOptions::ColorCamera);
	ofMesh meshWireframe = kinect.getDepthSource()->getMesh(true, ofxKFW2::Source::Depth::PointCloudOptions::ColorCamera);
	ofTexture & texture = kinect.getBodyIndexSource()->getTexture();
	ofTexture & colorTexture = kinect.getColorSource()->getTexture();
	ofTexture & depthTexture = kinect.getDepthSource()->getTexture();
	shader.begin();
	shader.setUniform1i("uWidth", kinect.getBodyIndexSource()->getWidth());
	shader.setUniformTexture("uBodyIndexTex", texture, 1);
	shader.setUniformTexture("depthTex", depthTexture, 3);
	shader.setUniform1f("time", ofGetElapsedTimef());
	shader.setUniformTexture("uColorTex", colorTexture, 2);
	shader.setUniform1f("lookalike", lookalike);
	mesh.draw();
	vector<size_t> & indices = meshWireframe.getIndices();
	//shuffle(meshWireframe.getIndices().begin(), meshWireframe.getIndices().end(), rng);
	for (int i = 0; i < indices.size(); i += 4 + 100 * lookalike)
	{
		int random = rand() % indices.size();
		swap(indices[i], indices[random]);
	}
	meshWireframe.setMode(OF_PRIMITIVE_LINES);
	shader.end();
	raytracing.begin();

	raytracing.setUniform1i("uWidth", kinect.getBodyIndexSource()->getWidth());
	raytracing.setUniformTexture("uBodyIndexTex", texture, 1);
	raytracing.setUniform1f("time", ofGetElapsedTimef());
	raytracing.setUniformTexture("uColorTex", colorTexture, 2);
	raytracing.setUniformTexture("depthTex", depthTexture, 3);
	raytracing.setUniform1f("lookalike", lookalike);
	meshWireframe.drawWireframe();
	raytracing.end();
	
	ofPopMatrix();
	cam.end();
	framebuffer.end();
	framebuffer.draw(0, 0);
	framebuffer.begin();
	ofFill();
	ofSetColor(0, 0, 0, 20 + 60 * lookalike);
	ofDrawRectangle(0, 0, 1400, 1050);
	framebuffer.end();
	rotate(coordinates.begin(), coordinates.begin() + 50, coordinates.end());
	maxBuffer = 0;
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
					coordinates[49 + index] = stream.str();
					reverse(coordinates[49 + index].begin(), coordinates[49 + index].end());
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
	for (int i = 0; i < 75; i++)
	{
		//font.drawFormatted(coordinates[i], 200, 10 * i + 10);
		trueTypeFont.drawString(coordinates[i], 200, 10 * i + 10);
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
		output[i * nChannels] = buffer[i % 75] / maxBuffer * sin(phase);
		output[i * nChannels + 1] = buffer[i % 75] / maxBuffer * sin(phase);
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
			lookalike = initialLookalike;
			lookalikeTarget = initialLookalikeTarget;
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
