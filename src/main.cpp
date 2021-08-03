#include "effects.h"
#include "server.h"

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
void setBrightness() {
  int value = json["val"];
  if (value < 0 || value > 255) {
    return;
    // send response back
  }
  FastLED.setBrightness(value);
  FastLED.show();
}

void setSpeed() {
  int value = json["val"];
  if (value < 0 || value > 10000) {
    Serial.println("Invalid speed value");
    return;
  }
  activePattern->setDelay(value);
}

void setColor() {
  int r = json["color"][0], g = json["color"][1], b = json["color"][2];
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
  if ((r < 0 || r > 256) || (g < 0 || g > 256) || (b < 0 || b > 256)) {
    Serial.println("Invalid RGB value");
    return;
  }
  ColoredDrawable* coloredActivePattern = dynamic_cast<ColoredDrawable*>(activePattern);
  if (!coloredActivePattern) {
    Serial.println("Cannot change color for selected pattern");
    return;
  }
  coloredActivePattern->color = CRGB(r,g,b);
}

void setEffect() {
  isPatternStarted = false;
  activePatternIndex = json["index"];
  activePattern = patterns[activePatternIndex];
  Serial.print("Activating: ");
  Serial.println(activePattern->name);
}
// END commands

void execute(const char* op) {
  if (strcmp(op, "brightness") == 0) {
    setBrightness();
  } else if (strcmp(op, "effect") == 0) {
    setEffect();
  } else if (strcmp(op, "speed") == 0) {
    setSpeed();
  } else if (strcmp(op, "color") == 0) {
    setColor();
  }
  json.clear();
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
