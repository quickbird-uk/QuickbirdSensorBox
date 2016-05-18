// DataStore.h

#ifndef _HumiditySensor_h
#define _HumiditySensor_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SHT1x.h>


class HumiditySensorClass
{
 private:
	#define dataPin  12
	#define clockPin 13
	 SHT1x sht1x(dataPin, clockPin);



 public:

	 float getTemperature()
	 {
		 return sht1x.readTemperatureC();
	 }
	 

	
	
};

static HumiditySensorClass HumiditySensor;

#endif





