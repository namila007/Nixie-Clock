#ifndef _shiftreg_hh    
#define _shiftreg_hh    

#include <Arduino.h>

#define DATA_PIN  6
#define LATCH_PIN 7
#define CLOCK_PIN  8
#define MASTERCLEAR_PIN  9
#define HOUR_ENABLEREG 10
#define MIN_ENABLEREG 11
#define SEC_ENABLEREG 12


extern uint8_t rtc_12H(uint8_t value);

void shiftreg_begin();

void shiftreg_write(int h,int m,int sec);

void shiftreg_clear();
void Shiftreg_enable(int pin,bool value);

uint8_t intToTwoValues(int value);
#endif // _shiftreg_H    
