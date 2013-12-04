//
//  samanNetwork.cpp
//  saman
//
//  Created by Dudley Smith on 22/10/13.
//
//

#include "samanNetwork.h"

// -------------------------------------------------
void samanNetwork::draw(bool _drawNodes, bool _drawControl){
    if(_drawNodes)      drawNodes();
    if(_drawControl)    drawControl();
}

// ------------------------------------------
void samanNetwork::drawNodes(){
    
    // Arbres à gauche
    drawANode(m_aNodes["1"], ofPoint(10,100), 200, 340);
    drawANode(m_aNodes["2"], ofPoint(130,182), 150, 300);
    drawANode(m_aNodes["3"], ofPoint(210,266), 130, 240);
    drawANode(m_aNodes["4"], ofPoint(280,310), 130, 200);
    drawANode(m_aNodes["5"], ofPoint(350,385), 110, 160);
    
    // Arbres à droite
    drawANode(m_aNodes["6"], ofPoint(505,350), 190, 240);
    drawANode(m_aNodes["7"], ofPoint(710,290), 250, 300);
    drawANode(m_aNodes["8"], ofPoint(900,130), 320, 450);
    
}

// ------------------------------------------
void samanNetwork::drawANode(ofxXbeeNode _nodeToDraw, ofPoint _pos, float _xSize, float _ySize){
    
    ofPoint startPt, endPt;
    
    // Base drawing
    _nodeToDraw.draw(_pos, _xSize, _ySize);
    // Additive drawing
    ofPushMatrix();
    ofPushStyle();
    ofSetLineWidth(3);
    
    ofTranslate(_pos);
    
    for(int idxPin=2; idxPin<=11; idxPin++){
        switch (idxPin) {
            case 2:
                // PIN 2 --------------------------------------------------------------------
                startPt.set(_xSize*0.45f, _ySize*1.0f);
                endPt.set(_xSize*0.43f, _ySize*0.6f);
                break;
                
            case 3:
                // PIN 3 --------------------------------------------------------------------
                startPt.set(_xSize*0.50f, _ySize*0.85f);
                endPt.set(_xSize*0.55f, _ySize*0.48f);
                break;
                
            case 4:
                // PIN 4 --------------------------------------------------------------------
                startPt.set(_xSize*0.15f, _ySize*0.7f);
                endPt.set(_xSize*0.4f, _ySize*0.9f);
                break;
                
            case 5:
                // PIN 5 --------------------------------------------------------------------
                startPt.set(_xSize*0.9f, _ySize*0.48f);
                endPt.set(_xSize*0.52f, _ySize*0.7f);
                break;
                
            case 6:
                // PIN 6 --------------------------------------------------------------------
                startPt.set(_xSize*0.0f, _ySize*0.35f);
                endPt.set(_xSize*0.46f, _ySize*0.51f);
                break;
                
            case 7:
                // PIN 7 --------------------------------------------------------------------
                startPt.set(_xSize*0.05f, _ySize*0.15f);
                endPt.set(_xSize*0.40f, _ySize*0.47f);
                break;
                
            case 8:
                // PIN 8 --------------------------------------------------------------------
                startPt.set(_xSize*0.25f, _ySize*0.15f);
                endPt.set(_xSize*0.52f, _ySize*0.47f);
                break;
                
            case 9:
                // PIN 9 --------------------------------------------------------------------
                startPt.set(_xSize*0.30f, _ySize*0.1f);
                endPt.set(_xSize*0.52f, _ySize*0.42f);
                break;
                
            case 10:
                // PIN 10 --------------------------------------------------------------------
                startPt.set(_xSize*0.75f, _ySize*0.08f);
                endPt.set(_xSize*0.47f, _ySize*0.38f);
                break;
                
            case 11:
                // PIN 11 --------------------------------------------------------------------
                startPt.set(_xSize*0.82f, _ySize*0.36f);
                endPt.set(_xSize*0.54f, _ySize*0.52f);
                
                break;
                
            default:
                break;
        }
        
        // --------------------------------------------------------------------
        ofSetColor(ofColor::black, 50);
        ofLine(startPt, endPt);
        // --------------------------------------------------------------------

        if (_nodeToDraw.getPins()[idxPin].getMode() == pinModePwm) {
            // --------------------------------------------------------------------
            ofSetColor(ofColor::white, 255*_nodeToDraw.getPins()[idxPin].getValue()/(float)VAL_MAX);
            ofLine(startPt, endPt);
            // --------------------------------------------------------------------
            
        } else if (_nodeToDraw.getPins()[idxPin].getMode() == pinModeDrop) {
            /*
            float   dropRatio = _nodeToDraw.getPins()[idxPin].getValue()/(float)VAL_MAX;
            ofPoint dropPosition = dropRatio*startPt + (1-dropRatio)*endPt;
            */
            
            string keyAnim = _nodeToDraw.getID() + ":" + ofToString(idxPin, 0, 2, '0') + ":Drop";
            map<string, ofxAnimatableFloat>::iterator   oneAnim;
            
            oneAnim = m_aAnims.find(keyAnim);
            
            if(oneAnim!=m_aAnims.end() && (*oneAnim).second.isAnimating()){
                float   dropRatio = (*oneAnim).second.val();
                ofPoint dropPosition = dropRatio*startPt + (1-dropRatio)*endPt;
                
                ofSetColor(ofColor::white);
                if(dropRatio<1) ofCircle(dropPosition, 3);
                
            }
            
            
            
            //ofLogVerbose() << "Drop Ratio : " << dropRatio << "Drop pos : " << dropPosition;
            
            // --------------------------------------------------------------------

            // --------------------------------------------------------------------
            
        } else {
            // --------------------------------------------------------------------
        }
        
        
    }
    
    
    ofPopStyle();
    ofPopMatrix();
    
    
}

//--------------------------------------------------------------
void samanNetwork::setupANode(string _nodeID){
    
    addNode(_nodeID);

    setupANodePin(_nodeID, 2);
    setupANodePin(_nodeID, 3);
    setupANodePin(_nodeID, 4);
    setupANodePin(_nodeID, 5);
    setupANodePin(_nodeID, 6);
    setupANodePin(_nodeID, 7);
    setupANodePin(_nodeID, 8);
    setupANodePin(_nodeID, 9);
    //setupANodePin(_nodeID, 10);
    //setupANodePin(_nodeID, 11);

}


// ----------------------------------------------------------------
void samanNetwork::setupANodePin(string _nodeID, int _pinNumber){
    
    string  animKey = _nodeID + ":" + ofToString(_pinNumber, 0, 2, '0') + ":Drop";
    
    // Setup a pin
    registerNodePin(_nodeID, _pinNumber, pinModePwm);
    
    m_aAnims[animKey] = ofxAnimatableFloat();
    m_aAnims[animKey].setRepeatType(LOOP);
    m_aAnims[animKey].setCurve(EASE_IN);
    
}

// --------------------------------------------------------------------
void samanNetwork::update(int _nbMaxMessagesWrite){
    
    float currentTime = ofGetElapsedTimef();
    float dt = currentTime - lastTime;
    
    lastTime = currentTime;
    
    map<string, ofxAnimatableFloat>::iterator oneAnim;
    for (oneAnim = m_aAnims.begin(); oneAnim != m_aAnims.end(); oneAnim++) {
        
        // Update aniamtions
        (*oneAnim).second.update(dt);
        
        //ofLogVerbose() << "KEY:" << (*oneAnim).first << " Value:" << ofToString((*oneAnim).second.val()) << " isAnimating:" << ofToString((*oneAnim).second.isAnimating());
        /*
        if((*oneAnim).second.isAnimating()==true){
            vector<string> wholeKey = ofSplitString((*oneAnim).first, ":");
            string  ID = wholeKey[0];
            int     pin = ofToInt(wholeKey[1]);
            
            
            sendNodeDrop(ID, pin, (*oneAnim).second.val());
            setNodeDrop(ID, pin, (*oneAnim).second.val());
            
        }
        */
    }
    
    // Old
    ofxXbeeNetwork::update(false, true, _nbMaxMessagesWrite);
    
    
}

// -----------------------------------------------------------------
void samanNetwork::animateDrop(string _nodeID, int _pinNumber, float _dropDuration){
    m_aAnims[_nodeID+":"+ofToString(_pinNumber, 0, 2, '0')+":Drop"].setDuration(_dropDuration);
    m_aAnims[_nodeID+":"+ofToString(_pinNumber, 0, 2, '0')+":Drop"].reset(0);
    m_aAnims[_nodeID+":"+ofToString(_pinNumber, 0, 2, '0')+":Drop"].animateFromTo(0, 1);
}

// -----------------------------------------------------------------
void samanNetwork::startDrop(string _nodeID, int _pinNumber, float _dropDuration){
    
    string keyAnim = _nodeID + ":" + ofToString(_pinNumber, 0, 2, '0') + ":Drop";
    map<string, ofxAnimatableFloat>::iterator   oneAnim;
    
    oneAnim = m_aAnims.find(keyAnim);
    
    if(oneAnim!=m_aAnims.end() && (*oneAnim).second.hasFinishedAnimating()){
        m_aAnims[keyAnim].setDuration(_dropDuration);
        m_aAnims[keyAnim].reset(0);
        m_aAnims[keyAnim].animateFromTo(0, 1);
    }
    
}

// -----------------------------------------------------------------
void samanNetwork::stopDrop(string _nodeID, int _pinNumber){
    
    string keyAnim = _nodeID + ":" + ofToString(_pinNumber, 0, 2, '0') + ":Drop";
    map<string, ofxAnimatableFloat>::iterator   oneAnim;
    
    oneAnim = m_aAnims.find(keyAnim);
    
    if(oneAnim!=m_aAnims.end()){
        m_aAnims[keyAnim].reset(0);
    }
    
}

// -----------------------------------------------------------------
ofxXbeeNode samanNetwork::getANode(string _nodeID){
    

    //map<string, ofxXbeeNode>            nodes = m_aNode;
    map<string, ofxXbeeNode>::iterator  nodeToReturn = m_aNodes.find(_nodeID);
    
    if (nodeToReturn != m_aNodes.end()) {
        return (*nodeToReturn).second;
    }else{
        return ofxXbeeNode();
    }
    
    
}

