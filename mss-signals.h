#ifndef _MSS_SIGNALS_H_
#define _MSS_SIGNALS_H_

#include <stdint.h>
#include "mss-wire-mux.h"
#include "mss-typedefs.h"



class SignalHead {
	public:
		SignalHead();
		void setAspect(SignalAspect_t aspect);
		SignalAspect_t getAspect();
		const char* getAspectText();
		void setSignalHeadType(SignalHeadType_t headType);
		void setSignalHeadPolarity(SignalPolarity_t headPolarity);
		uint8_t getConfigByte();
		static const uint8_t OPTION_COMMON_CATHODE = 0x80;
		static const uint8_t OPTION_COMMON_ANODE   = 0x40;
		static const uint8_t OPTION_CA_CC_SENSE    = 0x00;
		static const uint8_t OPTION_SIGNAL_THREE_LIGHT = 0x00;
		static const uint8_t OPTION_SIGNAL_SEARCHLIGHT = 0x01;
	private:
		void calculateConfigByte();
		SignalAspect_t aspect;
		SignalHeadType_t headType;
		SignalPolarity_t headPolarity;
		uint8_t config;
};

class SignalController 
{
	public:
		SignalHead A1;
		SignalHead A2;
		SignalHead B1;
		SignalHead B2;
		SignalHead C1;
		SignalHead C2;
		SignalHead D1;
		SignalHead D2;
		SignalController();
		void begin(WireMux* mux, uint8_t muxID);
		void updateOutputs();
		void updateConfig();
		static const uint8_t SIGNAL_CONTROLLER_I2C_ADDR = 0x40;
	private:
		WireMux* wireMux;
		uint8_t muxID;
		uint8_t lastConfigBuffer[8];

};

#endif
