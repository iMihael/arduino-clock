// Config.h

#ifndef _CONFIG_h
#define _CONFIG_h

#include <DS3231.h>
#include "Digits.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

class Config {
private:
	bool alarm;
	bool beep;
	uint8_t alarmH;
	uint8_t alarmM;
	String command;
	StaticJsonBuffer<200> jsonBuffer;

	void parseSerial(char c);
	void setAlarm(JsonObject& root);
	void sendAlarm();
	void sendData(String str);
	
public:
	SoftwareSerial * bt;// (8, 9); // RX, TX
	Config();
	~Config();
	void worker(Time & t, Digit & d);
};


#define ALARM_H_EEPROM 0
#define ALARM_M_EEPROM 1
#define BEEP_EEPROM 2

#endif

