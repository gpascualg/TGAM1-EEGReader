#include "HC.h"

namespace HC
{

    void Reset(AbstractSerial* bluetooth)
    {
        // Send all pending data
        bluetooth->flush();
        delay(500);
		bluetooth->end();

        // Power off and on, delaying 1/2s
        digitalWrite(PIN_POWER, LOW);
        delay(500);
        digitalWrite(PIN_POWER, HIGH);
    }

    void Mode(AbstractSerial* bluetooth, WorkMode mode, Baudrate baudrate)
    {
        digitalWrite(PIN_KEY, mode == MODE_WORK ? LOW : HIGH);
        Reset(bluetooth);
        bluetooth->begin((unsigned long)baudrate);
    }

}
