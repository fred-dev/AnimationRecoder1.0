#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0);

	devices = grabber.listDevices();
	startCamera(0);

	// Each run saves into its own folder in bin/data/captures.
	sessionFolder = "captures/" + ofGetTimestampString("%Y-%m-%d_%H-%M-%S");
	ofDirectory::createDirectory(sessionFolder, true, true);
}

//--------------------------------------------------------------
void ofApp::startCamera(int index){
	if (devices.empty()) {
		ofLogError("AnimationRecoder") << "No camera found";
		return;
	}
	deviceIndex = index % devices.size();
	if (grabber.isInitialized()) grabber.close();
	grabber.setDeviceID(devices[deviceIndex].id);
	grabber.setup(CAM_W, CAM_H);
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
	if (mode == MODE_PLAYBACK && !frames.empty()) {
		playbackPosition += playbackFps * ofGetLastFrameTime();
		playbackPosition = fmod(playbackPosition, float(frames.size()));
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofRectangle left = operatorArea();
	ofRectangle right = outputArea();

	switch (mode) {
		case MODE_CAPTURE:
			// Live camera on both screens, with the last frame on top as an onion skin.
			for (auto & area : {left, right}) {
				ofSetColor(255);
				if (grabber.isInitialized()) drawFitted(grabber, grabber.getWidth(), grabber.getHeight(), area);
				if (onionSkin && !frames.empty()) {
					ofEnableAlphaBlending();
					ofSetColor(255, 255 * onionAlpha);
					drawFitted(frames.back(), frames.back().getWidth(), frames.back().getHeight(), area);
					ofSetColor(255);
				}
			}
			break;

		case MODE_CHECK:
			// Thumbnails on the left, the hovered frame full size on the right.
			for (size_t i = 0; i < frames.size() && i < thumbnails.size(); i++) {
				ofSetColor(255);
				frames[i].draw(thumbnails[i]);
				if ((int)i == hovered) {
					ofNoFill();
					ofSetColor(255, 60, 60);
					ofSetLineWidth(3);
					ofDrawRectangle(thumbnails[i]);
					ofFill();
				}
			}
			ofSetColor(255);
			if (hovered >= 0) drawFitted(frames[hovered], frames[hovered].getWidth(), frames[hovered].getHeight(), right);
			break;

		case MODE_PLAYBACK:
			if (!frames.empty()) {
				ofImage & f = frames[int(playbackPosition) % frames.size()];
				ofSetColor(255);
				drawFitted(f, f.getWidth(), f.getHeight(), right);
				drawFitted(f, f.getWidth(), f.getHeight(), ofRectangle(left.x, left.y, left.width, left.height * 0.5f));
			}
			break;
	}

	ofSetColor(255);
	std::string modeName = mode == MODE_CAPTURE ? "CAPTURE" : mode == MODE_CHECK ? "CHECK" : "PLAYBACK";
	ofDrawBitmapStringHighlight(modeName + "   frames: " + ofToString(frames.size())
		+ (mode == MODE_PLAYBACK ? "   fps: " + ofToString(playbackFps, 1) : "")
		+ "   h: help", 20, ofGetHeight() - 20);
	if (showHelp) drawHelp();
}

//--------------------------------------------------------------
void ofApp::drawFitted(const ofBaseDraws & image, float w, float h, const ofRectangle & area){
	if (w <= 0 || h <= 0) return;
	float s = std::min(area.width / w, area.height / h);
	image.draw(area.getCenter().x - w * s / 2, area.getCenter().y - h * s / 2, w * s, h * s);
}

//--------------------------------------------------------------
void ofApp::drawHelp(){
	std::stringstream s;
	s << "AnimationRecoder\n\n"
	  << "1 capture   2 check   3 playback\n"
	  << "space          capture a frame\n"
	  << "backspace      remove the last frame\n"
	  << "o              onion skin on/off (" << (onionSkin ? "on" : "off") << ")\n"
	  << "[ ]            onion skin opacity (" << int(onionAlpha * 100) << "%)\n"
	  << "up / down      thumbnail size (check mode)\n"
	  << "left / right   thumbnail spacing (check mode)\n"
	  << "- / =          playback speed\n"
	  << "click          remove a frame (check mode)\n"
	  << "v              next camera\n"
	  << "f              fullscreen\n"
	  << "h              hide this help\n\n"
	  << "Frames are saved to bin/data/" << sessionFolder << "\n\n"
	  << "Cameras:\n";
	for (size_t i = 0; i < devices.size(); i++) {
		s << ((int)i == deviceIndex ? "> " : "  ") << devices[i].deviceName << "\n";
	}
	ofDrawBitmapStringHighlight(s.str(), 20, 30);
}

//--------------------------------------------------------------
void ofApp::captureFrame(){
	if (!grabber.isInitialized() || !grabber.getPixels().isAllocated()) return;
	ofImage frame;
	frame.setFromPixels(grabber.getPixels());
	savedCount++;
	frame.save(sessionFolder + "/frame_" + ofToString(savedCount, 4, '0') + ".png");
	frames.push_back(std::move(frame));
	layoutThumbnails();
}

//--------------------------------------------------------------
void ofApp::layoutThumbnails(){
	thumbnails.clear();
	ofRectangle area = operatorArea();
	float aspect = float(CAM_H) / CAM_W;
	float x = spacing, y = spacing;
	for (size_t i = 0; i < frames.size(); i++) {
		if (x + thumbWidth > area.getRight()) {
			x = spacing;
			y += thumbWidth * aspect + spacing;
		}
		thumbnails.push_back(ofRectangle(x, y, thumbWidth, thumbWidth * aspect));
		x += thumbWidth + spacing;
	}
}

//--------------------------------------------------------------
int ofApp::thumbnailAt(float x, float y) const {
	for (size_t i = 0; i < thumbnails.size(); i++) {
		if (thumbnails[i].inside(x, y)) return i;
	}
	return -1;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case '1': mode = MODE_CAPTURE; break;
		case '2': mode = MODE_CHECK; layoutThumbnails(); break;
		case '3': mode = MODE_PLAYBACK; playbackPosition = 0; break;
		case ' ': if (mode == MODE_CAPTURE) captureFrame(); break;
		case OF_KEY_BACKSPACE:
			if (!frames.empty()) { frames.pop_back(); layoutThumbnails(); }
			break;
		case 'o': onionSkin = !onionSkin; break;
		case '[': onionAlpha = ofClamp(onionAlpha - 0.1f, 0.1f, 0.9f); break;
		case ']': onionAlpha = ofClamp(onionAlpha + 0.1f, 0.1f, 0.9f); break;
		case OF_KEY_UP: thumbWidth = std::min(thumbWidth + 10, 600.f); layoutThumbnails(); break;
		case OF_KEY_DOWN: thumbWidth = std::max(thumbWidth - 10, 40.f); layoutThumbnails(); break;
		case OF_KEY_RIGHT: spacing = std::min(spacing + 2, 100.f); layoutThumbnails(); break;
		case OF_KEY_LEFT: spacing = std::max(spacing - 2, 0.f); layoutThumbnails(); break;
		case '-': playbackFps = std::max(playbackFps - 1, 1.f); break;
		case '=': playbackFps = std::min(playbackFps + 1, 60.f); break;
		case 'v': startCamera(deviceIndex + 1); break;
		case 'f': ofToggleFullscreen(); layoutThumbnails(); break;
		case 'h': showHelp = !showHelp; break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	hovered = (mode == MODE_CHECK) ? thumbnailAt(x, y) : -1;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (mode != MODE_CHECK) return;
	int i = thumbnailAt(x, y);
	if (i < 0) return;
	// Removes the frame from the animation; the saved PNG stays on disk.
	frames.erase(frames.begin() + i);
	hovered = -1;
	layoutThumbnails();
}
