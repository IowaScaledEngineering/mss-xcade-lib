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

		MSSPortIndication_t indicationReceivedGet();
		void setLocalOccupancy(bool localOccupancy);
		void cascadeFromPort(MSSPort& port, bool diverging=false);
		void cascadeFromIndication(MSSPortIndication_t indication, bool diverging=false);
  
		void getRawOutputs(bool* S_out, bool* A_out, bool* AA_out, bool* DA_out);
		void setRawInputs(bool S_in, bool A_in, bool AA_in, bool DA_in);

		bool getDivergingOut();
		bool getSingleBlockApproach();
		bool getDoubleBlockApproach();


		// DANGER!  These are really only for XCade object to call within its own 
		//  updateInputs and updateOutputs routines.  Don't use them.
		uint8_t updateOutputs();
		void updateInputs(uint8_t mssRawInputsBitmask);
		void printDebugStr();
	private:
		Debouncer<uint8_t> debouncedInputs;
		bool S_in();
		bool A_in();
		bool AA_in();
		bool DA_in();
		bool DA_mask();
		bool S_out;
		bool A_out;
		bool AA_out;
		bool DA_out;
};


#endif
