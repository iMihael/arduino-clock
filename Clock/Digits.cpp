#include "Digits.h"

const int Digit::digits[4][7] = {
	{ 3, 5, 7, 6, 2, 0, 1 },
	{ 14, 15, 8, 12, 11, 10, 9 },
	{ 19, 20, 23, 22, 21, 17, 16 },
	{ 26, 28, 30, 27, 29, 25, 24 }
};

const int Digit::nums[11][7] = {
	{ 1, 1, 1, 1, 1, 1, 0 }, //0
	{ 0, 1, 1, 0, 0, 0, 0 }, //1
	{ 1, 1, 0, 1, 1, 0, 1 }, //2
	{ 1, 1, 1, 1, 0, 0, 1 }, //3
	{ 0, 1, 1, 0, 0, 1, 1 }, //4
	{ 1, 0, 1, 1, 0, 1, 1 }, //5
	{ 1, 0, 1, 1, 1, 1, 1 }, //6
	{ 1, 1, 1, 0, 0, 0, 0 }, //7
	{ 1, 1, 1, 1, 1, 1, 1 }, //8
	{ 1, 1, 1, 1, 0, 1, 1 }, //9
	{ 0, 0, 0, 0, 0, 0, 0 } // clear
};

const int Digit::melodies[2][98] = {
	{
		NG4,NE5,ND5,NC5,NG4,NG4,NE5,ND5,NC5,NA4,
		NA4,NF5,NE5,ND5,NB4,NG5,NG5,NF5,ND5,NE5,
	NG4,NE5,ND5,NC5,NG4,NG4,NE5,ND5,NC5,NA4,
	NA4,NF5,NE5,ND5,NG5,NG5,NG5,NG5,NG5,NA5,NG5,NF5,ND5,NC5,NG5,
	NE5,NE5,NE5,NE5,NE5,NE5,NE5,NG5,NC5,ND5,NE5,
	NF5,NF5,NF5,NF5,NF5,NF5,NE5,NE5,NE5,NE5,NE5,ND5,ND5,NE5,ND5,NG5,
	NE5,NE5,NE5,NE5,NE5,NE5,NE5,NG5,NC5,ND5,NE5,
	NF5,NF5,NF5,NF5,NF5,NF5,NE5,NE5,NE5,NE5,NG5,NG5,NF5,ND5,NC5,
	},
	{
		NG4,NC5,NC5,ND5,NC5,NB4,NA4,NA4,
		NA4,ND5,ND5,NE5,ND5,NC5,NB4,NG4,
	NG4,NE5,NE5,NF5,NE5,ND5,NC5,NA5,NG4,NG4,NA4,ND5,NB4,NC5,
	NG4,NC5,NC5,ND5,NC5,NB4,NA4,NA4,
	NA4,ND5,ND5,NE5,ND5,NC5,NB4,NG4,
	NG4,NE5,NE5,NF5,NE5,ND5,NC5,NA5,NG4,NG4,NA4,ND5,NB4,NC5,
	NG4,NC5,NC5,NC5,NB4,NB4,NC5,NB4,NA4,NG4,
	ND5,NE5,ND5,ND5,NC5,NC5,NG5,NG4,NG4,NG4,NA4,ND5,NB4,NC5
	}
};

const int Digit::durations[2][98] = {
	{
		8,8,8,8,2,8,8,8,8,2, // 10
		8,8,8,8,2,8,8,8,8,2, // 10
	8,8,8,8,2,8,8,8,8,2, // 10
	8,8,8,8,8,8,8,16,16,8,8,8,8,4,4, // 15
	8,8,4,8,8,4,8,8,8,8,2, // 11
	8,8,8,16,16,8,8,8,16,16,8,8,8,8,4,4, // 16
	8,8,4,8,8,4,8,8,8,8,2, // 11
	8,8,8,16,16,8,8,8,16,16,8,8,8,8,2, // 15
	},
	{
		4,4,8,8,8,8,4,4,
		4,4,8,8,8,8,4,4,
	4,4,8,8,8,8,4,4,8,8,4,4,4,2,
	4,4,8,8,8,8,4,4,
	4,4,8,8,8,8,4,4,
	4,4,8,8,8,8,4,4,8,8,4,4,4,2,
	4,4,4,4,2,4,4,4,4,2,
	4,4,8,8,8,8,4,4,8,8,4,4,4,2,
	}

};

Digit::~Digit() {
	delete this->sr;
}

Digit::Digit() {
	this->sr = new ShiftRegister74HC595(this->numberOfShiftRegisters, this->serialDataPin, this->clockPin, this->latchPin);
	this->sr->setAllLow();
}

void Digit::setNum(int digit, int num) {
	for (int i = 0; i < 7; i++) {
		this->sr->set(Digit::digits[digit][i], Digit::nums[num][i]);
	}
}

void Digit::blink(uint8_t second) {
	this->sr->set(13, second % 3);
}

void Digit::setTime(int h, int m) {
	if (h > 9) {
		this->setNum(0, h / 10);
		this->setNum(1, h % 10);
	} else {
		this->setNum(0, 0);
		this->setNum(1, h);
	}

	if (m > 9) {
		this->setNum(2, m / 10);
		this->setNum(3, m % 10);
	} else {
		this->setNum(2, 0);
		this->setNum(3, m);
	}
}

void Digit::playMelody() {

	for (int i = 0; i < 2; i++) {
		for (int thisNote = 0; thisNote < 98; thisNote++) {

			// to calculate the note duration, take one second
			// divided by the note type.
			//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
			int noteDuration = 1000 / Digit::durations[i][thisNote];
			tone(5, Digit::melodies[i][thisNote], noteDuration);

			// to distinguish the notes, set a minimum time between them.
			// the note's duration + 30% seems to work well:
			int pauseBetweenNotes = noteDuration * 1.30;
			delay(pauseBetweenNotes);
			// stop the tone playing:
			noTone(5);
		}
	}
}


