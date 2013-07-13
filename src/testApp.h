#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#include "MyTestObject.h"
#include "ofxQTKitVideoGrabber.h"
#define capture 1
#define check 2
#define playback 3

class testApp : public ofBaseApp {
	
public:
	

	void setup();
    void update();
    void draw();
     void keyPressed(int key);
    vector<MyTestObject*> objects;
    ofxQTKitVideoGrabber myGrabber;
    int imageCounter, xPos, yPos,objectWidth, objectHeight, spacing;
    
    void saveImage();
    void reDrawObjects();
    ofImage zaveImage;
    
    string fadeString;
    //ofxQTKitVideoGrabber 		myGrabber;
    unsigned char * 	videoInverted;
    unsigned char * 	videoCumul;
	bool gCulumate;
    ofTexture			videoTexture;
    int 				camWidth;
    int 				camHeight;
    float					drawRatio;
	int gFadeCounter;
	bool gFading;
	bool gRedCap, gGreenCap, gBlueCap;
    int savedNum;
	int picCounter;
	bool playMode;
    bool loadingImages;
	string picName;
	vector<string> videoDevices;
    bool showHelp;
    
    ofPixels tempPixels;
    ofImage tempImage;
    int pageNumber, pageNumberDraw, pageInView, blendAmount, modeCounter, frameCounter, speedRegulator, speedCounter;
    

};

#endif

