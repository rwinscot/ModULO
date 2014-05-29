
#define BRIGHTNESS   12 // 0=min, 15=max
#define I2C_ADDR   0x70

#include <TinyWireM.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include "anim.h"   

void ledCmd(uint8_t x) { 
  TinyWireM.beginTransmission(I2C_ADDR);
  TinyWireM.write(x);
  TinyWireM.endTransmission();
}

void clear(void) {
  TinyWireM.beginTransmission(I2C_ADDR);
  for(uint8_t i=0; i<17; i++) TinyWireM.write(0);
  TinyWireM.endTransmission();
}

void setup() {
  pinMode( 1, OUTPUT );
  pinMode( 3, OUTPUT );

  TinyWireM.begin();
  clear();

  ledCmd(0x21);              // Turn on oscillator
  ledCmd(0xE0 | BRIGHTNESS); // Set brightness
  ledCmd(0x81);              // Display on, no blink
}

void loop() {
  digitalWrite( 3, HIGH );

  for(int i=0; i<sizeof(anim); i) { 
    TinyWireM.beginTransmission(I2C_ADDR);
    TinyWireM.write(0);
    for(uint8_t j=0; j<8; j++) {
      TinyWireM.write(pgm_read_byte(&anim[i++]));
      TinyWireM.write(0);

      analogWrite( 1, i * sizeof(anim) );
    }
    TinyWireM.endTransmission();
    delay(pgm_read_byte(&anim[i++]) * 10);
  }
}


