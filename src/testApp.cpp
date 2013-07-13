#include "testApp.h"

void testApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(25);
	//obj.set(300, 50, 100, 200);
    camWidth=1920;
    camHeight=1080;
    myGrabber.listDevices();
    myGrabber.setDeviceID(1);
    myGrabber.initGrabber(camWidth, camHeight);
    imageCounter=0;
    xPos=0;
    yPos=0;
    objectWidth=myGrabber.getWidth()/9;
    objectHeight=myGrabber.getHeight()/9;
    spacing=50;
    
    drawRatio = float(ofGetScreenHeight())/camHeight;
    tempPixels.allocate(camWidth, camHeight, 4);
    tempImage.allocate(camWidth, camHeight, OF_IMAGE_COLOR_ALPHA);
    pageNumber=0;
    showHelp=false;
    playMode=false;
    frameCounter=0;
    modeCounter=capture;
    speedCounter=20;
    
    
}


void testApp::update() {
    myGrabber.update();
    
//    switch (playMode) {
//        case capture:
//            <#statements#>
//            break;
//        case check:
//            <#statements#>
//            break;
//        case playback:
//            <#statements#>
//            break;
//
//    }
	

    
    
}

void testApp::draw() {
    switch (modeCounter) {
        case capture:
            ofBackground(0, 0, 0);
            ofPushStyle();
            if (objects.size()!=0) {
                
                ofEnableAlphaBlending();
                ofSetColor(255,255,255,127);
                ofRect(0,0,1920,1080);
                objects[objects.size()-1]->objectImage.draw(0,0);
                objects[objects.size()-1]->objectImage.draw(1920,0);
                //ofDisableAlphaBlending();
            }
            myGrabber.draw(0,0);
            myGrabber.draw(1920,0);
            ofPopStyle();
            break;
        case check:
            for (int i =0; i<objects.size(); i++) {
                objects[i]->draw();
                if (objects[i]->isMouseOver()==true) {
                    objects[i]->objectImage.draw(1920, 0,1920, 1080);
                    if (objects[i]->isMousePressed()) {
                        objects[i]->enabled=false;
                        objects.erase(objects.begin()+i);
                        xPos=0;
                        yPos=0;
                        imageCounter=0;
                        for (int i=0; i<objects.size(); i++) {
                            xPos=imageCounter*objectWidth+(imageCounter*spacing);
                            if (1920-xPos<objectWidth) {
                                if (1080-yPos<objectHeight) {
                                    pageNumberDraw++;
                                }
                                xPos=0;
                                imageCounter=0;
                                yPos+=objectHeight+spacing;
                            }
                            objects[i]->set(xPos, yPos, objectWidth, objectHeight);
                            objects[i]->pageNumber=pageNumberDraw;
                            imageCounter++;
                        }
                    }
                }
            }
            
            break;
        case playback:
            if (objects.size()!=0) {
                if (frameCounter==objects.size()) {
                    frameCounter=0;
                }
                objects[frameCounter]->objectImage.draw(1920,0, 1920, 1080);
                if (speedRegulator>100) {
                    speedRegulator=0;
                    frameCounter++;
                }
                speedRegulator+=speedCounter;
                
            }
           
            break;
        }
    if (showHelp) {
        ofPushStyle();
        ofSetColor(255, 255, 255);
        ofDrawBitmapString(ofToString(objects.size()), 10,10);
        int startY;
        startY=80;
        ofDrawBitmapString("Key Commands", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press space bar to save your image", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'c' to clear the screen", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'p' to toggle play record mode", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'q' for fullscreen mode", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'f' to toggle fade mode: Fading is "+fadeString, 20,startY);
        startY+=20;
        ofDrawBitmapString("In playback mode keys 1 - 0 show recorded images", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'h' to show and hide these instructions", 20,startY);
        startY+=20;
        ofDrawBitmapString("Press 'v' to change video input device - this will only work when you can see this message", 20,startY);
        startY+=50;
        ofDrawBitmapString("VIDEO DEVICE", 20, startY);
        startY+=20;
        for(int i = 0; i < videoDevices.size(); i++){
            if(i == myGrabber.getVideoDeviceID()){
                ofSetColor(255, 100, 100);
            }
            else{
                ofSetColor(255);
            }
            ofDrawBitmapString(videoDevices[i], 20, startY+i*20);
        }
        ofPopStyle();
    }
    
}

void testApp::reDrawObjects(){

        xPos=0;
        yPos=0;
        imageCounter=0;
        for (int i=0; i<objects.size(); i++) {
            xPos=imageCounter*objectWidth+(imageCounter*spacing);
            if (1920-xPos<objectWidth) {
                if (1080-yPos<objectHeight) {
                    pageNumberDraw++;
                }
                xPos=0;
                imageCounter=0;
                yPos+=objectHeight+spacing;
            }
            objects[i]->set(xPos, yPos, objectWidth, objectHeight);
            objects[i]->pageNumber=pageNumberDraw;
            imageCounter++;
        }
        
        
        
}
void testApp::keyPressed(int key){
    if (key == 'f' || key == 'F'){
		gFading=!gFading;
	}
	if (key == 'h' || key == 'H'){
		showHelp=!showHelp;
	}
    if (key == 'q'){
		ofToggleFullscreen();
    }
    
    if (key=='1') {
        modeCounter=capture;
        for (int i=0; i<objects.size(); i++) {
            objects[i]->enabled=false;
        }
    }
    if (key=='2') {
        for (int i=0; i<objects.size(); i++) {
            objects[i]->enabled=true;
        }
        modeCounter=check;
    }
    if (key=='3') {
        modeCounter=playback;
        for (int i=0; i<objects.size(); i++) {
            objects[i]->enabled=false;
        }
    }

    if (key==OF_KEY_RIGHT) {
        spacing+=1;
        reDrawObjects();
    }
    if (key==OF_KEY_LEFT) {
        spacing-=1;
        reDrawObjects();
    }
    
    if (key==OF_KEY_UP) {
        objectHeight+=1;
        objectWidth+=1;
        reDrawObjects();
    }
    if (key==OF_KEY_DOWN) {
        objectHeight-=1;
        objectWidth-=1;
        reDrawObjects();
    }
    if(key == 'v'){
        if (showHelp &&!playMode){
            myGrabber.setVideoDeviceID( (myGrabber.getVideoDeviceID()+1) % videoDevices.size() );
        }
    }
	if (key == ' '){
        saveImage();
        }
    if (key=='s') {
        speedCounter-=1;
    }
    if (key=='S') {
        speedCounter+=1;
    }
	
}
    void testApp::saveImage(){
        MyTestObject *object=new MyTestObject;
        xPos=imageCounter*objectWidth+(imageCounter*spacing);
        if (1920-xPos<objectWidth) {
            if (1080-yPos<objectHeight) {
                pageNumber++;
            }
            xPos=0;
            imageCounter=0;
            yPos+=objectHeight+spacing;
        }
        
       
        object->setup();
        object->set(xPos, yPos, objectWidth, objectHeight);
        
        
        tempImage.setFromPixels(myGrabber.getPixelsRef());
        string tempName = ofToString(ofGetTimestampString())+".png";
        tempImage.saveImage(tempName);
        object->pageNumber=pageNumber;
        object->loadImage(tempName);
        object->enabled=false;
        objects.push_back(object);
 
        imageCounter+=1;
    }
    





