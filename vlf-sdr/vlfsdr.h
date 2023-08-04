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

#define _DEBUG    1

/* Hardware Definitions */
#define W_CLK     8       /* Pin 8 - connect to AD9850 module word load clock pin (CLK) */
#define FQ_UD     9       /* Pin 9 - connect to freq update pin (FQ) */
#define DATA      10      /* Pin 10 - connect to serial data load pin (DATA) */
#define RESET     11      /* Pin 11 - connect to reset pin (RST). */
#define RADIO_TX  12      /* Pin 12 - transmitter frequency command */
#define ENC_CW    5
#define ENC_ACW   6
/*
 * Do not use:
 * A4 - (SDA)
 * A5 - (SCL)
 */

/* Configuration Definitions */
#define AD9850_FREQ  125000000 /* 125MHz */
#define TUNING_VAL   4294967295 /* AD9850 Tuning word - used to trim freq out */
#define IF_FREQ   455000  /* 455KHz */
#define FREQ_MIN  135700
#define FREQ_MAX  137800
#define LCD_ROWS  4
#define LCD_COLS  20
#define LCD_ADDR  0x27
 
#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
