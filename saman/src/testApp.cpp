#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
	ofBackground(255);
	
    // Change the data path
    ofEnableDataPath();
    ofSetDataPathRoot(dataPath);
    
    ofDirectory  dirDataPath = ofToDataPath("");
    if(dirDataPath.exists() && !dirDataPath.isDirectoryEmpty("")){
        ofLogVerbose() << dirDataPath.getAbsolutePath() << " exists and full of files" ;
    }else{
        ofLogError() << dirDataPath.getAbsolutePath() << ": It's emtpy or not exists.";
    }
    
    // GUI --
    setupGui();
    m_pxDataPath = dirDataPath.getAbsolutePath();
    
    // Logs --
    m_lbLogFile = "log_"+ofGetTimestampString("%Y%m%d")+".log";
    ofLogToFile(m_lbLogFile);
    
    // BACKGROUND --
    m_oBackgroundImage.loadImage(m_pxBackgroundImage);
    if(m_oBackgroundImage.isAllocated()){
        ofSetWindowShape(m_oBackgroundImage.width, m_oBackgroundImage.height);
    }
    
    // Network --
    m_oXbees.setup(m_pxConnection);
    m_oXbees.setupANode(m_lbRecep01);
    m_oXbees.setupANode(m_lbRecep02);
    m_oXbees.setupANode(m_lbRecep03);
    m_oXbees.setupANode(m_lbRecep04);
    m_oXbees.setupANode(m_lbRecep05);
    m_oXbees.setupANode(m_lbRecep06);
    m_oXbees.setupANode(m_lbRecep07);
    m_oXbees.setupANode(m_lbRecep08);
    
    // OSC and its gui
    m_oOsc.loadParameters();
    m_pnGuiOscSettings.setup(m_oOsc.getSettings());
    m_pnGuiOscSettings.add(m_lbHost.setup("Host OSC", m_oOsc.getHost()));
    m_pnGuiOscSettings.add(m_lbSet.setup("Set OSC",m_oOsc.getSet()));
    m_pnGuiOscSettings.add(m_pxMaxMessagesOsc.set("MaxMessagesOsc", 64, 0, 128));
    m_pnGuiOscSettings.add(m_pxMaxMessagesXbee.set("MaxMessagesXbee", 64, 0, 128));
    
    m_pnGuiOscSettings.setPosition(510, 10);
    m_pnGuiOscSettings.loadFromFile("settings.xml");
    m_oOsc.setup(EASYOSC_IN);
    
    ofSetFrameRate(30);
    
}


//--------------------------------------------------------------
void testApp::setupGui(){
    
    m_bDisplayGui = false;
    
    // SETTINGS ---------------------------------------------------------------------------
    m_pnSettings.setup("others", "settings.xml", 10, 10);
    
    m_pnSettings.add(m_lbConnections.setup("Connections PXws", ""));
    m_pnSettings.add(m_pxDataPath.set("Data Path", "none, don't care"));
    m_pnSettings.add(m_pxConnection.set("USB Connection", "tty.usbserial-A600KMNU"));
    m_pnSettings.add(m_pxBackgroundImage.set("Background Image", "background.jpg"));
    m_pnSettings.add(m_btVerbose.ofxToggle::setup("Verbose", true));
    m_pnSettings.add(m_btLogToFile.ofxToggle::setup("LogToFile", false));
    m_pnSettings.add(m_lbLogFile.setup("log file", ""));
    
    m_pnSettings.add(m_lblAnimParams.setup("Animations", ""));
    m_pnSettings.add(m_pxDropDurationMin.set("DropDuration Min", 2, 0, 10));
    m_pnSettings.add(m_pxDropDurationMax.set("DropDuration Max", 2, 0, 10));
    m_pnSettings.add(m_pxDropSmoothness.set("DropSmoothness", 0.1, 0, 1));
    m_pnSettings.add(m_pxDispGenCorrection.set("DispGenCorrection", 0.05, 0, 0.1));
    m_pnSettings.add(m_pxDispNodeCorrection.set("DispNodeCorrection", 0.05, 0, 0.1));
    m_pnSettings.add(m_pxIntensityMin.set("Intensity Min", 0, 0, 1));
    m_pnSettings.add(m_pxIntensityMax.set("Intensity Max", 1, 0, 1));

    m_pnSettings.loadFromFile("settings.xml");
    
    // Devices -----------------------------------
    m_pnDevices.setup("Devices","devices.xml", 260, 10);

    m_pnDevices.add(m_lbEmitter.setup("Emitter", "xxxx"));
    
    m_pnDevices.add(m_lbRecep01.setup("Recep 01", ofxXbeeDummyProtocol::getCardIdString(1)));
    m_pnDevices.add(m_lbRecep02.setup("Recep 02", ofxXbeeDummyProtocol::getCardIdString(2)));
    m_pnDevices.add(m_lbRecep03.setup("Recep 03", ofxXbeeDummyProtocol::getCardIdString(3)));
    m_pnDevices.add(m_lbRecep04.setup("Recep 04", ofxXbeeDummyProtocol::getCardIdString(4)));
    m_pnDevices.add(m_lbRecep05.setup("Recep 05", ofxXbeeDummyProtocol::getCardIdString(5)));
    m_pnDevices.add(m_lbRecep06.setup("Recep 06", ofxXbeeDummyProtocol::getCardIdString(6)));
    m_pnDevices.add(m_lbRecep07.setup("Recep 07", ofxXbeeDummyProtocol::getCardIdString(7)));
    m_pnDevices.add(m_lbRecep08.setup("Recep 08", ofxXbeeDummyProtocol::getCardIdString(8)));
    
    m_pnDevices.loadFromFile("devices.xml");
    // -------------------------------------------------
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    // GUI --
    m_lbHost = m_oOsc.getHost();
    m_lbSet = m_oOsc.getSet();
    updateGui();
    
    // --
    m_oOsc.update(m_pxMaxMessagesOsc);

    // OSC
    updateOscInput();

    // Update network : Send / Read --
    m_oXbees.update(m_pxMaxMessagesXbee);
    
}

//--------------------------------------------------------------
void testApp::updateOscInput(){
    
    string nameKey = "";
    float value0 = 0;
    //double duration = 0;
    
    
    // --
    float genIntensity = m_oOsc.getConstValue("/params/general/intensity");
    float genCenter = m_oOsc.getConstValue("/params/general/center");
    float genDispersion = m_oOsc.getConstValue("/params/general/disp");
    float genDispersionMin = m_oOsc.getConstValue("/params/general/dispMin");
    
    genDispersion = ofMap(genDispersion, 0, 1, m_pxDispGenCorrection, 1);
    
    int idxNodePin = 0;
    int idxGeneralPin = 0;
    
    // --
    map<string, ofxXbeeNode>::iterator oneNode;
    map<string, ofxXbeeNode>           nodes = m_oXbees.getNodes();
    
    float stripLightness = 0;
    
    for (oneNode=nodes.begin(); oneNode!=nodes.end(); oneNode++) {
        
        // --
        float nodeMix = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/mix");
        float nodeIntensity = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/intensity");
        float nodeCenter = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/center");
        float nodeDispersion = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/disp");
        float nodeDispersionMin = m_oOsc.getConstValue("/params/"+(*oneNode).first+"/dispMin");
        

        
        nodeDispersion = ofMap(nodeDispersion, 0, 1, m_pxDispNodeCorrection, 1);
        
        // Check all messages --
        map<int, ofxXbeeNodePin>::iterator onePin;
        map<int, ofxXbeeNodePin>           pins = (*oneNode).second.getPins();
        idxNodePin = 0;
        
        for (onePin=pins.begin(); onePin!=pins.end(); onePin++) {
            
            stripLightness = 0;
            
            float nodePinRatio = (float)idxNodePin/(float)pins.size();
            float generalPinRatio = (float)idxGeneralPin/(float)(pins.size()*m_oXbees.getNodes().size());
            
            // --
            nameKey = (*oneNode).first+"/"+ofToString((*onePin).first);
            
            // On envoie alors le message ---------------------------------
            // Si une animation est en cours, c'est de la goutte de pluie, sinon c'est whole strip
            
            string keyAnim = (*oneNode).first + ":" + ofToString((*onePin).first, 0, 2, '0') + ":Drop";
            map<string, ofxAnimatableFloat>::iterator   oneAnim;
            
            oneAnim = m_oXbees.m_aAnims.find(keyAnim);
            
            // new drop
            /*
            float dropOnOff = m_oOsc.getConstValue("/drops/onoff/"+(*oneNode).first+"/"+ofToString((*onePin).first));
            
            if(dropOnOff==1){
                
             
                if(fabs(m_pxDropDurationMax-m_pxDropDurationMin)>FLT_EPSILON){
                    // --
                    duration = ofMap(dropTime, 0, 1, m_pxDropDurationMax, m_pxDropDurationMin, true);
                }
                
                m_oXbees.startDrop((*oneNode).first, (*onePin).first, duration);
                
             
            }else if(dropOnOff==0){
                m_oXbees.stopDrop((*oneNode).first, (*onePin).first);
                
                //m_oXbees.sendNodeDrop((*oneNode).first, (*onePin).first, 0);
                //m_oXbees.setNodeDrop((*oneNode).first, (*onePin).first, 0);
                
            }
            */
            
            // --
            bool isDropEvent = m_oOsc.getEvent("onoff", nameKey, value0);
            float dropTime = m_oOsc.getConstValue("/drops/time/"+(*oneNode).first+"/"+ofToString((*onePin).first));
            
            
            if (isDropEvent==true) {
                // ----------------------------------------------------
                if(value0==1){
                    
                    float duration = m_pxDropDurationMax;
                    
                    if(fabs(m_pxDropDurationMax-m_pxDropDurationMin)>FLT_EPSILON){
                        // --
                        duration = ofMap(value0, 0, 1, m_pxDropDurationMax, m_pxDropDurationMin, true);
                    }else{
                        duration = m_pxDropDurationMax;
                    }
                    
                    m_oXbees.startDrop((*oneNode).first, (*onePin).first, duration);
                    
                    m_oXbees.sendNodeDrop((*oneNode).first, (*onePin).first, value0);
                    m_oXbees.setNodeDrop((*oneNode).first, (*onePin).first, value0);
                    
                }else if (value0==0){
                    
                    m_oXbees.stopDrop((*oneNode).first, (*onePin).first);
                    
                    m_oXbees.sendNodeDrop((*oneNode).first, (*onePin).first, 0);
                    m_oXbees.setNodeDrop((*oneNode).first, (*onePin).first, 0);
                    
                }
                
            }else{
            
                // ----------------------------------------------------
                // GLOBAL COMMAND -------------------------------------
                // --
                float nodePinValue = 0;
                float genPinValue = 0;
                
                // --
                if(nodePinRatio>=nodeCenter && nodePinRatio<=nodeCenter+nodeDispersion){
                    nodePinValue = ofMap(nodePinRatio, nodeCenter, nodeCenter+nodeDispersion, 1, nodeDispersionMin, true);
                }else if(nodePinRatio>=nodeCenter-nodeDispersion && nodePinRatio<=nodeCenter){
                    nodePinValue = ofMap(nodePinRatio, nodeCenter, nodeCenter-nodeDispersion, 1, nodeDispersionMin, true);
                }else{
                    nodePinValue = 0;
                }
                nodePinValue *= nodeIntensity;
                //--
                
                // --
                if(generalPinRatio>=genCenter && generalPinRatio<=genCenter+genDispersion){
                    genPinValue = ofMap(generalPinRatio, genCenter, genCenter+genDispersion, 1, genDispersionMin, true);
                }else if(generalPinRatio>=genCenter-genDispersion && generalPinRatio<=genCenter){
                    genPinValue = ofMap(generalPinRatio, genCenter, genCenter-genDispersion, 1, genDispersionMin, true);
                }else{
                    genPinValue = 0;
                }
                genPinValue *= genIntensity;
                //--
                
                stripLightness = ofMap(nodeMix*nodePinValue + (1-nodeMix)*genPinValue, 0, 1, m_pxIntensityMin, m_pxIntensityMax);
                
                // For animation
                if(stripLightness>0.5){
                    m_oXbees.sendNodePwm((*oneNode).first, (*onePin).first, 1);
                    m_oXbees.setNodeAllStrip((*oneNode).first, (*onePin).first, 1);
                }else{
                    m_oXbees.sendNodePwm((*oneNode).first, (*onePin).first, 0);
                    m_oXbees.setNodeAllStrip((*oneNode).first, (*onePin).first, 0);
                }
                
                
                //ofLogVerbose() << " : " << (*oneNode).first<< " : " <<(*onePin).first<< " : " << stripLightness;
                
            }
            
            // We can not send every 25 fps, arduino can not folllow !!!!!!!!
            //
            /*
            bool bSendForReal = false;
            
            // Time Frequency Mode
            int  iSkipFrequency = 2;
            if(ofGetFrameNum()%iSkipFrequency == 0){
                bSendForReal = true;
            }
            */
            /*
            // Only one node after all
            int idxNode_A = 1+ofGetFrameNum()%(int)(0.5*m_oXbees.getNodes().size());
            string keyNodeToSend_A = ofxXbeeDummyProtocol::getCardIdString(idxNode_A);
            
            int idxNode_B = 4+ofGetFrameNum()%(int)(0.5*m_oXbees.getNodes().size());
            string keyNodeToSend_B = ofxXbeeDummyProtocol::getCardIdString(idxNode_B);
            
            if(keyNodeToSend_A == (*oneNode).first || keyNodeToSend_B == (*oneNode).first){
                bSendForReal = true;
                // --
                ofLogVerbose() << "Only this node will be realy updated : " << (*oneNode).first;
            }
            */
            /*
            bSendForReal=true;
            
            if(oneAnim != m_oXbees.m_aAnims.end()){
                if(m_oXbees.m_aAnims[keyAnim].isAnimating()){
                    // For communication ----------
                    if(bSendForReal==true){
                        //m_oXbees.sendNodeDrop((*oneNode).first, (*onePin).first, m_oXbees.m_aAnims[keyAnim].val());
                    }
                    // For animation
                    //m_oXbees.setNodeDrop((*oneNode).first, (*onePin).first, m_oXbees.m_aAnims[keyAnim].val());
                }else{
                    // For communication ----------
                    if(bSendForReal==true){
                        //m_oXbees.sendNodePwm((*oneNode).first, (*onePin).first, stripLightness);
                    }
                    // For animation
                    //m_oXbees.setNodeAllStrip((*oneNode).first, (*onePin).first, stripLightness);
                }
            }
            */
            idxNodePin++;
            idxGeneralPin++;
            
        }
        
    }
    
    
    
    
    
}

//--------------------------------------------------------------
void testApp::updateGui(){
    
    // Verbose log ?
    if(m_btVerbose==true){
        ofSetLogLevel(OF_LOG_VERBOSE);
    }else{
        ofSetLogLevel(OF_LOG_ERROR);
    }
    
    // Verbose log ?
    if(m_btLogToFile==true){
        
    }else{
        ofLogToConsole();
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    m_oBackgroundImage.draw(0, 0);
    
    
    // GUI --
    if (m_bDisplayGui==true) {
        // Display OSC messages
        list<string> messages = m_oOsc.getRoughMessages();
        list<string>::iterator oneMessage;
        int idxMessage = 0;
        
        ofPushMatrix();
        ofTranslate(10, ofGetHeight()*0.65);
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()*0.5, 0);
            // Xbee network drawing
            m_oXbees.draw(false, true);
        ofPopMatrix();
        
        // OSC Messages
        ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
        for(oneMessage=messages.begin(); oneMessage!=messages.end(); oneMessage++){
            ofDrawBitmapString((*oneMessage), 100, 10*idxMessage++);
            ofLogNotice() << "OSC Message [" << idxMessage << "] : " << ofToString((*oneMessage), 0, 2, '0');
        }
        
        ofPopMatrix();
        
        m_pnSettings.draw();
        m_pnGuiOscSettings.draw();
        m_pnDevices.draw();
        
    }else{
        
        ofDrawBitmapString("Type letter [h] to show GUI", 10, ofGetHeight() - 10);
    }
    
    // Arbres ˆ gauche
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep01), ofPoint(010,100), 200, 340);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep02), ofPoint(130,182), 150, 300);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep03), ofPoint(210,266), 130, 240);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep04), ofPoint(280,310), 130, 200);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep05), ofPoint(350,385), 110, 160);
    // Arbres ˆ droite
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep06), ofPoint(505,350), 190, 240);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep07), ofPoint(710,290), 250, 300);
    m_oXbees.drawANode(m_oXbees.getANode(m_lbRecep08), ofPoint(900,130), 320, 450);
    
}

//--------------------------------------------------------------
void testApp::drawBackground(){

    // First Draw background
    int imageW;
    int imageH;
    float imageRatioW_H;
    
    int realW;
    int realH;
    
    imageW = m_oBackgroundImage.width;
    imageH = m_oBackgroundImage.height;
    imageRatioW_H = (float)imageW / (float)imageH;
    
    realW   = ofGetWidth();
    realH   = ofGetWidth()/imageRatioW_H;
    
//    ofLogVerbose() << imageW << ":" << imageH << ":" << imageRatioW_H << ":" << realW << ":" << realH;
    

    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofLogVerbose() << "KEY PRESSED : " << key;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    ofLogVerbose() << "KEY RELEASED : " << key;
    
    if (key == 'h') {
        m_bDisplayGui = !m_bDisplayGui;
    }
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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