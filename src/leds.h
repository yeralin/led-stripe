#include <FastLED.h>

// Information about the LED strip itself
#define LED_PIN 27
#define INITIAL_BRIGHTNESS 128
#define NUM_LEDS    300
#define CHIPSET     WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void setupLeds() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection(TypicalSMD5050);
  FastLED.setBrightness(INITIAL_BRIGHTNESS);
}