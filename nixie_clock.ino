#include "rtc.hh"
#include "shiftreg.hh"
//This button0 can change 3 states in FALLING Edges
#define CONFIG_BUTTON 2 
#define NIXIE_BUTTON 4

//states of the nixie clock. initially Clock mode is activated;
enum STATES { CLOCK, CONFIG, TIMER};
enum NIXIES {HOUR1, HOUR0, MIN1,MIN0,SEC1,SEC0};

//time struct
ts rtcTime;
volatile enum STATES state;
volatile enum NIXIES nixie;
unsigned long bounce_time = 150; 
int twoValues(int value);

void setup() {
  pinMode(CONFIG_BUTTON,INPUT_PULLUP);
  pinMode(NIXIE_BUTTON,INPUT);
  
  state=CLOCK;
  nixie=HOUR1;
  Serial.begin(9600);
  rtc_begin();
  shiftreg_begin();
  
  attachInterrupt(digitalPinToInterrupt(CONFIG_BUTTON),configButtonInterrupt,FALLING);
  
  
  
}

void loop() {
  Serial.print("STATE :");
    Serial.println(state);
   
    if(state== CONFIG){  
      Serial.print("nixiebulb : ");
      Serial.println(nixie);
     
  
    } else if(state== TIMER){
      Serial.println("TIMER");
      
    } else {
      rtc_getTime(&rtcTime);
      Serial.println(rtcTime.sec);
      shiftreg_write(intToTwoValues(rtcTime.sec));
      delay(950);
   }
  

}

//dividing int into 4bytes.|XXXX XXXX | XXXX <digit1, XXXX<digit0
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
        if(nixie==SEC0) {
          state=((state+1)%3);
        }
        nixie = ((nixie+1)%6);

      
      
   }
   else {
    state=((state+1)%3);
    Serial.print("STATE CHANGE :");
    Serial.println(state);
   }
 }
 last_interrupt_time = interrupt_time;
}


