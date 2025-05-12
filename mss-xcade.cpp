#include "mss-xcade.h"

XCade::XCade()
{
	this->wire = NULL;
	this->muxID = 0;
}




bool XCade::begin(uint8_t muxID, TwoWire* wire)
{
	if (0 == muxID)
	{
		this->masterWireMux.begin(this->wire);
		this->wireMux = &this->masterWireMux;
	} else {
		return false;
	}

	// Select this XCade on the I2C bus mux
	this->wireMux->setPort(this->muxID);

	// Initialize the three TCA9555 GPIO drivers
	this->ioexDriverAB.begin(0x20, this->wire, 0x0F0F, 0x0000);
	this->ioexDriverCD.begin(0x21, this->wire, 0x0F0F, 0x0000);

	this->mssPortA.begin();
	this->mssPortB.begin();
	this->mssPortC.begin();
	this->mssPortD.begin();

	this->gpio.begin(this->wireMux, this->muxID);


	return true;
}

void XCade::updateInputs()
{
	if (NULL == this->wire)
		return;

	// These work out because the wires are mapped per the defines from mss-port.h
	// MSS_PORT_AA_IN_MASK  0x01
	// MSS_PORT_A_IN_MASK   0x02
	// MSS_PORT_S_IN_MASK   0x04
	// MSS_PORT_DA_IN_MASK  0x08

	this->wireMux->setPort(this->muxID);

	uint16_t i = this->ioexDriverAB.read();
	this->mssPortA.updateInputs(i & 0x0F);
	this->mssPortB.updateInputs((i>>8) & 0x0F);

	i = this->ioexDriverCD.read();
	this->mssPortC.updateInputs(i & 0x0F);
	this->mssPortD.updateInputs((i>>8) & 0x0F);

	// Update GPIO debouncer
	this->gpio.doRead();
}

void XCade::updateOutputs()
{

	this->gpio.doWrite();
}
