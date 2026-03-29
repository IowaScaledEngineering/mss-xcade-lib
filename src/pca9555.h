#ifndef _PCA9555_H_
#define _PCA9555_H_

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include <stdbool.h>

class PCA9555 {
	public:
		PCA9555();
		void begin(uint8_t addr, TwoWire* wire = &Wire);
		void begin(uint8_t addr, TwoWire* wire, uint16_t dir, uint16_t out);
		uint8_t write(uint16_t out, bool deferWrite = false);
		uint8_t write(uint8_t pin, bool state, bool deferWrite = false);
		uint8_t writeDeferredValues();
		uint16_t read(bool useCached = false);
		bool read(uint8_t pin, bool useCached = false);
		uint8_t setDirection(uint16_t dir);
		uint8_t setDirection(uint8_t pin, bool isInput);
		uint8_t reset(uint16_t dir = 0xFFFF, uint16_t out = 0xFFFF);

	private:
		TwoWire* wire;
		uint8_t addr;
		uint16_t dir;
		uint16_t out;
		uint16_t in;
};

typedef enum
{
	PCA9555_REG_INPUT0               = 0,
	PCA9555_REG_INPUT1               = 1,
	PCA9555_REG_OUTPUT0              = 2,
	PCA9555_REG_OUTPUT1              = 3,
	PCA9555_REG_INVERT0              = 4,
	PCA9555_REG_INVERT1              = 5,
	PCA9555_REG_DIRECTION0           = 6,
	PCA9555_REG_DIRECTION1           = 7
} PCA555_Registers;

#endif