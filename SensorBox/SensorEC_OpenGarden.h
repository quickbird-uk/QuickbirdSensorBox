// SensorEC.h

#ifndef _SENSOREC_h
#define _SENSOREC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include  "Variables.h"

static const float point_1_cond = 12880;   // Write here your EC calibration value of the solution 1 in �S/cm
static const float point_1_cal = 100;      // Write here your EC value measured in resistance with solution 1
static const float point_2_cond = 1413;  // Write here your EC calibration value of the solution 2 in �S/cm
static const float point_2_cal = 800;    // Write here your EC value measured in resistance with solution 2

class SensorEC_DFR0300 
{



 protected:
	 byte sensorPin = 0; 
	 byte _powerPinEC = 0;
	 byte _powerPinPH = 0;
	 uint16_t samples[numberOfSamples];
	 byte currentSample = 0;


	 //
	 //	Adjust EC for temperature variations
	 //
	 float static voltageToConductivity(unsigned long _resistanceEC, float temperature = 25)
	 {
		 float value;
		 float SW_condK;
		 float SW_condOffset;

		 // Calculates the cell factor of the conductivity sensor and the offset from the calibration values
		 SW_condK = point_1_cond * point_2_cond * ((point_1_cal - point_2_cal) / (point_2_cond - point_1_cond));
		 SW_condOffset = (point_1_cond * point_1_cal - point_2_cond*point_2_cal) / (point_2_cond - point_1_cond);

		 // Converts the resistance of the sensor into a conductivity value
		 value = SW_condK * 1 / (_resistanceEC + SW_condOffset);


		 //DEBUG_PRINTLN("SW_condK:");
		 //DEBUG_PRINTLN(SW_condK);
		 //DEBUG_PRINTLN("SW_condOffset:");
		 //DEBUG_PRINTLN(SW_condOffset);
		 //DEBUG_PRINTLN("voltage:");
		 //DEBUG_PRINTLN(_resistanceEC);
		 float tempCoefficient = 1.0 + 0.0185 * (temperature - 25.0);
		 float correctedEC = (value ) / tempCoefficient;
		 return correctedEC;
	 }
	 

	 float calculateResistanceEC(float EC) {
		 float resistance; 

		 if (EC <= 11) {
			 resistance = EC / 0.050;
		 }
		 else if ((EC > 11) && (EC <= 23)) {
			 resistance = ((EC - 0.44) / 0.0480);
		 }
		 else if ((EC > 23) && (EC <= 47)) {
			 resistance = ((EC - 1.7170) / 0.0453);
		 }
		 else if ((EC > 47) && (EC <= 94)) {
			 resistance = ((EC - 7.8333) / 0.0392);
		 }
		 else if ((EC > 94) && (EC <= 162)) {
			 resistance = ((EC - 34.16) / 0.0272);
		 }
		 else if ((EC > 162) && (EC <= 186)) {
			 resistance = ((EC - 36.667) / 0.0267);
		 }
		 else if ((EC > 186) && (EC <= 259)) {
			 resistance = ((EC - 93.0909) / 0.0161);
		 }
		 else if ((EC > 259) && (EC <= 310)) {
			 resistance = ((EC - 157.0) / 0.0102);
		 }
		 else if ((EC > 310) && (EC <= 358)) {
			 resistance = ((EC - 207.1429) / 0.0069);
		 }
		 else if ((EC > 358) && (EC <= 401)) {
			 resistance = ((EC - 272.0) / 0.0039);
		 }
		 else if ((EC > 401) && (EC <= 432)) {
			 resistance = ((EC - 327.9286) / 0.0022);
		 }
		 else if ((EC > 432) && (EC <= 458)) {
			 resistance = ((EC - 373.8095) / 0.0012);
		 }
		 else if ((EC > 458) && (EC <= 479)) {
			 resistance = ((EC - 413.3750) / 0.0007);
		 }
		 else if ((EC > 479) && (EC <= 525)) {
			 resistance = ((EC - 473.8889) / 0.0001);
		 }
		 else if ((EC > 525)) {
			 resistance = ((EC - 525.7778) / 0.00001);
		 }

		 return resistance;
	 }

 public:

	 SensorEC_DFR0300(byte pin, byte powerPinEC, byte powerPinPH)
	 {
		 _powerPinEC = powerPinEC;
		 _powerPinPH = powerPinPH;
		 sensorPin = pin;
		 pinMode(sensorPin, INPUT);
	 }

	 void TakeSample()
	 {
		 digitalWrite(_powerPinPH, LOW);
		 delay(1);
		 digitalWrite(_powerPinEC, HIGH);
		 delay(1);
		 samples[currentSample] = analogRead(sensorPin);
		 currentSample = (currentSample + 1) % numberOfSamples;

		 digitalWrite(_powerPinEC, LOW);
		 digitalWrite(_powerPinPH, HIGH);
		 
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

		 float ecResistance = calculateResistanceEC(value); 

		 reading.duration = networkTus;
		 reading.SensorTypeID = 13;
		 reading.value = voltageToConductivity(ecResistance, temp);

		 return reading; 
	 }



	
};



#endif

