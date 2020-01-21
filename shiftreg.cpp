#include "shiftreg.hh"

/** in k155id1 truth table is shifted, 
 *  so i had to map the integer with previous bin number 
 *                0    , 1    , 2    , 3    , 4    , 5    , 6    , 7    , 8    , 9      
**/
 char knumber[] = {0b00001001,0b00000000,0b00000001,0b00000010,0b00000011,0b00000100,0b00000101,0b00000110,0b00000111,0b00001000};


void shiftreg_begin(){
  Serial.println("ShiftReg begin");
   //Setting pinmodes
  pinMode(LATCH_PIN,OUTPUT);
  pinMode(DATA_PIN,OUTPUT);
  pinMode(CLOCK_PIN,OUTPUT);
  pinMode(MASTERCLEAR_PIN,OUTPUT);
  pinMode(HOUR_ENABLEREG,OUTPUT);
  pinMode(MIN_ENABLEREG,OUTPUT);
  pinMode(SEC_ENABLEREG,OUTPUT);
  //setting Master Clear =1 to work
  digitalWrite(MASTERCLEAR_PIN,HIGH);
  //enabling all shiftreg o/ps
  digitalWrite(SEC_ENABLEREG,LOW);
  digitalWrite(MIN_ENABLEREG,LOW);
  digitalWrite(HOUR_ENABLEREG,LOW);
}

//function to write data to shift register

void shiftreg_write(int sec,int m,int h){
  //is 12H mode is enabled??
  h = rtc_12H(h);
  //clearing first
   digitalWrite(MASTERCLEAR_PIN,LOW);
   digitalWrite(MASTERCLEAR_PIN,HIGH);
  //Serial.println(value,BIN);
 digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, intToTwoValues(sec)); 
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, intToTwoValues(m)); 
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, intToTwoValues(h)); 
  digitalWrite(LATCH_PIN, HIGH); 
}


void shiftreg_clear() {
  Serial.print("Clearing Registers");
  digitalWrite(MASTERCLEAR_PIN,LOW);
  digitalWrite(MASTERCLEAR_PIN,HIGH);
}

//if value==true we need to OE=LOW to on the register o/p
void Shiftreg_enable(int pin,bool value){
  if(value)digitalWrite(pin,LOW);
  else digitalWrite(pin,HIGH);
}

//dividing 8bit int into 2 nibbles.|XXXX XXXX | XXXX <digit1, XXXX<digit0
byte intToTwoValues(int value) {
  byte digit1 = value/10;
  byte digit0 = value%10;
  digit0 = knumber[digit1]<<(4) | knumber[digit0];
  return digit0;
}
