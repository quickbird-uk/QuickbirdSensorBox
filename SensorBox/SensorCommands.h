// DataStore.h

#ifndef _SensorCommands_h
#define _SensorCommands_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include <math.h> 


class SensorCommandsClass
{
 private:
	

	 const int BH1750address = 0x23; //setting i2c address
	 byte buff[2];

	 int BH1750_Read(int address) //
	 {
		 int i = 0;
		 Wire.beginTransmission(address);
		 Wire.requestFrom(address, 2);
		 while (Wire.available()) //
		 {
			 buff[i] = Wire.read();  // receive one byte
			 i++;
		 }
		 Wire.endTransmission();
		 return i;
	 }

	 void BH1750_Init(int address)
	 {
		 Wire.beginTransmission(address);
		 Wire.write(0x10);//1lx reolution 120ms
		 Wire.endTransmission();
	 }

 public:

	 



	 uint16_t getLight()
	 {
		 BH1750_Init(BH1750address);
		 if (2 == BH1750_Read(BH1750address))
		 {
			 uint16_t val = ((buff[0] << 8) | buff[1]) / 1.2;
			 return val; 

		 }
	 }
	 

	
	
};

static SensorCommandsClass SensorCommands;

#endif





