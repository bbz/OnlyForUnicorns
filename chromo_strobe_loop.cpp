#include "OnlyForUnicorns.h"
#include "gestures.h"
#include "fast_led_helpers.h"

uint32_t e1 = 0;

bool chromo_strobe_loop() {
 
  bool flashed = false;

  float x = accel.acceleration.x;
  float y = accel.acceleration.y;
  float z = accel.acceleration.z;
  float u_x = abs(x);
  float u_y = abs(y);
  float u_z = abs(z);

  //Serial.printf("Accel X:%d\tY:%d\tZ:%d\tm/s^2\r\n", (int32_t) accel.acceleration.x, (int32_t) accel.acceleration.y, (int32_t) accel.acceleration.z);

  //Flash sweeping hue with brightness according for force if swiping left to right.
  if ((!flashed) && (u_y >= SWEEP_ACCEL_THRESH)) {
    fill_solid(leds, NUM_LEDS, SWEEP_COLOR);

    uint8_t over = u_y - SWEEP_ACCEL_THRESH;
    uint8_t bright = map(over > SWEEP_CLIP_VAL ? SWEEP_CLIP_VAL : over, 0, SWEEP_CLIP_VAL, LED_MIN_BRIGHTNESS, LED_MAX_BRIGHTNESS);

    uint32_t delta = (((bright + 1) / 1.25) * E1_DIV);
    e1 += delta;
    if (e1 > (LED_MAX_BRIGHTNESS * E1_DIV))
    {
      e1 = LED_MAX_BRIGHTNESS * E1_DIV;
    }

    FastLED.setBrightness(e1 / E1_DIV);
    FastLED.show();
    delay(SWEEP_FLASH_TIME);

    flashed = true;
  } else {
    if (e1 > 0)
    {
      uint32_t e1_decay = E1_DECAY /* * lfi */;
      if (e1 > e1_decay)
      {
        e1 -= e1_decay;
      } else {
        e1 = 0;
      }

      fill_solid(leds, NUM_LEDS, SWEEP_COLOR);
      FastLED.setBrightness((e1 / E1_DIV));
      FastLED.show();
      delay(SWEEP_FLASH_TIME);
    }
  }

  //Flash white with brightness relative to force, but skip if held upright or swiping left to right.
  if ((!flashed) && (u_x < ACCEL_THRESH) && (u_y < ACCEL_THRESH))
    //if((!flashed) && (y < ACCEL_THRESH))  //if using x for strobe duration, use this if condition
  {
    int16_t duration = STROBE_FLASH_TIME;

    flashed = flash_on_thresh(u_z, STROBE_ACCEL_THRESH, false, STROBE_CLIP_VAL, STROBE_COLOR, duration);
    if (flashed)
    {
      //Serial.printf("S\r\n");
      e1 = 0;

      color_duration(CRGB::Black, 0, 0);
    }
  }

  //Turn on full white if held upright, but skip if swiping left to right.
  if ((!flashed) && (u_y < ACCEL_THRESH)) {
    flashed = flash_on_thresh(accel.acceleration.x, (-1 * FULL_ON_ACCEL), true, 0, FULL_ON_COLOR, FULL_ON_FLASH_TIME);
    if ((!flashed) && IsXDown(x, y, z))
    {
      e1 = 0;

      color_duration(FULL_ON_COLOR, LED_MAX_BRIGHTNESS, FULL_ON_FLASH_TIME);
      color_duration(CRGB::Black, 0, 0);
    }
  }

  return true;
}



