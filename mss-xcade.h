#ifndef _MSS_XCADE_H_
#define _MSS_XCADE_H_

#include "debounce.h"
#include "pca9555.h"

#include "mss-typedefs.h"
#include "mss-port.h"
#include "mss-wire-mux.h"
#include "mss-gpio.h"



class XCade { 
	public:
		XCade();
		MSSPort mssPortA;
		MSSPort mssPortB;
		MSSPort mssPortC;
		MSSPort mssPortD;
		GPIO gpio;
/*		SignalHead signalHeadA1;
		SignalHead signalHeadA2;
		SignalHead signalHeadB1;
		SignalHead signalHeadB2;
		SignalHead signalHeadC1;
		SignalHead signalHeadC2;
		SignalHead signalHeadD1;
		SignalHead signalHeadD2;*/

		bool begin(uint8_t muxID, TwoWire* wire = &Wire);
		void updateInputs();
		void updateOutputs();

	private:
		TwoWire* wire;
		WireMux* wireMux;
		uint8_t muxID;
		WireMux masterWireMux; // Only used on XCade 0
		PCA9555 ioexDriverAB;
		PCA9555 ioexDriverCD;
};
/*
void XCade::begin(uint8_t muxID)
{
  this->muxID = muxID;
  i2cMuxSelectBoard(this->muxID);
  pca9555_setup(&this->mss_AB, 0x20, 0x0F0F, 0x0000);
  pca9555_setup(&this->mss_CD, 0x21, 0x0F0F, 0x0000);
  pca9555_setup(&this->senseio, 0x22, 0xFFFF, 0x0000);
	this->gpio.begin(&this->senseio, this->muxID);
}
*/

#endif