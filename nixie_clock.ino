#define DATA_PIN  2
#define LATCH_PIN 3
#define CLOCK_PIN  4
#define MASTERCLEAR_PIN  5

#include <ds3231.h>
#include "rtc.hh"

ts rtcTime;

void writeData(uint8_t val);
void clearData();
int twoValues(int value);

void setup() {
  //Setting pinmodes
  pinMode(LATCH_PIN,OUTPUT);
  pinMode(DATA_PIN,OUTPUT);
  pinMode(CLOCK_PIN,OUTPUT);
  pinMode(MASTERCLEAR_PIN,OUTPUT);
  Serial.begin(9600);
  rtc_begin();
  
  //setting Master Clear =1 to work
  digitalWrite(MASTERCLEAR_PIN,HIGH);
  
  
}

void loop() {
 rtc_getTime(&rtcTime);
 Serial.println(rtcTime.sec);
 writeData(twoValues(rtcTime.sec));
 delay(950);

}
int twoValues(int value) {
  int digit1 = value/10;
  int digit0 = value%10;
  digit0 = digit1<<(4) | digit0;
  //Serial.println((digit0),BIN);
  return digit0;
}

void writeData(int val){
  Serial.println(val,BIN);
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, val); 
  digitalWrite(LATCH_PIN, HIGH); 
}

void clearData() {
  Serial.print("Clearing Registers");
  digitalWrite(MASTERCLEAR_PIN,LOW);
  digitalWrite(MASTERCLEAR_PIN,HIGH);
}

