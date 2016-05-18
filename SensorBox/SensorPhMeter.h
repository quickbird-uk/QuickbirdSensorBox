// SensorEC.h

#ifndef _SENSORPH_h
#define _SENSORPH_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include  "Variables.h"


#define calibration_point_4 2560  //Write here your measured value in mV of pH 4
#define calibration_point_7 2363  //Write here your measured value in mV of pH 7
#define calibration_point_10 2168 //Write here your measured value in mV of pH 10


class SensorPH
{
protected:
	byte sensorPin = 0;
	byte _powerPin = 0; 
	uint16_t samples[numberOfSamples];
	byte currentSample = 0;


public:

	SensorPH(byte AnaloguePin, byte powerPin)
	{
		_powerPin = powerPin; 
		sensorPin = AnaloguePin;
		pinMode(sensorPin, INPUT);
		pinMode(_powerPin, OUTPUT);
		digitalWrite(_powerPin, HIGH); 
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
		reading.SensorTypeID = 4;
		reading.value = voltageToPH(voltage);

		return reading;
	}

	//
	//	Adjust EC for temperature variations
	//
	float static voltageToPH(float _mvpH)
	{
		//DEBUG_PRINTLN("PH voltage:");
		//DEBUG_PRINTLN(_mvpH);

		float sensitivity;
		// Two ranges calibration
		if (_mvpH > calibration_point_7) {
			// The sensitivity is calculated using the other two calibration values
			// Asumme that the pH sensor is lineal in the range.
			// sensitivity = pHVariation / volts
			// Divide by 3 = (pH) 7 - (pH) 4
			sensitivity = (calibration_point_4 - calibration_point_7) / 3;

			// The value at pH 7.0 is taken as reference
			// => Units balance => 7 (pH) + (volts) / ((pH) / (volts))
			return 7.0 + (calibration_point_7 - _mvpH) / sensitivity;
			// | |
			// (pH 7 voltage - Measured volts) = Variation from the reference
		}
		else {
			// The sensitivity is calculated using the other two calibration values
			sensitivity = (calibration_point_7 - calibration_point_10) / 3;

			return 7.0 + (calibration_point_7 - _mvpH) / sensitivity;
		}
	}

};



#endif

