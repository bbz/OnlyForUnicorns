/********************************************************************************************************/
/* This is from https://raw.githubusercontent.com/FastLED/FastLED/master/examples/Fire2012/Fire2012.ino */
/********************************************************************************************************/

//#include <FastLED.h>
#include "OnlyForUnicorns.h"

//#define LED_PIN     5
//#define COLOR_ORDER GRB
//#define CHIPSET     WS2811
//#define NUM_LEDS    30

//#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60

char orientation = 0;

/*
CRGB leds[NUM_LEDS];

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
}
*/

void fire_loop_int(char type);
void Fire2012(char type);

void fire_loop() {
  fire_loop_int(0);
}

void fire_loop_int(char type) {
  // type 0 = fire
  // type 1 = rainbow
  
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

  orientation = get_orientation();

  Fire2012(type); // run simulation frame
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  75

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void Fire2012(char type)
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      switch (type) {
        case 0:
          heat[i] = qsub8( heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
          //heat[i] = qsub8( heat[i],  random8(50));
          //heat[i] = heat[i] - min(heat[i], random8(8));
          break;
        case 1:
          heat[i] = qsub8( heat[i], random8(0, ((COOLING * 7) / NUM_LEDS) + 2));
          break;
      }
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
//    for( int k= NUM_LEDS - 1; k >= 2; k--) {
//      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
//    }
    for( int k= NUM_LEDS; k >= 4; k--) {
      switch (k % 4) {
      case 0: heat[k] = (5 * heat[k] + heat[k - 3] + 3 * heat[k - 4]               ) / 10; break;
      case 1:
      case 2: heat[k] = (5 * heat[k] + heat[k - 3] + 3 * heat[k - 4] + heat[k - 5] ) / 10; break;
      case 3: heat[k] = (5 * heat[k] +             + 3 * heat[k - 4] + heat[k - 5] ) / 10; break;
      }
    }
//    for( int k= 8; k >= 4; k--) {
//      heat[k] = (heat[k - 3] + heat[k - 4] ) / 2;
//    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    switch (type) {
      case 0:
        if(random8() < SPARKING ) {
          char y = random8(4);
          heat[y] = qadd8( heat[y], random8(160,255) );
        }
        break;
      case 1:
        if(random8() < SPARKING ) {
          char y = random8(8);
          heat[y] = qadd8( heat[y], random8(160,255) );
        }
        break;
    }
    
    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      char this_heat = heat[j];
      CRGB color;
      switch (type) {
        case 0:
          if (this_heat > 32) { this_heat = this_heat - random8(32); } // add flicker
          if (j < 4) { this_heat = max(80, this_heat); } // keep bottom row warm
          this_heat = min(this_heat, 170); // prevent white pixels
          color = HeatColor(this_heat);
          break;
        case 1:
          if (j < 4) { this_heat = max(80, this_heat); } // keep bottom row warm
          CHSV hsv;
          hsv.hue = 255 - this_heat;
          if (this_heat < 40) {
            hsv.val = 0;
          } else {
            hsv.val = 255;
          }
          hsv.sat = 240;
          color = hsv;            
          break;
      }
      int pixelnumber;
      if( orientation == X_DOWN || orientation == Y_UP ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      if (orientation == Y_UP || orientation == Y_DOWN) {
        leds[pixelnumber] = color;
      } else {
        leds[ (int)(pixelnumber / 4) + (pixelnumber % 4) * 8  ] = color;
      }
    }
}

