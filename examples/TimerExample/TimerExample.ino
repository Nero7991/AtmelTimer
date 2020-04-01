#include <Arduino.h>
#include <Switch.h>

TimerClass T1, T2, T3;

volatile const uint8_t LedPin = 13;
volatile bool LedState = true;

void timerOne(uint8_t TimerId){
    if(LedState)
    digitalWrite(LedPin, LOW);
    else
    digitalWrite(LedPin, HIGH);
    LedState ^= 1;
}


volatile bool TimerTwoFlag = false, SleepEnabled = false;
void timerTwo(uint8_t TimerId){
  TimerTwoFlag = true;
}

void enableSleep(uint8_t TimerId){
  SleepEnabled = true;
}

void sleep(){
  // put micro to sleep
  //Serial.println("Sleeping...");
  //delay(10);
  Serial.end();
  T1.powerDown(SLEEP_MODE_PWR_SAVE);
  Serial.begin(9600);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LedPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Setting up things...");
  T1.begin();                               //Always call begin() on any instance before any other calls
  T1.initializeTimer();
  T2.initializeTimer();
  T3.initializeTimer();
  T1.setCallBackTime(500, TIMER_MODE_REPEAT, timerOne);
  T2.setCallBackTime(5000, TIMER_MODE_REPEAT, timerTwo);
  T3.setCallBackTime(20000, TIMER_MODE_ONCE, enableSleep);
  Serial.println("Done.");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(TimerTwoFlag){
    Serial.println("+5 seconds");
    TimerTwoFlag = false;
  }
  if(SleepEnabled){
    sleep();
  }
}
