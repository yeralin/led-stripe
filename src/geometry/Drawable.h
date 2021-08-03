
#include <Arduino.h>
#include "leds.h"
#ifndef Drawable_H
#define Drawable_H

class Drawable {
 public:
  char* name;
  int returnDelay;

  // a single frame should be drawn as fast as possible, without any delay or
  // blocking return how many millisecond delay is requested before the next
  // call to drawFrame()
  virtual unsigned int drawFrame() {
    // backgroundLayer.fillScreen({ 0, 0, 0 });
    return 0;
  };

  virtual void start(){};
  virtual void stop(){};

  virtual void setDelay(int returnDelay) {
    this->returnDelay = returnDelay;
  }

};

class ColoredDrawable: public Drawable {
 public:
  CRGB color;

  virtual void setColor(CRGB color) {
    this->color = color;
  }

};

class WheelDrawable : public Drawable {
 public:
  // used by rainbowCycle and theaterChaseRainbow
  virtual byte* wheel(byte WheelPos) {
    static byte c[3];

    if (WheelPos < 85) {
      c[0] = WheelPos * 3;
      c[1] = 255 - WheelPos * 3;
      c[2] = 0;
    } else if (WheelPos < 170) {
      WheelPos -= 85;
      c[0] = 255 - WheelPos * 3;
      c[1] = 0;
      c[2] = WheelPos * 3;
    } else {
      WheelPos -= 170;
      c[0] = 0;
      c[1] = WheelPos * 3;
      c[2] = 255 - WheelPos * 3;
    }

    return c;
  }
};

#endif