#include "HC05.h"

namespace TGAM
{

	void Setup(AbstractSerial* stream, Config config, Baudrate baudrate)
	{
	  // Default TGAM 2.8 baudrate is 9600, start it
	  stream->begin((long)BAUD_NORMAL);
	  // while(!Serial) // Might be necessary for some arduino boards

	  // First off, we must send the baudrate change command
	  // 00000010 (0x02): 57.6k baud, normal+raw output mode
	  stream->print((uint8_t)config);
	  delay(1000);

	  // HACK: We should now wait for a complete packet to be received,
	  // as to verify everything is still working
	  delay(2000);
	  stream->flush();
	  stream->begin((long)baudrate);
	  // while(!Serial) // Might be necessary for some arduino boards

	  // We now can send which raw information we want
	  // Page 1 (0001 << 8) | (0011)
	  stream->print(0x13);
	  delay(1000);

	  // Measuraments
	  // Page 2 (0010 << 8) | (0011)
	  stream->print(0x23);
	  delay(1000);

	  // eSense
	  // Page 3 (0011 << 8) | (0011)
	  stream->print(0x33);
	  delay(1000);

	  // Baud (page 6) will not be modified as it has already been done
	}

}
