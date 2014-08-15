#include <SoftwareSerial.h>

// Define which HC and TGAM version we will be using
#define USING_HC05
#include <EEGReader.h>

// Application level defines
//#define VERBOSE_SERIAL

// Bluetooth
SoftwareSerial bluetooth(PIN_RX, PIN_TX);

void setup() 
{
    // Start Serial at bluetooth baud rate
    Serial.begin((long)BAUD_FAST);

    // Setup bt module
    // We will communicate at a RAW EEG baudrate, even if we are at normal mode
    HC::Setup(&bluetooth, HC::ROLE_MASTER, BAUD_FAST);
}


#define PACKET_SYNC_1   1
#define PACKET_SYNC_2   2
#define PACKET_SYNC_3   3
#define PACKET_BYTES    4
#define PACKET_RAW_1    5
#define PACKET_RAW_2    6
#define PACKET_CRC      7

uint8_t parsingStep = PACKET_SYNC_1;
int rawEEG = 0;
unsigned char crc = 0;

void loop()
{  
#ifndef VERBOSE_SERIAL

    while (bluetooth.available())
        Serial.write(bluetooth.read());

#else

    while (bluetooth.available())
    {
        uint8_t data = bluetooth.read();
        
        switch (parsingStep)
        {
            case PACKET_SYNC_1:
            case PACKET_SYNC_2:
            case PACKET_SYNC_3:
                if (data == 0xFA + (parsingStep - 1))
                {
                    ++parsingStep;
                }
                else
                {
                    parsingStep = PACKET_SYNC_1;
                }
                break;

            case PACKET_BYTES:
                Serial.write("\nNum. bytes: ");
                Serial.print(data);
                crc = data;
                ++parsingStep;
                break;

            case PACKET_RAW_1:
                crc += data;
                rawEEG = (int)(data) << 8;
                ++parsingStep;
                break;

            case PACKET_RAW_2:
                crc += data;
                rawEEG |= (int)data;
                Serial.print("Raw EEG value: ");
                Serial.print(rawEEG);
                ++parsingStep;
                break;

            case PACKET_CRC:
                crc = ~crc;
                Serial.print("\n\tCRC: ");
                Serial.print(crc);
                Serial.print(" ");
                Serial.print(data);
                Serial.print(" ");
                Serial.print(crc == data);
                parsingStep = PACKET_SYNC_1;
                break;

            default:
                parsingStep = PACKET_SYNC_1;
                break;
        }
    }
#endif
}
