#include "OnlyForUnicorns.h"
#include "gestures.h"
#include "fast_led_helpers.h"

char counter = 0;

void rainbow_loop(char jump, char roll) {
  
  float x = accel.acceleration.x;
  float y = accel.acceleration.y;
  float z = accel.acceleration.z;

  counter = (counter + roll) % 256;
  fill_rainbow(leds, NUM_LEDS, (int)(10*(x+y) + counter) % 256, jump);
  FastLED.show();
      


}

