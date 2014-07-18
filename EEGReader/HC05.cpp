#include "HC05.h"

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
        bluetooth->print("AT\r\n");
        delay(1000);

        // Setup UART mode (baudrate, stop bit, parity)
        bluetooth->print("AT+UART=");
        bluetooth->print((long)baudrate);
        bluetooth->print(",0,0\r\n");
        delay(1000);

        // We are the slave
        bluetooth->print("AT+ROLE=");
        bluetooth->print((uint8_t)role);
        bluetooth->print("\r\n");
        delay(1000);

        // Let's switch back to work mode
        Mode(bluetooth, MODE_WORK, baudrate);
    }

}
