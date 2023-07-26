/* 
 * This sketch allows simple testing of the module and it's connections
 * Based on numerous online articles, no ownership is expressed.
 *
 * This code should work unmodified on any AVR-based Arduino
 */
 
#define W_CLK     8       /* Pin 8 - connect to AD9850 module word load clock pin (CLK) */
#define FQ_UD     9       /* Pin 9 - connect to freq update pin (FQ) */
#define DATA      10      /* Pin 10 - connect to serial data load pin (DATA) */
#define RESET     11      /* Pin 11 - connect to reset pin (RST). */
#define RADIO_TX  12      /* Pin 12 - transmitter frequency command */
#define AD9850_FREQ  125000000 /* 125MHz */
#define TUNING_VAL   4294967295 /* AD9850 Tuning word - used to trim freq out */
#define IF_FREQ   455000  /* 455KHz */
 
#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }

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
  Serial.println("Send AD9850 Command");
  digitalWrite(LED_BUILTIN, !digitalRead(RADIO_TX));
  sendFrequency(137500);
  delay (1000);
  sendFrequency(137500);  // freq
  delay(1000);
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
