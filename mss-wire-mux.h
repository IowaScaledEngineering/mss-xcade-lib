#ifndef _MSS_WIREMUX_H_
#define _MSS_WIREMUX_H_

#include "Arduino.h"
#include "Wire.h"

class WireMux
{
	public:
		WireMux();
		bool setPort(uint8_t muxPort, bool force=false);
		bool begin(TwoWire* wire, int resetPin=-1);
		void reset();
		uint8_t getPort();
		TwoWire* getWire();
	private:
		TwoWire* wire;
		uint8_t currentPort;
		int8_t resetPin;
};

#endif
