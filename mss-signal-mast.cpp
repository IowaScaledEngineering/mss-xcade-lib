#include "mss-signal-mast.h"

const static IndicationRule_t singleHead4Indication[] = 
{
	{ INDICATION_STOP,                  SignalMast::DIVMASK_ALL, ASPECT_RED,       ASPECT_OFF,       ASPECT_OFF       },
	{ INDICATION_APPROACH,              SignalMast::DIVMASK_ALL, ASPECT_YELLOW,    ASPECT_OFF,       ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING,    SignalMast::DIVMASK_ALL, ASPECT_YELLOW,    ASPECT_OFF,       ASPECT_OFF       },
	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_ALL, ASPECT_FL_YELLOW, ASPECT_OFF,       ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_ALL, ASPECT_FL_YELLOW, ASPECT_OFF,       ASPECT_FL_YELLOW },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_ALL, ASPECT_GREEN,     ASPECT_OFF,       ASPECT_OFF       }
};

const static IndicationRule_t singleHead3Indication[] = 
{
	{ INDICATION_STOP,                  SignalMast::DIVMASK_ALL, ASPECT_RED,       ASPECT_OFF,       ASPECT_OFF       },
	{ INDICATION_APPROACH,              SignalMast::DIVMASK_ALL, ASPECT_YELLOW,    ASPECT_OFF,       ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING,    SignalMast::DIVMASK_ALL, ASPECT_YELLOW,    ASPECT_OFF,       ASPECT_OFF       },
	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_ALL, ASPECT_GREEN,     ASPECT_OFF,       ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_ALL, ASPECT_GREEN,     ASPECT_OFF,       ASPECT_OFF       },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_ALL, ASPECT_GREEN,     ASPECT_OFF,       ASPECT_OFF       }
};

const static IndicationRule_t doubleHead4IndicationWesternUS[] = 
{
	{ INDICATION_STOP,                  SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_RED,       ASPECT_RED,       ASPECT_OFF       },
	{ INDICATION_APPROACH,              SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_YELLOW,    ASPECT_RED,       ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING,    SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_YELLOW,    ASPECT_YELLOW,    ASPECT_OFF       },
	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_FL_YELLOW, ASPECT_RED,       ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_FL_YELLOW, ASPECT_RED,       ASPECT_OFF       },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_GREEN,     ASPECT_RED,       ASPECT_OFF       },

	{ INDICATION_STOP,                  SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_RED,       ASPECT_OFF       },
	{ INDICATION_APPROACH,              SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF,      },
	{ INDICATION_APPROACH_DIVERGING,    SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF,      },

	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_DIV_FULL_SPD,  ASPECT_RED,       ASPECT_FL_YELLOW, ASPECT_OFF,      },
	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_DIV_NO_FULL ,  ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF,      },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_DIV_FULL_SPD,  ASPECT_RED,       ASPECT_FL_YELLOW, ASPECT_OFF,      },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_DIV_NO_FULL ,  ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF,      },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_DIV_FULL_SPD,  ASPECT_RED,       ASPECT_GREEN,     ASPECT_OFF,      },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_DIV_NO_FULL ,  ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF,      }
};

const static IndicationRule_t doubleHead4IndicationEasternUS[] = 
{
	{ INDICATION_STOP,                  SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_RED,       ASPECT_RED,       ASPECT_OFF       },
	{ INDICATION_APPROACH,              SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_YELLOW,    ASPECT_RED,       ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING,    SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_YELLOW,    ASPECT_GREEN,     ASPECT_OFF       },
	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_YELLOW,    ASPECT_YELLOW,    ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_YELLOW,    ASPECT_YELLOW,    ASPECT_OFF       },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_GREEN,     ASPECT_RED,       ASPECT_OFF       },

	{ INDICATION_STOP,                  SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_RED,       ASPECT_OFF       },
	{ INDICATION_APPROACH,              SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING,    SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF       },

	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_DIV_FULL_SPD,  ASPECT_RED,       ASPECT_FL_YELLOW, ASPECT_OFF       },
	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_DIV_NO_FULL ,  ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_DIV_FULL_SPD,  ASPECT_RED,       ASPECT_FL_YELLOW, ASPECT_OFF       },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_DIV_NO_FULL ,  ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF       },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_DIV_FULL_SPD,  ASPECT_RED,       ASPECT_GREEN,     ASPECT_OFF       },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_DIV_NO_FULL ,  ASPECT_RED,       ASPECT_YELLOW,    ASPECT_OFF       }
};

const static IndicationRule_t tripleHead4Indication[] = 
{
	{ INDICATION_STOP,                  SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_RED,       ASPECT_RED,       ASPECT_RED       },
	{ INDICATION_APPROACH,              SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_YELLOW,    ASPECT_RED,       ASPECT_RED       },
	{ INDICATION_APPROACH_DIVERGING,    SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_YELLOW,    ASPECT_YELLOW,    ASPECT_RED       },
	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_FL_YELLOW, ASPECT_RED,       ASPECT_RED       },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_FL_YELLOW, ASPECT_RED,       ASPECT_RED       },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_NOT_DIVERGING, ASPECT_GREEN,     ASPECT_RED,       ASPECT_RED       },

	{ INDICATION_STOP,                  SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_RED,       ASPECT_RED       },


	{ INDICATION_APPROACH,              SignalMast::DIVMASK_DIV_SLOW_SPD,  ASPECT_RED,       ASPECT_RED,       ASPECT_YELLOW    },
	{ INDICATION_APPROACH,              SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_YELLOW,    ASPECT_RED       },

	{ INDICATION_APPROACH_DIVERGING,    SignalMast::DIVMASK_DIV_SLOW_SPD,  ASPECT_RED,       ASPECT_YELLOW,    ASPECT_GREEN     },
	{ INDICATION_APPROACH_DIVERGING,    SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_YELLOW,    ASPECT_GREEN,     ASPECT_RED       },

	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_DIV_SLOW_SPD,  ASPECT_RED,       ASPECT_RED,       ASPECT_FL_YELLOW },
	{ INDICATION_APPROACH_DIVERGING_AA, SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_FL_YELLOW, ASPECT_RED       },
	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_DIV_SLOW_SPD,  ASPECT_RED,       ASPECT_RED,       ASPECT_FL_YELLOW },
	{ INDICATION_ADVANCE_APPROACH,      SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_FL_YELLOW, ASPECT_RED       },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_DIV_SLOW_SPD,  ASPECT_RED,       ASPECT_RED,       ASPECT_GREEN     },
	{ INDICATION_CLEAR,                 SignalMast::DIVMASK_ALL_DIVERGING, ASPECT_RED,       ASPECT_GREEN,     ASPECT_RED       } 
};

void SignalMast::setDefaultSignalRules()
{
	this->singleHeadRules = singleHead4Indication;
	this->singleHeadRuleLen = sizeof(singleHead4Indication) / sizeof(IndicationRule_t);
	this->doubleHeadRules = doubleHead4IndicationWesternUS;
	this->doubleHeadRuleLen = sizeof(doubleHead4IndicationWesternUS) / sizeof(IndicationRule_t);
}

void SignalMast::setSingleHeadRules(const IndicationRule_t* indicationRules, uint16_t indicationRulesLen)
{
	this->singleHeadRules = indicationRules;
	this->singleHeadRuleLen = indicationRulesLen;;
}

void SignalMast::setDoubleHeadRules(const IndicationRule_t* indicationRules, uint16_t indicationRulesLen)
{
	this->doubleHeadRules = indicationRules;
	this->doubleHeadRuleLen = indicationRulesLen;
}


void SignalMast::addSignalHeads(SignalHead* h1, SignalHead* h2, SignalHead* h3)
{
	this->h1 = h1;
	this->h2 = h2;
	this->h3 = h3;
}

SignalMast::SignalMast(SignalHead* h1, SignalHead* h2, SignalHead* h3)
{
	this->setDefaultSignalRules();
	this->addSignalHeads(h1, h2, h3);
}

void SignalMast::setIndication(MSSPort& port, DivergingRoute_t diverging, bool mastLit)
{
	this->setIndication(port.indicationReceivedGet(), diverging, mastLit);
}

void SignalMast::setIndication(MSSPortIndication_t indication, DivergingRoute_t diverging, bool mastLit)
{
	uint32_t rulesLen = 0;
	uint8_t divergingTestMask = (diverging<DIVERGING_END)?(1<<diverging):0;
	bool ruleMatch = false;
	const IndicationRule_t* rulesPtr = NULL;

	if (NULL != this->h1 && NULL == this->h2 && NULL == this->h3)
	{
		rulesLen = this->singleHeadRuleLen;
		rulesPtr = this->singleHeadRules;
	} else if (NULL != h1 && NULL != h2 && NULL == h3) {
		rulesLen = this->doubleHeadRuleLen;
		rulesPtr = this->doubleHeadRules;
	} else if (NULL != h1 && NULL != h2 && NULL != h3) {
		rulesLen = this->tripleHeadRuleLen;
		rulesPtr = this->tripleHeadRules;
	} else {
		return;
	}

	for(uint32_t i=0; i<rulesLen && NULL != rulesPtr; i++, rulesPtr++)
	{
		if (indication == rulesPtr->indication && 0 != (divergingTestMask & rulesPtr->divergingMask))
		{
			if (NULL != this->h1)
				this->h1->setAspect(rulesPtr->head1Aspect);
			if (NULL != this->h2)
				this->h2->setAspect(rulesPtr->head2Aspect);
			if (NULL != this->h3)
				this->h3->setAspect(rulesPtr->head3Aspect);
			ruleMatch = true;
			break;
		}
	}

	if (!mastLit || !ruleMatch)
	{
		if (NULL != this->h1)
			this->h1->setAspect(ASPECT_OFF);
		if (NULL != this->h2)
			this->h2->setAspect(ASPECT_OFF);
		if (NULL != this->h3)
			this->h3->setAspect(ASPECT_OFF);
	}
}
