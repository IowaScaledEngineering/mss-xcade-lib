#ifndef _MSS_TYPEDEFS_H_
#define _MSS_TYPEDEFS_H_

typedef enum
{
	ASPECT_OFF          = 0,
	ASPECT_GREEN        = 1,
	ASPECT_FL_GREEN     = 2,
	ASPECT_YELLOW       = 3,
	ASPECT_FL_YELLOW    = 4,
	ASPECT_RED          = 5,
	ASPECT_FL_RED       = 6,
	ASPECT_LUNAR        = 7,
	ASPECT_END
} SignalAspect_t;

typedef enum 
{
	INDICATION_STOP                  = 0,
	INDICATION_APPROACH              = 1,
	INDICATION_ADVANCE_APPROACH      = 2,
	INDICATION_APPROACH_DIVERGING_AA = 3,
	INDICATION_APPROACH_DIVERGING    = 4,
	INDICATION_CLEAR                 = 5,
	INDICATION_END
} MSSPortIndication_t;

typedef enum
{
	SIGNAL_HEAD_THREE_LIGHT    = 0,
	SIGNAL_HEAD_SEARCHLIGHT    = 1,
	SIGNAL_HEAD_END
} SignalHeadType_t;

typedef enum
{
	SIGNAL_POL_BOARD_SENSING    = 0,
	SIGNAL_POL_COMMON_ANODE     = 1,
	SIGNAL_POL_COMMON_CATHODE   = 2,
	SIGNAL_POL_END
} SignalPolarity_t;

typedef enum
{
	NOT_SPECIFIED               = 0,
	NOT_DIVERGING               = 1,
	DIVERGING_FULL_SPEED        = 2,
	DIVERGING_LIMITED_SPEED     = 3,
	DIVERGING_MEDIUM_SPEED      = 4,
	DIVERGING_SLOW_SPEED        = 5,
	DIVERGING_END
} DivergingRoute_t;

typedef struct {
	MSSPortIndication_t indication;
	uint8_t divergingMask;
	SignalAspect_t head1Aspect;
	SignalAspect_t head2Aspect;
	SignalAspect_t head3Aspect;
} IndicationRule_t;

#endif
