#include "Config.h"
#include <EEPROM.h>
Config::Config() {
	this->alarm = false;

	this->alarmH = EEPROM.read(ALARM_H_EEPROM);
	this->alarmM = EEPROM.read(ALARM_M_EEPROM);

	//TODO: implement beep period
	this->beep = EEPROM.read(BEEP_EEPROM);
	

	this->bt = new SoftwareSerial(8, 9);
	this->bt->begin(9600);
}

Config::~Config() {
	delete this->bt;
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

	if (Serial.available()) {
		char c = Serial.read();
		this->parseSerial(c);
	}
	
	if (this->bt->available()) {
		char c = this->bt->read();
		this->parseSerial(c);
	}

}

void Config::sendData(String str) {
	Serial.println(str);
	this->bt->println(str);
}

void Config::parseSerial(char c) {
	if (c == '\n') {
		JsonObject& root = jsonBuffer.parseObject(command);
		String cnf = root["config"].asString();

		if (cnf == "setAlarm") {
			this->setAlarm(root);
		}
		else if(cnf == "getAlarm") {
			this->sendAlarm();
		}

		command = "";
	} else {
		this->command += c;
	}
}

void Config::sendAlarm() {
	JsonObject& root = jsonBuffer.createObject();
	root["config"] = "getAlarm";
	root["hour"] = this->alarmH;
	root["minute"] = this->alarmM;
	String t;
	root.printTo(t);
	this->sendData(t);
}

void Config::setAlarm(JsonObject& root) {
	this->alarmH = root["hour"];
	this->alarmM = root["minute"];

	EEPROM.write(ALARM_H_EEPROM, this->alarmH);
	EEPROM.write(ALARM_M_EEPROM, this->alarmM);

	this->sendAlarm();
}


