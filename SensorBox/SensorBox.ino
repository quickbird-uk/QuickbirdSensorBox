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
#include <Wire.h>
#include <OneWire.h>
#include <SHT1x.h>
#include "SensorWaterFlow_EGO_A_75Q.h"
#include "LightSensor.h"
#include "PubSubClient\PubSubClient.h"


uint32_t lastNetowrkSend;
uint32_t lastAnalogueReading; 

// Sensors: 
SHT1x sht1x(8, 9);
SensorWaterFlow_EGO_A_75Q* waterSensor; 

const byte numberOfReadings = 7; 
Reading readings[numberOfReadings];

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(115200);
	Wire.begin(); 

#ifdef DEBUG
	// this check is only needed on the Leonardo:
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}


	Serial.print("Boot number: ");
	Serial.println(DataStore.getRestarts()); 
#endif


	waterSensor = &SensorWaterFlow_EGO_A_75Q::init(3);
	Network.init(DataStore.getMacAddress());
}

void loop() {
	Network.Maintain();

	if (micros() - lastAnalogueReading > AnalogueSampleT)
	{
		lastAnalogueReading = micros();
		//collect analogue readings
	}
	if (micros() - lastNetowrkSend > networkTus)
	{
		lastNetowrkSend = micros();

		//Take readings
		readings[0].value = sht1x.readHumidity();
		readings[0].duration = 0;
		readings[0].SensorTypeID = 5; 
		readings[1].value = sht1x.readTemperatureC();
		readings[1].duration = 0;
		readings[1].SensorTypeID = 6;
		readings[2] = LightSensor.getLight(); 
		readings[3] = waterSensor->GetReading(); 

		//Send data 
		if (Network.SendData(readings, numberOfReadings))
		{
			//something
		}
		else
		{
			//something else?
		}
	}

}

