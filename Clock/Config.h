// Config.h

#ifndef _CONFIG_h
#define _CONFIG_h

#include <DS3231.h>
#include "Digits.h"
#include <SoftwareSerial.h>


class Config {
private:
	bool alarm;
	uint16_t alarmStart;

	bool beep;
	uint8_t beepStart;
	uint8_t beepEnd;
	uint8_t alarmH;
	uint8_t alarmM;
	String command;

	void parseSerial(char c);
	void sendData(String str, bool newLine = true);

	void setAlarm(uint8_t hour, uint8_t minute);
	void sendAlarm();
	void sendTime();
	void setTime(uint8_t hour, uint8_t minute);
	void setBeep(bool enable, uint8_t start, uint8_t end);
	void sendBeep();

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
#define BEEP_START 3
#define BEEP_END 4


#endif

