#include <SoftwareSerial.h>

// Define which HC and TGAM version we will be using
#define USING_HC05
#include <EEGReader.h>

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
#define PACKET_ERRORS   3
#define PACKET_RAW_1    4
#define PACKET_RAW_2    5

uint8_t parsingStep = PACKET_SYNC_1;
int rawEEG = 0;

void loop()
{  
    if (bluetooth.available())
    {
        uint8_t data = bluetooth.read();
        Serial.write("R:");
        Serial.write(data);
        
        switch (parsingStep)
        {
            case PACKET_SYNC_1:
            case PACKET_SYNC_2:
                if (data == 0xFF)
                {
                    ++parsingStep;
                }
                else
                {
                    parsingStep = PACKET_SYNC_1;
                }
                break;

            case PACKET_ERRORS:
                Serial.write("Num. bytes: ");
                Serial.write(data);
                ++parsingStep;
                break;

            case PACKET_RAW_1:
                rawEEG = (int)(data) << 8;
                ++parsingStep;
                break;

            case PACKET_RAW_2:
                rawEEG |= data;
                Serial.print("Raw EEG value: ");
                Serial.print(rawEEG);

                parsingStep = PACKET_SYNC_1;
                break;

            default:
                parsingStep = PACKET_SYNC_1;
                break;
        }
    }
}
