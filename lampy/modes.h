#ifndef MODES_H
#define MODES_H

#include <FastLED.h>

#define LERP_DURATION 5000

typedef void (*voidfunc_t)(CRGB*, uint8_t);
typedef struct {
  int delay;
  voidfunc_t func;
} mode_entry_t;
typedef struct {
  const uint32_t *palette;
  TBlendType blend;
} palette_entry_t;
int step=0;
uint8_t startIndex = 0;
CRGBPalette16 currentPalette = RainbowColors_p;
TBlendType    currentBlend = BLEND;

////////////////////////////////////////////////////////////////////
// Mode off - by Russell Hay
// Turns off all of the leds
void off(CRGB* leds, uint8_t numPix) {
  FastLED.clear();
  FastLED.show();
}

////////////////////////////////////////////////////////////////////
// Mode White - by Russell Hay
// Turns all of the leds grey
void all_white(CRGB* leds, uint8_t numPix) {
  FastLED.showColor(CRGB::White);
}


////////////////////////////////////////////////////////////////////
// Mode Blue - by Russell Hay
// Turns all of the leds Blue
void all_blue(CRGB* leds, uint8_t numPix) {
  FastLED.showColor(CRGB::Blue);
}

////////////////////////////////////////////////////////////////////
// Mode Red - by Russell Hay
// Turns all of the leds Red
void all_red(CRGB* leds, uint8_t numPix) {
  FastLED.showColor(CRGB::Red);
}

////////////////////////////////////////////////////////////////////
// Mode Green - by Russell Hay
// Turns all of the leds Green
void all_green(CRGB* leds, uint8_t numPix) {
  FastLED.showColor(CRGB::Green);
}

////////////////////////////////////////////////////////////////////
// Mode Police - by Russell Hay
// Alternating Red and Blue leds that switch color every half second
void police(CRGB* leds, uint8_t numPix) {
  fill_solid(leds, numPix/2, step ? CRGB::Blue : CRGB::Red);
  fill_solid(&(leds[numPix/2]), numPix/2, step ? CRGB::Red : CRGB::Blue);
  step = (step+1) % 2;
  FastLED.show();
}

////////////////////////////////////////////////////////////////////
// Mode RandomDecay - by Russell Hay
// 25% change of a random led turning on, and then fading out to
// it's neighbors
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
  
  if (random8(100) < 25) { 
    leds[random(numPix)] = CRGB(random8(), random8(), random8());
  }
  FastLED.show();
}


////////////////////////////////////////////////////////////////////
// Mode Periodic Color Palette Change - by FastLED
// This mode taken from the FastLED example
// https://github.com/FastLED/FastLED/blob/master/examples/ColorPalette/ColorPalette.ino
void _ChangePalettePeriodically() {
  uint8_t secondHand = (millis() / 1000) % 60;

  if (step != secondHand) {
    step = secondHand;
    switch(secondHand) {
      case 0:
        currentPalette = RainbowColors_p;
        currentBlend = BLEND;
        break;
      case 15:
        currentPalette = PartyColors_p;
        currentBlend = BLEND;
        break;
      case 30:
        currentPalette = RainbowStripeColors_p;
        currentBlend = NOBLEND;
        break;
      case 45:
        currentPalette = CloudColors_p;
        currentBlend = BLEND;
        break;
    }
  }
}

inline void _FillLEDsFromPaletteColors(CRGB* leds, uint8_t numPix, uint8_t colorIndex) {

  for(int i=0; i<numPix; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, 255 /* Brightness */, currentBlend);
    colorIndex += 3;
  }
}

void palette_swap(CRGB* leds, uint8_t numPix) {
  _ChangePalettePeriodically();
 
  startIndex += 1;  // Motion Speed;
  
  _FillLEDsFromPaletteColors(leds, numPix, startIndex);
  FastLED.show();
}

void palette_wave(CRGB* leds, uint8_t numPix) {
  _ChangePalettePeriodically();
  
  startIndex += 1;
  _FillLEDsFromPaletteColors(leds, numPix, cubicwave8(startIndex));
  FastLED.show();
}


#endif
