#include "rtc.hh"
#include "shiftreg.hh"
//This button0 can change 3 states in FALLING Edges
#define CONFIG_BUTTON 2 
#define NIXIE_BUTTON 3
#define SAVE_BUTTON 4

//states of the nixie clock. initially Clock mode is activated;
enum STATES { CLOCK, CONFIG, TIMER};
enum NIXIES {HOUR, MIN,SEC};

//time struct
ts rtcTime;
volatile enum STATES state;
volatile enum NIXIES nixie;
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;
int addCount =0;
int maxAddCount=13;
int lastSaveButtonState;
int saveButtonState;

void setup() {
  pinMode(CONFIG_BUTTON,INPUT_PULLUP);
  pinMode(NIXIE_BUTTON,INPUT_PULLUP);
  pinMode(SAVE_BUTTON,INPUT);
  //initial states
  state=CLOCK;
  nixie=HOUR;
  lastSaveButtonState=LOW;
  
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
       
//      Serial.print("nixiebulb : ");
//      Serial.println(nixie);
//      Serial.print("Add Count: ");
//      Serial.println(addCount);

      shiftreg_write(rtcTime.hour,rtcTime.min,rtcTime.sec);
      if(isSaveButtonPressed()){
        Serial.println("YAAAAAAAAAAAAAAAAAAAY");
        rtc_saveTime(rtcTime);
      }
    
  
    } else if(state== TIMER){
      Serial.println("TIMER");
      
    } else {
      rtc_getTime(&rtcTime); 
      shiftreg_write(rtcTime.hour,rtcTime.min,rtcTime.sec);
     
     // delay(200);
   }
      Serial.print(rtcTime.hour);
      Serial.print(":");
      Serial.print(rtcTime.min);
      Serial.print(":");
      Serial.println(rtcTime.sec);

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
   if(state==CONFIG) {
        if(nixie==SEC) {
          addCount=rtcTime.sec;
          state=((state+1)%3);
        }
        nixie = ((nixie+1)%3);
        if(nixie==HOUR){
          maxAddCount=13;
          addCount=rtcTime.hour;
        }
        else {
          maxAddCount=60;
          addCount=rtcTime.min;
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
 if (add_interrupt_time - last_add_interrupt_time > debounceDelay)
 {
   Serial.println("ADD INTURRUPT");
   if(state==CONFIG) {
    
      addCount=((addCount+1)%maxAddCount);
      if(nixie==HOUR && addCount==0)addCount=1;
      rtc_setValues(nixie,&rtcTime,addCount);
   }
 }
 last_add_interrupt_time = add_interrupt_time;
}

//code from arduino debounce
bool isSaveButtonPressed(){
  int reading = digitalRead(SAVE_BUTTON);
  
  // If the switch changed, due to noise or pressing:
  if (reading != lastSaveButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != saveButtonState) {
      saveButtonState = reading;
      if (saveButtonState == HIGH) {
          Serial.println("SAVE PRESSED");
          lastSaveButtonState = reading;
          return true;
      }
    }
    
}
    lastSaveButtonState = reading;
    return false;
}



