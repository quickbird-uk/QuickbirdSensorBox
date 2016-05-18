// SensorEC.h

#ifndef _SENSOREC_h
#define _SENSOREC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include  "Variables.h"

class SensorEC_DFR0300 
{
 protected:
	 byte sensorPin = 0; 
	 byte _powerPin = 0;
	 uint16_t samples[numberOfSamples];
	 byte currentSample = 0;


	 //
	 //	Adjust EC for temperature variations
	 //
	 float static voltageToConductivity(float voltageEC, float temperature = 25)
	 {

		 float _ecReading = voltageEC;
		 return _ecReading;
	 }
	 

 public:

	 SensorEC_DFR0300(byte pin, byte powerPin)
	 {
		 _powerPin = powerPin;
		 sensorPin = pin;
		 pinMode(sensorPin, INPUT);
	 }

	 void TakeSample()
	 {
		 samples[currentSample] = analogRead(sensorPin);
		 currentSample = (currentSample + 1) % numberOfSamples;
	 }


	 /*Reaturns a reading as voltage*/
	 Reading GetReading(float temp)
	 {
		 Reading reading; 
		 uint32_t cumulativeValue = 0;
		 for (int i = 0; i < numberOfSamples; i++)
		 {
			 cumulativeValue += samples[i];
		 }
		 float value = cumulativeValue / numberOfSamples;
		 float voltage = value * 4.8828125;;

		 //this method return temperature
		 reading.duration = networkTus;
		 reading.SensorTypeID = 13;
		 reading.value = voltageToConductivity(voltage, temp); 

		 return reading; 
	 }

	
};



#endif

