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
    BAUD_NORMAL     = 9600,
    BAUD_SLOW       = 1200,
    BAUD_COMMANDS   = 38400,
    BAUD_FAST       = 57600
};

#include "Brain.h"
#include "HC.h"
#include "TGAM.h"

#endif
