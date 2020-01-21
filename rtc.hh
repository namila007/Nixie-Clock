#ifndef _rtc_hh    // Put these two lines at the top of your file.
#define _rtc_hh    // (Use a suitable name, usually based on the file name.)

#include <Arduino.h>
#include <ds3231.h>

extern bool rtc12HMode;
void rtc_begin();

void rtc_getTime(struct ts *t);

void rtc_saveTime(struct ts t);

void rtc_setValues(int nixie,struct ts* t,int value);

uint8_t rtc_12H(uint8_t value);
#endif // _RTC_H    // Put this line at the end of your file.
