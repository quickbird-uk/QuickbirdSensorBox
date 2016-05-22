// DataStore.h

#ifndef _SensorCommands_h
#define _SensorCommands_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//#include <Wire.h>
#include <math.h> 
#include "Variables.h"


class LightSensorClass
{
 private:
	

	 const int BH1750address = 0x23; //setting i2c address
	 byte buff[2];
	 bool initalised = false; 

	 int BH1750_Read(int address) //
	 {
		 int i = 0;
		// 
		// Wire.beginTransmission(address);
		// i = Wire.requestFrom(address, 2);
		// if(i == 2)
		// {
		//	 buff[0] = Wire.read();  // receive one byte
		//	 buff[1] = Wire.read();  // receive one byte
		// }
		// else
		// {
		//	 while (Wire.available())
		//	 { //empty the buffer
		//		 Wire.read();
		//		 i--; 
		//	 }
		//	 return 0; 
		// }

		// Wire.endTransmission();


		 //DEBUG_PRINTLN("LightStatusCode");
		 //DEBUG_PRINTLN(code);

		 return i;
	 }

	 bool BH1750_Init(int address)
	 {
		 byte sent = 0; 
		 //Wire.beginTransmission(address);
		 //sent += Wire.write(7); //Reset
		 //Wire.endTransmission();
		 //Wire.beginTransmission(address);
		 //sent += Wire.write(0x10);//1lx reolution 120ms
		 //Wire.endTransmission();



		 return sent;
	 }

 public:

	 
	 Reading getLight()
	 {
		 Reading result; 
		 result.duration = 0;
		 result.SensorTypeID = 11; 

		 if (initalised == false)
		 {
			 if(BH1750_Init(BH1750address) == 0);
				initalised = true; 
		 }

		 if (2 == BH1750_Read(BH1750address))
		 {
			 uint16_t val = ((buff[0] << 8) | buff[1]) / 1.2;
			 result.value = val;
		 }
		 else
		 {
			 result.value = -2000000;
			 initalised = false;
		 }
		 return result; 
	 }
	 

	
	
};

static LightSensorClass LightSensor;

#endif





