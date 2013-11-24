#pragma once

#define dataPath "../../../../../saman_commons/datas/"

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxEasyOsc.h"
#include "samanNetwork.h"

class testApp : public ofBaseApp{
	
public:
    void setup();
    void setupGui();
    
    void update();
    void updateGui();
    void updateOscInput();
    
    void draw();
    void drawBackground();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // -------------------------------------------------
    // GUI ---------------------------------------------
    bool        m_bDisplayGui;
    
    // PARAMETERS --------------------------------------
    ofxPanel    m_pnSettings;
    
    ofxLabel    m_lbConnections;
    ofParameter<string> m_pxDataPath;
    ofParameter<string> m_pxConnection;
    ofParameter<string> m_pxBackgroundImage;
    ofxToggle           m_btVerbose;
    ofxToggle           m_btLogToFile;
    ofxLabel            m_lbLogFile;
    
    ofxLabel   m_lblAnimParams;
    ofParameter<float>  m_pxDropDurationMin;
    ofParameter<float>  m_pxDropDurationMax;
    ofParameter<float>  m_pxDropSmoothness;
    ofParameter<float>  m_pxDispGenCorrection;
    ofParameter<float>  m_pxDispNodeCorrection;
    
    // Networks of arduino boards
    samanNetwork                  m_oXbees;
    
    // Background
    ofImage m_oBackgroundImage;
    
    // Osc Shit
    ofxEasyOSc  m_oOsc;
    ofxPanel    m_pnGuiOscSettings;
    ofxLabel    m_lbHost;
    ofxLabel    m_lbSet;
    ofParameter<int>  m_pxMaxMessagesOsc;
    ofParameter<int>  m_pxMaxMessagesXbee;
    
    // Devices -----------------------------------
    ofxPanel    m_pnDevices;
    
    ofxLabel    m_lbEmitter;
    
    ofxLabel    m_lbRecep01;
    ofxLabel    m_lbRecep02;
    ofxLabel    m_lbRecep03;
    ofxLabel    m_lbRecep04;
    ofxLabel    m_lbRecep05;
    ofxLabel    m_lbRecep06;
    ofxLabel    m_lbRecep07;
    ofxLabel    m_lbRecep08;
    
};

