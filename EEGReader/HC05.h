/*
*
* HC-05 is a bluetooth module which can work in both slave and master mode.
* It has a full (thought weird) AT command interface, which may be triggered
* through an specific pin
*
*/

#ifndef __HC05_H__
#define __HC05_H__

#include "Arduino.h"
#include "AbstractSerial.h"
#include "EEGReader.h"

namespace HC
{

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
    void Setup(AbstractSerial* bluetooth, Role role, Baudrate baudrate);

}

#endif
