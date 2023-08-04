
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
#include <LiquidCrystal_I2C.h>

long oldValue  = 0;
long frequency = FREQ_MIN;
Encoder vfoDial(ENC_CW, ENC_ACW);
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

/*
 * Standard Arduino Statup
 */
void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  // configure arduino data pins for output
  Serial.println("Hardware Setup");
  lcd.setCursor(0, 0);
  lcd.print("Startup");
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
  vfoDial.read();   /* Start up the encoder */
  lcd.setCursor(0, 0);
  lcd.print("                   ");
  displayFrequency(frequency);
}

void loop() {
  /* This is a problem for now.  Need to bump frequency by 100, not by 1 */
  long newValue = vfoDial.read();
  if(newValue != oldValue) {
    if(oldValue > newValue)
      frequency -= 5;
    else
      frequency += 5;
    if(frequency > FREQ_MAX)
      frequency = FREQ_MAX;
    if(frequency < FREQ_MIN)
      frequency = FREQ_MIN;
    Serial.print("Send AD9850 Command (");
    Serial.print(frequency);
    Serial.print("), TX=");  
    Serial.println(digitalRead(RADIO_TX));
    digitalWrite(LED_BUILTIN, !digitalRead(RADIO_TX));
    sendFrequency(frequency);
    displayFrequency(frequency);
    oldValue = newValue;
  }
  displayTx();
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

void displayFrequency(long freq)
{
    lcd.setCursor(0, 0);
    lcd.print("VFO1:");
    lcd.setCursor(6, 0);
//    lcd.print(String((float)freq/1000));
    lcd.print((float)freq/1000);
    lcd.setCursor(13, 0);
    lcd.print("KHz");
}

void displayTx() {
    lcd.setCursor(0, 3);
    if(!digitalRead(RADIO_TX)) {
      lcd.print("TX");
    }
    else {
      lcd.print("RX");  
    }

}
