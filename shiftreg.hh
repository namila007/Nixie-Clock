#ifndef _shiftreg_hh    
#define _shiftreg_hh    

#include <Arduino.h>
#define DATA_PIN  8
#define LATCH_PIN 9
#define CLOCK_PIN  10
#define MASTERCLEAR_PIN  11

void shiftreg_begin();

int shiftreg_write(int value);

void shiftreg_clear();
#endif // _shiftreg_H    
