#include "OnlyForUnicorns.h"
#include "gestures.h"
#include "fast_led_helpers.h"

void chromo_strobe_loop(void);
void sensorapi_loop(void);
void rainbow_loop(void);

Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();
sensors_event_t accel, mag, gyro, temp;
CRGB leds[NUM_LEDS];
char mode = 0;

void loop() {
  lsm.getEvent(&accel, &mag, &gyro, &temp);
  switch (get_mode()) {
    case 0:   chromo_strobe_loop(); break;
    case 1:   sensorapi_loop();     break;
    case 2:   rainbow_loop();       break;
  }
}

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.begin(115200);

  Serial.println("Init NeoPixels...\r\n");
  FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  //set_max_power_in_volts_and_milliamps(3.7, 100); // FastLED 2.1 Power management set at 3.7V, 500mA

  FastLED.setBrightness(LED_MIN_BRIGHTNESS);
  color_duration(CRGB::Blue, LED_MIN_BRIGHTNESS, INIT_FLASH_TIME);

  Serial.println("Init LSM9DS0...\r\n");
  if (!lsm.begin())
  {
    color_duration(CRGB::Red, LED_MIN_BRIGHTNESS, INIT_FLASH_TIME);
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
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);

  // 3.) Setup the gyroscope
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS); //Default
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);

  color_duration(CRGB::Purple, LED_MIN_BRIGHTNESS, INIT_FLASH_TIME);

  set_mode();
  
  Serial.println("Initialization complete.\r\n");

  FastLED.setBrightness(0);
  color_duration(CRGB::Black, LED_MIN_BRIGHTNESS, 0);

  digitalWrite(13, LOW);

  return;
}

void set_mode() {
  // check orientation and set mode based on it
  lsm.getEvent(&accel, &mag, &gyro, &temp);

  float x = accel.acceleration.x;
  float y = accel.acceleration.y;
  float z = accel.acceleration.z;

  float abs_x = abs(x);
  float abs_y = abs(y);
  float abs_z = abs(z);

  if (abs_z > abs_x && abs_z > abs_y) {
    if (z > 0) {
      mode = 0; // +z
    } else {
      mode = 1; // -z
    }
  } else if  (abs_y > abs_x && abs_y > abs_z) {
    if (y > 0) {
      mode = 2; // +y
    } else {
      mode = 3; // -y
    } 
  } else {
    if (x > 0) {
      mode = 4; // +x
    } else {
      mode = 5; // -x
    } 
  }
}

char get_mode() {
  return mode;
}

