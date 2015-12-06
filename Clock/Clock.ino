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

int alarmH = 19;
int alarmM = 30;

void setup() {
	Serial.begin(9600);
	rtc.begin();

	/*rtc.setDate(6, 12, 2015);
	rtc.setTime(18, 13, 00);
	rtc.setDOW(SUNDAY);*/
}

void loop() {
	Time t = rtc.getTime();
	d.setTime(t.hour, t.min);
	
	if (t.hour == alarmH && t.min == alarmM) {
		d.playMelody();
	}
	d.blink(t.sec);
}