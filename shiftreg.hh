#ifndef _shiftreg_hh    
#define _shiftreg_hh    

#include <Arduino.h>
#define DATA_PIN  5
#define LATCH_PIN 6
#define CLOCK_PIN  7
#define MASTERCLEAR_PIN  8
#define HOUR_ENABLEREG 9
#define MIN_ENABLEREG 10
#define SEC_ENABLEREG 11

void shiftreg_begin();

int shiftreg_write(int value);

void shiftreg_clear();
void Shiftreg_enable(int pin,bool value);
#endif // _shiftreg_H    
