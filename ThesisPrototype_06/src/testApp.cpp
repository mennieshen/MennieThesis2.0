#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetCircleResolution(100);
    ofBackground(255);

    

    /*
    //---Get Latest Modified File and Send it to Data folder
    ofDirectory dir("/Volumes/Inkling/My Sketches");
    dir.listDir();
    dir.sort();
    vector<ofFile> files = dir.getFiles();
    //cout << files.back().getFileName() << endl;
    string getLatestFileName = files.back().getFileName();
    sendInkFile.copyFromTo("/Volumes/Inkling/My Sketches/" + getLatestFileName, "/Users/mennieshen/Documents/of_v0.8.0_osx_release/MennieThesis/ThesisPrototype_06/bin/data", true);
    */
    

    
    // load a new paper ink converter with thersholds set to 0:
    //Mypaper = new PaperInkConverter(ofToDataPath(getLatestFileName).c_str(), 0,0,0);
    Mypaper = new PaperInkConverter(ofToDataPath("letter01.WPI").c_str(), 0,0,0);
    

    
    int count;
    while ( (count = Mypaper->GetNumberOfPoints()) != -1){
        
        // create a temporary Stroke object:
        Stroke TmpStroke;
        
        short * xs = Mypaper->GetPenPositionX();
        short * ys = Mypaper->GetPenPositionY();
        short * pressure = Mypaper->GetPenPressure();
        // unsigned char * pentiltx = Mypaper->GetPenTiltX();
        // unsigned char * pentilty = Mypaper->GetPenTiltY();
        
        float lastPrs =0;
        for (int i = 0; i < count; i++){
            
            dot tmpDot;
            tmpDot.pos = ofVec2f( xs[i] * 0.05, ys[i] * 0.05);
            float currentPrs = pressure[i];
            tmpDot.prs = currentPrs;
            // tmpDot.angl = ofVec2f( pentiltx[i], pentilty[i]);
            TmpStroke.line.push_back(tmpDot);
            
        }
        
        lines.push_back(TmpStroke);
        Mypaper->ReadNextStroke();
        
    }
    
    
    for (int i = 0; i < lines.size(); i++){
        
        lines[i].normpreasure();
        lines[i].getLength();
        lines[i].getCenter();
        
    }
    
    //---Scale the template
    float originX = lines[0].center.x;
    for (int i = 1; i < lines.size(); i++){
        if ( lines[i].center.x < originX ){
            originX = lines[i].center.x;
        }
        
    }
    float originY = lines[0].center.y;
    for (int i = 1; i < lines.size(); i++){
        if ( lines[i].center.y < originY ){
            originY = lines[i].center.y;
        }
        
    }
    origin.x = originX;
    origin.y = originY;
    
    float endX = lines[0].center.x;
    for (int i = 1; i < lines.size(); i++){
        if ( lines[i].center.x > endX ){
            endX = lines[i].center.x;
        }
        
    }
    float endY = lines[0].center.y;
    for (int i = 1; i < lines.size(); i++){
        if ( lines[i].center.y > endY ){
            endY = lines[i].center.y;
        }
        
    }
    final.x = endX;
    final.y = endY;
    
    immensity.x = final.x - origin.x;
    immensity.y = final.y - origin.y;
    
    for (int i = 0; i < lines.size(); i++){
        
        lines[i].getAngle(origin.x, final.x);
    }
    
    // Find the longest stroke ever
    biggestStroke = lines[0].length;
    for (int i = 1; i < lines.size(); i++){
        if ( lines[i].length > biggestStroke ){
            biggestStroke = lines[i].length;
        }
        
    }
    
    /*
    //---Print!
    printer.listPrinters();
    printer.setPrinterName("HP_Photosmart_D110_series");
    printer.setJobTitle("Lettergraphy");
    

    //---Eject USB
    system("hdiutil eject -force /Volumes/Inkling");
     */
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    triangulation.reset();
    printer.updatePrinterInfo();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth()/2, 0);
    //ofNoFill();
    ofSetColor(0);
    
    for ( int i = 0; i < lines.size(); i++){
        
//        ofPushStyle();
//        ofSetColor(255, 0, 0);
//        ofCircle(lines[i].center, 3);
//        ofPopStyle();
//        
//        //lines[i].Debugdraw();
//        
//        ofPushStyle();
//        ofSetColor(0, 255, 0);
//        lines[i].firstStrokePoint();
//        ofPopStyle();
        
        ofVec2f triPoint = lines[i].center;
        triangulation.addPoint(triPoint);
        
        ofVec2f a = lines[i-2].center;
        ofVec2f b = lines[i-1].center;
        ofVec2f c = lines[i].center;
        
        if (a.x - b.x > ofGetWindowWidth()/4 || b.x - c.x > ofGetWindowWidth()/4 || c.x - a.x > ofGetWindowWidth()/4){
        
            a.set(0,0);
            b.set(0,0);
            c.set(0,0);
        
        }
        else{
        lines[i].drawWavyLine(a, b);
        lines[i].drawWavyLine(b, c);
        lines[i].drawWavyLine(c, a);
        }
        
        
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(0, ofGetWindowHeight()/2);
        //ofSetColor(0, 0, 255);
        lines[i].branch(0.7, 50, 8);

        ofPopStyle();
        ofPopMatrix();
        
        
        
    }
    
    
    triangulation.triangulate();
    ofPushStyle();
    ofNoFill();
    triangulation.draw();
    ofPopStyle();
    ofPopMatrix();
    

    saveImg.grabScreen(0,0,ofGetWindowWidth(), ofGetWindowHeight());
    saveImg.saveImage("Lettergraphy-"+ofToString(snapCounter)+".png");

    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    

    if ( key == 'p'){
        
           printer.printImage("Lettergraphy-0.png");
    }
    
    
}

//---------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
