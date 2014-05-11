//
//  Stroke.cpp
//  ThesisPrototype_06
//
//  Created by MennieShen on 14/5/8.
//
//

#include "Stroke.h"

Stroke::Stroke() {
};

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void Stroke::normpreasure() {
    
    float lastPreasure = 0;
    for (int i = 0; i < line.size(); i++) {
        float nmrprs;
        if (i>0) {
            nmrprs = line[i].prs - lastPreasure;
            nmrprs = ofMap(nmrprs, -100, 100, 0.0, 1.0, true);
            lastPreasure =line[i].prs;
            line[i].prs = nmrprs;
        }
        if (i == 0) {
            nmrprs = 0.0;
        }
    }
    avrDeltaPress =0;
    for (int i = 0; i < line.size(); i++) {
        avrDeltaPress += line[i].prs;
    }
    avrDeltaPress = avrDeltaPress/line.size();
    
    topDeltaPress = 0;
    for (int t = 0; t < line.size(); t++) {
        if ( line[t].prs < 0.99) {
            if ( line[t].prs > topDeltaPress ) {
                topDeltaPress = line[t].prs;
                
            }
        }
    }
    
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void Stroke::getLength() {
    
    length = 0;
    
    for (int i =0; i< line.size()-1; i++) {
        float tmplength = ofDist(line[i].pos.x, line[i].pos.y, line[i+1].pos.x, line[i+1].pos.y);
        
        length = length+tmplength;
    }
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void Stroke::getCenter() {
    
    float x = 0;
    float y = 0;
    
    for (int i = 0; i < line.size(); i++) {
        x += line[i].pos.x;
        y += line[i].pos.y;
    }
    
    x = x /line.size();
    y = y /line.size();
    
    center.x = x;
    center.y = y;
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void Stroke::getAngle(float min, float max) {
    
    angle = ofMap(center.x, min, max, 0, 360);
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void Stroke::Debugdraw() {
    
    ofSetColor(0);
    
    for (int i = 0; i < line.size(); i++) {
        
        ofCircle(line[i].pos, 1);
        
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofCircle(center, 3);
        ofPopStyle();
        
    }
    
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void Stroke::drawLength() {
    
    ofSetColor(255);
    ofLine(0, 0, length, 0);
    
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void Stroke::branch(float threshold, float lengthMult, float _maxit, float _dg) {
    
    branchAngle = _dg;
    maxit = _maxit;
    
    ofPushStyle();
    ofSetColor(0);
    ofSetLineWidth(0.1);
    ofNoFill();
    
    ofPushMatrix();
    
    ofRotate(angle);
    
    //do the branch
    //first we normalize the theshold to a value that we can use on any situation
    normThr = ofMap(threshold, 0, 1, 0, topDeltaPress);
    
    //cout << topDeltaPress << endl;
    
    
    //now the branch algorithm
    ofVec2f lastPoint;
    lastPoint.set(0, 0);
    remaining = line.size();
    //cout << remaining << endl;
    
    for (int i =0; i<remaining-1; i++) {
        
        //cout << remaining-1 << endl;
        ofVec2f currentPoint;
        //cout << i << "|" << topDeltaPress << "|" << normThr << "|" << line[i].prs << endl;
        
        
        if ( line[i].prs < 0.99 && line[i].prs > normThr) {
            
            ofPushStyle();
            ofSetColor(0, 0, 255);
            ofSetColor(0);
            nested(lastPoint, 10, lengthMult, line.size()-i, 1);
            ofPopStyle();
            
            ofPushStyle();
            ofSetColor(0);
            ofFill();
            ofCircle (lastPoint, 3);
            ofPopStyle();
            //cout << i << "|" << line[i].prs << "|" << normThr << endl;
            break;
        }
        
        
        float dist = ofDist(line[i].pos.x, line[i].pos.y, line[i+1].pos.x, line[i+1].pos.y);
        dist = dist*lengthMult;
        dist = abs(dist);
        float angleint = ofNoise(((float)i/line.size())*100);
        angleint = ofMap(angleint, 0, 1, 0, PI/4, true);
        currentPoint.x = (cos(angleint))*dist;
        currentPoint.y = (sin(angleint))*dist;
        ofLine(lastPoint, currentPoint+lastPoint );
        //cout << currentPoint.x << endl;
        lastPoint += currentPoint;
        remaining = remaining-1;
        
        
    }
    
    
    //ofLine(40, 0, abs(length)+40, 0);
    
    
    // end the branch
    ofPopMatrix();
    ofPopStyle();
    
    
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void Stroke::nested(ofVec2f current, float _angle, float _lengthMult, float left, float lvl) {
    
    float _lvl = lvl+1;
    float lengthMult = _lengthMult;
    float _left = left;
    
    ofPushMatrix();
    ofTranslate(current);
    ofRotate( _angle );
    
    ofVec2f lastPoint;
    lastPoint.set(0,0);
    //ofLine(0, 0, 20, 0);
    for (int i =_left; i<line.size(); i++) {
        
        
        if ( line[i].prs < 0.99 && line[i].prs > normThr && _lvl < maxit) {
            nested(lastPoint, 10, lengthMult, line.size()-i, _lvl);
            break;
        }
        
        
        ofVec2f currentPoint;
        float dist = ofDist(line[i-1].pos.x,line[i-1].pos.y, line[i].pos.x, line[i].pos.y);
        dist = dist*lengthMult;
        dist = abs(dist);
        dist  *= 0.1;
        
        float angleint = ofNoise(((float)i/line.size())*100);
        angleint = ofMap(angleint, 0, 1, 0, PI/4, true);
        currentPoint.x = (cos(angleint))*dist;
        currentPoint.y = (sin(angleint))*dist;
        ofLine(lastPoint, currentPoint+lastPoint );
        lastPoint += currentPoint;
        
        break;
    }
    
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(current);
    ofRotate( -(_angle) );
    
    ofVec2f lastPointb;
    lastPointb.set(0,0);
    // ofLine(0, 0, 20, 0);
    for (int i =_left; i<line.size(); i++) {
        if ( line[i].prs < 0.99 && line[i].prs > normThr && _lvl < maxit) {
            nested(lastPoint, 10,  lengthMult, line.size()-i, _lvl);
            break;
        }
        ofVec2f currentPoint;
        float dist = ofDist(line[i-1].pos.x,line[i-1].pos.y, line[i].pos.x, line[i].pos.y);
        dist = dist*lengthMult;
        dist = abs(dist);
        dist  *= 0.1;
        float angleint = ofNoise(((float)i/line.size())*100);
        angleint = ofMap(angleint, 0, 1, 0, PI/4, true);
        currentPoint.x = (cos(angleint))*dist;
        currentPoint.y = (sin(angleint))*dist;
        ofLine(lastPointb, currentPoint+lastPointb );
        lastPointb += currentPoint;
        
        
        break;
    }
    
    
    
    ofPopMatrix();
    
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void Stroke::drawWavyLine(ofVec2f a, ofVec2f b) {
    
	ofVec2f diff = (b-a);
	float length = diff.length();
	
	
	// now calculate the normal, normalized
	ofVec2f n = diff/length;
	
	// turn (x, y) into (y, -x) - this flips the vector 90 degrees
	float ny = n.y;
	n.y = -n.x;
	n.x = ny;
	
    
	glBegin(GL_LINE_STRIP);
    
    
	for(float f = 0; f < PI*2; f+=PI/5.f) {
		float d = f/TWO_PI;
		
		float window = 1-4*(d-0.5)*(d-0.5);
		ofVec2f sine = n * sin(f+1000)*ofMap(length, 0, 200, 0, 8)*window;
		ofVec2f p = sine + a + diff * d;
        
		glVertex2f(p.x, p.y);
	}
	glVertex2f(b.x, b.y);
	
	glEnd();
}


