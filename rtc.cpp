#include <Wire.h>
#include "rtc.hh"
#include <ds3231.h>
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

