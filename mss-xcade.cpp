#include "mss-xcade.h"

XCade::XCade()
{
	this->wireMux = NULL;
	this->muxID = 255;
}

bool XCade::begin(TwoWire* wire)
{
	this->masterWireMux.begin(wire);
	this->wireMux = &this->masterWireMux;
	this->muxID = 255; // Basically bypass ever using it

	return this->beginCommon();

}

bool XCade::begin(WireMux* wireMux, uint8_t muxID)
{
	this->wireMux = wireMux;
	this->muxID = muxID;
	return this->beginCommon();
}


bool XCade::beginCommon()
{
	// Select this XCade on the I2C bus mux
	this->wireMux->setPort(this->muxID);

	// Update the config switches if this is board 0
	if (0 == this->wireMux->getPort())
		this->configSwitches.begin();

		// Initialize the three TCA9555 GPIO drivers
	this->ioexDriverAB.begin(0x20, this->wireMux->getWire(), 0x0F0F, 0x0000);
	this->ioexDriverCD.begin(0x21, this->wireMux->getWire(), 0x0F0F, 0x0000);

	this->mssPortA.begin();
	this->mssPortB.begin();
	this->mssPortC.begin();
	this->mssPortD.begin();

	this->gpio.begin(this->wireMux, this->muxID);
	this->signals.begin(this->wireMux, this->muxID);

	for(uint8_t i=0; i<4; i++)
		this->updateInputs(); // Prime the debouncer

	return true;
}


void XCade::updateInputs()
{
	// These work out because the wires are mapped per the defines from mss-port.h
	// MSS_PORT_AA_IN_MASK  0x01
	// MSS_PORT_A_IN_MASK   0x02
	// MSS_PORT_S_IN_MASK   0x04
	// MSS_PORT_DA_IN_MASK  0x08

	this->wireMux->setPort(this->muxID);

	uint16_t i = this->ioexDriverAB.read();

//	Serial.printf("IOEX AB - 0x%04x\n", i);

	this->mssPortA.updateInputs(i & 0x0F);
	this->mssPortB.updateInputs((i>>8) & 0x0F);

	i = this->ioexDriverCD.read();
	this->mssPortC.updateInputs(i & 0x0F);
	this->mssPortD.updateInputs((i>>8) & 0x0F);
	
	// Update GPIO debouncer
	this->gpio.doRead();

	// Update the config switches if this is board 0
	if (0 == this->wireMux->getPort())
		this->configSwitches.update();
}

void XCade::updateOutputs()
{
	// These work out because the wires are mapped per the defines from mss-port.h
	// MSS_PORT_AA_IN_MASK  0x01
	// MSS_PORT_A_IN_MASK   0x02
	// MSS_PORT_S_IN_MASK   0x04
	// MSS_PORT_DA_IN_MASK  0x08

	this->wireMux->setPort(this->muxID);

	uint16_t i = this->mssPortA.updateOutputs() | ((uint16_t)this->mssPortB.updateOutputs()<<8);
//  Serial.printf("ioexAB - 0x%04X", i);
  this->ioexDriverAB.write(i);

	i = this->mssPortC.updateOutputs() | ((uint16_t)this->mssPortD.updateOutputs()<<8);
	this->ioexDriverCD.write(i);
//  Serial.printf("  ioexCD - 0x%04X\n", i);

	this->gpio.updateOutputs();
	this->signals.updateOutputs();
}
