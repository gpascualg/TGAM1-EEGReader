#ifndef __BRAIN_H__
#define __BRAIN_H__

#include "Arduino.h"

#define MAX_PAYLOAD_LENGTH  169
#define MAX_PACKET_LENGTH   MAX_PAYLOAD_LENGTH + 4 // Acording to documentation, most large packet can be of 169+4 bytes
#define EEG_POWER_BANDS     8

enum BrainState
{
    STATE_SYNC_1,
    STATE_SYNC_2,
    STATE_PLENGTH,
    STATE_PAYLOAD,
    STATE_CHKSUM
};

#define ERROR_BRAIN_PACKET_LENGTH       0x01
#define ERROR_BRAIN_PACKET_CHECKSUM     0x02
#define ERROR_BRAIN_PARSING             0x03
#define ERROR_BRAIN_UNKNOWN_STATE       0x04
#define ERROR_BRAIN_UNEXPECTED_80       0x05
#define ERROR_BRAIN_UNEXPECTED_81       0x06
#define ERROR_BRAIN_UNEXPECTED_83       0x07


class Brain
{
public:
    Brain(Stream &_brainStream);    

    // Run this in the main loop.
    boolean update();

    // Most recent error.
    inline uint8_t getLastError();

    // Returns comme-delimited string of all available brain data.
    // Sequence is as below.
    char* readCSV();

    // Individual pieces of brain data.
    uint8_t readSignalQuality();
    uint8_t readAttention();
    uint8_t readMeditation();
    inline int readRaw();
    uint32_t* readPowerArray();
    uint32_t readDelta();
    uint32_t readTheta();
    uint32_t readLowAlpha();
    uint32_t readHighAlpha();
    uint32_t readLowBeta();
    uint32_t readHighBeta();
    uint32_t readLowGamma();
    uint32_t readMidGamma();
    
    
private:
    Stream* brainStream;        
    uint8_t packetData[MAX_PACKET_LENGTH];
    uint8_t state;
    uint8_t packetIndex;
    uint8_t packetLength;
    uint8_t checksumAccumulator;
    uint8_t eegPowerLength;
    boolean hasPower;

    inline void clear();
    void clearPacket();
    void clearEegPower();
    boolean parsePacket();
    
    void printPacket();
    void init();
    void printCSV(); // maybe should be public?      
    void printDebug();

    // With current hardware, at most we would have...
    // 3 x 3 char uint8_t
    // 8 x 10 char uint32_t
    // 10 x 1 char commas
    // 1 x 1 char 0 (string termination)
    // -------------------------
    // 100 characters       
    char csvBuffer[100];
    
    uint8_t lastError;
    
    uint8_t signalQuality;
    uint8_t attention;
    uint8_t meditation;
    int rawValue;

    boolean hasPacket;
    
    // Lighter to just make this public, instead of using the getter?
    uint32_t eegPower[EEG_POWER_BANDS];
};

uint8_t Brain::getLastError()
{
    return lastError;
}

int Brain::readRaw()
{
    return rawValue;
}

void Brain::clear()
{
    state = STATE_SYNC_1;
    packetIndex = 0;
    checksumAccumulator = 0;
}

#endif
