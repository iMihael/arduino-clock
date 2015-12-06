/*
 Name:		Clock.ino
 Created:	12/6/2015 2:00:08 PM
 Author:	michael
*/

// the setup function runs once when you press reset or power the board
#include "Digits.h"
#include "Shift.h"
#include <DS3231\DS3231.h>


Digit d;
DS3231 rtc(SDA, SCL);

uint8_t alarmH = 23;
uint8_t alarmM = 9;

bool alarm = false;
bool beep = true;

void setup() {
	Serial.begin(9600);
	rtc.begin();
	pinMode(6, INPUT);
	

	/*rtc.setDate(6, 12, 2015);
	rtc.setTime(18, 13, 00);
	rtc.setDOW(SUNDAY);*/
}

void loop() {
	Time t = rtc.getTime();
	d.setTime(t.hour, t.min);
	
	if  ( (t.hour == alarmH && t.min == alarmM ) || alarm) {
		alarm = true;
		if (d.playMelody()) {
			alarm = false;
			//TODO: fix this bug
			delay(60000);
		}
	}

	if (beep && t.min == 0) {
		d.beep();
	}

	d.blink(t.sec);
}