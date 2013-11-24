
#include "ofxXbeeDummyProtocolDefine.h"
#include "FastSPI_LED2.h"

#define     NUM_LEDS 32

const int   nbSecondsToStartUp = 5;
const int   greenPin = 12; // led sign for good infos -> Green
const int   redPin = 13; // led sign for good infos -> Red

const int   pinInA0 = A0; // led sign for good infos -> Red
const int   pinInA1 = A1; // led sign for good infos -> Red
const int   pinInA2 = A2; // led sign for good infos -> Red
const int   pinInA3 = A3; // led sign for good infos -> Red

const CRGB  white = CRGB(255,255,255);

const int  basicSmoothness = 10;

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

#define nbLoopsForHeartBeat 10000
double nbLoops;

String sXbeeID;
String reading="";
      
void setup() {
  
  nbLoops = 0;
  // initialize the LED pin as an output
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  
  pinMode(pinInA0, INPUT);
  pinMode(pinInA1, INPUT);
  pinMode(pinInA2, INPUT);
  pinMode(pinInA3, INPUT);
  // Initialize LEDS
  isBad();
  setupLEDS();
  isGood();
  
  // initialize serial communication:
  for(int nbLoopsStart=0; nbLoopsStart<=nbSecondsToStartUp; nbLoopsStart++){
    isBad();
    delay(500);
    isGood();
    delay(500);
  }
  Serial1.begin(9600);
  while(!Serial1){
    isBad();
    delay(250);
    isGood();
    delay(250);
    isBad();
    delay(250);
    isGood();
    delay(250);
  }
  Serial1.println();
  Serial1.println("Init");
  
  nbLoops = 0;
  isGoodBad();
}

void setupLEDS() {
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
  
  allLEDS(getArray(2), 0);
  allLEDS(getArray(3), 0);
  allLEDS(getArray(4), 0);
  allLEDS(getArray(5), 0);
  allLEDS(getArray(6), 0);
  allLEDS(getArray(7), 0);
  allLEDS(getArray(8), 0);
  allLEDS(getArray(9), 0);
  allLEDS(getArray(10), 0);
  allLEDS(getArray(11), 0);
  
  LEDS.setBrightness(255);    
  LEDS.show();
}

void loop() {
  
  bool messageDone = false;
  
  // Sending heartbeat every n loops
  nbLoops++;
  if(nbLoops>nbLoopsForHeartBeat){
    nbLoops = 0;
    sXbeeID = getXbeeID();
    isGoodBad();
    Serial1.println(HEAD + sXbeeID +  HEARTBEAT + TAIL);
  }
  
  // Receiving message character 1 by 1 --
  while(Serial1.available()>0 && messageDone==false){
    // Reading a char --
    reading += (char)Serial1.read();
    
    if(reading.startsWith(HEAD)){
      isGood();
      // Good Message, how is the tail ?
      if(reading.endsWith(TAIL)){
        // The tail is good
        // We can treat
        doYourStuffWithMessage(reading);
        // and reset
        reading = "";
      }
    }else{
      isGoodBad();
      // Bad message --
      // We reset
      reading = "";
    }
  }
}  

void doYourStuffWithMessage(String _message){
   
  bool idControl = false;
  String sIDReceived;
  String Mode = "";
  int    pinNum = 0;
  int    value = 0;
  
  //Serial1.println(_message);
  
    // On controle l'entete
    // On controle l'ID
    sIDReceived = _message.substring(CARDID_BEG,CARDID_BEG+CARDID_LEN);
    if(sIDReceived == sXbeeID){
      // On cherche le big Mode
      Mode  = _message.substring(MODE_BEG,MODE_BEG+MODE_LEN);
      pinNum    = _message.substring(PIN_BEG,PIN_BEG+PIN_LEN).toInt();
      value      = _message.substring(VAL_BEG,VAL_BEG+VAL_LEN).toInt();
      /*
      Serial1.println(sXbeeID);
      Serial1.println(Mode);
      Serial1.println(pinNum);
      Serial1.println(value);
      */
      // GOOD DATAS ----------------
      // We're driving outputs
      if(Mode==Mode_All){
        /*
        The whole strip
        [4A0200]
        [4A0299]
        all leds on among value received
        */
        isGood();
        allLEDS(getArray(pinNum), value);
        LEDS.show();
      }
      else if(Mode==Mode_Drop){
        /*
        Drop mode
        [0D0299]
        Drop the LEDS
        */ 
        isGood();
        drop(getArray(pinNum), value, basicSmoothness);
        LEDS.show();
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

void drop(CRGB _leds[NUM_LEDS], int _position, int _smoothness){
  
  int realPosition = ((float)_position/100.0f)*NUM_LEDS;
  int minPosition = max(0,((float)(_position-_smoothness)/100.0f)*NUM_LEDS);
  
  memset(_leds, 0,  NUM_LEDS * sizeof(struct CRGB));
  
  for(int idxLed = minPosition; idxLed<=realPosition; idxLed++){
   float ratio = (float)(realPosition-idxLed)/(realPosition-minPosition);
   
   CRGB color;
   color.r = 128*ratio*ratio;
   color.g = 128*ratio*ratio;
   color.b = 128*ratio*ratio;
   
   //Serial1.println(color.r);
   
   _leds[idxLed] = color;
  
  }
  
  
}

void allLEDS(CRGB _leds[NUM_LEDS], float _value){
  //Serial1.println(_value);
  memset(_leds, valueToRgb(_value),  NUM_LEDS * sizeof(struct CRGB));
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

String getXbeeID(){
  
  int iXbeeID = 0;
  
  if(digitalRead(pinInA0)==HIGH)  iXbeeID += 1;
  if(digitalRead(pinInA1)==HIGH)  iXbeeID += 2;
  if(digitalRead(pinInA2)==HIGH)  iXbeeID += 4;
  if(digitalRead(pinInA3)==HIGH)  iXbeeID += 8;
  
  return String(iXbeeID);
}

int valueToRgb(int _value){
  return 255*(float)(_value/100.0f);
}
