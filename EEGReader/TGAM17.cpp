#define USING_TGAM17
#include "TGAM.h"

namespace TGAM
{
    
    namespace TGAM17
    {

        int Setup(AbstractSerial* stream, Config config, Baudrate initialBaudrate, Baudrate targetBaudrate)
        {
            // Default TGAM 2.8 baudrate is 9600, start it
            stream->begin((unsigned long)initialBaudrate);
            // while(!Serial) // Might be necessary for some arduino boards

            // First off, we must send the baudrate change command
            // 00000010 (0x02): 57.6k baud, normal+raw output mode
            stream->write((uint8_t)config.configMode);
            delay(1000);

            // HACK: We should now wait for a complete packet to be received,
            // as to verify everything is still working
            delay(2000);
            stream->flush();
            stream->write((unsigned long)targetBaudrate);
            // while(!Serial) // Might be necessary for some arduino boards

            // Config - Page 1
            // [0] RAW
            // [1] 10/8 bits (ENABLED)
            // [2] RAW Marker (DISABLED)
            uint8_t mode = 0x12 | (config.configMode == CONFIG_RAW ? 0x01 : 0x00);
            stream->write(mode);
            delay(1000);

            // Measuraments - Page 2
            // [1] Quality
            // [2] Power - int
            // [3] Power - float (DISABLED)
            // [4] Battery (DISABLED)
            mode = 0x20 | (config.quality ? 0x01 : 0x00) | (config.eegPowers ? 0x02 : 0x00);
            stream->write(mode);
            delay(1000);

            // eSense - Page 3 (0011 << 8) | (0011)
            mode = 0x30 | (config.attention ? 0x01 : 0x00) | (config.meditation ? 0x02 : 0x00);
            stream->write(mode);
            delay(1000);

            // Baud (page 6) will not be modified as it has already been done		
            return 0;
        }

    }

}
