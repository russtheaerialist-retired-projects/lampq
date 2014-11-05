#include <Serial.h>
#include <Encoder.h>
#include <FastLED.h>
#define NUM_LEDS 25
#define DATA_PIN 2
#define CLOCK_PIN 3

#define ENCODER_PIN_1 7
#define ENCODER_PIN_2 8
#define MAX_MODES 25
int mode=0, next_mode=0, data=0;
int old_position=0;
Encoder control(ENCODER_PIN_1, ENCODER_PIN_2);
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();
  leds[mode] = CRGB::Blue;
  FastLED.show();
}

void loop() {
  data = control.read();
  if ((data != old_position)&&(data%4 == 0)) {
    next_mode += (data - old_position)/4;
    old_position = data;
  }
  
  if(next_mode != mode) {
    if (next_mode < 0) {
      next_mode = MAX_MODES - 1;
    }
    next_mode %= MAX_MODES;
    mode = next_mode;
    FastLED.clear();
    leds[mode] = CRGB::Blue;
    FastLED.show();
  }
}
