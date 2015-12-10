#include "Config.h"
#include <EEPROM.h>
Config::Config() {

	this->alarm = false;
	this->alarmStart = 0;

	this->alarmH = EEPROM.read(ALARM_H_EEPROM);
	this->alarmM = EEPROM.read(ALARM_M_EEPROM);
	this->alarmEnabled = EEPROM.read(ALARM_ENABLED);
	this->alarmMelody = EEPROM.read(ALARM_MELODY);

	this->beepStart = EEPROM.read(BEEP_START);
	this->beepEnd = EEPROM.read(BEEP_END);

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

	//TODO: implement interval half minute for this action
	Time t = this->rtc->getTime();
	this->d.setTime(t.hour, t.min);


	//uint16_t unix = this->rtc->getUnixTime(t);
	//Serial.println(unix);

	d.blink(t.sec);
	
	if (this->alarmEnabled) {
		if ((t.hour == this->alarmH && t.min == this->alarmM) || this->alarm) {
			this->alarm = true;

			if (d.playMelody(this->alarmMelody)) {
				this->alarm = false;
				//TODO: fix this bug
				if (t.min == this->alarmM) {
					delay(60000);
				}
			}

			if (alarmStart == 0) {
				alarmStart = this->rtc->getUnixTime(t);
			}
			else {
				uint16_t diff = this->rtc->getUnixTime(t) - alarmStart;
				if (diff > 600) {
					this->alarmStart = 0;
					this->alarm = false;
				}
			}
		}
	}

	if (this->beep && t.min == 0 && t.hour >= this->beepStart && t.hour <= this->beepEnd) {
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
			//this->setAlarm(command.substring(9, idx).toInt(), command.substring(idx + 1).toInt());
			this->setAlarm(command.substring(9, 10).toInt(), command.substring(11, 12).toInt(), command.substring(13, idx).toInt(), command.substring(idx + 1).toInt());
		}
		else if (command == "getTime") {
			this->sendTime();
		}
		else if (command.substring(0, 7) == "setTime") {
			int idx = command.lastIndexOf(':');
			this->setTime(command.substring(8, idx).toInt(), command.substring(idx + 1).toInt());
		}
		else if (command.substring(0, 7) == "setBeep") {
			int idx = command.lastIndexOf(':');
			this->setBeep(command.substring(8, 9).toInt(), command.substring(10, idx).toInt(), command.substring(idx + 1).toInt());
		}
		else if (command == "getBeep") {
			this->sendBeep();
		}


		this->command = "";
		
	} else {
		this->command += c;
	}
}

void Config::sendBeep() {
	this->sendData(String("beep:") + String(this->beep) + ":" + String(this->beepStart) + ":" + String(this->beepEnd));
}

void Config::setBeep(bool enable, uint8_t start, uint8_t end) {
	EEPROM.write(BEEP_EEPROM, enable);
	EEPROM.write(BEEP_START, start);
	EEPROM.write(BEEP_END, end);

	this->beepStart = start;
	this->beepEnd = end;
	this->beep = enable;

	this->sendBeep();
}

void Config::setTime(uint8_t hour, uint8_t minute) {
	this->rtc->setTime(hour, minute, 0);
	this->sendTime();
}

void Config::sendTime() {
	this->sendData(String("time:") + this->rtc->getTimeStr());
}

void Config::sendAlarm() {
	this->sendData(String("alarm:") + String(this->alarmEnabled) + ":" + String(this->alarmMelody) + ":" + String(this->alarmH) + ":" + String(this->alarmM));
}

void Config::setAlarm(bool enable, uint8_t melody, uint8_t hour, uint8_t minute) {
	this->alarmH = hour;
	this->alarmM = minute;
	this->alarmMelody = melody;
	this->alarmEnabled = enable;

	EEPROM.write(ALARM_H_EEPROM, this->alarmH);
	EEPROM.write(ALARM_M_EEPROM, this->alarmM);
	EEPROM.write(ALARM_ENABLED, this->alarmEnabled);
	EEPROM.write(ALARM_MELODY, this->alarmMelody);

	this->sendAlarm();
}


