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
#include <Encoder.h>
#include <LiquidCrystal_I2C.h>
#include "thermistor.h"


long oldValue  = 0;
long frequency = FREQ_MIN;
uint16_t temperature;
uint16_t oldTx = LOW;

Encoder vfoDial(ENC_CW, ENC_ACW);
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
THERMISTOR thermistor(THERM, THERM_RES, THERM_BETA, THERM_SERIES);

/*
 * Standard Arduino Statup
 */
void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
#ifdef _DEBUG
  Serial.begin(9600);
  Serial.println("Hardware Setup");
#endif
  lcd.setCursor(0, 0);
  lcd.print("Startup Ver:");
  lcd.setCursor(13, 0);
  lcd.print(SW_VERSION);
  
  // configure arduino data pins for output  
  pinMode(FQ_UD, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RADIO_TX, INPUT_PULLUP);
  pinMode(TX_RELAY, OUTPUT);

  // Reset the AD9850
#ifdef _DEBUG
  Serial.println("AD9850 Startup");
#endif
  pulseHigh(RESET);
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);
  
#ifdef _DEBUG
  Serial.println("Encoder Startup");
#endif
  oldTx = digitalRead(RADIO_TX);
  vfoDial.read();   /* Start up the encoder */
  lcd.clear();
  displayFrequency(frequency);
}

void loop() {
  if(oldTx == digitalRead(RADIO_TX)) {
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
  #ifdef _DEBUG
      Serial.print("Send AD9850 Command (");
      Serial.print(frequency);
      Serial.print("), TX=");  
      Serial.println(digitalRead(RADIO_TX));
  #endif
      digitalWrite(LED_BUILTIN, !digitalRead(RADIO_TX));
      sendFrequency(frequency);
      displayFrequency(frequency);
      oldValue = newValue;
    }
  }
  else {
    oldTx = digitalRead(RADIO_TX);
  }
  displayTx();
  displayTemp();
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

void sendFrequency(double frequency) {
  if(digitalRead(RADIO_TX) == HIGH)
    frequency += IF_FREQ;
  int32_t freq = frequency * TUNING_VAL/AD9850_FREQ;
  for (int b=0; b<4; b++, freq>>=8) {
    transfer_byte(freq & 0xFF);
  }
  transfer_byte(0x000);
  pulseHigh(FQ_UD);
}

void displayFrequency(long freq)
{
    lcd.setCursor(0, 0);
    lcd.print("VFO1:");
    lcd.setCursor(6, 0);
    lcd.print((float)freq/1000);
    lcd.setCursor(13, 0);
    lcd.print("KHz");
}

void displayTx() {
    lcd.setCursor(0, 3);
    if(!digitalRead(RADIO_TX)) {
      lcd.print("TX");
      digitalWrite(TX_RELAY, HIGH);
    }
    else {
      lcd.print("RX");  
      digitalWrite(TX_RELAY, LOW);
    }

}

void displayTemp() {
  lcd.setCursor(5, 3);
  lcd.print("T:");
  lcd.setCursor(7, 3);
  lcd.print(temperature);
}
