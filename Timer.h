/*
 * Timer.h
 *
 * Created: 11/3/2017 2:56:11 PM
 *  Author: orencollaco
 * 
MIT License

Copyright (c) 2020 Oren Collaco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "avr/sleep.h"
#include "avr/io.h"


#define TIMER_MODE_REPEAT 01
#define TIMER_MODE_ONCE   00

typedef void (*Fptr)(uint8_t);
#define TIMER1_COUNT_1MS	F_CPU/1000

class TimerClass{
	
		public:
			
			static uint8_t i;
			
			static uint64_t Time;
			
			static TimerClass *Timers[32];
			
			static void begin();
			
			static void callAllTimerObjects();

			static void milliHappened(uint8_t IntArg);   //Attach this function to a interrupt that triggers every millisecond
			
			static void Init_CTC_T1(uint8_t prescale, uint16_t count);

			static void Init_CTC_T2(uint8_t prescale, uint8_t count);

			static uint64_t millis();
			
			static void powerDown(uint8_t mode);

			void initializeTimer();
			
			uint16_t Count, Count_Save;
			
			uint8_t Mode, Timer_ID;
			
			uint16_t NCount, NCount_Save; 
					
			bool NCount_EN, Count_EN;
			
			Fptr CallBackPtr;	
			
			void setCallBackTime(uint16_t time, uint8_t mode, Fptr func);
			
			uint16_t getCallBackTime();
			
			void resetCallbackTimer();
			
			void setTime(uint16_t time);
			
			uint16_t getTime();
			
			void resetTimer();	
			
			uint8_t getTimerID();

			void milliPassed();

			void allowSleep(bool Enable);
			
};

extern TimerClass Timer;

#endif /* TIMER_H_ */