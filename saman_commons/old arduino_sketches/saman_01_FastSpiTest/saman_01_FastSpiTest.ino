// Uncomment this line if you have any interrupts that are changing pins - this causes the library to be a little bit more cautious
// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1

// Uncomment this line to force always using software, instead of hardware, SPI (why?)
// #define FORCE_SOFTWARE_SPI 1

// Uncomment this line if you want to talk to DMX controllers
// #define FASTSPI_USE_DMX_SIMPLE 1

#include "FastSPI_LED2.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// test code
//
//////////////////////////////////////////////////

#define NUM_LEDS 33

CRGB leds_1[NUM_LEDS];
CRGB leds_2[NUM_LEDS];
CRGB leds_3[NUM_LEDS];
CRGB leds_4[NUM_LEDS];
CRGB leds_5[NUM_LEDS];

void setup() {
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);

   	// For safety (to prevent too high of a power draw), the test case defaults to
   	// setting brightness to 25% brightness
   	LEDS.setBrightness(64);

   	LEDS.addLeds<WS2811, 2>(leds_1, NUM_LEDS);
        LEDS.addLeds<WS2811, 3>(leds_2, NUM_LEDS);
        LEDS.addLeds<WS2811, 4>(leds_3, NUM_LEDS);
        LEDS.addLeds<WS2811, 5>(leds_4, NUM_LEDS);
        LEDS.addLeds<WS2811, 6>(leds_5, NUM_LEDS);
}

void drop(CRGB _leds[NUM_LEDS], int _delay, CRGB _color){
  for(int iLed = 0; iLed < NUM_LEDS; iLed++) {
	memset(_leds, 0,  NUM_LEDS * sizeof(struct CRGB));
        _leds[iLed] = _color;
        // and now, show your led array! 
        LEDS.setBrightness(255);
	LEDS.show();
        delay(_delay);
  }
  
  LEDS.setBrightness(0);
  LEDS.show();

}

void loop() { 
        
        drop(leds_1, 10, CRGB(255,255,255));
        drop(leds_2, 10, CRGB(255,255,255));
        drop(leds_3, 10, CRGB(255,255,255));
        drop(leds_4, 10, CRGB(255,255,255));
        drop(leds_5, 10, CRGB(255,255,255));
        
        LEDS.setBrightness(0);
	LEDS.show();
        delay(1000);
  
        /*
	for(int i = 0; i < 3; i++) {
		for(int iLed = 0; iLed < NUM_LEDS; iLed++) {
			memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));

			switch(i) { 
				// You can access the rgb values by field r, g, b
			 	case 0: leds[iLed].r = 128; break;

			 	// or by indexing into the led (r==0, g==1, b==2) 
			 	case 1: leds[iLed][i] = 128; break;

			 	// or by setting the rgb values for the pixel all at once
			 	case 2: leds[iLed] = CRGB(0, 0, 128); break;
			}

			// and now, show your led array! 
			LEDS.show();
			delay(10);
		}

		// fade up
		for(int x = 0; x < 128; x++) { 
			// The showColor method sets all the leds in the strip to the same color
			LEDS.showColor(CRGB(x, 0, 0));
			delay(10);
		}

		// fade down
		for(int x = 128; x >= 0; x--) { 
			LEDS.showColor(CRGB(x, 0, 0));
			delay(10);
		}

		// let's fade up by scaling the brightness
		for(int scale = 0; scale < 128; scale++) { 
			LEDS.showColor(CRGB(0, 128, 0), scale);
			delay(10);
		}

		// let's fade down by scaling the brightness
		for(int scale = 128; scale > 0; scale--) { 
			LEDS.showColor(CRGB(0, 128, 0), scale);
			delay(10);
		}
	}
        */
}
