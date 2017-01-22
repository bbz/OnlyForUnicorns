#include <Adafruit_LSM9DS0.h>
#include <FastLED.h>
#include "gestures.h"
#include "OnlyForUnicorns.h"

Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();
CRGB leds[NUM_LEDS];

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.begin(115200);

  Serial.println("Init NeoPixels...\r\n");
  FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  set_max_power_in_volts_and_milliamps(3.7, 100); // FastLED 2.1 Power management set at 3.7V, 500mA

  FastLED.setBrightness(LED_MIN_BRIGHTNESS);
  color_duration(CRGB::Blue, LED_MAX_BRIGHTNESS, INIT_FLASH_TIME);

  Serial.println("Init LSM9DS0...\r\n");
  if (!lsm.begin())
  {
    color_duration(CRGB::Red, LED_MAX_BRIGHTNESS, INIT_FLASH_TIME);
    while (1)
    {
      Serial.println("LSM9DS0 init failed.\r\n");
    }
  }
  color_duration(CRGB::Green, LED_MIN_BRIGHTNESS, INIT_FLASH_TIME);

  Serial.println("Setup LSM9DS0...\r\n");

  // 1.) Set the accelerometer range
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G); //Default
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);

  // 2.) Set the magnetometer sensitivity
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS); //Default
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);

  // 3.) Setup the gyroscope
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS); //Default
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);

  color_duration(CRGB::Purple, LED_MIN_BRIGHTNESS, INIT_FLASH_TIME);

  Serial.println("Initialization complete.\r\n");

  FastLED.setBrightness(0);
  color_duration(CRGB::Black, LED_MIN_BRIGHTNESS, 0);

  digitalWrite(13, LOW);

  return;
}


sensors_event_t accel; //, mag, gyro, temp;

uint32_t e1 = 0;

void loop() {
  bool flashed = false;

  lsm.getEvent(&accel, 0, 0, 0);

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

  return;
}

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


