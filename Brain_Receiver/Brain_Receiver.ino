#include <SoftwareSerial.h>

// Define which HC and TGAM version we will be using
#define USING_HC05
#include <EEGReader.h>

SoftwareSerial bluetooth(PIN_RX, PIN_TX);

void setup() 
{
    // Setup bt module
    // We will communicate at a RAW EEG baudrate, even if we are at normal mode
    HC::Setup(&bluetooth, HC::ROLE_MASTER, BAUD_FAST);

    // Start Serial at bluetooth baud rate
    Serial.begin((long)BAUD_FAST);
}


void loop()
{
    while (1)
    {
        if (bluetooth.available())
        {
            // Read byte from bluetooth and print on serial
            Serial.write(bluetooth.read());
        }
    }
}
