// Config.h

#ifndef _CONFIG_h
#define _CONFIG_h

#include <DS3231.h>
#include "Digits.h"
#include <SoftwareSerial.h>


class Config {
private:
	bool alarm;
	bool beep;
	uint8_t alarmH;
	uint8_t alarmM;
	String command;

	void parseSerial(char c);
	void sendData(String str, bool newLine = true);

	void setAlarm(uint8_t hour, uint8_t minute);
	void sendAlarm();
	void sendTime();
	//void setTime(uint8_t hour, uint8_t minute);

	Digit d;
	DS3231 * rtc;

public:
	SoftwareSerial * bt;// (8, 9); // RX, TX
	Config();
	~Config();
	void worker();
};


#define ALARM_H_EEPROM 0
#define ALARM_M_EEPROM 1
#define BEEP_EEPROM 2

#endif

