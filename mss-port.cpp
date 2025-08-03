#include "Arduino.h"
#include "mss-port.h"

MSSPort::MSSPort()
{
	this->A_out = this->AA_out = this->S_out = this->DA_out = false;
}

void MSSPort::begin()
{
	this->debouncedInputs.init(0);
}

void MSSPort::setLocalOccupancy(bool localOccupancy)
{
	this->S_out = localOccupancy;
}

bool MSSPort::DA_mask()
{
  // The purpose of this function is to see if we've asserted DA_out within the
  //  cycle count of the debouncer.  If we have, then DA_in is unreliable and should
  //  not be used.

  return (this->DA_out || (0 != (this->debouncedInputs.getDebouncedState() & MSS_PORT_DA_OUT_MASK)));
}

MSSPortIndication_t MSSPort::indicationReceivedGet()
{
//	Serial.printf("Port - S=%d A=%d AA=%d DA=%d\n", this->S_in(), this->A_in(), this->AA_in(), this->DA_in());

	if (this->S_out || this->S_in())
		return INDICATION_STOP;
	
	if (this->A_in() && this->DA_in() && !this->DA_mask())
		return INDICATION_APPROACH_DIVERGING;

	if (this->A_in())
		return INDICATION_APPROACH;

	if (this->AA_in() && this->DA_in() && !this->DA_mask())
		return INDICATION_APPROACH_DIVERGING_AA;

	if (this->AA_in())
		return INDICATION_ADVANCE_APPROACH;

	return INDICATION_CLEAR;
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

void MSSPort::setRawInputs(bool S_in, bool A_in, bool AA_in, bool DA_in)
{
	uint8_t mssRawInputsBitmask = 0;
	mssRawInputsBitmask |= (S_in)?MSS_PORT_S_IN_MASK:0;
	mssRawInputsBitmask |= (A_in)?MSS_PORT_A_IN_MASK:0;
	mssRawInputsBitmask |= (AA_in)?MSS_PORT_AA_IN_MASK:0;
	mssRawInputsBitmask |= (DA_in)?MSS_PORT_DA_IN_MASK:0;
  mssRawInputsBitmask |= (this->DA_out)?MSS_PORT_DA_OUT_MASK:0;
	this->debouncedInputs.debounce(mssRawInputsBitmask);
}

bool MSSPort::getDivergingOut()
{
	return this->DA_out;
}

bool MSSPort::getSingleBlockApproach()
{
	return this->S_in() || this->S_out;
}

bool MSSPort::getDoubleBlockApproach()
{
	return this->S_in() || this->S_out || (this->A_in() && !this->DA_in());
}


void MSSPort::getRawOutputs(bool* S_out, bool* A_out, bool* AA_out, bool* DA_out)
{
	if (NULL != S_out)
		*S_out = this->S_out;

	if (NULL != A_out)
		*A_out = this->A_out;

	if (NULL != AA_out)
		*AA_out = this->AA_out;

	if (this->DA_out && !this->A_out)
	{
		if (NULL != A_out)
			*A_out = true;
		if (NULL != DA_out)
			*DA_out = true;	
	}
}

void MSSPort::updateInputs(uint8_t mssRawInputsBitmask)
{
	this->debouncedInputs.debounce(mssRawInputsBitmask);
}

uint8_t MSSPort::updateOutputs()
{
	uint8_t outputs = 0;

	if (this->S_out)
		outputs |= MSS_PORT_S_OUT_MASK;

	if (this->A_out)
		outputs |= MSS_PORT_A_OUT_MASK;

	if (this->AA_out)
		outputs |= MSS_PORT_AA_OUT_MASK;

	if (this->DA_out && !this->A_out)
		outputs |= MSS_PORT_DA_OUT_MASK | MSS_PORT_A_OUT_MASK;

	return outputs;
}

void MSSPort::printDebugStr()
{
  const char* indicationName = "UNKNOWN";
  switch(this->indicationReceivedGet())
  {
    case INDICATION_STOP:
      indicationName = "STOP";
      break;

    case INDICATION_APPROACH_DIVERGING:
      indicationName = "APPROACH_DIV";
      break;

    case INDICATION_APPROACH:
      indicationName = "APPROACH";
      break;

    case INDICATION_APPROACH_DIVERGING_AA:
      indicationName = "APPROACH_DIV";
      break;

    case INDICATION_ADVANCE_APPROACH:
      indicationName = "ADV_APPROACH";
      break;

    case INDICATION_CLEAR:
      indicationName = "CLEAR";
      break;

    default:
      break;
   }

	Serial.printf("[%-12.12s] Sin=%d Ain=%d AAin=%d DAin=%d Sout=%d Aout=%d AAout=%d DAout=%d", indicationName, this->S_in(), this->A_in(), this->AA_in(), this->DA_in(), this->S_out, this->A_out, this->AA_out, this->DA_out);
}


void MSSPort::cascadeFromPort(MSSPort& port, bool diverging)
{
	this->A_out = port.S_in() || port.S_out;
	this->AA_out = port.A_in();
	this->DA_out = diverging;
}

void MSSPort::cascadeFromIndication(MSSPortIndication_t indication, bool diverging)
{
	this->AA_out = false;
	this->A_out  = false;
	this->DA_out = diverging;

	switch(indication)
	{
		case INDICATION_STOP:
			this->A_out = true;
			break;

		case INDICATION_APPROACH:
		case INDICATION_APPROACH_DIVERGING:
			this->AA_out = true;
			break;

		case INDICATION_APPROACH_DIVERGING_AA:
		case INDICATION_ADVANCE_APPROACH:
		case INDICATION_CLEAR:
		default:
			break;
	}
}
