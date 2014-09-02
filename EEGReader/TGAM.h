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
    #define TGAM_ENUMS_ONLY
#endif

#include "Arduino.h"
#include "AbstractSerial.h"
#include "EEGReader.h"

namespace TGAM
{

    /**
     * TGAM's possible work modes
     */
    enum ConfigMode
    {
       CONFIG_DEFAULT  = 0x00, // Outputs normal data at BAUDRATE_NORMAL
       CONFIG_SLOW     = 0x01, // Outputs normal data at BAUDRATE_SLOW
       CONFIG_RAW      = 0x02, // Outputs normal and RAW data at BAUDRATE_FAST
       CONFIG_FFT      = 0x03, // Outputs FFT data at BAUDRATE_FAST
    };

    struct Config
    {
        ConfigMode configMode;
        bool quality;
        bool attention;
        bool meditation;
        bool eegPowers;

        Config(ConfigMode _configMode, bool _quality, bool _attention, bool _meditation, bool _eegPowers):
            configMode(_configMode),
            quality(_quality),
            attention(_attention),
            meditation(_meditation),
            eegPowers(_eegPowers)
        {
        }

        Config(ConfigMode configMode)
        {
            Config(configMode, true, true, true, true);
        }

        Config()
        {
            Config(CONFIG_DEFAULT, true, true, true, true);
        }
    };

#ifndef TGAM_ENUMS_ONLY

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
     * @param config TGAM configuration
     * @param targetBaudrate Baudrate to set the TGAM to
     * @return If successful 0, -1 if config mode is not supported by TGAM
     */
    inline int Setup(AbstractSerial* stream, Config config, Baudrate targetBaudrate)
    {
        return TGAM_NAMESPACE::Setup(stream, config, BAUD_NORMAL, targetBaudrate);
    }


    /**
     * Sets up the TGAM module given a Serial (either Software or Hardware) connection to
     * it, the requiered configuration and a targetBaudrate
     * to stablish.
     * Default TGAM baudrate is BAUDRATE_NORMAL
     *
     * @param stream Serial communication
     * @param configMode TGAM config mode
     * @param targetBaudrate Baudrate to set the TGAM to
     * @return If successful 0, -1 if config mode is not supported by TGAM
     */
    inline int Setup(AbstractSerial* stream, ConfigMode configMode, Baudrate targetBaudrate)
    {
        return TGAM_NAMESPACE::Setup(stream, Config(configMode), BAUD_NORMAL, targetBaudrate);
    }

    /**
     * Sets up the TGAM module given a Serial (either Software or Hardware) connection to
     * it and the requiered configuration.
     * to stablish.
     * Default TGAM baudrate is BAUDRATE_NORMAL
     * Target baudrate is deducted from the config parameter
     *
     * @param stream Serial communication
     * @param configMode TGAM config mode
     * @return If successful 0, -1 if config mode is not supported by TGAM
     */
    inline int Setup(AbstractSerial* stream, ConfigMode configMode)
    {
        switch (configMode)
        {
            case CONFIG_DEFAULT:
                return Setup(stream, configMode, BAUD_NORMAL);

            case CONFIG_SLOW:
                return Setup(stream, configMode, BAUD_SLOW);

            case CONFIG_RAW:
            case CONFIG_FFT:
                return Setup(stream, configMode, BAUD_FAST);
        }

        return -1;
    }


	/**
     * Sets up the TGAM module given a Serial (either Software or Hardware) connection to
     * it and the requiered configuration.
     * to stablish.
     * Default TGAM baudrate is BAUDRATE_NORMAL
     * Target baudrate is deducted from the config parameter
     *
     * @param stream Serial communication
     * @param configMode TGAM config mode
     * @return If successful 0, -1 if config mode is not supported by TGAM
     */
    inline int SetupEx(AbstractSerial* stream, Config config, Baudrate initialBaudrate)
    {
        switch (config.configMode)
        {
            case CONFIG_DEFAULT:
                return TGAM_NAMESPACE::Setup(stream, config, initialBaudrate, BAUD_NORMAL);

            case CONFIG_SLOW:
                return TGAM_NAMESPACE::Setup(stream, config, initialBaudrate, BAUD_SLOW);

            case CONFIG_RAW:
            case CONFIG_FFT:
                return TGAM_NAMESPACE::Setup(stream, config, initialBaudrate, BAUD_FAST);
        }

        return -1;
    }

#endif

}


#endif
