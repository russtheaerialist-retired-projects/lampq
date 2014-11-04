#include <Serial.h>

#define ENCODER_PIN_1 2
#define ENCODER_PIN_2 3
#define MAX_MODES 10
uint8_t old_AB = 0;
int mode=0, next_mode=0;

void setup() {
  Serial.begin(9600);
  pinMode(ENCODER_PIN_1, INPUT);
  digitalWrite(ENCODER_PIN_1, HIGH); // Turn on Pullup
  pinMode(ENCODER_PIN_2, INPUT);
  digitalWrite(ENCODER_PIN_2, HIGH); // Turn on Pullup

}

void loop() {
  if (mode != next_mode) {
    mode = next_mode;
    Serial.println(mode);
  }
}

int8_t read_encoder()
{
  static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; 

  old_AB <<= 2;                   //remember previous state
  old_AB |= ((digitalRead(ENCODER_PIN_2)<<1) | digitalRead(ENCODER_PIN_1));
  next_mode += ( enc_states[( old_AB & 0x0f )]);
  next_mode %= MAX_MODES;
}
