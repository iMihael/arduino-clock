/*
 Name:		Clock.ino
 Created:	12/6/2015 2:00:08 PM
 Author:	michael
*/

// the setup function runs once when you press reset or power the board
#include <EEPROM.h>
#include <DS3231.h>
#include <SoftwareSerial.h>
#include "Config.h"

Config c;

void setup() {
	pinMode(6, INPUT);
	Serial.begin(9600);

	//implement brightnes modes
	pinMode(10, OUTPUT);
	//analogWrite(10, 0);
	digitalWrite(10, 0);

}

void loop() {
	c.worker();
}