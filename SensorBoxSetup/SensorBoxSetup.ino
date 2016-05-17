/*
 Name:		SensorBoxSetup.ino
 Created:	5/17/2016 1:36:03 PM
 Author:	Vladimir Akopyan
*/

/*
String to Integer conversion

Reads a serial input string until it sees a newline, then converts
the string to a number if the characters are digits.

The circuit:
No external components needed.

created 29 Nov 2010
by Tom Igoe

This example code is in the public domain.
*/

#include <EEPROM.h>

const int serialNumberEEpromAddress = 0;
const int MACEEpromAddress = 16;

struct SerialNumber
{
public: uint32_t part1;
public: uint16_t part2;
public: uint16_t part3;
public: uint16_t part4;
public: uint16_t part51;
public: uint32_t part52;
};

struct MacAddress
{
public: uint32_t part1;
public: uint16_t part2;

};



String inString = "";    // string to hold input

void printSerial()
{
	SerialNumber sn;
	EEPROM.get(serialNumberEEpromAddress, sn);
	Serial.print(sn.part1, HEX);
	Serial.print("-");
	Serial.print(sn.part2, HEX);
	Serial.print("-");
	Serial.print(sn.part3, HEX);
	Serial.print("-");
	Serial.print(sn.part4, HEX);
	Serial.print("-");
	Serial.print(sn.part51, HEX);
	Serial.println(sn.part52, HEX);
}

void printMacAddress()
{
	MacAddress mc;
	EEPROM.get(MACEEpromAddress, mc);
	Serial.print(mc.part1, HEX); 
	Serial.print("-");
	Serial.println(mc.part2, HEX);
}


unsigned long hex2int(char *a, unsigned int len)
{
	int i;
	unsigned long val = 0;

	for (i = 0; i<len; i++)
		if (a[i] <= 57)
			val += (a[i] - 48)*(1 << (4 * (len - 1 - i)));
		else
			val += (a[i] - 55)*(1 << (4 * (len - 1 - i)));
	return val;
}

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(115200);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	Serial.println("Arduino Setup project is running ");
	Serial.println("Current Arduino Serial Number is: ");
	printSerial(); 
	// send an intro:
	Serial.println("Current Arduino Mac Address is:");
	printMacAddress();
	Serial.println("Type 1 to set SerialNumber OR 2 to set MAC:");
}



void loop() {
	// Read serial input:
	while (Serial.available() > 0) {
		int inChar = Serial.read();
		if (inChar == '2')
		{
			Serial.println("Enter MAC address for the device in HEX format, 12 digits");
			int i = 0;
			char input[8];

			do
			{
				char entered = Serial.read();
				if (isHexadecimalDigit(entered))
				{
					input[i] = entered;
					i++;
				}
			} while (i < 8);

			uint32_t number = strtoul(input, NULL, 16);
			i = 0; 
			char input2[8];
			do
			{
				char entered = Serial.read();
				if (isHexadecimalDigit(entered))
				{
					input[ i + 4] = entered;
					i++;
				}
			} while (i < 4);

			Serial.println(input2); 

			uint32_t number2 = strtoul(input2, NULL, 16);

			MacAddress mac;

			mac.part1 = number;
			mac.part2 = number2;

			Serial.println(number, HEX);
			Serial.println(number2, HEX);

			EEPROM.put( MACEEpromAddress,mac);
			printMacAddress(); 
			Serial.println("result");


		}
		else if (inChar == '1')
		{
			Serial.println("Enter serial number for the device in HEX format, 32 digits");
			int i = 0;
			char input[32]; 

			do 
			{
				char entered = Serial.read();
				if (isHexadecimalDigit(entered))
				{
					input[i] = entered;
					i++;
				}				
			} while (i < 32);

			Serial.println("Got It");
			Serial.println(input);

			SerialNumber sn;
			
			sn.part1 = hex2int(&input[0], 4) << 16;
			sn.part1 += hex2int(&input[4], 4);
			sn.part2 = hex2int(&input[8], 4);
			sn.part3 = hex2int(&input[12], 4);
			sn.part4 = hex2int(&input[16], 4);
			sn.part51 = hex2int(&input[20], 4);
			sn.part52 = hex2int(&input[24], 4) << 16;
			sn.part52 += hex2int(&input[28], 4);
			EEPROM.put(serialNumberEEpromAddress, sn);
			Serial.println("Serial Number changed, it is now:");
			printSerial(); 
		}
		else
			Serial.println("command not recognised"); 
		//if (isDigit(inChar)) {
		//	// convert the incoming byte to a char
		//	// and add it to the string:
		//	inString += (char)inChar;
		//}
		//// if you get a newline, print the string,
		//// then the string's value:
		//if (inChar == '\n') {
		//	Serial.print("Value:");
		//	Serial.println(inString.toInt());
		//	Serial.print("String: ");
		//	Serial.println(inString);
		//	// clear the string for new input:
		//	inString = "";
		//}
	}
}

byte hex_ascii_to_databyte(char c1) {
	byte res = 0;
	if (c1 >= 48 && c1 <= 57) res = c1 - 48;
	else if (c1 >= 65 && c1 <= 70) res = c1 - 65 + 0xa;
	else if (c1 >= 97 && c1 <= 102) res = c1 - 97 + 0xa;
	else {//error
	}
	//idem c2 in res2
	res = res << 4;
	return res;
}


