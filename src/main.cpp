#include "ofMain.h"
#include "ofApp.h"

int main(){
	// Spans two 1920x1080 screens side by side: operator on the left, output on the right.
	// Press f to go fullscreen across both.
	ofGLFWWindowSettings settings;
	settings.setSize(1920, 540);
	settings.windowMode = OF_WINDOW;
	settings.multiMonitorFullScreen = true;
	auto window = ofCreateWindow(settings);
	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
