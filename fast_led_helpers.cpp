#include "fast_led_helpers.h"
#include "OnlyForUnicorns.h"

void color_duration(CRGB color, uint8_t bright, uint16_t duration) {
  fill_solid(leds, NUM_LEDS, color);
  FastLED.setBrightness(bright);
  FastLED.show();
  if (duration) {
    delay(duration);
  }
  return;
}

bool flash_on_thresh(float value, float thresh, bool neg, float clip, CRGB color, uint16_t duration) {
  uint8_t over;
  bool flash = false;

  if (!neg) {
    if (value > thresh) {
      over = value - thresh;
      flash = true;
    }
  } else {
    if (value < thresh) {
      over = thresh - value;
      flash = true;
    }
  }

  if (flash) {
    uint8_t bright;

    if (clip == 0) {
      bright = LED_MAX_BRIGHTNESS;
    } else {
      bright = map(over > clip ? clip : over, 0, clip, LED_MIN_BRIGHTNESS, LED_MAX_BRIGHTNESS);
    }

    color_duration(color, bright, duration);
  }

  return flash;
}


