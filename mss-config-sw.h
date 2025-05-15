#ifndef _MSS_CONFIGSWITCH_H_
#define _MSS_CONFIGSWITCH_H_

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "debounce.h"

class MSSConfigSwitches {
	public:
		MSSConfigSwitches();
		void begin();
		uint8_t readRaw();
		uint8_t readDebounced();
		void update();
		bool getSwitch(uint8_t switchNum);
	private:
		Debouncer<uint8_t> debouncedInputs;
};

#endif