#include <SoftwareSerial.h>

// Define which HC and TGAM version we will be using
#define USING_HC05
#define USING_TGAM17
#include <EEGReader.h>

SoftwareSerial bluetooth(PIN_RX, PIN_TX);
Brain brain(Serial);

void setup() 
{
    // Setup TGAM module
    TGAM::Setup(&Serial, TGAM::Config(TGAM::CONFIG_RAW, false, false, false, false), BAUD_FAST);

    // Setup bt module
    // We will communicate at a RAW EEG baudrate, even if we are at normal mode
    HC::Setup(&bluetooth, HC::ROLE_SLAVE, BAUD_FAST);
}


void loop()
{
    while (1)
    {
        if (brain.update())
        {
            // Transmission start
            bluetooth.write(0xFF);
            bluetooth.write(0xFF);

            // Send error (1 byte) + Raw (2 bytes)
            bluetooth.write(brain.getLastError());
            bluetooth.write((uint8_t)((brain.readRaw() >> 8) & 0xFF));
            bluetooth.write((uint8_t)(brain.readRaw() & 0xFF));
        }
    }
}
