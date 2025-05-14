#ifndef _MSS_SIGNALMAST_H_
#define _MSS_SIGNALMAST_H_

#include "mss-signals.h"
#include "mss-port.h"
#include "mss-typedefs.h"

class SignalMast
{
	public:
		SignalMast(SignalHead* h1 = NULL, SignalHead* h2 = NULL, SignalHead* h3 = NULL);
		void addSignalHeads(SignalHead* h1, SignalHead* h2 = NULL, SignalHead* h3 = NULL);
		void setDefaultSignalRules();
		void setSingleHeadRules(const IndicationRule_t* indicationRules, uint16_t indicationRulesLen);
		void setDoubleHeadRules(const IndicationRule_t* indicationRules, uint16_t indicationRulesLen);

		void setIndication(MSSPort& port, DivergingRoute_t diverging = NOT_DIVERGING, bool mastLit = true);
		void setIndication(MSSPortIndication_t indication, DivergingRoute_t diverging = NOT_DIVERGING, bool mastLit = true);

		const static uint8_t DIVMASK_NOT_DIVERGING  = (1<<NOT_DIVERGING);
		const static uint8_t DIVMASK_DIV_HIGH_SPD = (1<<DIVERGING_HIGH_SPEED);
		const static uint8_t DIVMASK_DIV_LTD_SPD = (1<<DIVERGING_LIMITED_SPEED);
		const static uint8_t DIVMASK_DIV_MED_SPD = (1<<DIVERGING_MEDIUM_SPEED);
		const static uint8_t DIVMASK_DIV_LOW_SPD = (1<<DIVERGING_LOW_SPEED);
		const static uint8_t DIVMASK_ALL = (DIVMASK_NOT_DIVERGING | DIVMASK_DIV_HIGH_SPD | DIVMASK_DIV_LTD_SPD | DIVMASK_DIV_MED_SPD | DIVMASK_DIV_LOW_SPD);
		const static uint8_t DIVMASK_ALL_DIVERGING = (DIVMASK_DIV_HIGH_SPD | DIVMASK_DIV_LTD_SPD | DIVMASK_DIV_MED_SPD | DIVMASK_DIV_LOW_SPD);
		const static uint8_t DIVMASK_DIV_NO_HSPD = (DIVMASK_DIV_LTD_SPD | DIVMASK_DIV_MED_SPD | DIVMASK_DIV_LOW_SPD);


	private:
		DivergingRoute_t diverging;
		MSSPortIndication_t indication;
		SignalHead* h1;
		SignalHead* h2;
		SignalHead* h3;
		const IndicationRule_t* singleHeadRules;
		uint16_t singleHeadRuleLen;
		const IndicationRule_t* doubleHeadRules;
		uint16_t doubleHeadRuleLen;
		const IndicationRule_t* tripleHeadRules;
		uint16_t tripleHeadRuleLen;

};

#endif
