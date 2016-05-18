// DataStore.h

#ifndef _HumiditySensor_h
#define _HumiditySensor_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <OneWire.h>


class HumiditySensorClass
{
 private:
	 OneWire myds(6);
	 byte readstage;
	 const byte resolution = 10;
	 unsigned long starttime;
	 unsigned long elapsedtime;
	 byte dsaddr[8];


 public:

	
	 

	
	
};

static HumiditySensorClass HumiditySensor;

#endif





