#include "HC.h"

namespace HC
{

    void Reset(AbstractSerial* bluetooth)
    {
        // Send all pending data
        bluetooth->flush();
        delay(500);

        // Power off and on, delaying 2s
        digitalWrite(PIN_POWER, LOW);
        delay(2000);
        digitalWrite(PIN_POWER, HIGH);
    }

    void Mode(AbstractSerial* bluetooth, WorkMode mode, Baudrate baudrate)
    {
        digitalWrite(PIN_KEY, mode == MODE_WORK ? LOW : HIGH);
        Reset(bluetooth);
        bluetooth->begin((int)baudrate);
    }

}
