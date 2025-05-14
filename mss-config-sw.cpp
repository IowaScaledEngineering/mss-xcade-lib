#include "mss-config-sw.h"

bool MSSConfigSwitches::digitalRead(uint8_t switchNum)
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
	retval |= gpio_get_level(GPIO_NUM_14)?0x40:0;
	retval |= gpio_get_level(GPIO_NUM_13)?0x20:0;
	retval |= gpio_get_level(GPIO_NUM_12)?0x10:0;
	retval |= gpio_get_level(GPIO_NUM_11)?0x08:0;
	retval |= gpio_get_level(GPIO_NUM_10)?0x04:0;
	retval |= gpio_get_level(GPIO_NUM_9)?0x02:0;
	retval |= gpio_get_level(GPIO_NUM_21)?0x01:0;
	return retval;
}


MSSConfigSwitches::MSSConfigSwitches()
{
	gpio_set_direction(GPIO_NUM_14, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_14, GPIO_PULLUP_ONLY);
	gpio_set_direction(GPIO_NUM_13, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_ONLY);
	gpio_set_direction(GPIO_NUM_12, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_12, GPIO_PULLUP_ONLY);
	gpio_set_direction(GPIO_NUM_11, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_11, GPIO_PULLUP_ONLY);
	gpio_set_direction(GPIO_NUM_10, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_10, GPIO_PULLUP_ONLY);
	gpio_set_direction(GPIO_NUM_9, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_9, GPIO_PULLUP_ONLY);
	gpio_set_direction(GPIO_NUM_21, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_21, GPIO_PULLUP_ONLY);
	this->debouncedInputs.init(this->readRaw());
}