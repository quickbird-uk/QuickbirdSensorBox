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

#include <SPI.h>
#include <Ethernet2.h>
#include <eeprom.h>
#include "DataStore.h"
#include "Variables.h"
#include "Network.h"

#define nRST 11 //reset pin for network

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(9600);


	pinMode(nRST, OUTPUT);
	digitalWrite(nRST, LOW);  //Reset W5200
	delay(10);
	digitalWrite(nRST, HIGH);

	// this check is only needed on the Leonardo:
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}

	Serial.print("Boot number: ");
	Serial.println(DataStore.getRestarts()); 

	Network.init(DataStore.getMacAddress());
}

void loop() {

}

