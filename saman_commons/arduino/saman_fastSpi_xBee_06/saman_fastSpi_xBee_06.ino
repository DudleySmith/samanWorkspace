

#include "FastSPI_LED2.h"

#define       NbLeds 50

const String  numVersion = "65";

const int   nbSecondsToStartUp = 2;
const int   greenPin = 12; // led sign for good infos -> Green
const int   redPin = 13; // led sign for good infos -> Red

const int   pinInA0 = A0; // led sign for good infos -> Red
const int   pinInA1 = A1; // led sign for good infos -> Red
const int   pinInA2 = A2; // led sign for good infos -> Red
const int   pinInA3 = A3; // led sign for good infos -> Red

const int    basicBrightness = 20;
const int    basicFPSForLeds = 25;
const int    basicNbPins     = 8;

const float  basicMaxValueReceived = 100.0f;
const float  basicMinDuration      = 600.0f;
const float  basicMaxDuration      = 1550.0f;
const float  basicDropSmoothness   = 0.10f;

const int cModeNone=0;
const int cModeDrop=1;
const int cModeFade=2;
const int cModeFire=3;

CRGB leds_2[NbLeds];
CRGB leds_3[NbLeds];
CRGB leds_4[NbLeds];
CRGB leds_5[NbLeds];
CRGB leds_6[NbLeds];
CRGB leds_7[NbLeds];
CRGB leds_8[NbLeds];
CRGB leds_9[NbLeds];

boolean   animOnOff[8];
int       animMode[8];
int       animTime[8];
float     animValue[8];

#define nbLoopsForHeartBeat 5000
double nbLoops;

String sXbeeID="";
String sReading="";

unsigned long lastCounterMillis;
      
void setup() {
  
  nbLoops = 0;
  // initialize the LED pin as an output
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  
  pinMode(pinInA0, INPUT);
  pinMode(pinInA1, INPUT);
  pinMode(pinInA2, INPUT);
  pinMode(pinInA3, INPUT);
  
  // initialize serial communication:
  for(int nbLoopsStart=0; nbLoopsStart<=nbSecondsToStartUp; nbLoopsStart++){
    isBad();
    delay(500);
    isGood();
    delay(500);
  }
  
  // Init Serial
  Serial1.begin(9600);
  while(!Serial1){
    isBad();
    delay(250);
    isGood();
    delay(250);
  }
  
  Serial1.println("Init LEDS ------------------------");
  
  // Initialize LEDS
  isBad();
  setupLEDS();
  isGood();
  
  nbLoops = 0;
  sReading = "";
  
  isGoodBad();
  
  for(int numAnim=0; numAnim<basicNbPins; numAnim++){
    Serial1.println("Init Pin "+ String(numAnim) + " ------------------------");
    
    animOnOff[numAnim] = false;
    animMode[numAnim] = cModeNone;
    animTime[numAnim] = 0;
    animValue[numAnim] = 0;
    
  }

  Serial1.println("Init Done ------------------------");
    
}

void setupLEDS() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  // For safety (to prevent too high of a power draw), the test case defaults to
  // setting brightness to 25% brightness
  
  LEDS.addLeds<WS2811, 2>(leds_2, NbLeds);
  LEDS.addLeds<WS2811, 3>(leds_3, NbLeds);
  LEDS.addLeds<WS2811, 4>(leds_4, NbLeds);
  LEDS.addLeds<WS2811, 5>(leds_5, NbLeds);
  LEDS.addLeds<WS2811, 6>(leds_6, NbLeds);
  LEDS.addLeds<WS2811, 7>(leds_7, NbLeds);
  LEDS.addLeds<WS2811, 8>(leds_8, NbLeds);
  LEDS.addLeds<WS2811, 9>(leds_9, NbLeds);
  
  LEDS.setBrightness(0);    
  LEDS.show();
  LEDS.setBrightness(basicBrightness);
}

void loop() {
  
  // Sending heartbeat every n loops
  nbLoops++;
  if(nbLoops>nbLoopsForHeartBeat){
    nbLoops = 0;
    sXbeeID = getXbeeID();
    isGoodBad();
    Serial1.println("[" + sXbeeID +  "H" + "]");
    Serial1.println("[WTF " + numVersion + " ]");
  }

  // Compteur de visu : frequence de rafraichissement --------------------
  if((millis() - lastCounterMillis) >= (1000/basicFPSForLeds)){
    lastCounterMillis =  millis();
    LEDS.show();
  }
  
  for(int numAnim=0; numAnim<basicNbPins; numAnim++){
    
    if(animOnOff[numAnim]==true){
      updateAnim(numAnim);
      if(animMode[numAnim]==cModeDrop){
        // DROP --
        updateAnim(numAnim);
        drop(getArray(numAnim+2), animValue[numAnim], basicDropSmoothness);
      }else if(animMode[numAnim]==cModeFade){
        // FADE --
        //Serial1.println(String(numAnim) + ":" + String(numAnim+2) + ":Fade:" + String((int)(100*animValue[numAnim])));
        //allLEDS(getArray(numAnim+2), animValue[numAnim]);
      }
      
    }else{
      //allLEDS(getArray(numAnim+2), 0.0f);
    } 
    
  }  
  
  // Receiving message character 1 by 1 --
  while(Serial1.available()>0){
    // Reading a char --
    sReading += (char)Serial1.read();
    //Serial1.println(sReading);
    
    if(sReading.startsWith("[")){
      // Good Message, how is the tail ?
      if(sReading.endsWith("]")){
        isGood();
        // The tail is good
        // We can treat
        doYourStuffWithMessage(sReading);
        //doYourStuffWithMessage();
        // and reset
        sReading = "";
      }
    }else{
      isGoodBad();
      // Bad message --
      // We reset
      sReading = "";
    }
  }
  
}  

void doYourStuffWithMessage(String sMessage){
  
    // On controle l'entete
    // On controle l'ID
    String   sIDReceived = "";
    String   sMode = "";
    int      iNumPin = 0;
    float    iValue = 0;
    
    sIDReceived = sMessage.substring(1,2);
    sMode = sMessage.substring(2,3);
    iNumPin = sMessage.substring(3,4).toInt();
    iValue = sMessage.substring(4,6).toInt();

    // On cherche le big Mode
    /*
    Serial1.println(sMessage);
    Serial1.println("ID =" + sIDReceived);
    Serial1.println("A=" + sMode + ":" + iNumPin + ":" + (int)iValue);
    */
    
    if(sIDReceived == sXbeeID){
      // GOOD DATAS ----------------
      // We're driving outputs
      if(setAnimation(sMode, iNumPin, iValue) == true){
        isGood();
      }else{
        isBad();
      }  
      
      //delay(2000);
      
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
    default:
      return leds_2;
  }
}

void drop(CRGB _leds[NbLeds], float _position, float _smoothness){
  
  int realPosition = _position*NbLeds;
  int minPosition = max(0,(_position-_smoothness)*NbLeds);
  
  memset(_leds, 0,  NbLeds * sizeof(struct CRGB));
  
  for(int idxLed = minPosition; idxLed<=realPosition; idxLed++){
    float ratio = (float)(realPosition-idxLed)/(realPosition-minPosition);
     
    CRGB color;
    color.r = 128*ratio*ratio;
    color.g = 128*ratio*ratio;
    color.b = 128*ratio*ratio;
     
    _leds[idxLed] = color;
  }
}

void allLEDS(CRGB _leds[NbLeds], float _value){
  
  //Serial1.println("C= ALL");
  //Serial1.println(_value);
  
  memset(_leds, valueToRgb(_value), NbLeds * sizeof(struct CRGB));
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

int valueToRgb(float _value){
  return 255*_value;
}


// Gestion Anim
boolean setAnimation(String sMode, int iNumPin, int iValue){
  // Pin number is not array index, fool !
  iNumPin -= 2;
  
  //Serial1.println("B=" + sMode + ":" + iNumPin + ":" + iValue);
  
  // it'is not the right index
  if(iNumPin<0 || iNumPin>8)
    return false;
    
  // -- 
  if(sMode=="A"){
    // whole strip --
    modeFade(iNumPin);
    animValue[iNumPin] = ((float)iValue/100.0f);
    allLEDS(getArray(iNumPin+2), animValue[iNumPin]);
  }else if(sMode=="D"){
    // whole strip --
    modeDrop(iNumPin);
    animTime[iNumPin] = basicMinDuration + (iValue/100.0f)*(basicMaxDuration-basicMinDuration);
  }else{
    // --
    return false;
  }
  
  // --
  if(iValue==0){
    stopAnim(iNumPin);
  }else if(iValue<basicMaxValueReceived){
    startAnim(iNumPin);
  }else{
    return false;
  }
  
  
  return true;
  
}

void startAnim(int numPin){
  animOnOff[numPin] = true;
}
void stopAnim(int numPin){
  animOnOff[numPin] = false;
  allLEDS(getArray(numPin+2), 0.0f);    
}
void modeFade(int numPin){
  animMode[numPin] = cModeFade;
}
void modeDrop(int numPin){
  animMode[numPin] = cModeDrop;
}
void modeNone(int numPin){
  animMode[numPin] = cModeNone;
}

void updateAnim(int numPin){
  
  if(animMode[numPin]==cModeDrop){
      // DROP --
      if(animTime[numPin]>0){
        
        float value = fmod(millis(),animTime[numPin])/(float)animTime[numPin];
      
        if(value>0 && value<=1){
          animValue[numPin] = 1-value;
        }else{
          animValue[numPin] = 0;
        }
      }

    }else if(animMode[numPin]==cModeFade){
      // FADE, pas d'anim --
    }else{
      animValue[numPin] = 0;
    }
      
  
}      
      
