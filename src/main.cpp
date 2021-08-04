#include "server.h"
#include "visuals.h"

FadeInOut fadeInOut(CRGB::Aqua, 0);
Strobe strobe(CRGB::Aqua, 10);
CycloneBounce cyclonBounce(CRGB::Aqua, 4, 10);
NewKITT newKITT(CRGB::Aqua, 8, 10);
Twinkle twinkle(CRGB::Aqua, 80);
TwinkleRandom twinkleRandom(20);
RunningLights runningLights(CRGB::Aqua, 40);
ColorWipe colorWipe(CRGB::Aqua, 30);
RainbowCycle rainbowCycle(30);
TheaterChaseRainbow theaterChaseRainbow(50);
Fire fire(55, 120, 30);
MeteorRain meteorRain(CRGB::Aqua, 10, 64, true, 30);

Drawable* patterns[] = {&fadeInOut,     &strobe,
                        &cyclonBounce,  &newKITT,
                        &twinkle,       &twinkleRandom,
                        &runningLights, &colorWipe,
                        &rainbowCycle,  &theaterChaseRainbow,
                        &fire,          &meteorRain};

bool isPatternStarted = false;
int activePatternIndex = 3;
Drawable* activePattern = patterns[activePatternIndex];

// START commands

void reportError(const char* err) {
  json.clear();
  json["error"] = err;
  sendData();
}

void sync() {
  json.clear();
  ColoredDrawable* coloredActivePattern = dynamic_cast<ColoredDrawable*>(activePattern);
  if (coloredActivePattern) {
    CRGB color = coloredActivePattern->color;
    JsonArray rgb = json.createNestedArray("color");
    rgb.add(color.r);
    rgb.add(color.g);
    rgb.add(color.b);
  }
  json["speed"] = activePattern->returnDelay;
  json["brightness"] = FastLED.getBrightness();
  json["visual"] = activePattern->name;
  json["index"] = activePatternIndex;
  sendData();
}

void setBrightness() {
  int value = json["val"];
  if (value < 0 || value > 255) {
    reportError("Invalid brightness value, allowed range [0, 255]");
    return;
  }
  FastLED.setBrightness(value);
  FastLED.show();
}

void setSpeed() {
  int value = json["val"];
  if (value < 0 || value > 10000) {
    reportError("Invalid speed value, allowed range [0, 10000] ms");
    return;
  }
  activePattern->setDelay(value);
}

void setColor() {
  int r = json["val"][0], g = json["val"][1], b = json["val"][2];
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
  if ((r < 0 || r > 256) || (g < 0 || g > 256) || (b < 0 || b > 256)) {
    reportError("Invalid RGB value, allowed range [0, 255]");
    return;
  }
  ColoredDrawable* coloredActivePattern = dynamic_cast<ColoredDrawable*>(activePattern);
  if (!coloredActivePattern) {
    reportError("Cannot change color for selected pattern");
    return;
  }
  coloredActivePattern->color = CRGB(r,g,b);
}

void setVisual() {
  isPatternStarted = false;
  activePatternIndex = json["val"];
  if (activePatternIndex < 0 || activePatternIndex > 11) {
    reportError("Invalid active pattern index value, allowed range [0, 11]");
    return;
  }
  activePattern = patterns[activePatternIndex];
  Serial.print("Activating: ");
  Serial.println(activePattern->name);
}
// END commands

void execute(const char* op) {
  if (strcmp(op, "brightness") == 0) {
    setBrightness();
  } else if (strcmp(op, "visual") == 0) {
    setVisual();
  } else if (strcmp(op, "speed") == 0) {
    setSpeed();
  } else if (strcmp(op, "color") == 0) {
    setColor();
  }
  sync();
}

void loop() {
  if (!isPatternStarted) {
    activePattern->start();
    isPatternStarted = true;
  }
  int requestedDelay = activePattern->drawFrame();
  delay(requestedDelay);
}

void setup() {
  delay(3000);  // power-up safety delay
  Serial.begin(115200);
  setupServer();
  setupLeds();
}
