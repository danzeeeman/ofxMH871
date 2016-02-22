//
//  MH871.hpp
//  xmlSettingsExample
//
//  Created by dantheman on 2/9/16.
//
//
#pragma once
#include "ofMain.h"
#include "ofxTiming.h"
class MH871{
public:
    void setup(float min = 0, float max = 20000);
    void setPrintSize(ofVec2f size);
    void setPrintSize(float width, float height);
    void setDrawingSize(ofVec2f size);
    void setDrawingSize(float width, float height);
    void update();
    void addPoint(ofPoint pt);
    void addPoint(ofVec2f pt);
    void addPolyline(ofPolyline line);
    void addPolylines(vector<ofPolyline> lines);
private:
    void penDown();
    void penUp();
    void initPlotter();
    void startPlot(ofPoint pt);
    void startPlot(ofVec2f pt);
    void endPlot();
    ofPolyline polyline;
    deque<string> cache;
    void sendCommand(string command);
    ofVec2f mapPoint(ofVec2f pt);
    ofVec2f mapPoint(ofPoint pt);
    ofSerial device;
    ofVec2f printSize;
    ofVec2f drawingSize;
    float min, max;
    float maxX, maxY;
    DelayTimer timer;
};

