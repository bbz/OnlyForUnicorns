#include <Adafruit_LSM9DS0.h>
#include <FastLED.h>

#define LED_PIN    6
#define CHIPSET    NEOPIXEL

#define LED_MATRIX_WIDTH  8
#define LED_MATRIX_HEIGHT 4
#define NUM_LEDS (LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT)
#define LED_MIN_BRIGHTNESS 4
#define LED_MAX_BRIGHTNESS 255

#define INIT_FLASH_TIME 100


#define ACCEL_THRESH        (SENSORS_GRAVITY_EARTH + 1.25)

//#define HUE_CLIP_VAL       ((SENSORS_GRAVITY_EARTH * 4)    * 1)

#define SWEEP_ACCEL_THRESH  (SENSORS_GRAVITY_EARTH - 3.50) //3.00 4.00
#define SWEEP_CLIP_VAL     ((SENSORS_GRAVITY_EARTH * 4)    * 1)
#define SWEEP_FLASH_TIME   15 //50 //100
#define SWEEP_COLOR        CRGB::Blue
//#define SWEEP_COLOR        CRGB::White

#define FULL_ON_ACCEL      ((SENSORS_GRAVITY_EARTH - 0.05) * 1)
#define FULL_ON_FLASH_TIME 100 //250
#define FULL_ON_COLOR      CRGB::White

//#define STROBE_ACCEL_THRESH (SENSORS_GRAVITY_EARTH + 4.50) //Broken sensor??
#define STROBE_ACCEL_THRESH (SENSORS_GRAVITY_EARTH + 2.25) //More sensitive
#define STROBE_CLIP_VAL    ((SENSORS_GRAVITY_EARTH * 4)    * (3.0 / 4))
//#define STROBE_CLIP_VAL    ((SENSORS_GRAVITY_EARTH * 4)    * 1)
#define STROBE_FLASH_TIME  15
#define STROBE_COLOR       CRGB::White

//#define E1_RAMP  500
#define E1_DECAY 15000 /*9000*/ /*8000*/ /*5000*/
#define E1_DIV   1000

#define Z_UP   0
#define Z_DOWN 1
#define Y_UP   2
#define Y_DOWN 3
#define X_UP   4
#define X_DOWN 5

extern Adafruit_LSM9DS0 lsm;
extern CRGB leds[NUM_LEDS];
extern sensors_event_t accel, mag, gyro, temp;

void color_duration(CRGB color, uint8_t bright, uint16_t duration);
char get_orientation(void);

