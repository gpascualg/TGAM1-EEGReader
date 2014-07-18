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

}

#if defined(USING_TGAM16)
    #include "TGAM16.h"
#elif defined(USING_TGAM17)
    #include "TGAM17.h"
#endif

#endif
