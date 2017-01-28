#include "OnlyForUnicorns.h"
#include "gestures.h"
#include "fast_led_helpers.h"

void rainbow_loop() {

  float x = accel.acceleration.x;
  float y = accel.acceleration.y;
  float z = accel.acceleration.z;

  fill_rainbow(leds, NUM_LEDS, (int)(10*(x+y)) % 256, 1);
  FastLED.show();
      


}

