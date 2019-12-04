#include "shiftreg.hh"

void shiftreg_begin(){
  Serial.println("ShiftReg begin");
   //Setting pinmodes
  pinMode(LATCH_PIN,OUTPUT);
  pinMode(DATA_PIN,OUTPUT);
  pinMode(CLOCK_PIN,OUTPUT);
  pinMode(MASTERCLEAR_PIN,OUTPUT);
  //setting Master Clear =1 to work
  digitalWrite(MASTERCLEAR_PIN,HIGH);
}

//function to write data to shift register

int shiftreg_write(int value){
  Serial.println(value,BIN);
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, value); 
  digitalWrite(LATCH_PIN, HIGH); 
}


void shiftreg_clear() {
  Serial.print("Clearing Registers");
  digitalWrite(MASTERCLEAR_PIN,LOW);
  digitalWrite(MASTERCLEAR_PIN,HIGH);
}
