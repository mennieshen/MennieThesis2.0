//
//  Stroke.h
//  ThesisPrototype_06
//
//  Created by MennieShen on 14/5/8.
//
//

#pragma once
#include "ofMain.h"
#include "Point.h"

class Stroke{
    
public:
    Stroke();
    
    void normpreasure();
    void getLength();
    void getCenter();
    void getAngle(float min, float max);
    void Debugdraw();
    void drawLength();
    void branch(float threshold, float lengthMult, float _maxit);
    void nested(ofVec2f current, float angle , float lengthMult, float left, float lvl);
    void drawWavyLine(ofVec2f a, ofVec2f b);
    void firstStrokePoint();
    
    
    float maxit;
    float length;
    ofVec2f center;
    float avrDeltaPress;
    float topDeltaPress;
    float normThr;
    float angle;
    float branchAngle;
    float remaining;
    
    float circleX;
    float circleY;
    
    vector <dot> line;
    
    
    
    
    
};