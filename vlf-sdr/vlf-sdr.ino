/**************************************
*        _  __                _       *
*       | |/ _|              | |      *
* __   _| | |_ ______ ___  __| |_ __  *
* \ \ / / |  _|______/ __|/ _` | '__| *
*  \ V /| | |        \__ \ (_| | |    *
*   \_/ |_|_|        |___/\__,_|_|    *
*                                     *
* Intelligent VLF Transceiver         *
* By Christopher F. Moran (VK2MEI)    *
*                                     *
***************************************/

#include "vlfsdr.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Please Wait...");
#ifdef _DEBUG
  Serial.begin(9600);
  Serial.println("Starting up");
#endif
  pinMode(FQ_UD, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RADIO_TX, INPUT_PULLUP);
#ifdef _DEBUG
  Serial.println("HW Config Done");
#endif
  /* Boot up the DDS Module */
  pulseHigh(RESET);
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);
#ifdef _DEBUG
  Serial.println("DDS Config Done");
#endif
}

void loop() {
}

/* Serial data transfer to AD9850 */
void transferByte(byte data) {
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(DATA, data & 0x01);
    pulseHigh(W_CLK);
  }
}

/* Set AD9850 operating frequency */
void sendFrequency(double frequency) {
  if(digitalRead(RADIO_TX) == HIGH)
    frequency += IF_FREQ;
  int32_t freq = frequency * TUNING_VAL/AD9850_FREQ;
  for (int b=0; b<4; b++, freq>>=8) {
    transferByte(freq & 0xFF);
  }
  transferByte(0x000);   // Final control byte, all 0 for 9850 chip
  pulseHigh(FQ_UD);
}

/* We update different bits of the LCD depending on what we're doing */
void displayFrequency(long freq)
{
    lcd.setCursor(0, 0);
    lcd.print("Freq:");
    lcd.setCursor(6,0);
    lcd.print(String(freq / 1000));
}
