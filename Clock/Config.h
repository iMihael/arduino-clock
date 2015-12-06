// Config.h

#ifndef _CONFIG_h
#define _CONFIG_h

#include <DS3231\DS3231.h>
#include "Digits.h"

class Config {
private:
	bool alarm;

public:
	bool beep;
	uint8_t alarmH;
	uint8_t alarmM;
	
	Config();
	void worker(Time & t, Digit & d);
};


#endif

