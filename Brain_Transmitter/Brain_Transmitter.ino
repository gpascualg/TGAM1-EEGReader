#include <SoftwareSerial.h>

// Define which HC and TGAM version we will be using
#define USING_HC05
#define USING_TGAM17
#define USING_PARSER
#include <EEGReader.h>

// Application defines
//#define BLUETOOTH_CONFIGURATION

// Variables and objects
SoftwareSerial bluetooth(PIN_RX, PIN_TX);
ThinkGearStreamParser parser;

void dataHandler(unsigned char extendedCodeLevel, unsigned char code, unsigned char numBytes, const unsigned char *value, void *customData )
{
    // Raw data
    if (extendedCodeLevel == 0 && code == 0x80)
    {        
        // Transmission start
        bluetooth.write(0xFA);
        bluetooth.write(0xFB);
        bluetooth.write(0xFC);

        // Set up CRC
        unsigned char crc = numBytes;
        
        // Send number of bytes
        bluetooth.write(numBytes);

        // Send each byte
        while (numBytes)
        {
            bluetooth.write(*value);
            crc += *value;
            ++value;
            --numBytes;
        }
        
        // Send CRC
        crc = ~crc;
        bluetooth.write(crc);
    }
}

void setup() 
{
    // Setup TGAM module
    TGAM::Setup(&Serial, TGAM::Config(TGAM::CONFIG_RAW, false, false, false, false), BAUD_FAST);
    //Serial.begin((unsigned long)BAUD_NORMAL);
    
    // Setup bt module
    // We will communicate at a RAW EEG baudrate, even if we are at normal mode
    HC::Setup(&bluetooth, HC::ROLE_SLAVE, BAUD_FAST);

    // Initialize the parser
    THINKGEAR_initParser(&parser, PARSER_TYPE_PACKETS, dataHandler, NULL);
}

void loop()
{
    if (Serial.available())
    {
        THINKGEAR_parseByte(&parser, Serial.read());
    }

#ifdef BLUETOOTH_CONFIGURATION

    if (bluetooth.available())
    {
        TGAM::SetupEx(&Serial, TGAM::Config((TGAM::ConfigMode)bluetooth.read()), BAUD_FAST);
        THINKGEAR_initParser(&parser, PARSER_TYPE_PACKETS, dataHandler, NULL);
    }
    
#endif
}

