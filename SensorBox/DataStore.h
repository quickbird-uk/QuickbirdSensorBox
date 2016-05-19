// DataStore.h

#ifndef _DATASTORE_h
#define _DATASTORE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <EEPROM.h>
#include "Ethernet2.h"
#include "Variables.h"


/*EEPROM MEMORY MAP:

	Bytes	0 - 16:		Serial Number
	Bytes	20 - 26:	Mac Address
	bytes	30 - 34		last Server IP
	Byte	99:			Initialised Flag
	Bytes	100 - 200:	Restart counter, wear leveling.
*/


class DataStoreClass
{
 private:

	 SerialNumber SN;
	 const int snEEPROMaddress = 0; 
	 const int macEEPROMaddress = 16; 
	 const int serverIpAddress = 30; 
	 const int initFlagAddress = 99; 

	 byte macAddress[6];



	 void setInitialised()
	 {
		 EEPROM.update(initFlagAddress, 0);
	 }

	 uint32_t _restarts; 

	 void AddRestart()
	 {
		 if (_restarts % 100 == 0)
		 {
			 byte first = EEPROM.read(100); 
			 EEPROM.write(100, first + 1);
		 }
		 else
		 {
			 byte lastByte = 0;
			 for (int i = 100; i < 200; i++)
			 {
				 byte thisByte = EEPROM.read(i);
				 if (thisByte < lastByte)
				 {
					 EEPROM.write(i, thisByte + 1);
					 break; 
				 }
				 else
				 {
					 lastByte = thisByte;
				 }
			 }
		}
	 }


	 uint32_t readRestarts()
	 {
		 uint32_t number = 0;
		 for (int i = 100; i < 200; i++)
		 {
			 number += EEPROM.read(i);
		 }
		 return number;
	 }


 public:

	 
	 boolean Initialised()
	 {
		 byte data = EEPROM.read(initFlagAddress);
		 if (data == 255)
			 return false;
		 else 
			 return true; 
	 }

	 DataStoreClass() {
		 if (Initialised() == false)   //Initialised() == false
		 {

			 for (int i = 100; i < 200; i++)
			 {
				EEPROM.write(i, 0);
			 }
			 setInitialised(); 
		 }
		 else
		 {	  
			 _restarts = readRestarts(); 
			 AddRestart();
		 }

		 //Load Relay Data into memory
		 SN = EEPROM.get(snEEPROMaddress, SN);
		 for (int i = 5; i > 0; i--)
		 {
			 macAddress[i] = EEPROM[macEEPROMaddress + i]; 
		 }

	 }

	 void saveServerIP(IPAddress ip)
	 {
		 EEPROM.put(serverIpAddress, ip); 
	 }

	 IPAddress getServerIP()
	 {
		 IPAddress ip; 
		 ip = EEPROM.get(serverIpAddress, ip);
		 return ip; 
	 }


	SerialNumber getSerial()
	{
		return SN; 
	}

	uint32_t getRestarts()
	{

		return _restarts;
	}

	byte* getMacAddress()
	{
		return macAddress;
	}
	
};

static DataStoreClass DataStore;

#endif





