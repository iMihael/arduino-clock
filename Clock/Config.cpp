// 
// 
// 

#include "Config.h"
#include <aJson-1.5\aJSON.h>

Config::Config() {
	this->alarm = false;
	this->alarmH = 0;
	this->alarmM = 50;
	this->beep = true;
}

void Config::worker(Time & t, Digit & d) {

	if ((t.hour == this->alarmH && t.min == this->alarmM) || this->alarm) {
		this->alarm = true;
		if (d.playMelody()) {
			this->alarm = false;
			//TODO: fix this bug
			if (t.min == this->alarmM) {
				delay(60000);
			}
		}
	}

	if (this->beep && t.min == 0) {
		d.beep();
		//TODO: fix this bug
		delay(75000);
	}
}


