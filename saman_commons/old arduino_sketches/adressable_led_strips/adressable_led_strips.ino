/* Digital RGB LED Stripe testprogram
/* for Stripe Type Art Nr: EKXMAGICXSTRIPIP45
/*
 * Lab3 12/2011 Nawrath
 * Kunsthochschule fuer Medien Koeln
 * Academy of Media Arts Cologne
 * http://interface.khm.de

thanks to adafruit, ideas have be taken from the
Arduino library for the  LPD8806-based strip.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see .
 */

int datapin = 2;  // Led Strip St Pin
int clockpin = 3; // Led Strip Ci Pin
int pinLed=13;    // Arduino Led Pin 13

#define nled 33  // total number of controlled leds

unsigned int rgbPixel[nled]; // rgb values of whole led strip
unsigned int cc;  // color
int r,g,b;

void setup() {

  Serial.begin(115200);
  Serial.println("DIGITAL LED STRIP TEST");

  pinMode(clockpin,OUTPUT);
  pinMode(datapin,OUTPUT);
  pinMode(pinLed,OUTPUT);

  r = random(31);
  g = random(31);
  b = random(31);

}

//*********************************************************
//*********************************************************

void loop() {

// uncomment function :  

   //colorChase(wheel(random(96)),50) ;
  //  randomFlash(50);
    rainbowWheel(100,2);
  //  test();
  // lightBarPot();

  digitalWrite(pinLed,!digitalRead(pinLed));  // blink arduino led pin 13

}

//*********************************************************
// send color values from pixel array to led strip
void show() {
  unsigned int ii,b1;
  for (ii=0; ii < nled; ii++ ) {

    bitWrite(PORTD,datapin,HIGH);
    bitWrite(PORTD,clockpin, HIGH);
    bitWrite(PORTD,clockpin, LOW);

    for ( b1=0x4000; b1; b1 >>= 1) {

      if(rgbPixel[ii] & b1) bitWrite(PORTD,datapin, HIGH);
      else                bitWrite(PORTD,datapin, LOW);
      bitWrite(PORTD,clockpin, HIGH);
      bitWrite(PORTD,clockpin, LOW);
    }
  }
  latchLeds(nled);
}
//*********************************************************
// activate new color pattern in ledstrip
void latchLeds(int n) {

  bitWrite(PORTD,datapin, LOW);
  for(int i = 8 * n; i>0; i--) {
    bitWrite(PORTD,clockpin, HIGH);
    bitWrite(PORTD,clockpin, LOW);
  }

}
//*********************************************************
// set color value as rgb argument , values = 0..31
unsigned int color(int r, int g, int b) {
  // color are 3 groups of 5 bit (0..31)

  r=r & 0x1F;
  g=g & 0x1F;
  b=b & 0x1F;
  return  (b << 10) | (r << 5) | g;
}
//*********************************************************
void colorChase(unsigned int c, int wait) {
  int i;
  for (i=0; i < nled; i++) {
    rgbPixel[i]= c;
    if (i == 0) {
      rgbPixel[nled-1]= 0;
    }
    else {
      rgbPixel[i-1]= 0;
    }
    show();
    delay(wait);

  }
}

//*********************************************************
void rainbowWheel(int wait,int wave) {
  static int si;
  int ii;

  for (int i=0; i < nled; i++) {
    ii=i+si;
    if (ii > 95 ) ii=ii-96;
    rgbPixel[i]=wheel(ii);
  }
  si=si+wave;
  if (si > 95 ) si=0;

  show();
  delay(wait);

}
//*********************************************************
void randomFlash(int wait) {
  for (int i=0; i < nled; i++) rgbPixel[i]=0x0;
  r = random(31);
  g = random(31);
  b = random(31);
  rgbPixel[random(nled)]= color(r,g,b);
  show();

  delay(wait);
}

//*********************************************************
// colorwheel with 96 colors, 0..95
unsigned int wheel(int WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 32)
  {
  case 0:
    r = 31 - WheelPos % 32;   //Red down
    g = WheelPos % 32;      // Green up
    b = 0;                  //blue off
    break;
  case 1:
    g = 31 - WheelPos % 32;  //green down
    b = WheelPos % 32;      //blue up
    r = 0;                  //red off
    break;
  case 2:
    b = 31 - WheelPos % 32;  //blue down
    r = WheelPos % 32;      //red up
    g = 0;                  //green off
    break;
  }

  return(color(r,g,b));
}

//*********************************************************
void test() {
  int aa;

  pinMode(14,OUTPUT);    // connect a potentiometer with input,wiper,output to
  pinMode(16,OUTPUT);    // analog pins A1,A2,A3
  digitalWrite(16,1);    //
  aa= analogRead(15)/32;

  rgbPixel[0]=color(aa,0,0);
  rgbPixel[1]=color(0,aa,0);
  rgbPixel[2]=color(0,0,aa);
  show();
  delay(100);
}

//*********************************************************
// light bar controlled by analog pin 1 ( potentiometer )
void lightBarPot() {
  int aa;

  pinMode(14,OUTPUT);    // connect a potentiometer with input,wiper,output to
  pinMode(16,OUTPUT);    // analog pins A1 pot+,A2 pot-wiper,A3 pot-
  digitalWrite(16,1);    //
  aa= analogRead(15);
  Serial.println(aa);
  aa= (500-aa);
  aa=aa/5;

  aa=aa+25;

  for (int i=0; i < nled; i++) {

    rgbPixel[i]=0x0;
    if (i < aa) rgbPixel[i]=color(31,31,31);
  }

  show();
  delay(5);
}
