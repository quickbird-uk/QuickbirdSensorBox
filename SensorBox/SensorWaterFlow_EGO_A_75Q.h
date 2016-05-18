// SensorWaterFlow_EGO_A_75Q.h

#ifndef _SENSORWATERFLOW_EGO-A-7.5Q_h
#define _SENSORWATERFLOW_EGO-A-7.5Q_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Variables.h"


class SensorWaterFlow_EGO_A_75Q 
{
protected:

	volatile uint32_t _pulseCount;
	float calibrationFactor = 12;

	SensorWaterFlow_EGO_A_75Q(SensorWaterFlow_EGO_A_75Q const&);              // Don't Implement.
	void operator=(SensorWaterFlow_EGO_A_75Q const&); // Don't implement

	bool initialised = false;

	unsigned long lastReadTime;

	SensorWaterFlow_EGO_A_75Q()
	{
		delay(10);
	}



public:
	static SensorWaterFlow_EGO_A_75Q& init()
	{
		static SensorWaterFlow_EGO_A_75Q instance;
		return instance;
	}

	/*You should only call this once to create a new object.
	This function enforces singleton pattern.
	Subsequent call will simply return the same reference*/
	static SensorWaterFlow_EGO_A_75Q& init(byte pin)
	{
		SensorWaterFlow_EGO_A_75Q& instance = init();

		pinMode(13, OUTPUT); //Turn on the LED

		if (!instance.initialised)
		{
			pinMode(pin, INPUT_PULLUP);
			instance.initialised = true;
			instance.lastReadTime = micros();
			attachInterrupt(digitalPinToInterrupt(3), SensorWaterFlow_EGO_A_75Q::IncrementPulse, FALLING);
		}

		return instance;
	}



	Reading GetReading()
	{
		Reading reading; 
		reading.value = -2000000;
		reading.SensorTypeID = 16;

		unsigned long thisReadTime = micros();
		//if (lastReadTime - thisReadTime > 4000)
		//{

		uint32_t pulseCountCopy;

		/*Very small NoInterrupts block*/
		noInterrupts();
		{
			thisReadTime = micros();
			pulseCountCopy = _pulseCount;
			_pulseCount = 0;
		}
		interrupts();

		unsigned long difference = thisReadTime - lastReadTime; //Apparently this handles overflow just fine! 

		float waterVolume = (float) pulseCountCopy / calibrationFactor;

		lastReadTime = thisReadTime;
		reading.duration = difference;
		reading.value = waterVolume;
		DEBUG_PRINTLN("number of pulces on the water meter"); 
		DEBUG_PRINTLN(pulseCountCopy);
		return reading;

		//}

	}

	void static IncrementPulse()
	{
		init()._pulseCount++;
		digitalWrite(13, init()._pulseCount % 2);
	}
};





#endif