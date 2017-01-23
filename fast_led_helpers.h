#include <Adafruit_LSM9DS0.h>
#include <FastLED.h>

void color_duration(CRGB color, uint8_t bright, uint16_t duration);
bool flash_on_thresh(float value, float thresh, bool neg, float clip, CRGB color, uint16_t duration);


