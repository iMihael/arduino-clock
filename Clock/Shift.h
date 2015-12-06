// Shift.h

#ifndef _SHIFT_h
#define _SHIFT_h

#include <Arduino.h>

class ShiftRegister74HC595
{
public:
	ShiftRegister74HC595(int numberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin);
	void setAll(uint8_t * digitalValues);
	uint8_t * getAll();
	void set(int pin, uint8_t value);
	void setAllLow();
	void setAllHigh();
	uint8_t get(int pin);

private:
	int _numberOfShiftRegisters;
	int _clockPin;
	int _serialDataPin;
	int _latchPin;
	uint8_t * _digitalValues;
};


#endif

