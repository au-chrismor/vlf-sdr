/* 
 * This sketch allows simple testing of the module and it's connections under the control
 * of a rotary encoder.
 * 
 * Based on numerous online articles, no ownership is expressed.
 *
 * This code should work unmodified on any AVR-based Arduino
 */

#include "vfotest1.h"
#include <Encoder.h>

long oldFrequency  = 0;
Encoder vfoDial(ENC_CW, ENC_ACW);

/*
 * Standard Arduino Statup
 */
void setup() {
  Serial.begin(9600);
  // configure arduino data pins for output
  Serial.println("Hardware Setup");
  pinMode(FQ_UD, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RADIO_TX, INPUT_PULLUP);

  // Reset the AD9850
  Serial.println("AD9850 Startup");
  pulseHigh(RESET);
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);  /*
                      * this pulse enables serial mode - Datasheet page 12 figure 10
                      */
}

void loop() {
  long newFrequency = vfoDial.read();
  if(newFrequency != oldFrequency) {
    if(newFrequency < FREQ_MIN)
      newFrequency = FREQ_MIN;
    if(newFrequency > FREQ_MAX)
      newFrequency = FREQ_MAX;
    Serial.print("Send AD9850 Command (");
    Serial.print(newFrequency);
    Serial.print("), TX=");  
    Serial.println(digitalRead(RADIO_TX));
    digitalWrite(LED_BUILTIN, !digitalRead(RADIO_TX));
    sendFrequency(newFrequency);
    oldFrequency = newFrequency;
  }
}
 
/*
 * Serial transfer to the AD9850
 */
void transfer_byte(byte data)
{
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(DATA, data & 0x01);
    pulseHigh(W_CLK);   //after each bit sent, CLK is pulsed high
  }
}

 // frequency calc from datasheet page 8 = <sys clock> * <frequency tuning word>/2^32
void sendFrequency(double frequency) {
  if(digitalRead(RADIO_TX) == HIGH)
    frequency += IF_FREQ;
  int32_t freq = frequency * TUNING_VAL/AD9850_FREQ;
  for (int b=0; b<4; b++, freq>>=8) {
    transfer_byte(freq & 0xFF);
  }
  transfer_byte(0x000);   // Final control byte, all 0 for 9850 chip
  pulseHigh(FQ_UD);  // Done!  Should see output
}
