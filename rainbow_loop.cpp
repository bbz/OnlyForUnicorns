#include "OnlyForUnicorns.h"
#include "gestures.h"
#include "fast_led_helpers.h"

char counter = 0;

void rainbow_loop() {

  float x = accel.acceleration.x;
  float y = accel.acceleration.y;
  float z = accel.acceleration.z;

  //counter = (counter + 1) % 256;
  fill_rainbow(leds, NUM_LEDS, (int)(10*(x+y) + counter) % 256, 1);
  FastLED.show();
      


}

