#include "timers.hh"


void blinkTimerInit(){
  Timer1.initialize(250000);
  Timer1.attachInterrupt(blinkNixie); // blinkLED to run every 0.15 Sec.

}
//remove the interrupt and turn on the last rtcPin
void blinkTimerStop(){
  Timer1.detachInterrupt();
  Shiftreg_enable(rtcPin,HIGH);
}

void blinkNixie(void){
  if (nixieTubeState == LOW){
    nixieTubeState = HIGH;
  }else {
    nixieTubeState = LOW;
  }
  Shiftreg_enable(rtcPin,nixieTubeState);
}
