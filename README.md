# Arduino Timer
ArduinoTimer aids in writing highly event driven code on Arduino Uno, Nano and other Atmega328P based boards. It uses Timer2 hardware internally to generate timing to determine when to make callbacks. This allows the CPU time to be used efficiently instead of wasting it on continuously polling. By putting the micro in different sleep modes while idle, power consumption can be greatly reduced.

* It's important to keep the execution time of code inside the callback function under 1ms as not doing so will cause later callbacks to go out of time.
* Always call begin() on any of the instance before making any other call.
* Call initializeTimer() on all of the instances.

The most useful function this library implements is the 
```
setCallBackTime(uint16_t TimeInMilliseconds, uint8_t Mode, *Fptr(uint8_t) FunctionName)
```
It takes 3 arguments
* Time in millisconds
* Mode (TIMER_MODE_ONCE, TIMER_MODE_REPEAT)
* Function pointer to the callback function(The callback function should have a uint8_t argument)

```
T1.setCallBackTime(100, TIMER_MODE_REPEAT, timerOne);
T1.setCallBackTime(100, 1, timerOne);
```

The follwing example code is also included in the examples folder as a .ino file
### Example code:
```
#include <Arduino.h>
#include <Timer.h>

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
  T1.powerDown(SLEEP_MODE_IDLE);          //Stop CPU and FLASH clocks (Using other sleep modes causes delay in wake up)
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
  T1.setCallBackTime(100, TIMER_MODE_REPEAT, timerOne);
  T2.setCallBackTime(2000, TIMER_MODE_REPEAT, timerTwo);
  T3.setCallBackTime(6000, TIMER_MODE_ONCE, enableSleep);
  Serial.println("Done.");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(TimerTwoFlag){
    Serial.print("+2 seconds. ");
    if(SleepEnabled)
      Serial.println("Sleep enabled");
    else
      Serial.println("Sleep disabled");
    TimerTwoFlag = false;
  }
  if(SleepEnabled){
    sleep();
  }
}
```
