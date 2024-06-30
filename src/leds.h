#include <FastLED.h>

// Information about the LED strip itself
#define LED_PIN 27
#define INITIAL_BRIGHTNESS 128
#define NUM_LEDS    80
#define CHIPSET     WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void setupLeds() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(INITIAL_BRIGHTNESS);
}
