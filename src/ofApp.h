#pragma once

#include "ofMain.h"

// Stop-motion capture for two screens side by side: the left half of the
// window is the operator view, the right half is the output.
class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;
	void mouseMoved(int x, int y) override;
	void mousePressed(int x, int y, int button) override;

private:
	enum Mode { MODE_CAPTURE = 1, MODE_CHECK = 2, MODE_PLAYBACK = 3 };

	void startCamera(int deviceID);
	void captureFrame();
	void layoutThumbnails();
	int thumbnailAt(float x, float y) const;
	void drawFitted(const ofBaseDraws & image, float w, float h, const ofRectangle & area);
	void drawHelp();

	ofRectangle operatorArea() const { return ofRectangle(0, 0, ofGetWidth() / 2, ofGetHeight()); }
	ofRectangle outputArea() const { return ofRectangle(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight()); }

	ofVideoGrabber grabber;
	std::vector<ofVideoDevice> devices;
	int deviceIndex = 0;
	const int CAM_W = 1920;
	const int CAM_H = 1080;

	std::vector<ofImage> frames;
	std::vector<ofRectangle> thumbnails;
	std::string sessionFolder;
	int savedCount = 0;

	Mode mode = MODE_CAPTURE;
	bool onionSkin = true;
	float onionAlpha = 0.5f;
	bool showHelp = true;
	int hovered = -1;

	float thumbWidth = 160;
	float spacing = 12;
	float playbackFps = 12;
	float playbackPosition = 0;
};
