//
//  samanNetwork.h
//  saman
//
//  Created by Dudley Smith on 22/10/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXbeeNetwork.h"
#include "ofxAnimatableFloat.h"

class samanNetwork : public ofxXbeeNetwork{
    
public:
    //
    void setupANode(string _nodeID);
    void setupANodePin(string _nodeID, int _pinNumber);
    
    void update(int _nbMaxMessagesWrite);
    
    void draw(bool _drawNodes, bool _drawControl);
    void drawNodes();
    void drawANode(ofxXbeeNode _nodeToDraw, ofPoint _pos, float _xSize, float _ySize);
    void drawANodeTube(float _value);
    
    
    // Animations object to drive boards -------------------
    map<string, ofxAnimatableFloat> m_aAnims;
    void animateDrop(string _nodeID, int _pinNumber, float _dropDuration);
    
    float lastTime;
    
    ofxXbeeNode getANode(string _nodeID);
    
};
