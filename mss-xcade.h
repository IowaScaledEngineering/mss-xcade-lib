#ifndef _MSS_XCADE_H_
#define _MSS_XCADE_H_

#include "debounce.h"
#include "pca9555.h"

#include "mss-typedefs.h"
#include "mss-port.h"
#include "mss-wire-mux.h"
#include "mss-gpio.h"
#include "mss-signals.h"
#include "mss-signal-mast.h"
#include "mss-config-sw.h"

#define XCADE_I2C_SDA  36
#define XCADE_I2C_SCL  37
#define XCADE_RGB_LED  45

class XCade { 
	public:
		XCade();
		MSSPort mssPortA;
		MSSPort mssPortB;
		MSSPort mssPortC;
		MSSPort mssPortD;
		GPIO gpio;
		SignalController signals;
		MSSConfigSwitches configSwitches;

		bool begin(TwoWire* wire = &Wire);
		bool begin(WireMux* wireMux, uint8_t muxID=0);
		void updateInputs();
		void updateOutputs();

	private:
		bool beginCommon();
		WireMux* wireMux;
		uint8_t muxID;
		WireMux masterWireMux; // Only used as a placeholder if no wiremux is passed in
		PCA9555 ioexDriverAB;
		PCA9555 ioexDriverCD;
};


#endif