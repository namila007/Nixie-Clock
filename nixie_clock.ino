#include "rtc.hh"
#include "shiftreg.hh"
#include "timers.hh"


//This button0 can change 3 states in FALLING Edges
#define CONFIG_BUTTON 2 
#define NIXIE_BUTTON 4
#define ADD_BUTTON 3
#define SAVE_BUTTON 5

bool rtc12HMode = true;

//states of the nixie clock. initially Clock mode is activated;
enum STATES { CLOCK, CONFIG, TIMER};
enum NIXIES {HOUR, MIN,SEC};

//time struct
ts rtcTime;
volatile enum STATES state;
enum NIXIES nixie;
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;
volatile int addCount =0;
int maxAddCount=13;
int lastSaveButtonState;
int saveButtonState;
int lastSelectButtonState;
int selectButtonState;
//bool nixieTubeState = HIGH;
int rtcPin;

void setup() {
  pinMode(CONFIG_BUTTON,INPUT_PULLUP);
  pinMode(NIXIE_BUTTON,INPUT);
  pinMode(ADD_BUTTON,INPUT_PULLUP);
  pinMode(SAVE_BUTTON,INPUT);

  
  
  //initial states
  state=CLOCK;
  lastSaveButtonState=LOW;
  
  Serial.begin(9600);
  rtc_begin();
  shiftreg_begin();
  
  attachInterrupt(digitalPinToInterrupt(CONFIG_BUTTON),configButtonInterrupt,RISING);
  attachInterrupt(digitalPinToInterrupt(ADD_BUTTON),addButtonInterrupt,RISING);
  
  
  
}

void loop() {
  Serial.print("STATE :");
    Serial.println(state);
    
    if(state== CONFIG){ 
      
       
      Serial.print("nixiebulb : ");
      Serial.println(nixie);
      Serial.print("Add Count: ");
      Serial.println(addCount);
      delay(500);
      shiftreg_write(rtcTime.hour,rtcTime.min,rtcTime.sec);
      if(isNixieSelectPressed()){
        Serial.print("NIXIE SELECTED: ");
        Serial.println(nixie);
      }
      if(isSaveButtonPressed()){
        Serial.println("YAAAAAAAAAAAAAAAAAAAY");
        rtc_saveTime(rtcTime);
        state=CLOCK;
      }
    
  
    } else if(state== TIMER){
      Serial.println("TIMER");
      
    } else {
      rtc_getTime(&rtcTime); 
     
     // delay(200);
   }
     printTime();
     shiftreg_write(rtcTime.sec,rtcTime.min,rtcTime.hour);

}



//BUTTON FUNCTIONS

void configButtonInterrupt()
{
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > debounceDelay)
 {
   Serial.println("INTURRUPT");
   
    state=(STATES)((state+1)%3);
    Serial.print("STATE CHANGE :");
    Serial.println(state);
    if(state==CONFIG) {
      //  setting the initial nixie==hour when config is selected
      nixie=HOUR;
      maxAddCount=25;
      addCount=rtcTime.hour;
      blinkTimerInit();
      rtcPin=10;
    }else {
      blinkTimerStop();
    }
   
 }
 last_interrupt_time = interrupt_time;
}

void addButtonInterrupt()
{
// static unsigned long last_add_interrupt_time = 0;
// unsigned long add_interrupt_time = millis();
// if (add_interrupt_time - last_add_interrupt_time > debounceDelay)
// {
   Serial.println("ADD INTURRUPT");
   if(state==CONFIG) {
    
      addCount=((addCount+1)%maxAddCount);
      if(nixie==HOUR && addCount==0)addCount=1;
      rtc_setValues(nixie,&rtcTime,addCount);
   }
// }
// last_add_interrupt_time = add_interrupt_time;
}

//code from arduino debounce
bool isSaveButtonPressed(){
  int reading = digitalRead(SAVE_BUTTON);

//  // If the switch changed, due to noise or pressing:
//  if (reading != lastSaveButtonState) {
//    // reset the debouncing timer
//    lastDebounceTime = millis();
//  }
//
//  if ((millis() - lastDebounceTime) > debounceDelay) {
//    // whatever the reading is at, it's been there for longer than the debounce
//    // delay, so take it as the actual current state:
//
//    // if the button state has changed:
    if (reading ==HIGH) {
      //saveButtonState = reading;
    //  if (saveButtonState == HIGH) {
          Serial.println("SAVE PRESSED");
           blinkTimerStop();
       //   lastSaveButtonState = reading;
          return true;
     // }
    }
    
//}
   // lastSaveButtonState = reading;
    return false;
}
/**
 * #define HOUR_ENABLEREG 10
  #define MIN_ENABLEREG 11
  #define SEC_ENABLEREG 12

 */
bool isNixieSelectPressed(){
  int reading = digitalRead(NIXIE_BUTTON);
  if(reading ==HIGH) {
    if(state==CONFIG) {
          nixie = (NIXIES)((nixie+1)%3);
          if(nixie==HOUR){
            maxAddCount=25;
            addCount=rtcTime.hour;
            rtcPin=10;
          }
          if(nixie==MIN) {
            maxAddCount=60;
            addCount=rtcTime.min;
            rtcPin=11;
          }
           if(nixie==SEC) {
            maxAddCount=60;
            addCount=rtcTime.sec;
            rtcPin=12;
          }
         
    return true;
    }
    return false;
  }
}

void printTime(){
   Serial.print(rtc_12H(rtcTime.hour));
      Serial.print(":");
      Serial.print(rtcTime.min);
      Serial.print(":");
      Serial.println(rtcTime.sec);

}
