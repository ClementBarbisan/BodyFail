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
	shader.load("shaders_gl3/bodyIndex.vert", "shaders_gl3/bodyIndex.frag", "shaders_gl3/bodyIndex.geom");
	raytracing.load("shaders_gl3/raytracer.vert", "shaders_gl3/raytracer.frag");
	largeFont.load("backFont.ttf", 12, false, false);
	//largeFont.setLineHeight(14.0f);
	//font.setup("backFont.ttf");
	//font.setSize(12);
	soundStream.setup(this, 2, 0, 44100, 512, 4);
	//pipeline.load("pipeline_data.grt"); //The MLP algorithm directly supports multi-dimensional outputs, so MDRegression is not required here
	buffer = new float[75];
	for (int i = 0; i < 75; i++)
		buffer[i] = 1;
	ofSetFullscreen(true);
	ofHideCursor();
	ofSetColor(255);
	ofBackground(0);
	initialLookalike = lookalike;
	initialLookalikeTarget = lookalikeTarget;
	//framebuffer[0].allocate(1280, 1024);
	//framebuffer[1].allocate(1280, 1024);
	/*framebufferEnd[0].allocate(1280, 1024);
	framebufferEnd[1].allocate(1280, 1024);*/
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	//timeToUpdate += ofGetLastFrameTime();
	if (abs(lookalikeTarget - lookalike) > 0.001)
	{
		if (lookalikeTarget > lookalike)
			lookalike += 0.001;
		else
			lookalike -= 0.001;
	}
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

//--------------------------------------------------------------
void ofApp::draw()
{
	//framebuffer[0].begin();
	stringstream ss;
	if (lookalike <= 0.01)
	{
		ofSetColor(255);
			maxBuffer = 1;
			for (int i = 0; i < 75; i++)
			{
				buffer[i] = sin(TWO_PI);
			}
			for (int i = 0; i < 50; i++)
			{
				if ((i + errorIndex) % 2 == 0)
					ss << "..yrteR" << endl;
				else
					ss << "detpurroc eroC : tluaF noitatnemgeS" << endl;
			}
			errorIndex++;
			if (errorIndex == 50)
				errorIndex = 0;
			ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
			largeFont.drawString(ss.str(), 10, 20);
			//font.drawString(ss.str(), 200, 20);

			//ofDrawBitmapString(ss.str(), 200, 20);
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofRotateY(180);
		ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);
		kinect.getBodySource()->drawProjected(0, 0, ofGetWidth(), ofGetHeight());
		ofPopMatrix();
		/*framebuffer[0].end();
		framebuffer[1].begin();
		raytracing.begin();
		framebuffer[0].draw(0, 0);
		raytracing.end();
		framebuffer[1].end();
		framebuffer[1].draw(0, 0);*/
		
		return;
	}
	//ofBackgroundGradient(ofColor(50), ofColor::black, OF_GRADIENT_BAR);
	//framebufferIndex = framebufferIndex % 2;
	/*ofSetColor(0);
	ofRect(0, 0, ofGetWidth() / 3.5, ofGetHeight());*/
	cam.begin();
	//ofLog() << cam.getPosition();
	cam.setPosition(0.0, 0.0, 0.0);
	ofPushMatrix();
	ofScale(100, 100, 100);
	ofRotateY(180);
	//framebuffer[framebufferIndex].begin();
	shader.begin();
	ofTexture & texture = kinect.getBodyIndexSource()->getTexture();
	//shader.setUniform3f("LightPosition_worldspace", ofVec3f(10.0, 10.0, -10.0));
	shader.setUniform1i("uWidth", kinect.getBodyIndexSource()->getWidth());
	shader.setUniformTexture("uBodyIndexTex", texture, 1);
	shader.setUniformTexture("depthTex", kinect.getDepthSource()->getTexture(), 3);
	shader.setUniform1f("time", ofGetElapsedTimef());
	shader.setUniformTexture("uColorTex", kinect.getColorSource()->getTexture(), 2);
	shader.setUniform1f("lookalike", lookalike);
	ofMesh mesh = kinect.getDepthSource()->getMesh(false, ofxKFW2::Source::Depth::PointCloudOptions::ColorCamera);
	mesh.draw();
	shader.end();
	//framebuffer[framebufferIndex].end();
	//framebuffer[framebufferIndex].draw(0, 0);
	//framebufferEnd[framebufferIndex].begin();
	//raytracing.begin();
	//framebuffer[(framebufferIndex + 1) % 2].draw(0, 0);
	//framebuffer[framebufferIndex].draw(0, 0);
	////framebufferEnd[(framebufferIndex + 1) % 2].draw(0, 0);
	//raytracing.end();
	//framebufferEnd[framebufferIndex].end();
	//framebufferEnd[framebufferIndex].draw(0, 0);
	//ofDisableDepthTest();
	/*raytracing.begin();
	raytracing.setUniform1i("uWidth", kinect.getBodyIndexSource()->getWidth());
	raytracing.setUniformTexture("uBodyIndexTex", texture, 1);
	raytracing.setUniform1f("time", ofGetElapsedTimef());
	mesh.draw();
	raytracing.end();*/
	//framebufferIndex++;
	ofPopMatrix();
	cam.end();
	rotate(coordinates.begin(), coordinates.begin() + 50, coordinates.end());
	//for (int i = 0; i < 25; i++)
	//coordinates[i] = coordinates[i + 25];
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
				stream << joint.first << " " << joint.second.getPosition() << endl;
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
		ss << coordinates[i];
	}
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
	largeFont.drawString(ss.str(), 10, 20);
	//font.draw(ss.str(), 12, 0, 0);
	//ofDrawBitmapString(ss.str(), 200, 20);
	//ofEnableDepthTest();
	/*framebuffer[0].end();
	framebuffer[1].begin();
	raytracing.begin();
	framebuffer[0].draw(0, 0);
	raytracing.end();
	framebuffer[1].end();
	framebuffer[1].draw(0, 0);*/
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{
	float phase = ofRandom(0, TWO_PI);
	for (int i = 0; i < bufferSize; i += 2)
	{
		if (phase > TWO_PI)
			phase -= TWO_PI;
		output[i * nChannels] = buffer[i % 75] / maxBuffer / 2 * sin(phase);
		output[i * nChannels + 1] = buffer[i % 75] / maxBuffer / 2 * sin(phase);
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
