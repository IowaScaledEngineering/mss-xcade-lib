#ifndef _MSS_PORT_H_
#define _MSS_PORT_H_

#include <stdint.h>
#include <stdbool.h>
#include "mss-typedefs.h"
#include "debounce.h"

#define MSS_PORT_AA_IN_MASK  0x01
#define MSS_PORT_A_IN_MASK   0x02
#define MSS_PORT_S_IN_MASK   0x04
#define MSS_PORT_DA_IN_MASK  0x08
#define MSS_PORT_DA_OUT_MASK 0x10
#define MSS_PORT_A_OUT_MASK  0x20
#define MSS_PORT_S_OUT_MASK  0x40
#define MSS_PORT_AA_OUT_MASK 0x80

class MSSPort {
	public:
		MSSPort();
		void begin();
		void updateOutputs();
		void updateInputs(bool S_in, bool A_in, bool AA_in, bool DA_in);
		void updateInputs(uint8_t mssRawInputsBitmask);


		/*		MSSPortIndication_t indicationReceivedGet();
		MSSPortIndication_t indicationReceivedGet(bool localOccupancy);
		void cascadeFrom(MSSPort& port, bool localOccupancy, bool diverging=false);
		void cascadeFrom(MSSPortIndication_t indication, bool localOccupancy=false, bool diverging=false);
		void localOccupancySet(bool localOccupancy);
		void indicationSet();
  
		void importInputs(uint8_t inputRead);
		uint8_t exportOutputs();*/

	private:
		Debouncer<uint8_t> debouncedInputs;
		bool S_in();
		bool A_in();
		bool AA_in();
		bool DA_in();

		bool S_out;
		bool A_out;
		bool AA_out;
		bool DA_out;

};


#endif
