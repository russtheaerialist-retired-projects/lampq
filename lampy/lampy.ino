#include <Serial.h>
#include <Encoder.h>
#include <FastLED.h>
#include "modes.h"
#define NUM_LEDS 25
#define DATA_PIN 2
#define CLOCK_PIN 3

#define ENCODER_PIN_1 7
#define ENCODER_PIN_2 8
#define MAX_MODES 7
#define FRAME_DELAY 500

int mode=0, next_mode=0, data=0;
int old_position=0;
Encoder control(ENCODER_PIN_1, ENCODER_PIN_2);
CRGB leds[NUM_LEDS];
long last_frame = 0;

const mode_entry_t modes[MAX_MODES] = {
  {-1, off},
  {-1, all_blue},
  {-1, all_red},
  {-1, all_green},
  {500, police},
  {100, random_decay},
  {100, palette_swap}
};

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.clear();
  FastLED.show();
}

void loop() {
  data = control.read();
  Serial.print(data);
  Serial.print("-");
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
    last_frame = 0;
    FastLED.clear();
  }
  
  if (last_frame == 0 || (millis() - last_frame > modes[mode].delay && modes[mode].delay > 0)) {
    modes[mode].func(leds, NUM_LEDS);
    last_frame = millis();
  }
  Serial.println(mode);
}
