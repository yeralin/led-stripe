#include "geometry/Drawable.h"

// ***************************************
// ** FastLed/NeoPixel Common Functions **
// ***************************************

// Apply LED color changes
void showStrip() { FastLED.show(); }

// Set a LED color (not yet visible)
void setPixel(int pixel, byte red, byte green, byte blue) {
  // FastLED
  leds[pixel].r = red;
  leds[pixel].g = green;
  leds[pixel].b = blue;
}

// Set all LEDs to a given color and apply it (visible)
void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

// Set a LED color (not yet visible)
void setPixel(int pixel, CRGB color) {
  leds[pixel].r = color.red;
  leds[pixel].g = color.green;
  leds[pixel].b = color.blue;
}

// Set all LEDs to a given color and apply it (visible)
void setAll(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    setPixel(i, color);
  }
  showStrip();
}

class FadeInOut : public ColoredDrawable {
 private:
  float i = 0;
  bool flip = false;

 public:
  explicit FadeInOut(CRGB color, int returnDelay) {
    this->name = (char*)"FadeInOut";
    this->returnDelay = returnDelay;
    this->color = color;
  }

  void start() override {
    i = 0;
    flip = false;
    setAll(0, 0, 0);
    showStrip();
  }

  unsigned int drawFrame() override {
    if (i == 256 || i == 0) {
      flip = !flip;
    }
    float multiplier = i / 256;
    float r = multiplier * color.red;
    float g = multiplier * color.green;
    float b = multiplier * color.blue;
    setAll(r, g, b);
    showStrip();
    i = flip ? i + 1 : i - 1;
    return returnDelay;
  }
};

class Strobe : public ColoredDrawable {
 public:
  explicit Strobe(CRGB color, int returnDelay) {
    this->name = (char*)"Strobe";
    this->returnDelay = returnDelay;
    this->color = color;
  }

  void start() override {
    setAll(color);
    showStrip();
  }

  unsigned int drawFrame() override {
    setAll(color);
    showStrip();
    delay(returnDelay);
    setAll(0, 0, 0);
    showStrip();
    return returnDelay;
  }
};

class CycloneBounce : public ColoredDrawable {
 private:
  int eyeSize;
  int i;
  bool flip;

 public:
  explicit CycloneBounce(CRGB color, int eyeSize, int returnDelay)
      : eyeSize(eyeSize) {
    this->name = (char*)"CycloneBounce";
    this->returnDelay = returnDelay;
    this->color = color;
  }

  void start() override {
    i = 1;
    flip = false;
    setAll(0, 0, 0);
    showStrip();
  }

  unsigned int drawFrame() override {
    if (i == NUM_LEDS - eyeSize - 3 || i == 0) {
      flip = !flip;
    }
    setAll(0, 0, 0);
    setPixel(i, color.red / 10, color.green / 10, color.blue / 10);
    for (int j = 1; j <= eyeSize; j++) {
      setPixel(i + j, color.red, color.green, color.blue);
    }
    setPixel(i + eyeSize + 1, color.red / 10, color.green / 10,
             color.blue / 10);
    showStrip();
    i = flip ? i + 1 : i - 1;
    return returnDelay;
  }
};

class NewKITT : public ColoredDrawable {
 private:
  int eyeSize;
  int i;
  int turn;

  // used by NewKITT
  static void centerToOutside(byte red, byte green, byte blue, int eyeSize,
                              int i) {
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= eyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + eyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= eyeSize; j++) {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - eyeSize - 1, red / 10, green / 10, blue / 10);
  }

  // used by NewKITT
  static void outsideToCenter(byte red, byte green, byte blue, int eyeSize,
                              int i) {
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= eyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + eyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= eyeSize; j++) {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - eyeSize - 1, red / 10, green / 10, blue / 10);
  }

  // used by NewKITT
  static void leftToRight(byte red, byte green, byte blue, int eyeSize, int i) {
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= eyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + eyeSize + 1, red / 10, green / 10, blue / 10);
  }

  // used by NewKITT
  static void rightToLeft(byte red, byte green, byte blue, int eyeSize, int i) {
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= eyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + eyeSize + 1, red / 10, green / 10, blue / 10);
  }

  void next() {
    turn++;
    i = -1;
    setAll(0, 0, 0);
  }

 public:
  explicit NewKITT(CRGB color, int eyeSize, int returnDelay)
      : eyeSize(eyeSize) {
    this->name = (char*)"NewKITT";
    this->returnDelay = returnDelay;
    this->color = color;
  }

  void start() override {
    i = -1;
    turn = 0;
    setAll(0, 0, 0);
    showStrip();
  }

  unsigned int drawFrame() override {
    byte red = color.red;
    byte green = color.green;
    byte blue = color.blue;
    switch (turn) {
      case 0:
        // Start
        if (i == -1) {
          i = NUM_LEDS - eyeSize - 2;
        }
        rightToLeft(red, green, blue, eyeSize, i);
        i--;
        // End
        if (i == 0) next();
        break;
      case 1:
        if (i == -1) {
          i = 0;
        }
        leftToRight(red, green, blue, eyeSize, i);
        i++;
        if (i == NUM_LEDS - eyeSize - 2) next();
        break;
      case 2:
        if (i == -1) {
          i = (NUM_LEDS - eyeSize) / 2;
        }
        centerToOutside(red, green, blue, eyeSize, i);
        i--;
        if (i == 0) next();
        break;
      case 3:
        if (i == -1) {
          i = 0;
        }
        outsideToCenter(red, green, blue, eyeSize, i);
        i++;
        if (i == (NUM_LEDS - eyeSize) / 2) next();
        break;
      default:
        turn = 0;
        break;
    }
    showStrip();
    return returnDelay;
  }
};

class Twinkle : public ColoredDrawable {
 public:
  explicit Twinkle(CRGB color, int returnDelay) {
    this->name = (char*)"Twinkle";
    this->returnDelay = returnDelay;
    this->color = color;
  }

  void start() override {
    setAll(0, 0, 0);
    showStrip();
  }

  unsigned int drawFrame() override {
    setAll(0, 0, 0);
    setPixel(random(NUM_LEDS), color.red, color.green, color.blue);
    showStrip();
    return returnDelay;
  }
};

class TwinkleRandom : public Drawable {

 public:
  explicit TwinkleRandom(int speedDelay) {
    name = (char*)"TwinkleRandom";
    this->returnDelay = returnDelay;
  }

  void start() override {
    setAll(0, 0, 0);
    showStrip();
  }

  unsigned int drawFrame() override {
    setAll(0, 0, 0);
    setPixel(random(NUM_LEDS), random(0, 255), random(0, 255), random(0, 255));
    showStrip();
    return returnDelay;
  }
};

class RunningLights : public ColoredDrawable {
 private:
  int position;
  int i;

 public:
  explicit RunningLights(CRGB color, int returnDelay) {
    this->name = (char*)"RunningLights";
    this->returnDelay = returnDelay;
    this->color = color;
  }

  void start() override {
    setAll(0, 0, 0);
    showStrip();
    position = 0;
    i = 0;
  }

  unsigned int drawFrame() override {
    if (i == NUM_LEDS * 2) {
      i = 0;
      position = 0;
    }
    position++;
    i++;
    for (int j = 0; j < NUM_LEDS; j++) {
      // sine wave, 3 offset waves make a rainbow!
      // float level = sin(i+Position) * 127 + 128;
      // setPixel(i,level,0,0);
      // float level = sin(i+Position) * 127 + 128;
      setPixel(j, ((sin(j + position) * 127 + 128) / 255) * color.red,
               ((sin(j + position) * 127 + 128) / 255) * color.green,
               ((sin(j + position) * 127 + 128) / 255) * color.blue);
    }
    showStrip();
    return returnDelay;
  }
};

class ColorWipe : public ColoredDrawable {
 private:
  CRGB colorClear;
  int i;
  bool flip;

 public:
  explicit ColorWipe(CRGB colorFill, int returnDelay) {
    this->name = (char*)"ColorWipe";
    this->returnDelay = returnDelay;
    this->color = color;
    colorClear = CRGB(0, 0, 0);
  }

  void start() override {
    setAll(0, 0, 0);
    showStrip();
    i = 0;
    flip = true;
  }

  unsigned int drawFrame() override {
    if (i == NUM_LEDS) {
      i = 0;
      flip = !flip;
    }
    CRGB color = flip ? color : colorClear;
    setPixel(i, color.red, color.green, color.blue);
    showStrip();
    i++;
    return returnDelay;
  }

};

class RainbowCycle : public WheelDrawable {
 private:
  int i;

 public:
  explicit RainbowCycle(int returnDelay) {
    this->name = (char*)"RainbowCycle";
    this->returnDelay = returnDelay;
  }

  void start() override {
    i = 0;
    setAll(0, 0, 0);
    showStrip();
  }

  unsigned int drawFrame() override {
    if (i == 256 * 5) {
      i = 0;
    }
    for (int j = 0; j < NUM_LEDS; j++) {
      byte* c = wheel(((j * 256 / NUM_LEDS) + i) & 255);
      setPixel(j, *c, *(c + 1), *(c + 2));
    }
    showStrip();
    i++;
    return returnDelay;
  }
};

class TheaterChaseRainbow : public WheelDrawable {
 private:
  int j;

 public:
  explicit TheaterChaseRainbow(int returnDelay) {
    name = (char*)"TheaterChaseRainbow";
    this->returnDelay = returnDelay;
  }

  void start() override {
    j = 0;
    setAll(0, 0, 0);
    showStrip();
  }

  unsigned int drawFrame() override {
    if (j == 256) {
      j = 0;
    }
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        byte* c = wheel((i + j) % 255);
        setPixel(i + q, *c, *(c + 1), *(c + 2));  // turn every third pixel on
      }
      showStrip();
      delay(returnDelay);
      for (int i = 0; i < NUM_LEDS; i = i + 3) {
        setPixel(i + q, 0, 0, 0);  // turn every third pixel off
      }
    }
    j++;
    return returnDelay;
  }
};

class Fire : public Drawable {
 private:
  byte heat[NUM_LEDS];
  int cooldown;

  int cooling;
  int sparking;

 public:
  const char* name = (char*)"Fire";
  explicit Fire(int cooling, int sparking, int returnDelay)
      : cooling(cooling), sparking(sparking) {
    this->returnDelay = returnDelay;
  }

  void start() override {
    setAll(0, 0, 0);
    showStrip();
  }

  void setPixelHeatColor(int Pixel, byte temperature) {
    // Scale 'heat' down from 0-255 to 0-191
    byte t192 = round((temperature / 255.0) * 191);

    // calculate ramp up from
    byte heatramp = t192 & 0x3F;  // 0..63
    heatramp <<= 2;               // scale up to 0..252

    // figure out which third of the spectrum we're in:
    if (t192 > 0x80) {  // hottest
      setPixel(Pixel, 255, 255, heatramp);
    } else if (t192 > 0x40) {  // middle
      setPixel(Pixel, 255, heatramp, 0);
    } else {  // coolest
      setPixel(Pixel, heatramp, 0, 0);
    }
  }

  unsigned int drawFrame() override {
    // Step 1.  Cool down every cell a little
    for (int i = 0; i < NUM_LEDS; i++) {
      cooldown = random(0, ((cooling * 10) / NUM_LEDS) + 2);

      if (cooldown > heat[i]) {
        heat[i] = 0;
      } else {
        heat[i] = heat[i] - cooldown;
      }
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if (random(255) < sparking) {
      int y = random(7);
      heat[y] = heat[y] + random(160, 255);
      // heat[y] = random(160,255);
    }

    // Step 4.  Convert heat to LED colors
    for (int j = 0; j < NUM_LEDS; j++) {
      setPixelHeatColor(j, heat[j]);
    }

    showStrip();
    return returnDelay;
  }
};

class MeteorRain : public ColoredDrawable {
 private:
  byte meteorSize;
  byte meteorTrailDecay;
  boolean meteorRandomDecay;
  int i;

 public:
  explicit MeteorRain(CRGB color, byte meteorSize, byte meteorTrailDecay,
                      boolean meteorRandomDecay, int returnDelay)
      : meteorSize(meteorSize),
        meteorTrailDecay(meteorTrailDecay),
        meteorRandomDecay(meteorRandomDecay) {
    this->name = (char*)"MeteorRain";
    this->returnDelay = returnDelay;
    this->color = color;
    i = 0;
  }

  void start() override {
    i = 0;
    setAll(0, 0, 0);
    showStrip();
  }

  unsigned int drawFrame() override {
    if (i == NUM_LEDS) {
      setAll(0, 0, 0);
      i = 0;
    }
    // fade brightness all LEDs one step
    for (int j = 0; j < NUM_LEDS; j++) {
      if ((!meteorRandomDecay) || (random(10) > 5)) {
        leds[j].fadeToBlackBy(meteorTrailDecay);
      }
    }

    for (int j = 0; j < meteorSize; j++) {
      if ((i - j < NUM_LEDS) && (i - j >= 0)) {
        setPixel(i - j, color.red, color.green, color.blue);
      }
    }
    showStrip();
    i++;
    return returnDelay;
  };
};
