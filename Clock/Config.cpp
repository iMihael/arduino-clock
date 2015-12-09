#include "Config.h"
#include <EEPROM.h>
Config::Config() {

	this->alarm = false;

	this->alarmH = EEPROM.read(ALARM_H_EEPROM);
	this->alarmM = EEPROM.read(ALARM_M_EEPROM);

	//TODO: implement beep period
	this->beep = EEPROM.read(BEEP_EEPROM);
	
	this->bt = new SoftwareSerial(8, 9); // RX, TX
	this->bt->begin(9600);

	this->rtc = new DS3231(SDA, SCL);
	this->rtc->begin();

}

Config::~Config() {
	delete this->bt;
}

void Config::worker() {

	Time t = this->rtc->getTime();
	this->d.setTime(t.hour, t.min);
	d.blink(t.sec);

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

	if (Serial.available()) {
		char c = Serial.read();
		this->parseSerial(c);
	}
	
	if (this->bt->available()) {
		char c = this->bt->read();
		this->parseSerial(c);
	}

}

void Config::sendData(String str, bool newLine) {
	if (newLine) {
		Serial.println(str);
		this->bt->println(str);
	}
	else {
		Serial.print(str);
		this->bt->print(str);
	}
}

void Config::parseSerial(char c) {
	if (c == '\n') {
		if (command == "getAlarm") {
			this->sendAlarm();
		}
		else if (command.substring(0, 8) == "setAlarm") {
			int idx = command.lastIndexOf(':');
			this->setAlarm(command.substring(9, idx).toInt(), command.substring(idx + 1).toInt());
		}
		else if (command == "getTime") {
			this->sendTime();
		}
		else if (command.substring(0, 7) == "setTime") {
			int idx = command.lastIndexOf(':');
			this->setTime(command.substring(8, idx).toInt(), command.substring(idx + 1).toInt());
		}


		this->command = "";
		
	} else {
		this->command += c;
	}
}

void Config::setTime(uint8_t hour, uint8_t minute) {
	this->rtc->setTime(hour, minute, 0);
	this->sendTime();
}

void Config::sendTime() {
	this->sendData(this->rtc->getTimeStr());
}

void Config::sendAlarm() {
	this->sendData(String(this->alarmH) + ":" + String(this->alarmM));
}

void Config::setAlarm(uint8_t hour, uint8_t minute) {
	this->alarmH = hour;
	this->alarmM = minute;

	EEPROM.write(ALARM_H_EEPROM, this->alarmH);
	EEPROM.write(ALARM_M_EEPROM, this->alarmM);

	this->sendAlarm();
}


