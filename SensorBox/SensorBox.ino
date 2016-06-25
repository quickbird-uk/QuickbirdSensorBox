/*
DHCP-based IP printer

This sketch uses the DHCP extensions to the Ethernet library
to get an IP address via DHCP and print the address obtained.
using an Arduino Wiznet Ethernet shield.

Circuit:
* Ethernet shield attached to pins 10, 11, 12, 13

created 12 April 2011
modified 9 Apr 2012
by Tom Igoe

*/

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x)     Serial.print (x)
#define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTDEC(x)
#define DEBUG_PRINTLN(x) 
#endif

#include <SPI.h>
#include <Ethernet2.h>
#include <eeprom.h>
#include "DataStore.h"
#include "Variables.h"
#include "Network.h"
#include <OneWire.h>

#include <SHT1x.h>
#include "SensorWaterFlow_EGO_A_75Q.h"

#include "PubSubClient\PubSubClient.h"

#include "SensorDS18B20.h"
#include "SensorPhMeter.h"
#include "SensorEC_OpenGarden.h"
#include "SensorLightAnalog.h"


uint32_t lastNetowrkSend = 0;
uint32_t lastAnalogueReading = 0; 

// Sensors: 
const byte floatSwitchPin = 0; 
SHT1x sht1x(8, 9);
SensorWaterFlow_EGO_A_75Q* waterSensor; 
SensorPH sensorPh(A0, A2); 
SensorEC_DFR0300 sensorEC(A1, A3, A2);
SensorLightAnalog lightSensor(A5); 


Reading readings[numberOfReadings];

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(115200);
	

#ifdef DEBUG
	// this check is only needed on the Leonardo:
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}


	Serial.print("Boot number: ");
	Serial.println(DataStore.getRestarts()); 
#endif

	pinMode(floatSwitchPin, INPUT_PULLUP);
	waterSensor = &SensorWaterFlow_EGO_A_75Q::init(3);
	Network.init(DataStore.getMacAddress());
}

void loop() {
	Network.Maintain();

	if (micros() - lastAnalogueReading > AnalogueSampleT)
	{
		lastAnalogueReading = micros();
		//collect analogue readings
		sensorPh.TakeSample(); 
		sensorEC.TakeSample(); 
		lightSensor.TakeSample(); 
	}
	if (micros() - lastNetowrkSend > networkTus)
	{
		lastNetowrkSend = micros();
		
		uint32_t time = micros(); 

		//Take readings
		readings[0].value = sht1x.readHumidity();
		readings[0].duration = 0;
		readings[0].SensorTypeID = 5; 
		readings[1].value = sht1x.readTemperatureC();
		readings[1].duration = 0;
		readings[1].SensorTypeID = 6;
		readings[2] = waterSensor->GetReading(); 
		readings[3].duration = 0; 
		readings[3].SensorTypeID = 19 ;
		readings[3].value = digitalRead(floatSwitchPin) * 100; //Makes this a percentage thing 
		readings[4] = SensorDS18B20.GetReading(); 
		readings[5] = sensorPh.GetReading(); 
		readings[6] = sensorEC.GetReading(readings[4].value); //giving it current temp
		readings[7] = lightSensor.GetReading();

		DEBUG_PRINTLN("time to sample:");
		DEBUG_PRINTLN(micros() - time);

		for (int i = 0; i < numberOfReadings; i++)
		{
			DEBUG_PRINTLN(readings[i].value);
		}

		//Send data 
		if (Network.SendData(readings))
		{
			//something
		}
		else
		{
			//something else?
		}
	}

}

