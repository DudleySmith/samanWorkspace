//
//  samanNode.h
//  saman
//
//  Created by Dudley Smith on 22/10/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXbeeNode.h"

class samanNode : public ofxXbeeNode
{
public:
    samanNode();
    virtual ~samanNode();
    
    samanNode operator = (const ofxXbeeNode &_n){
        
        samanNode _sn = _n;
        
        _sn.m_sID = _n.getID();
        _sn.m_aMessages = _n.getMessages();
        _sn.m_aPins     = _n.getPins();
        
        return _sn;
    }
    
};
