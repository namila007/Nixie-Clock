#include "rtc.hh"
#include "shiftreg.hh"
//This button0 can change 3 states in FALLING Edges
#define CONFIG_BUTTON 2 
#define NIXIE_BUTTON 3

//states of the nixie clock. initially Clock mode is activated;
enum STATES { CLOCK, CONFIG, TIMER};
enum NIXIES {HOUR, MIN,SEC};

//time struct
ts rtcTime;
volatile enum STATES state;
volatile enum NIXIES nixie;
unsigned long bounce_time = 150; 
unsigned long add_bounce_time = 75; 
int addCount =0;
int maxAddCount=13;


void setup() {
  pinMode(CONFIG_BUTTON,INPUT_PULLUP);
  pinMode(NIXIE_BUTTON,INPUT);

  //initial states
  state=CLOCK;
  nixie=HOUR;
  
  Serial.begin(9600);
  rtc_begin();
  shiftreg_begin();
  
  attachInterrupt(digitalPinToInterrupt(CONFIG_BUTTON),configButtonInterrupt,RISING);
  attachInterrupt(digitalPinToInterrupt(NIXIE_BUTTON),addButtonInterrupt,RISING);
  
  
  
}

void loop() {
//  Serial.print("STATE :");
//    Serial.println(state);
   
    if(state== CONFIG){  
      Serial.print("nixiebulb : ");
      Serial.println(nixie);
      Serial.print("Add Count: ");
      Serial.println(addCount);
      //delay(400);
  
    } else if(state== TIMER){
      Serial.println("TIMER");
      
    } else {
      rtc_getTime(&rtcTime);
      Serial.println(rtcTime.sec);
      shiftreg_write(intToTwoValues(rtcTime.hour));
      shiftreg_write(intToTwoValues(rtcTime.min));
      shiftreg_write(intToTwoValues(rtcTime.sec));
      delay(500);
   }
  

}

//dividing 8bit int into 4bits.|XXXX XXXX | XXXX <digit1, XXXX<digit0
int intToTwoValues(int value) {
  int digit1 = value/10;
  int digit0 = value%10;
  digit0 = digit1<<(4) | digit0;
  //Serial.println((digit0),BIN);
  return digit0;
}

void configButtonInterrupt()
{
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > bounce_time)
 {
   Serial.println("INTURRUPT");
   if(state==CONFIG) {
        if(nixie==SEC) {
          state=((state+1)%3);
        }
        nixie = ((nixie+1)%3);
        if(nixie==HOUR){
          maxAddCount=13;
          addCount=0;
        }
        else {
          maxAddCount=60;
          addCount=0;
        }
   }
   else {
    state=((state+1)%3);
    Serial.print("STATE CHANGE :");
    Serial.println(state);
   }
 }
 last_interrupt_time = interrupt_time;
}

void addButtonInterrupt()
{
 static unsigned long last_add_interrupt_time = 0;
 unsigned long add_interrupt_time = millis();
 if (add_interrupt_time - last_add_interrupt_time > add_bounce_time)
 {
   Serial.println("ADD INTURRUPT");
   if(state==CONFIG) {
      addCount=((addCount+1)%maxAddCount);
      if(nixie==HOUR && addCount==0)addCount=1;
   }
 }
 last_add_interrupt_time = add_interrupt_time;
}


