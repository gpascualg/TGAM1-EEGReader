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

#ifndef __TGAM_H__
#define __TGAM_H__

#if defined(USING_TGAM16)
    #define TGAM_NAMESPACE TGAM16
#elif defined(USING_TGAM17)
    #define TGAM_NAMESPACE TGAM17
#else
    #define TGAM_ENUMS
#endif

#include "Arduino.h"
#include "AbstractSerial.h"
#include "EEGReader.h"

namespace TGAM
{

    /**
     * TGAM's possible work modes
     */
    enum Config
    {
       CONFIG_DEFAULT  = 0x00, // Outputs normal data at BAUDRATE_NORMAL
       CONFIG_SLOW     = 0x01, // Outputs normal data at BAUDRATE_SLOW
       CONFIG_RAW      = 0x02, // Outputs normal and RAW data at BAUDRATE_FAST
       CONFIG_FFT      = 0x03, // Outputs FFT data at BAUDRATE_FAST
    };

#ifndef TGAM_ENUMS

    /**
     * Sets up the TGAM module given a Serial (either Software or Hardware) connection to
     * it, the requiered configuration, an initial baudrate at which to communicate and a targetBaudrate
     * to stablish.
     *
     * @param stream Serial communication
     * @param config TGAM config mode
     * @param initialBaudrate Baudrate at which to connect at first
     * @param targetBaudrate Baudrate to set the TGAM to
     * @return If successful 0, -1 if config mode is not supported by TGAM
     */
    namespace TGAM16 __attribute__((visibility(hidden)))
    {    
        int Setup(AbstractSerial* stream, Config config, Baudrate initialBaudrate, Baudrate targetBaudrate);
    }

    namespace TGAM17 __attribute__((visibility(hidden)))
    {
        int Setup(AbstractSerial* stream, Config config, Baudrate initialBaudrate, Baudrate targetBaudrate);
    }

    /**
     * Sets up the TGAM module given a Serial (either Software or Hardware) connection to
     * it, the requiered configuration and a targetBaudrate
     * to stablish.
     * Default TGAM baudrate is BAUDRATE_NORMAL
     *
     * @param stream Serial communication
     * @param config TGAM config mode
     * @param targetBaudrate Baudrate to set the TGAM to
     * @return If successful 0, -1 if config mode is not supported by TGAM
     */
    inline int Setup(AbstractSerial* stream, Config config, Baudrate targetBaudrate)
    {
        return TGAM_NAMESPACE::Setup(stream, config, BAUD_NORMAL, targetBaudrate);
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
     * @return If successful 0, -1 if config mode is not supported by TGAM
     */
    inline int Setup(AbstractSerial* stream, Config config)
    {
        switch (config)
        {
            case CONFIG_DEFAULT:
                return Setup(stream, config, BAUD_NORMAL);

            case CONFIG_SLOW:
                return Setup(stream, config, BAUD_SLOW);

            case CONFIG_RAW:
            case CONFIG_FFT:
                return Setup(stream, config, BAUD_FAST);
        }

        return -1;
    }

#endif

}


#endif
