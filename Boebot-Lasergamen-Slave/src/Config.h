#ifndef CONFIG_H
#define CONFIG_H

// debug = 0 is off || 1 is on 
#define DEBUG 1

// SPI interrupt timer
#define PRESCALER 16000  
#define TIMERTICKS 5000

// SPI state machine time between states
#define TIMEBETWEENCMDS 2

// Steppermotor amount of steps per wenteling
#define STEPSPERREVOLUTION 2048

// Timers offsets in UI_LAYER
#define TIMERSTATUS 100 // time between next status command
#define TIMERSHOOT  1000 // time between next reload

// Check for stickdrift in the controller
#define STICKDRIFT  20

// SPI 
#define SPICLK      1000000 // 1 MHz clock
#define DATALENGTH  4
#define SSPINTIME   1 // time for the cs pin to go low

//PS4 controller
#define MAC_PS4 "70:EA:23:1B:FC:FF"

// Pin definitions:
#define motorRechtsPWM 5
#define motorRechtsDIR 19
#define motorLinksPWM 23
#define motorLinksDIR 18
#define motorTurretPWM 13

#define HSPI_MISO   26
#define HSPI_MOSI   27
#define HSPI_SCLK   25
#define HSPI_SS     14

#define GPIO_TXs 17
#define GPIO_RXs 16

#define servopin 2

#endif