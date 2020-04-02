/*
 * Timer.cpp
 *
 * Created: 11/3/2017 2:55:58 PM
 *  Author: orencollaco

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


#include "Arduino.h"
#include "Timer.h"


TimerClass Timer;

TimerClass *TimerClass::Timers[32];

uint64_t TimerClass::Time = 0;

uint8_t TimerClass::i = 1;


void TimerClass::begin(){
	//Init_CTC_T1(1, TIMER1_COUNT_1MS);
	Init_CTC_T2(5, 125);

	i = 1;
}

void TimerClass::Init_CTC_T1(uint8_t prescale, uint16_t count){
	OCR1A = count;
	TCCR1A = 0;
	prescale &= 0x07;
	prescale |= (1 << WGM12);
	TCCR1B = prescale;
	TIMSK1 = (1 << OCIE1A);
	sei();
}

void TimerClass::Init_CTC_T2(uint8_t prescale, uint8_t count){
	TCCR2A = 0x02;
	OCR2A = count;
	prescale &= 0x07;
	TCCR2B |= prescale;
	TIMSK2 = (1 << OCIE2A);
}

void TimerClass::initializeTimer(){
	Timers[i] = this;
	Timer_ID = i;
	i += 1;
	#ifdef DEBUG_TIMER
	printStringCRNL("Timer initialized : ");
	printNumber(Timer_ID);
	#endif
}

void TimerClass::setCallBackTime(uint16_t time, uint8_t mode, Fptr func){
	Count = time;
	Count_Save = time;
	CallBackPtr = func;
	Count_EN = 1;
	Mode = mode;
	//allowSleep(0);
}


void TimerClass::resetCallbackTimer(){
	Count_EN = 0;
	Count = 0;	
}

uint16_t TimerClass::getCallBackTime(){
	return Count_Save - Count;
}

void TimerClass::setTime(uint16_t time){
	NCount = time;
	NCount_EN = 1;
	NCount_Save = time;
	//allowSleep(0);
}

uint16_t TimerClass::getTime(){
	return NCount_Save - NCount;
}

void TimerClass::resetTimer(){
	NCount = 0;
	NCount_EN = 0;
}


uint64_t TimerClass::millis(){
	return Time;
}

void TimerClass::milliPassed(){
	if(Count_EN){
		#ifdef DEBUG_TIMER
		//printChar('T');
		#endif
		if(Count)
		Count -= 1;
		if(!Count){
			if(Mode == TIMER_MODE_REPEAT){
				Count = Count_Save;
			}
			if(Mode == TIMER_MODE_ONCE){
				Count_EN = 0;
				Count = 0;
			}
			if(CallBackPtr != NULL){
				//#ifdef DEBUG_TIMER
				//printStringCRNL("Timer Done : ");
				//printNumber(Timer_ID);
				//#endif
				//allowSleep(1);
				CallBackPtr(Timer_ID);
			}
		}
	}
	if(NCount_EN){
		if(NCount)
		NCount -= 1;
		if(!NCount){
			NCount_EN = 0;
		}
	}
}

void TimerClass::callAllTimerObjects(){
	for(uint8_t a = 0; a < i; a += 1){
		if(Timers[a] != NULL)
		Timers[a]->milliPassed();
	}
}

uint8_t TimerClass::getTimerID(){
	return Timer_ID;
}


void TimerClass::milliHappened(uint8_t IntArg){
	//printChar('I');
	Timer.Time += 1;
	Timer.callAllTimerObjects();
	//printChar('O');
}

void TimerClass::allowSleep(bool Enable){

}

void TimerClass::powerDown(uint8_t mode){
	// uint8_t t;
	// t = 0x07 & prescale;
	// prescale &= 0x08;
	// prescale = prescale << 2;
	// prescale |= t;
	// prescale |= (1 << WDIE) | (1 << WDE);
	// WDTCR = prescale;
	// asm volatile("cli"::);
	// asm volatile("wdr"::);
	// /* Clear WDRF in MCUSR */
	// MCUSR &= ~(1<<WDRF);
	// /* Start timed sequence */
	// WDTCR = (1<<WDCE) | (1<<WDE);
	// /* Set new prescaler (time-out) value = 64K cycles (~0.5 s) */
	// WDTCR = prescale;
	asm volatile("sei"::);
	//PRR = (1 << PRUSART0);
	//sleep_bod_disable();
	set_sleep_mode(mode);
	sleep_enable();
	sleep_mode();
}

ISR(TIMER2_COMPA_vect){
	//printChar('I');
	//milliHappened();
	Timer.Time += 1;
	Timer.callAllTimerObjects();
	//printChar('O');
}