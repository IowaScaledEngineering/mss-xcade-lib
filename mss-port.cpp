#include "mss-port.h"

MSSPort::MSSPort()
{
	this->A_out = this->AA_out = this->S_out = this->DA_out = false;
}

void MSSPort::begin()
{
	this->debouncedInputs.init(0);
}

void MSSPort::updateInputs(bool S_in, bool A_in, bool AA_in, bool DA_in)
{
	uint8_t mssRawInputsBitmask = 0;
	mssRawInputsBitmask |= (S_in)?MSS_PORT_S_IN_MASK:0;
	mssRawInputsBitmask |= (A_in)?MSS_PORT_A_IN_MASK:0;
	mssRawInputsBitmask |= (AA_in)?MSS_PORT_AA_IN_MASK:0;
	mssRawInputsBitmask |= (DA_in)?MSS_PORT_DA_IN_MASK:0;
	this->debouncedInputs.debounce(mssRawInputsBitmask);
}

bool MSSPort::S_in() 
{
	return 0 != (this->debouncedInputs.getDebouncedState() & MSS_PORT_S_IN_MASK);
}

bool MSSPort::A_in() 
{
	return 0 != (this->debouncedInputs.getDebouncedState() & MSS_PORT_A_IN_MASK);
}

bool MSSPort::AA_in() 
{
	return 0 != (this->debouncedInputs.getDebouncedState() & MSS_PORT_AA_IN_MASK);
}

bool MSSPort::DA_in() 
{
	return 0 != (this->debouncedInputs.getDebouncedState() & MSS_PORT_DA_IN_MASK);
}


void MSSPort::updateInputs(uint8_t mssRawInputsBitmask)
{
	this->debouncedInputs.debounce(mssRawInputsBitmask);
}

void MSSPort::updateOutputs()
{
/*	if (NULL == this->ioex)
		return;

	this->ioex->write(this->bit_S_out, this->S_out, true);
	this->ioex->write(this->bit_A_out, this->A_out, true);
	this->ioex->write(this->bit_AA_out, this->AA_out, true);
	this->ioex->write(this->bit_DA_out, this->DA_out, true);
	this->ioex->writeDeferredValues();*/
}
