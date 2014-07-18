/*
*
* ThinkGear ASIC Module (TGAM) serial communication code.
* Note that 17 stands for firmware version and not board revision
* Firmware 1.7 (17) has been found on rev2.8 boards.
*
* 2.3 and 2.8 boards are pin compatible but differ in firmware configuration.
* Specific code must be written for each of them.
*
*/

#ifndef __TGAM17_H__
#define __TGAM17_H__

#include "Arduino.h"
#include "AbstractSerial.h"
#include "EEGReader.h"

namespace TGAM
{

    /**
     * Sets up the TGAM module given a Serial (either Software or Hardware) connection to
     * it, the requiered configuration, an initial baudrate at which to communicate and a targetBaudrate
     * to stablish.
     *
     * @param stream Serial communication
     * @param config TGAM config mode
     * @param initialBaudrate Baudrate at which to connect at first
     * @param targetBaudrate Baudrate to set the TGAM to
     */
    void Setup(AbstractSerial* stream, Config config, Baudrate initialBaudrate, Baudrate targetBaudrate);

    /**
     * Sets up the TGAM module given a Serial (either Software or Hardware) connection to
     * it, the requiered configuration and a targetBaudrate
     * to stablish.
     * Default TGAM baudrate is BAUDRATE_NORMAL
     *
     * @param stream Serial communication
     * @param config TGAM config mode
     * @param targetBaudrate Baudrate to set the TGAM to
     */
    inline void Setup(AbstractSerial* stream, Config config, Baudrate targetBaudrate)
    {
        Setup(stream, config, BAUD_NORMAL, targetBaudrate);
    }

    /**
     * Sets up the TGAM module given a Serial (either Software or Hardware) connection to
     * it and the requiered configuration.
     * to stablish.
     * Default TGAM baudrate is BAUDRATE_NORMAL
     * Target baudrate is deducted from the config parameter
     *
     * @param stream Serial communication
     * @param config TGAM config mode
     */
    inline void Setup(AbstractSerial* stream, Config config)
    {
        switch (config)
        {
            case CONFIG_DEFAULT:
                Setup(stream, config, BAUD_NORMAL, BAUD_NORMAL);
                break;

            case CONFIG_SLOW:
                Setup(stream, config, BAUD_NORMAL, BAUD_SLOW);
                break;

            case CONFIG_RAW:
            case CONFIG_FFT:
                Setup(stream, config, BAUD_NORMAL, BAUD_FAST);
                break;
        }

    }

}

#endif
