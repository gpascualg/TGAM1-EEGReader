#include <SoftwareSerial.h>

// Define which HC and TGAM version we will be using
#define USING_HC05
#define USING_TGAM17
#define USING_PARSER
#include <EEGReader.h>

SoftwareSerial bluetooth(PIN_RX, PIN_TX);
ThinkGearStreamParser parser;

void dataHandler(unsigned char extendedCodeLevel, unsigned char code, unsigned char numBytes, const unsigned char *value, void *customData )
{
    // Raw data
    if (extendedCodeLevel == 0 && code == 0x80)
    {        
        // Transmission start
        bluetooth.write(0xFF);
        bluetooth.write(0xFF);

        // Send number of bytes
        bluetooth.write(numBytes);

        // Send each byte
        while (numBytes)
        {
            bluetooth.write(*value);
            ++value;
            --numBytes;
        }
    }
}

void setup() 
{
    // Setup TGAM module
    TGAM::Setup(&Serial, TGAM::Config(TGAM::CONFIG_RAW, false, false, false, false), BAUD_FAST);

    // Setup bt module
    // We will communicate at a RAW EEG baudrate, even if we are at normal mode
    HC::Setup(&bluetooth, HC::ROLE_SLAVE, BAUD_FAST);

    // Initialize the parser
    THINKGEAR_initParser(&parser, PARSER_TYPE_PACKETS, dataHandler, NULL );
}

void loop()
{
    while (1)
    {
        if (Serial.available())
        {
            THINKGEAR_parseByte(&parser, Serial.read());
        }
    }
}
