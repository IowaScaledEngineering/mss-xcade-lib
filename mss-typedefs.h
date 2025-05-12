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

#endif
