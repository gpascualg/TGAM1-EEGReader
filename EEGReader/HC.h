/*
*
* HC-05 is a bluetooth module which can work in both slave and master mode.
* It has a full (thought weird) AT command interface, which may be triggered
* through an specific pin
*
*/

#ifndef __HC_H__
#define __HC_H__

#if defined(USING_HC05)
    #define HC_NAMESPACE HC05
#elif defined(USING_HC06)
    #define HC_NAMESPACE HC06
#else
    #define HC_ENUMS_ONLY
#endif

#include "Arduino.h"
#include "AbstractSerial.h"
#include "EEGReader.h"

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

#ifndef HC_ENUMS_ONLY

    namespace HC05 __attribute__((visibility(hidden)))
    {
        void Setup(AbstractSerial* bluetooth, Role role, Baudrate baudrate);
    }

    namespace HC06 __attribute__((visibility(hidden)))
    {
        void Setup(AbstractSerial* bluetooth, Role role, Baudrate baudrate);
    }

    /**
     * Power-cycles an HC module
     *
     * @param bluetooth Serial communication
     */
    void Reset(AbstractSerial* bluetooth);

    /**
     * Stablishes HC work mode and target baudrate
     *
     * @param bluetooth Serial comunnication
     * @param mode Target work mode
     * @param baudrate Baudrate at which to connect
     */
    void Mode(AbstractSerial* bluetooth, WorkMode mode, Baudrate baudrate);

    /**
     * Sets up the HC device, including role and target baudrate
     *
     * @param bluetooth Serial comunnication
     * @param role Slave or Master mode
     * @param baudrate Baudrate at which to set HC
     */
    inline void Setup(AbstractSerial* bluetooth, Role role, Baudrate baudrate)
    {
        HC_NAMESPACE::Setup(bluetooth, role, baudrate);
    }

#endif

}

#endif
