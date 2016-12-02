//
//  MH871.cpp
//  xmlSettingsExample
//
//  Created by dantheman on 2/9/16.
//
//

#include "ofxMH871.h"
void MH871::setup(string serialPort){
    device.setup(serialPort, 19200);
    initPlotter();
    penUp();
    timer.setFramerate(30);
}
void MH871::update(){
    if(timer.tick()){
        if(ptsCache.size() > 0){
            currentPoint = ptsCache.front();
            ptsCache.pop_front();
//            cout<<currentPoint<<endl;
        }
        
        if(cache.size() > 0 && device.isInitialized()){
            sendCommand(cache.front());
            cache.pop_front();
        }else{
            if(cache.size() > 0 && !device.isInitialized()){
                cache.clear();
                ofLog(OF_LOG_ERROR)<<"cache cleared no serial device"<<endl;
            }
        }
    }
}

void MH871::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofNoFill();
    ofSetColor(255, 255, 0);
    ofDrawRectangle(0, 0, drawingSize.x, drawingSize.y);
    ofSetColor(255, 0, 255);
    ofDrawCircle(currentPoint, 10);
    ofPopStyle();
    ofPopMatrix();
}

void MH871::setPrintSize(float width, float height){
    printSize.x = width;
    printSize.y = height;
}
void MH871::setPrintSize(ofVec2f size){
    setPrintSize(size.x, size.y);
}

void MH871::setDrawingSize(float width, float height){
    drawingSize.x = width;
    drawingSize.y = height;
}

void MH871::setDrawingSize(ofVec2f size){
    setDrawingSize(size.x, size.y);
}

void MH871::addPoint(ofPoint pt){
    ofVec2f fooP = mapPoint(pt);
    int x = (int)fooP.x;
    int y = (int)fooP.y;
    ptsCache.push_back(pt);
    cache.push_back("PA"+ofToString(x)+","+ofToString(y)+";");
}
void MH871::addPoint(ofVec2f pt){
    ofVec2f fooP = mapPoint(pt);
    int x = (int)fooP.x;
    int y = (int)fooP.y;
    ptsCache.push_back(pt);
    cache.push_back("PA"+ofToString(x)+","+ofToString(y)+";");
}
void MH871::plotPolyline(ofPolyline line){
    line = line.getResampledByCount(line.getVertices().size()*2);
    vector<ofPoint> verts = line.getVertices();
    for(float i = 0; i <= 1.0; i+=0.01){
        if(i == 0){
            startPlot(line.getPointAtIndexInterpolated(line.getIndexAtPercent(i)));
        }else{
            addPoint(line.getPointAtIndexInterpolated(line.getIndexAtPercent(i)));
        }
    }
    endPlot();
}
void MH871::plotPolylines(vector<ofPolyline> lines){
    for(int j = 0; j < lines.size(); j++){
        lines[j] = lines[j].getResampledByCount(lines[j].getVertices().size()*10);
        lines[j] = lines[j].getResampledBySpacing(0.01);
        ofPoint firstPt;
        float length = lines[j].getLengthAtIndex(lines[j].size()-1);
        for(float i = 0; i <= length; i+=1.0){
            if(i == 0){
                firstPt = lines[j].getPointAtIndexInterpolated(lines[j].getIndexAtPercent(i/length));
                startPlot(firstPt);
            }else{
                addPoint(lines[j].getPointAtIndexInterpolated(lines[j].getIndexAtPercent(i/length)));
            }
        }
        addPoint(firstPt);
        ptsCache.push_back(firstPt);
        cache.push_back("PU;");
    }
    endPlot();
}
void MH871::endPlot(){
    cache.push_back("PU;");
    cache.push_back("PA0,0;");
    ptsCache.push_back(ofVec2f(0, 0));
    ptsCache.push_back(ofVec2f(0, 0));
    
}
void MH871::startPlot(ofPoint pt){
    ofVec2f fooP = mapPoint(pt);
    int x = (int)fooP.x;
    int y = (int)fooP.y;
    ptsCache.push_back(pt);
    ptsCache.push_back(pt);
    cache.push_back("PA"+ofToString(x)+","+ofToString(y)+";");
    cache.push_back("PD;");
}
void MH871::startPlot(ofVec2f pt){
    ofVec2f fooP = mapPoint(pt);
    int x = (int)fooP.x;
    int y = (int)fooP.y;
    ptsCache.push_back(pt);
    ptsCache.push_back(pt);
    cache.push_back("PA"+ofToString(x)+","+ofToString(y)+";");
    cache.push_back("PD;");
}

void MH871::initPlotter(){
    sendCommand("IN;");
    endPlot();
}

void MH871::penDown(){
    sendCommand("PD;");
}

void MH871::penUp(){
    sendCommand("PU;");
}

ofVec2f MH871::mapPoint(ofPoint pt){
    return ofVec2f(ofMap(pt.x, 0, drawingSize.x, printSize.x, 0, true), ofMap(pt.y, 0, drawingSize.y, 0, printSize.y, true));
}

ofVec2f MH871::mapPoint(ofVec2f pt){
    return ofVec2f(ofMap(pt.x, 0, drawingSize.x, printSize.x, 0, true), ofMap(pt.y, 0, drawingSize.y, printSize.y, 0, true));
}

void MH871::sendCommand(string command){
    ofBuffer buf;
    buf.set(command);
    device.writeBytes(buf);
    //    device.flush();
}
