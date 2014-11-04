#include <Serial.h>

#define ENCODER_PIN_1 2
#define ENCODER_PIN_2 3
#define DEBOUNCE_TIME 250
#define MAX_MODES 10
uint8_t old_AB = 0;
int tmpdata;
int mode=0, next_mode=0;
uint8_t pin1, pin2;
unsigned long last_read = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ENCODER_PIN_1, INPUT);
  digitalWrite(ENCODER_PIN_1, HIGH); // Turn on Pullup
  pinMode(ENCODER_PIN_2, INPUT);
  digitalWrite(ENCODER_PIN_2, HIGH); // Turn on Pullup
  Serial.println("start");
}

void loop() {
  if (millis() - last_read > DEBOUNCE_TIME) {
    tmpdata = read_encoder();

    if (tmpdata) {
      last_read = millis();
      next_mode += tmpdata;
      if (next_mode < 0) {
        next_mode = MAX_MODES - 1;
      } else {
        mode = next_mode % MAX_MODES;
      }
      Serial.println(mode);
    }
  }
}

int8_t read_encoder()
{
  static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; 

  old_AB <<= 2;                   //remember previous state
  pin1 = digitalRead(ENCODER_PIN_2);
  pin2 = digitalRead(ENCODER_PIN_1);
  old_AB |= (pin2<<1) | pin1;
  return ( enc_states[( old_AB & 0x0f )]);
}
