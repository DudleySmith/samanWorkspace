
#include "ofxXbeeDummyProtocolDefine.h"
#include "FastSPI_LED2.h"

#define NUM_LEDS 32

const int   greenPin = 12; // led sign for good infos -> Green
const int   redPin = 13; // led sign for good infos -> Red

const CRGB  white = CRGB(255,255,255);

CRGB leds_2[NUM_LEDS];
CRGB leds_3[NUM_LEDS];
CRGB leds_4[NUM_LEDS];
CRGB leds_5[NUM_LEDS];
CRGB leds_6[NUM_LEDS];
CRGB leds_7[NUM_LEDS];
CRGB leds_8[NUM_LEDS];
CRGB leds_9[NUM_LEDS];
CRGB leds_10[NUM_LEDS];
CRGB leds_11[NUM_LEDS];

#define nbLoopsForHeartBeat 5000
double nbLoops;

String  sXbeeID;
String  receivedMsg = "";
  
void setup() {
  nbLoops = 0;
  // initialize serial communication:
  Serial.begin(9600);
  //Serial.setTimeout(100);
  
  Serial.println();
  Serial.println("-------------------");
  Serial.println("Init");
  
  sXbeeID = XBEEID_NONE;
  // initialize the LED pin as an output
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  // Initialize LEDS
  setupLEDS();
}

void setupLEDS() {
  Serial.println("---------------- Setup Leds -----------------");
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  // For safety (to prevent too high of a power draw), the test case defaults to
  // setting brightness to 25% brightness
  LEDS.addLeds<WS2811, 2>(leds_2, NUM_LEDS);
  LEDS.addLeds<WS2811, 3>(leds_3, NUM_LEDS);
  LEDS.addLeds<WS2811, 4>(leds_4, NUM_LEDS);
  LEDS.addLeds<WS2811, 5>(leds_5, NUM_LEDS);
  LEDS.addLeds<WS2811, 6>(leds_6, NUM_LEDS);
  LEDS.addLeds<WS2811, 7>(leds_7, NUM_LEDS);
  LEDS.addLeds<WS2811, 8>(leds_8, NUM_LEDS);
  LEDS.addLeds<WS2811, 9>(leds_9, NUM_LEDS);
  LEDS.addLeds<WS2811, 10>(leds_10, NUM_LEDS);
  LEDS.addLeds<WS2811, 11>(leds_11, NUM_LEDS);
  
  allLEDS(getArray(2), 100);
  allLEDS(getArray(3), 100);
  allLEDS(getArray(4), 100);
  allLEDS(getArray(5), 100);
  allLEDS(getArray(6), 100);
  allLEDS(getArray(7), 100);
  allLEDS(getArray(8), 100);
  allLEDS(getArray(9), 100);
  allLEDS(getArray(10), 100);
  allLEDS(getArray(11), 100);
  
  LEDS.setBrightness(255);    
  LEDS.show();
}

void loop() {
  
  char    *receivedAllMsg;
  int nbCar = 0;


  nbLoops++;
  if(nbLoops>nbLoopsForHeartBeat){

    nbLoops = 0;
    
    if(sXbeeID==XBEEID_NONE){
      sXbeeID = "0001";
      //isBad();
      //Serial.println(HEAD + sXbeeID + ID_ASKING + TAIL);
    }else{
      //isGood();
      //Serial.println(HEAD + sXbeeID +  HEARTBEAT + TAIL);
    }
  }
    
  nbCar = Serial.available();
  
  if (nbCar>0){
    
    int idxHead = -1;
    int idxTail = -1;
    int idxMsg = 0;
    String realMsg = "";
    
    receivedMsg = Serial.readString();
    
    do{
      // Détection des messages --
      idxHead = receivedMsg.indexOf(HEAD, idxMsg);
      idxTail = receivedMsg.indexOf(TAIL, idxMsg);
      if(idxHead!=-1 && idxTail!=-1){
        // --
        realMsg = receivedMsg;
        realMsg = realMsg.substring(idxHead, 1+idxTail);
        // --
        doYourStuffWithMessage(realMsg);
        // --
        receivedMsg = receivedMsg.substring(idxTail);
        realMsg = "";
      }
    }while(idxHead!= -1 && idxTail!=-1);

  }
  
}  

void doYourStuffWithMessage(String _message){
   
  bool idControl = false;
  String sIDReceived;
  String bigMode = "";
  String fineMode = "";
  int    pinNum = 0;
  int    arg1 = 0;
  int    arg2 = 0;
  
  Serial.println(_message);
  
  if(_message.startsWith(HEAD)==true && _message.endsWith(TAIL)==true){
    // On controle l'entete
    // On controle l'ID
    sIDReceived = _message.substring(3,7);
    if(sIDReceived == sXbeeID){
    
      //[[[0001OUTALL003255]
      
      // On cherche le big Mode
      bigMode   = _message.substring(7,10);
      fineMode  = _message.substring(10,13);
      pinNum    = _message.substring(13,16).toInt();
      arg1      = _message.substring(16,19).toInt();
      arg2      = _message.substring(19,22).toInt();
      
      // GOOD DATAS ----------------
      if(bigMode==BigMode_Out){
        // We're driving outputs
        if(fineMode==FineMode_All){
          // The whole strip
          // all leds on among value received
          isGood();
          Serial.println(bigMode);
          Serial.println(fineMode);
          Serial.println(pinNum);
          Serial.println(arg1);
          Serial.println(arg2);
      
          allLEDS(getArray(pinNum), arg1);
          LEDS.show();
        }
        else if(fineMode==FineMode_Drop){
          // Drop mode
          // drop the LEDS
          isGood();
          drop(getArray(pinNum), arg1);
          LEDS.show();
        }else{
          isBad();
        }
        
      }else{
        isBad();
      }
      
    }else{
      isBad();
    }
  
  }else{
    isBad();
  }
  
}

// LEDS Section -----------------------------------
CRGB* getArray(int _pinNum){
  switch(_pinNum){
  case 2:
    return leds_2;
    break;
  case 3:
    return leds_3;
    break;
  case 4:
    return leds_4;
    break;
  case 5:
    return leds_5;
    break;
  case 6:
    return leds_6;
    break;
  case 7:
    return leds_7;
    break;
  case 8:
    return leds_8;
    break;
  case 9:
    return leds_9;
    break;
  case 10:
    return leds_10;
    break;
  case 11:
    return leds_11;
    break;
  default:
    return leds_2;
  }
}

void drop(CRGB _leds[NUM_LEDS], int _position){

  int realPosition = ((float)_position/255.0f)*NUM_LEDS;

  memset(_leds, 0,  NUM_LEDS * sizeof(struct CRGB));
  _leds[realPosition] = white;

}

void allLEDS(CRGB _leds[NUM_LEDS], float _value){
  memset(_leds, _value,  NUM_LEDS * sizeof(struct CRGB));
}

void isGood(){
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
}
void isBad(){
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, HIGH);
}
void isGoodBad(){
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, HIGH);
}
