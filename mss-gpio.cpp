#include "mss-gpio.h"

GPIO::GPIO() {
	this->ddr = 0xFFFF;
	this->out = 0x0000;
	this->inversionMask = 0x03FF;
	this->debouncedIn.init(0xFFFF);
}

void GPIO::begin(WireMux* wireMux, uint8_t muxID)
{
	this->wireMux = wireMux;
	this->muxID = muxID;
	this->ioex.begin(0x22, this->wireMux->getWire(), 0xFFFF, 0x0000);
}

void GPIO::doRead()
{
	if (NULL == this->wireMux)
		return;
	this->wireMux->setPort(this->muxID);
	this->debouncedIn.debounce(this->ioex.read());
}

void GPIO::updateOutputs()
{
	if (NULL == this->wireMux)
		return;

	this->wireMux->setPort(this->muxID);
	this->ioex.write(this->out);
}

uint8_t GPIO::gpioNumToBit(uint8_t gpioNum)
{
	uint8_t retvals[17] = { 255, 
		15, // GPIO 1
		14, // GPIO 2
		13, // GPIO 3
		12, // GPIO 4
		11, // GPIO 5
		10, // GPIO 6
		0, // Sensor 1
		1, // Sensor 2
		2, // Sensor 3
		3, // Sensor 4
		4, // Sensor 5
		5, // Sensor 6
		6, // Sensor 7,
		7, // Sensor 8
		8, // Sensor 9
		9, // Sensor 10
	};
	if (0 == gpioNum || gpioNum > (sizeof(retvals)/sizeof(retvals[0])))
		return 255;
	return retvals[gpioNum];
}

void GPIO::pinMode(uint8_t gpioNum, uint8_t gpioMode)
{
	uint8_t gpioBit = this->gpioNumToBit(gpioNum);
	if (255 == gpioBit)
		return;  // Out of range, do nothing
	uint16_t bitmask = 1<<gpioBit;

	if (gpioMode == INPUT || gpioMode == INPUT_PULLUP)
		this->ddr |= bitmask;
	else
		this->ddr &= ~bitmask;

	this->wireMux->setPort(this->muxID);
	this->ioex.setDirection(this->ddr);
}

void GPIO::invertInput(uint8_t gpioNum, bool inverted)
{
	uint8_t gpioBit = this->gpioNumToBit(gpioNum);
	if (255 == gpioBit)
		return;  // Out of range, do nothing
	uint16_t bitmask = 1<<gpioBit;

	if (inverted)
		this->inversionMask |= bitmask;
	else
		this->inversionMask &= ~bitmask;
}

void GPIO::digitalWrite(uint8_t gpioNum, uint8_t value, bool immediate)
{
	uint8_t gpioBit = this->gpioNumToBit(gpioNum);
	if (255 == gpioBit)
		return;  // Out of range, do nothing
	uint16_t bitmask = 1<<gpioBit;

	if (value)
		this->out |= bitmask;
	else
		this->out &= ~bitmask;

	if (immediate)
		this->updateOutputs();
}

bool GPIO::digitalRead(uint8_t gpioNum, bool immediate)
{
	uint8_t gpioBit = this->gpioNumToBit(gpioNum);
	if (255 == gpioBit)
		return false;  // Out of range, do nothing

	uint16_t in = this->debouncedIn.getDebouncedState();

	if (immediate)
	{
		this->wireMux->setPort(this->muxID);
		in = this->ioex.read();
	}

	in ^= this->inversionMask;
	return (in & (1<<gpioBit))?true:false;
}