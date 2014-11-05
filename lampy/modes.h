#ifndef MODES_H
#define MODES_H

#include <FastLED.h>

typedef void (*voidfunc_t)(CRGB*, uint8_t);
typedef struct {
  int delay;
  voidfunc_t func;
} mode_entry_t;
int step=0;

void all_blue(CRGB* leds, uint8_t numPix) {
  FastLED.showColor(CRGB::Blue);
}

void off(CRGB* leds, uint8_t numPix) {
  FastLED.clear();
  FastLED.show();
}

void all_red(CRGB* leds, uint8_t numPix) {
  FastLED.showColor(CRGB::Red);
}

void all_green(CRGB* leds, uint8_t numPix) {
  FastLED.showColor(CRGB::Green);
}

void police(CRGB* leds, uint8_t numPix) {
  for(int i=0;i<numPix;i++) {
    leds[i] = (i+step) % 2 ? CRGB::Blue : CRGB::Red;
  }
  step = (step+1) % 2;
  FastLED.show();
}

void random_decay(CRGB *leds, uint8_t numPix) {
  int r, g, b;
  for(int i=0;i<numPix;i++) {
    r = leds[i].r;
    g = leds[i].g;
    b = leds[i].b;
    
    if (i==0) {
      r = (r + leds[i+1].r) / 2;
      g = (g + leds[i+1].g) / 2;
      b = (b + leds[i+1].b) / 2;
    } else if (i==numPix-1) {
      r = (r + leds[i-1].r) / 2;
      g = (g + leds[i-1].g) / 2;
      b = (b + leds[i-1].b) / 2;
    } else {
      r = (r + leds[i+1].r + leds[i-1].r) / 3;
      g = (g + leds[i+1].g + leds[i-1].g) / 3;
      b = (b + leds[i+1].b + leds[i-1].b) / 3;
    }
    
    leds[i].r = max(qadd8(r, 0) - 5, 0);
    leds[i].g = max(qadd8(g, 0) - 5, 0);
    leds[i].b = max(qadd8(b, 0) - 5, 0);
  }
  
  if (random8(100) < 10) { 
    leds[random(numPix)] = CRGB(random8(), random8(), random8());
  }
  FastLED.show();
}

#endif
