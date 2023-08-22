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
#define TX_RELAY  2       /* Pin 2 - Transmit Relay Control */
#define TBD_D3    3       /* Digital 3 */
#define TBD_D4    4       /* Digital 4 */
#define TBD_D5    5       /* Digital 5 */
#define TBD_D6    6       /* Digital 6 */
#define TBD_D7    7       /* Digital 7 */
#define W_CLK     8       /* Pin 8 - connect to AD9850 module word load clock pin (CLK) */
#define FQ_UD     9       /* Pin 9 - connect to freq update pin (FQ) */
#define DATA      10      /* Pin 10 - connect to serial data load pin (DATA) */
#define RESET     11      /* Pin 11 - connect to reset pin (RST). */
#define RADIO_TX  12      /* Pin 12 - transmitter frequency command */
#define ENC_CW    6
#define ENC_ACW   7
#define TBD_A0    A0      /* Analog 0 */
#define THERM     A1      /* Pin A1 - Thermistor input for TX temperature */
#define TBD_A2    A2      /* Analog 2 */
#define TBD_A3    A3      /* Analog 3 */
/*
 * Do not use:
 * A4 - (SDA)
 * A5 - (SCL)
 */
#define TBD_A6    A6      /* Analog 6 */
#define TBD_A7    A7      /* Analog 7 */

/* Configuration Definitions */
#define AD9850_FREQ   125000000 /* 125MHz */
#define TUNING_VAL    4294967295 /* AD9850 Tuning word - used to trim freq out */
#define IF_FREQ       455000  /* 455KHz */
#define FREQ_MIN      135700
#define FREQ_MAX      137800
#define LCD_ROWS      4
#define LCD_COLS      20
#define LCD_ADDR      0x3F
#define THERM_RES     10000
#define THERM_BETA    3950
#define THERM_SERIES  9980

#define SW_VERSION "0.1.0"

#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
