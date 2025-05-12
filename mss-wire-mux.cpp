#include "mss-wire-mux.h"

WireMux::WireMux() 
{
	this->currentPort = 255;
	this->resetPin = -1;
}

bool WireMux::setPort(uint8_t muxPort, bool force)
{
	if (muxPort > 4)
		return false;

	if (!force && (muxPort == this->currentPort))
		return true;
	
	this->wire->beginTransmission(0x70);
	this->wire->write(1 << muxPort);
	bool success = (0 == this->wire->endTransmission());

	if (success)
		this->currentPort = muxPort;
	return success;
}

void WireMux::reset()
{
	if (-1 != this->resetPin)
	{
		// Set the I2C mux reset line low for 10ms 
		pinMode(this->resetPin, OUTPUT);
		digitalWrite(this->resetPin, LOW);
		delay(5);
		digitalWrite(this->resetPin, HIGH);
	}
}

bool WireMux::begin(TwoWire* wire, int resetPin)
{
	this->wire = wire;
	this->currentPort = 255; // Set to an invalid port so it forces it to change on the first shot
	this->resetPin = (int8_t)resetPin;
	if (-1 != this->resetPin)
	{
		pinMode(this->resetPin, OUTPUT);
		this->reset();
	}
	return true;
}

uint8_t WireMux::getPort()
{
	return this->currentPort;
}

TwoWire* WireMux::getWire()
{
	return this->wire;
}
