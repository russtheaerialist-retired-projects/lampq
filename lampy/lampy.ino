
#include <Encoder.h>
#include <FastLED.h>
#include "modes.h"
#define NUM_LEDS 25
#define DATA_PIN 2
#define CLOCK_PIN 3

#define ENCODER_PIN_1 7
#define ENCODER_PIN_2 8
#define BUTTON_PIN 9
#define MAX_MODES 9
#define FRAME_DELAY 500
#define DEBOUNCE_TIME 250
#define AUTO_CYCLE_TIME 60000
#define START_MODE 1
#define OFF_MODE 0

int mode=START_MODE, next_mode=START_MODE, data=0;
boolean cycle_modes = false;
int button_state = HIGH, last_button_state = HIGH;
int last_button_press = 0;
long last_mode_change = 0;
int old_position=0;
Encoder control(ENCODER_PIN_1, ENCODER_PIN_2);
CRGB leds[NUM_LEDS];
long last_frame = 0;

const mode_entry_t modes[MAX_MODES] = {
  {-1, off},
  {-1, all_white},
  {-1, all_blue},
  {-1, all_red},
  {-1, all_green},
  {500, police},
  {100, random_decay},
  {100, palette_swap},
  {100, palette_wave}
};

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.clear();
  FastLED.show();

}

inline void read_button() {
   button_state = digitalRead(BUTTON_PIN);
  if (button_state == LOW && 
      last_button_state != LOW &&
      millis() - last_button_press > DEBOUNCE_TIME) {
    cycle_modes = !cycle_modes;
    last_button_press = millis();
    for(int i=0;i<3;i++) {
      fill_solid(leds, NUM_LEDS, cycle_modes ? CRGB::Red : CRGB::Green);
      FastLED.show();
      FastLED.delay(500);
      FastLED.clear();
      FastLED.show();
      FastLED.delay(250);
    }
    Serial.print(cycle_modes ? "Starting" : "Stopping");
    Serial.println(" Cycle mode");
    next_mode = START_MODE;
    mode = OFF_MODE;
    last_mode_change = millis();
  }
  last_button_state = button_state;
}

void loop() {
  data = control.read();
  read_button();
  
  if ((data != old_position)&&(data%4 == 0)) {
    next_mode += (data - old_position)/4;
    old_position = data;
  }
  
  if (cycle_modes && (millis() - last_mode_change) > AUTO_CYCLE_TIME) {
    next_mode = (mode + 1) % MAX_MODES;
    if (next_mode == OFF_MODE) {
      next_mode = START_MODE;  // skip off mode
    }
    last_mode_change = millis();
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
}
