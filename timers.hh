#ifndef _timers_hh    
#define _timers_hh    

#include <Arduino.h>
#include <TimerOne.h>

extern int rtcPin;
extern void Shiftreg_enable(int pin,bool value);
bool nixieTubeState=HIGH;

void blinkTimerInit();
void blinkTimerStop();
void blinkNixie(void);

void nixiePoisonInit();

#endif
