// SensorWaterFlow_EGO_A_75Q.h

#ifndef _SensorLightAnalog
#define _SensorLightAnalog

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Variables.h"


class SensorLightAnalog
{
protected:
	byte sensorPin = 0;
	uint16_t samples[numberOfSamples];
	byte currentSample = 0;


public:
	SensorLightAnalog(byte pin)
	{
		sensorPin = pin;
		pinMode(sensorPin, INPUT);
	}
	
	void TakeSample()
	{
		samples[currentSample] = analogRead(sensorPin);
		currentSample = (currentSample + 1) % numberOfSamples;
	}

	/*Reaturns a reading as voltage*/
	Reading GetReading()
	{
		Reading reading;
		uint32_t cumulativeValue = 0;
		for (int i = 0; i < numberOfSamples; i++)
		{
			cumulativeValue += samples[i];
		}
		float value = cumulativeValue / numberOfSamples;
		float voltage = value * 4.8828125;//204.8;

										  //this method return temperature
		reading.duration = networkTus;
		reading.SensorTypeID = 11;
		reading.value = voltage; 

		return reading;
	}

};





#endif