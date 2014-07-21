#define USING_TGAM16
#include "TGAM.h"

namespace TGAM
{
    
    namespace TGAM16
    {

        
        int Setup(AbstractSerial* stream, Config config, Baudrate initialBaudrate, Baudrate targetBaudrate)
        {
            if (config.configMode == CONFIG_FFT)
            {
                // TGAM firmware 1.6 does not have FFT mode
                return -1;
            }

            // Default TGAM 2.3 baudrate is 9600, start it
            stream->begin((long)initialBaudrate);
            // while(!Serial) // Might be necessary for some arduino boards


            // Page 0
            // [0] = attention
            // [1] = meditation
            // [2] = raw
            // [3] = 57.6k/9600
            uint8_t mode = (config.attention ? 0x01 : 0x00) |
                            (config.meditation ? 0x02 : 0x00) |
                            (config.configMode == CONFIG_RAW ? 0x0C : 0x00); // If RAW mode, set to 0x3 | 0x4 | 0x8 = 0x3 | 0xC
            stream->print(mode);
            delay(1000);

            // HACK: We should now wait for a complete packet to be received,
            // as to verify everything is still working
            delay(2000);
            stream->flush();
            stream->begin((long)targetBaudrate);
            // while(!Serial) // Might be necessary for some arduino boards

            // We now can send which raw information we want
            // Page 1
            // [0] EEG Power
            // [1] RAW 10/8 (ENABLED)
            mode = 0x12 | (config.eegPowers ? 0x01 : 0x00);
            stream->print(mode);
            delay(1000);

            return 0;
        }

    }

}
