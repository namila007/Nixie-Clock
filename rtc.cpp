#include <Wire.h>
#include "rtc.hh"


void rtc_begin(){
  //Serial.begin(9600);
  Serial.println("RTC BEGIN");
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  Serial.println("RTC END");
}

void rtc_getTime(struct ts* t){
  DS3231_get(t);
}

void  rtc_saveTime(struct ts t) {
  DS3231_set(t); 
}

void rtc_setValues(int nixie,struct ts* t,int value){
  //HOUR NIXIE TUBES
  if(nixie==0) {
    t->hour = value;
  }
  //MINUTE NIXIE TUBES 
  else if (nixie==1) {
    t->min = value;
  } else {
    t->sec = value;
  }
}


uint8_t rtc_12H(uint8_t value) {
   if(rtc12HMode) {
    if (value%12!=0) return value%12;
    } 
   else return value;
}
  
