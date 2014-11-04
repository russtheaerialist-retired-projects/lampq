#include <Serial.h>
#include <Encoder.h>

#define ENCODER_PIN_1 2
#define ENCODER_PIN_2 3
#define MAX_MODES 10
int mode=0, next_mode=0, data=0;
int old_position=0;
Encoder control(ENCODER_PIN_1, ENCODER_PIN_2);

void setup() {
  Serial.begin(9600);
  Serial.println("start");
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
    Serial.println(mode);
  }
}
