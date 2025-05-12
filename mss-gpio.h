#ifndef _MSS_GPIO_H_
#define _MSS_GPIO_H_

#include <stdint.h>
#include "mss-typedefs.h"
#include "debounce.h"
#include "pca9555.h"
#include "mss-wire-mux.h"

// This is a wrapper class around the gpio PCA9555 that allows an Arduino-friendly naming style

#define GPIO_SENSOR_BASE  7
#define SENSOR_1_PIN      (GPIO_SENSOR_BASE + 0)
#define SENSOR_2_PIN      (GPIO_SENSOR_BASE + 1)
#define SENSOR_3_PIN      (GPIO_SENSOR_BASE + 2)
#define SENSOR_4_PIN      (GPIO_SENSOR_BASE + 3)
#define SENSOR_5_PIN      (GPIO_SENSOR_BASE + 4)
#define SENSOR_6_PIN      (GPIO_SENSOR_BASE + 5)
#define SENSOR_7_PIN      (GPIO_SENSOR_BASE + 6)
#define SENSOR_8_PIN      (GPIO_SENSOR_BASE + 7)
#define SENSOR_9_PIN      (GPIO_SENSOR_BASE + 8)
#define SENSOR_10_PIN     (GPIO_SENSOR_BASE + 9)

class GPIO
{
	public:
		GPIO();
		void begin(WireMux* wireMux, uint8_t muxID);
		void pinMode(uint8_t gpioNum, uint8_t gpioMode);
		void digitalWrite(uint8_t gpioNum, uint8_t value, bool immediate = false);
		bool digitalRead(uint8_t gpioNum, bool immediate = false);
		void invertInput(uint8_t gpioNum, bool inverted);
		void doRead();
		void doWrite();

	private:
		WireMux* wireMux;
		PCA9555 ioex;
		Debouncer<uint16_t> debouncedIn;
		static uint8_t gpioNumToBit(uint8_t gpioNum);
		uint16_t ddr;
		uint16_t out;
		uint16_t inversionMask;
		uint8_t muxID;
};


#endif
