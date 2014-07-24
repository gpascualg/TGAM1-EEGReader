#ifndef __EEG_READER_H__
#define __EEG_READER_H__

enum Pins
{
    PIN_KEY         = 9,
    PIN_RX          = 10,
    PIN_TX          = 11,
    PIN_POWER       = 12
};

enum Baudrate
{
    BAUD_SLOW       = 1200,
    BAUD_NORMAL     = 9600,
    BAUD_COMMANDS   = 38400,
    BAUD_FAST       = 57600
};

#if defined(USING_PARSER)
    #include "ThinkGearStreamParser.h"
#endif

#if defined(USING_HC05) || defined (USING_HC06)
	#include "HC.h"
#endif

#if defined(USING_TGAM16) || defined(USING_TGAM17)
	#include "TGAM.h"
#endif

#endif
