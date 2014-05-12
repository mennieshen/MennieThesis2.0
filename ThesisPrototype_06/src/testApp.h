#pragma once

#include "ofMain.h"
#include "PaperInkConverter.h"
#include "Stroke.h"
#include "Point.h"
#include "ofxDelaunay.h"
#include "ofxCUPS.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    PaperInkConverter * Mypaper;
    
    
    vector < Stroke > lines;
    
    
    
    ofVec2f origin;
    ofVec2f final;
    ofVec2f immensity;
    ofVec2f triPoint;
    
    float biggestStroke;
    
    float th;
    float it;
    float dg;
    float multp;
    
    ofxDelaunay triangulation;
    
    // Save Img
    ofImage saveImg;
    int snapCounter;
    
    ofFile sendInkFile;
    ofxCUPS printer;
    
    

    
    
    
    
    
    
    
};
