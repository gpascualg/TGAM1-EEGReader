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
    TGAM::Setup(&Serial, TGAM::CONFIG_RAW, BAUD_FAST);

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
            // Send packet via bluetooh
            // TODO: This must be better handled
            bluetooth.write(brain.readCSV());
            bluetooth.write('\n');
        }
    }
}
