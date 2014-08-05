#define USING_HC05
#include "HC.h"

namespace HC
{

    namespace HC05
    {

        void Setup(AbstractSerial* bluetooth, Role role, Baudrate baudrate)
        {
            if (role == UNSUPPORTED)
            {
                role = ROLE_SLAVE;
            }

            // Start with KEY pin at low
            pinMode(PIN_KEY, OUTPUT);

            // Start with no power to the bt module
            pinMode(PIN_POWER, OUTPUT);
			
            // Setup it to accept commands (baudrate should be that of the commands)
            HC::Mode(bluetooth, MODE_COMMANDS, BAUD_COMMANDS);
			
            // Acknowledge we are here
            bluetooth->write("AT\r\n");
			delay(1000);
			
			char buffer[30] = {0};

            // Setup UART mode (baudrate, stop bit, parity)
			sprintf(buffer, "AT+UART=%u,0,0\r\n", baudrate);
			bluetooth->write(buffer);
            delay(1000);

            // We are the slave
			sprintf(buffer, "AT+ROLE=%d\r\n", role);
			bluetooth->write(buffer);
            delay(1000);

            // Let's switch back to work mode
            HC::Mode(bluetooth, MODE_WORK, baudrate);
        }

    }

}
