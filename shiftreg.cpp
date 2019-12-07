#include "shiftreg.hh"

byte number[10] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};

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

void shiftreg_write(int h,int m,int sec){
  //Serial.println(value,BIN);
 digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, intToTwoValues(h)); 
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, intToTwoValues(m)); 
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, intToTwoValues(sec)); 
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
  digit0 = digit1<<(4) | digit0;
  return digit0;
}

