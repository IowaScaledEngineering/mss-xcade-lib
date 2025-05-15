#include "mss-config-sw.h"

bool MSSConfigSwitches::getSwitch(uint8_t switchNum)
{
	if (switchNum > 7)
		return false;

	return 0 != ((1<<switchNum) & this->debouncedInputs.getDebouncedState());
}

uint8_t MSSConfigSwitches::readDebounced()
{
	return this->debouncedInputs.getDebouncedState();
}

void MSSConfigSwitches::update()
{
	this->debouncedInputs.debounce(this->readRaw());
}

uint8_t MSSConfigSwitches::readRaw()
{
	uint8_t retval = 0;
	retval |= digitalRead(9)?0:0x40;
	retval |= digitalRead(10)?0:0x20;
	retval |= digitalRead(11)?0:0x10;
	retval |= digitalRead(12)?0:0x08;
	retval |= digitalRead(13)?0:0x04;
	retval |= digitalRead(14)?0:0x02;
	retval |= digitalRead(21)?0:0x01;
	return retval;
}

void MSSConfigSwitches::begin()
{
	pinMode(21, INPUT_PULLUP);
	pinMode(14, INPUT_PULLUP);
	pinMode(13, INPUT_PULLUP);
	pinMode(12, INPUT_PULLUP);
	pinMode(11, INPUT_PULLUP);
	pinMode(10, INPUT_PULLUP);
	pinMode(9, INPUT_PULLUP);
	this->debouncedInputs.init(this->readRaw());
}

MSSConfigSwitches::MSSConfigSwitches()
{
	this->debouncedInputs.init(0);
}