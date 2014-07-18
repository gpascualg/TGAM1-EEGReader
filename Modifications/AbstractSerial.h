#ifndef __ABSTRACTSERIAL_H__
#define __ABSTRACTSERIAL_H__

#include "Stream.h"

class AbstractSerial : public Stream
{
  public:
  	AbstractSerial() : Stream() {};

    virtual void begin(unsigned long);
    virtual void end();

  	virtual size_t write(uint8_t byte) = 0;
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
};

#endif
