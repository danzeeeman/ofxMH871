#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255,255,255);
    ofDisableAntiAliasing();
    
    //-----------
    //the string is printed at the top of the app
    //to give the user some feedback
    message = "loading mySettings.xml";
    
    //we load our settings file
    //if it doesn't exist we can still make one
    //by hitting the 's' key
    if( XML.loadFile("mySettings.xml") ){
        message = "mySettings.xml loaded!";
    }else{
        message = "unable to load mySettings.xml check data/ folder";
    }
    
    //read the colors from XML
    //if the settings file doesn't exist we assigns default values (170, 190, 240)
    red		= XML.getValue("BACKGROUND:COLOR:RED", 170);
    green	= XML.getValue("BACKGROUND:COLOR:GREEN", 190);
    blue	= XML.getValue("BACKGROUND:COLOR:BLUE", 240);
    
    /*
     "BACKGROUND:COLOR:RED" referes to a structure like this:
     
     <BACKGROUND>
     <COLOR>
     <RED>101.103516</RED>
     </COLOR>
     </BACKGROUND>
     */
    
    //we initalize some of our variables
    lastTagNumber	= 0;
    pointCount		= 0;
    lineCount		= 0;
    
    
    //-------
    //this is a more advanced use of ofXMLSettings
    //we are going to be reading multiple tags with the same name
    
    //lets see how many <STROKE> </STROKE> tags there are in the xml file
    int numDragTags = XML.getNumTags("STROKE:PT");
    
    //if there is at least one <STROKE> tag we can read the list of points
    //and then try and draw it as a line on the screen
    if(numDragTags > 0){
        
        //we push into the last STROKE tag
        //this temporarirly treats the tag as
        //the document root.
        XML.pushTag("STROKE", numDragTags-1);
        
        //we see how many points we have stored in <PT> tags
        int numPtTags = XML.getNumTags("PT");
        
        if(numPtTags > 0){
            
            //We then read those x y values into our
            //array - so that we can then draw the points as
            //a line on the screen
            
            //we have only allocated a certan amount of space for our array
            //so we don't want to read more than that amount of points
            int totalToRead = MIN(numPtTags, NUM_PTS);
            
            for(int i = 0; i < totalToRead; i++){
                //the last argument of getValue can be used to specify
                //which tag out of multiple tags you are refering to.
                int x = XML.getValue("PT:X", 0, i);
                int y = XML.getValue("PT:Y", 0, i);
                //					dragPts[i].set(x, y);
                line.addVertex(x, y);
                pointCount++;
            }
        }
        
        //this pops us out of the STROKE tag
        //sets the root back to the xml document
        XML.popTag();
    }
    
    //load a monospaced font
    //which we will use to show part of the xml structure
    TTF.load("mono.ttf", 7);
    
    plotter.setup();
    plotter.setDrawingSize(ofGetWidth(), ofGetHeight());
    plotter.setPrintSize(20000, 20000*(ofGetHeight())/(ofGetWidth()));
}

//--------------------------------------------------------------
void ofApp::update(){
    plotter.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    //---------
    //Lets draw the stroke as a continous line
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofBeginShape();
    for(int i = 0; i < line.getVertices().size(); i++){
        ofVertex(line.getVertices()[i].x, line.getVertices()[i].y);
    }
    ofEndShape(false);
    
    ofSetColor(240, 240, 240);
    TTF.drawString("To plot the drawing press the space bar   status: "+message, 170, 12);
   
    plotter.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
    //no data gets saved unless you hit the s key
    if(key == 's'){
        XML.saveFile("mySettings.xml");
        message ="settings saved to xml!";
    }
    
    if(key == ' '){
        plotter.plotPolyline(line);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    //-------
    //we change the background color based on
    //the two mouse coords coming in
    float xpct = (float)x / ofGetWidth();
    float ypct = (float)y / ofGetHeight();
    red			= xpct * 255.0;
    green		= ypct * 255.0;
    blue		= (int)(red - green) % 255;
    
    
    //-------------
    //we also want to record the stroke
    
    //lets store the drag of the user.
    //we will push into the most recent
    //<STROKE> tag - add the mouse points
    //then pop out
    if( XML.pushTag("STROKE", lastTagNumber) ){
        
        //now we will add a pt tag - with two
        //children - X and Y
        
        int tagNum = XML.addTag("PT");
        XML.setValue("PT:X", x, tagNum);
        XML.setValue("PT:Y", y, tagNum);
        
        XML.popTag();
    }
    
    //-------------
    //here we make a string of text that looks like how the xml data
    //is stored in the settings file - this is just so people can
    //visually see how the data is stored.
    
    //if the text is about to go off screen
    if(lineCount > 64){
        //we find the first <PT> tag with the
        //x and y data and we remove it from the begining
        //this way the displayed text always shows the newest data
        //without going offscreen.
        int pos = xmlStructure.find("</PT>");
        xmlStructure = xmlStructure.substr(pos+6);
    }
    
    //here we add the most recent point to our fake xml string
    xmlStructure += "    <PT>\n        <X>"+ofToString(x)+"</X>\n        <Y>"+ofToString(y)+"</Y>\n    </PT>\n";
    lineCount+=4; //we have added 4 lines so increment by 4
    
    //------------
    //we also record the x y points into an array - so we can draw it
    if(pointCount < NUM_PTS -1){
        line.addVertex(x, y);
        pointCount++;
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    //reset so we don't store extra tags
    XML.clear();
    
    //we can also add tags with the same name.
    //here we are just adding an empty tag
    //and when the user drags their mouse
    //we will store the pts in this tag
    lastTagNumber	= XML.addTag("STROKE");
    xmlStructure	= "<STROKE>\n";
    
    //We start a new stroke
    lineCount		= 0;
    pointCount		= 0;
    
    line.clear();
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    //update the colors to the XML structure when the mouse is released
    XML.setValue("BACKGROUND:COLOR:RED", red);
    XML.setValue("BACKGROUND:COLOR:GREEN", green);
    XML.setValue("BACKGROUND:COLOR:BLUE", blue);
    
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

