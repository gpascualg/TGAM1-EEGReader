/*
*
* HC-05 is a bluetooth module which can work in both slave and master mode.
* It has a full (thought weird) AT command interface, which may be triggered
* through an specific pin
*
*/

#ifndef __HC_H__
#define __HC_H__

namespace HC
{

    /**
     * HC command mode or work mode
     */
    enum WorkMode
    {
        MODE_WORK       = 0, // Work mode enables to use bluetooth functionalities
        MODE_COMMANDS   = 1  // Commands mode allows to use AT Commands
    };

    /**
     * HC role, slave or master.
     * Note that only HC-05 does have this function, HC-06 is limited and comes
     * pre-stablished to slave or master
     */
    enum Role
    {
        ROLE_SLAVE      = 0,
        ROLE_MASTER     = 1,
        UNSUPPORTED     = 2
    };

}

#if defined(USING_HC05)
    #include "HC05.h"
#elif defined(USING_HC06)
    #include "HC06.h"
#endif

#endif
