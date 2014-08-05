#ifndef __ABSTRACTSERIAL_H__
#define __ABSTRACTSERIAL_H__

#include "Stream.h"

class AbstractSerial : public Stream
{
public:
    AbstractSerial() : Stream() {};

    virtual void begin(unsigned long);
    virtual void end();

    virtual size_t write(uint8_t) = 0;
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print    
	
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
};

#endif
