#include "pca9555.h"

PCA9555::PCA9555()
{
	this->wire = NULL;
	this->dir = 0xFFFF;
	this->in = 0xFFFF;
	this->out = 0xFFFF;
}

void PCA9555::begin(uint8_t addr, TwoWire* wire)
{
	this->addr = addr;
	this->wire = wire;
	this->reset();
}

void PCA9555::begin(uint8_t addr, TwoWire* wire, uint16_t dir, uint16_t out)
{
	this->addr = addr;
	this->wire = wire;
	this->reset(dir, out);
}

uint8_t PCA9555::write(uint16_t out, bool deferWrite)
{
	if (NULL == this->wire)
		return 4; // same as "other error" from endTransmission
	this->out = out;

	if (!deferWrite)
		return this->writeDeferredValues(); 

	return 0; // Success because it was a cached write
}

uint8_t PCA9555::write(uint8_t pin, bool state, bool deferWrite)
{
	if (pin > 15)
		return 0;
	uint16_t mask = (uint16_t)1<<pin;
	if (state)
		this->out |= mask;
	else
		this->out &= ~(mask);
	return this->write(this->out, deferWrite);
}

uint8_t PCA9555::writeDeferredValues()
{
	this->wire->beginTransmission(this->addr);
	this->wire->write(PCA9555_REG_OUTPUT0);
	this->wire->write(this->out & 0xFF);
	this->wire->write((this->out>>8) & 0xFF);
	return (uint8_t)this->wire->endTransmission(); 
}

uint16_t PCA9555::read(bool useCached)
{
	if (NULL == this->wire)
		return 0;

	if (!useCached)
	{
		uint16_t retval = 0;
		this->wire->beginTransmission(this->addr);
		this->wire->write(PCA9555_REG_INPUT0);
		this->wire->endTransmission(false);
		this->wire->requestFrom((int)this->addr, (int)2);
		retval = this->wire->read();
		retval |= ((uint16_t)this->wire->read()<<8);
		if (0 == this->wire->endTransmission())
			this->in = retval;
	}
	return this->in;
}

bool PCA9555::read(uint8_t pin, bool useCached)
{
	if (pin > 15)
		return false;
	uint16_t mask = (uint16_t)1<<pin;
	return  (this->read(useCached) & mask)?true:false;
}


uint8_t PCA9555::setDirection(uint16_t dir)
{
	if (NULL == this->wire)
		return 4; // same as "other error" from endTransmission
	this->dir = dir;
	this->wire->beginTransmission(this->addr);
	this->wire->write(PCA9555_REG_DIRECTION0);
	this->wire->write(this->dir & 0xFF);
	this->wire->write((this->dir>>8) & 0xFF);
	return (uint8_t)this->wire->endTransmission();  
}

uint8_t PCA9555::setDirection(uint8_t pin, bool isInput)
{
	if (pin > 15)
		return 0;
	uint16_t mask = (uint16_t)1<<pin;
	if (isInput)
		this->dir |= mask;
	else
		this->dir &= ~(mask);

	return this->setDirection(this->dir);
}

uint8_t PCA9555::reset(uint16_t dir, uint16_t out)
{
	if (NULL == this->wire)
		return 4; // same as "other error" from endTransmission

	// Reset the inversion registers
	this->wire->beginTransmission(this->addr);
	this->wire->write(0x04);
	this->wire->write(0);
	this->wire->write(0);
	this->wire->endTransmission();  

	this->setDirection(dir);
	return this->write(out);
}

