/*
 Name:		Clock.ino
 Created:	12/6/2015 2:00:08 PM
 Author:	michael
*/

// the setup function runs once when you press reset or power the board
#include <EEPROM.h>
#include <DS3231.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "Config.h"
#include "Digits.h"
#include "Shift.h"

Digit d;
Config c;
DS3231 rtc(SDA, SCL);

void setup() {
	Serial.begin(9600);
	rtc.begin();
	pinMode(6, INPUT);
}

void loop() {
	Time t = rtc.getTime();
	d.setTime(t.hour, t.min);
	c.worker(t, d);
	d.blink(t.sec);
}