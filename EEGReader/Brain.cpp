#include "Arduino.h"
#include "Brain.h"

#define BRAIN_SYNC 0xAA

Brain::Brain(Stream &_brainStream)
{
    brainStream = &_brainStream;
    
    // Keep the rest of the initialization process in a separate method in case
    // we overload the constructor.
    init();
}

void Brain::init()
{
    // It's up to the calling code to start the stream
    // Usually Serial.begin(9600);
    hasPacket = false;
    state = (uint8_t)STATE_SYNC_1;
    packetIndex = 0;
    packetLength = 0;
    eegPowerLength = 0;
    hasPower = false;
    checksumAccumulator = 0;

    signalQuality = 200;
    attention = 0;
    meditation = 0;

    clearEegPower();
}

boolean Brain::update()
{
    // We have something to read
    if (brainStream->available())
    {
        uint8_t readByte = brainStream->read();

        switch (state)
        {
            case STATE_SYNC_1:
            case STATE_SYNC_2:
                if (readByte == BRAIN_SYNC)
                {
                    ++state;
                }
                break;

            case STATE_PLENGTH:
                // In case we receive just another SYNC, go back one step
                if (readByte == BRAIN_SYNC)
                {
                    --state;
                }
                else
                {
                    // Save packet length
                    packetLength = readByte;
                    ++state;

                    // If there is no data at all, we can go directly to the checksum
                    if (packetLength == 0)
                    {
                        ++state;
                    }
                    // If data is larger than max allowed
                    else if (packetLength > MAX_PAYLOAD_LENGTH)
                    {
                        sprintf(latestError, "ERROR: Packet too long %i", packetLength);
                        state = STATE_SYNC_1;
                    }
                }
                break;

            case STATE_PAYLOAD:
                packetData[packetIndex++] = readByte;
                checksumAccumulator += readByte;

                // We have completely read the payload
                if (packetIndex == packetLength)
                {
                    ++state;
                }
                break;

            case STATE_CHKSUM:
                if (~checksumAccumulator != readByte)
                {
                    sprintf(latestError, "ERROR: Checksum");
                }
                else
                {
                    // Try parsing packet
                    hasPacket = parsePacket();
                    if (!hasPacket)
                    {
                        sprintf(latestError, "ERROR: Parsing error");
                    }
                }

                clear();
                break;

            default:
                // We should never get here
                sprintf(latestError, "ERROR: Unexpected state");
                clear();
                break;
        }
    }
    
    if (hasPacket)
    {
        hasPacket = false;
        return true;
    }

    return false;
}

void Brain::clearPacket() {
    for (uint8_t i = 0; i < MAX_PACKET_LENGTH; i++) {
        packetData[i] = 0;
    }    
}

void Brain::clearEegPower() {
    // Zero the power bands.
    for(uint8_t i = 0; i < EEG_POWER_BANDS; i++) {
        eegPower[i] = 0;
    }
}

boolean Brain::parsePacket() {
    // Loop through the packet, extracting data.
    // Based on mindset_communications_protocol.pdf from the Neurosky Mindset SDK.
    // Returns true if passing succeeds
    hasPower = false;
    boolean parseSuccess = true;
	
    clearEegPower();    // clear the eeg power to make sure we're honest about missing values
    
    int expected;
    for (uint8_t i = 0; i < packetLength; i++) {
        switch (packetData[i]) {
            case 0x2:
                signalQuality = packetData[++i];
                break;
            case 0x3:
                ++i; // heartRate
                break;                
            case 0x4:
                attention = packetData[++i];
                break;
            case 0x5:
                meditation = packetData[++i];
                break;
            case 0x6:
                ++i; // 8 bit raw wave value
                break;
            case 0x7:
                ++i; // RAW_MARKER, always 0x00
                break;
                
            case 0x80:
                // We dont' use this value so let's skip it and just increment i
                // uint8_t packetLength = packetData[++i];
                expected = packetData[++i]; // It should be 2
                if (expected > 2)
                {
                    sprintf(latestError, "ERROR: Unexpected 0x80 %d", expected);
                    i += (expected - 2);
                }

                rawValue = ((int)packetData[++i] << 8) | packetData[++i];
                break;

            case 0x81:
                expected = packetData[++i]; // It should be 32
                if (expected > 32)
                {
                    sprintf(latestError, "ERROR: Unexpected 0x81 %d", expected);
                    i += (expected - 32);
                }

                for (int j = 0; j < EEG_POWER_BANDS; j++)
                {
                    i += 4; // We should read a 4 bytes IEEE floating value here
                }

                break;

            case 0x83:
                // ASIC_EEG_POWER: eight big-endian 3-uint8_t unsigned integer values representing delta, theta, low-alpha high-alpha, low-beta, high-beta, low-gamma, and mid-gamma EEG band power values           
                // The next uint8_t sets the length, usually 24 (Eight 24-bit numbers... big endian?)
                // We dont' use this value so let's skip it and just increment i
                expected = packetData[++i]; // It should be 24
                if (expected > 24)
                {
                    sprintf(latestError, "ERROR: Unexpected 0x83 %d", expected);
                    i += (expected - 24);
                }

                // Extract the values
                for (int j = 0; j < EEG_POWER_BANDS; j++)
                {
                    eegPower[j] = ((uint32_t)packetData[++i] << 16) | ((uint32_t)packetData[++i] << 8) | (uint32_t)packetData[++i];
                }

                hasPower = true;
                // This seems to happen once during start-up on the force trainer. Strange. Wise to wait a couple of packets before
                // you start reading.
                break;

            case 0x86:
                i += packetData[++i]; // We don't use RRINTERVAL at all (this should skip 3 bytes)
                break;

            default:
                // Broken packet ?
                /*
                Serial.print(F("parsePacket UNMATCHED data 0x"));
                Serial.print(packetData[i], HEX);
                Serial.print(F(" in position "));
                Serial.print(i, DEC);
                printPacket();
                */
                parseSuccess = false;
                break;
        }
    }

    return parseSuccess;
}

// Keeping this around for debug use
void Brain::printCSV() {
    // Print the CSV over serial
    brainStream->print(signalQuality, DEC);
    brainStream->print(",");
    brainStream->print(attention, DEC);
    brainStream->print(",");
    brainStream->print(meditation, DEC);

    if (hasPower) {
        for(int i = 0; i < EEG_POWER_BANDS; i++) {
            brainStream->print(",");
            brainStream->print(eegPower[i], DEC);
        }
    }
 
    brainStream->println("");
}

char* Brain::readErrors() {
    return latestError;
}

char* Brain::readCSV() {
    // spit out a big string?
    // find out how big this really needs to be 
    // should be popped off the stack once it goes out of scope?
    // make the character array as small as possible
    
    if(hasPower) {
        
        sprintf(csvBuffer,"%d,%d,%d,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu",
            signalQuality,
            attention,
            meditation,
            eegPower[0],
            eegPower[1],
            eegPower[2],
            eegPower[3],
            eegPower[4],
            eegPower[5],
            eegPower[6],
            eegPower[7]
        );
        
        return csvBuffer;
    }
    else {
        sprintf(csvBuffer,"%d,%d,%d",
            signalQuality,
            attention,
            meditation
        );
        
        return csvBuffer;
    }
}

// For debugging, print the entire contents of the packet data array.
void Brain::printPacket() {
    brainStream->print("[");
    for (uint8_t i = 0; i < MAX_PACKET_LENGTH; i++) {
        brainStream->print(packetData[i], DEC);
 
            if (i < MAX_PACKET_LENGTH - 1) {
                brainStream->print(", ");
            }
    }
    brainStream->println("]");
}

void Brain::printDebug() {
    brainStream->println("");    
    brainStream->println("--- Start Packet ---");
    brainStream->print("Signal Quality: ");
    brainStream->println(signalQuality, DEC);
    brainStream->print("Attention: ");
    brainStream->println(attention, DEC);
    brainStream->print("Meditation: ");
    brainStream->println(meditation, DEC);

    if (hasPower) {
        brainStream->println("");
        brainStream->println("EEG POWER:");
        brainStream->print("Delta: ");
        brainStream->println(eegPower[0], DEC);
        brainStream->print("Theta: ");
        brainStream->println(eegPower[1], DEC);
        brainStream->print("Low Alpha: ");
        brainStream->println(eegPower[2], DEC);
        brainStream->print("High Alpha: ");
        brainStream->println(eegPower[3], DEC);
        brainStream->print("Low Beta: ");
        brainStream->println(eegPower[4], DEC);
        brainStream->print("High Beta: ");
        brainStream->println(eegPower[5], DEC);
        brainStream->print("Low Gamma: ");
        brainStream->println(eegPower[6], DEC);
        brainStream->print("Mid Gamma: ");
        brainStream->println(eegPower[7], DEC);
    }

    brainStream->println("");
    brainStream->print("Checksum Calculated: ");
    brainStream->println(checksumAccumulator, DEC);

    brainStream->println("--- End Packet ---");
    brainStream->println("");    
}

uint8_t Brain::readSignalQuality() {
    return signalQuality;
}

uint8_t Brain::readAttention() {
    return attention;
}

uint8_t Brain::readMeditation() {
    return meditation;
}

uint32_t* Brain::readPowerArray() {
    return eegPower;
}

uint32_t Brain::readDelta() {
    return eegPower[0];
}

uint32_t Brain::readTheta() {
    return eegPower[1];
}

uint32_t Brain::readLowAlpha() {
    return eegPower[2];
}

uint32_t Brain::readHighAlpha() {
    return eegPower[3];
}

uint32_t Brain::readLowBeta() {
    return eegPower[4];
}

uint32_t Brain::readHighBeta() {
    return eegPower[5];
}

uint32_t Brain::readLowGamma() {
    return eegPower[6];
}

uint32_t Brain::readMidGamma() {
    return eegPower[7];
}
