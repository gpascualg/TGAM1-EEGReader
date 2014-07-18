#define USING_HC06
#include "HC.h"

namespace HC
{

    namespace HC06
    {

        void Setup(AbstractSerial* bluetooth, Role role, Baudrate baudrate)
        {
            if (role == UNSUPPORTED)
            {
                role = ROLE_SLAVE;
            }

            // Start with no power to the bt module
            pinMode(PIN_POWER, OUTPUT);
            digitalWrite(PIN_POWER, LOW);

            // Start with KEY pin at low
            pinMode(PIN_KEY, OUTPUT);

            // Setup it to accept commands (baudrate should be that of the commands)
            Mode(bluetooth, MODE_COMMANDS, BAUD_COMMANDS);

            // Acknowledge we are here
            bluetooth->print("AT");
            delay(1000);

            switch (baudrate)
            {
                case BAUD_SLOW:
                    bluetooth->print("AT+BAUD1");
                    break;

                case BAUD_NORMAL:
                    bluetooth->print("AT+BAUD4");
                    break;

                case BAUD_COMMANDS:
                    bluetooth->print("AT+BAUD6");
                    break;

                case BAUD_FAST:
                    bluetooth->print("AT+BAUD7");
                    break;
            }
            delay(1000);

            // NOTE: HC-06 DOES NOT support role changing!

            // Let's switch back to work mode
            Mode(bluetooth, MODE_WORK, baudrate);
        }

    }

}
