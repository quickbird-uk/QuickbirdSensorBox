// Network.h

#ifndef _NETWORK_h
#define _NETWORK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DataStore.h"
#include "Ethernet2.h"

//PIns for the ETH. Shield
#define SS    10   //Chip Select Pin
#define nRST  8  //Reset Pin
#define sdCardPin  4  //Self-explanotary



class NetworkClass
{
 protected:
	 // Update these with values suitable for your network.
	 EthernetClient ethClient;
	 const uint16_t port = 1337;
	 const uint8_t serverIP[4] = { 192, 168, 1, 53};	 
	 static const byte rBufferLength = 100; 
	 char recieveBuffer[rBufferLength];
	 byte rBufferIndex = 0; 
	 boolean overflow = false; 
	 EthernetUDP UDPsocket; 
	 const uint16_t UDPPort = 44000;



 public:


	bool init(uint8_t* mac)
	{

		pinMode(SS, OUTPUT);  
		pinMode(nRST, OUTPUT);
		digitalWrite(nRST, LOW);  //Reset W5200
		delay(10);
		digitalWrite(nRST, HIGH);
		delay(200);       // wait for w5500 work

		// start the Ethernet connection:
		if (Ethernet.begin(mac) == 0) {
			Serial.println("Failed to configure Ethernet using DHCP");

			return false; 
		}
		// print your local IP address:
		Serial.print("My IP address: ");
		for (byte thisByte = 0; thisByte < 4; thisByte++) {
			// print the value of each byte of the IP address:
			Serial.print(Ethernet.localIP()[thisByte], DEC);
			Serial.print(".");
		}
		Serial.println();


		//Absolutely essential when using netowrk and relays together,
		//this reduces the amount of time arduino waits for conenctin attempt to complete
		w5500.setRetransmissionTime(0x03E8);
		w5500.setRetransmissionCount(3); 
		UDPsocket.begin(UDPPort); 
		return true; 
	}
	

	

	//If not connected, will tyr to connect. Reports on success
	boolean _ConnectionWorks()
	{
		boolean status = ethClient.connected();

		if (!status)
		{
			ethClient.stop(); 
			status = ethClient.connect(serverIP, port);

			if (status)
			{
				//lastConnectedTime = now();
				//TimeRequest();
			}
		}
		else
		{//Update last time the arduino was connected
			//lastConnectedTime = now();
		}
		return status;
	}

	//void TimeRequest()
	//{
	//	if (_ConnectionWorks())
	//	{
	//		MQTTclient.publish("time/Request", "");
	//	}
	//	else
	//	{
	//		//Serial.print("Client not connected");
	//	}
	//}



	
	void Maintain()
	{
		Ethernet.maintain(); 
		int packetSize = UDPsocket.parsePacket();
		if (packetSize)
		{
			Serial.print("Received packet of size ");
			Serial.println(packetSize);
			Serial.print("From ");
			IPAddress remote = UDPsocket.remoteIP();
			for (int i = 0; i < 4; i++)
			{
				Serial.print(remote[i], DEC);
				if (i < 3)
				{
					Serial.print(".");
				}
			}
			Serial.print(", port ");
			Serial.println(UDPsocket.remotePort());

			// read the packet into packetBufffer
			UDPsocket.read( recieveBuffer,  rBufferLength);
			Serial.println("Contents:");
			Serial.println( recieveBuffer);
		}

		//while(ethClient.available() > 0)
		//{
		//	unsigned char byte = ethClient.read(); 

		//	

		//	//Serial.print(byte);
		//	//Serial.print(" ");

		//	if (byte == readStopChar)
		//		rSepCharsDetected++;
		//	else
		//	{
		//		recieveBuffer[rBufferIndex] = byte;
		//		rSepCharsDetected = 0;
		//		rBufferIndex = (rBufferIndex + 1) % rBufferLength;

		//		//Manage an error condition
		//		overflow = rBufferIndex == 0;
		//	}


		//	
		//	if(rSepCharsDetected == numberOfStopChars)
		//	{		
		//		if(! overflow)
		//		{
		//			//-1 because we increment at the endo f the operation, regardless of the next bit
		//			unsigned int length = rBufferIndex - 1; 
		//			if(recieveBuffer[0] == length)
		//			{
		//				//do stuff;
		//				uint8_t* payload = recieveBuffer + 2;

		//				//Which type of message is it
		//				if(recieveBuffer[1] == 0)
		//				{
		//					SetTime(payload, length-1); //length is -1 becuase we do not count messagetype byte
		//				}
		//				else if(recieveBuffer[1] == 1)
		//				{
		//					SetRelays(payload, length-1);
		//				}
		//				else
		//				{
		//					Serial.println("unknown message");
		//				}
		//			}
		//			else
		//			{

		//				Serial.println("bufferlength not equal to length-delimeter");

		//				//there was an error or dropped byte						
		//			}
		//		}
		//		else
		//		{
		//			Serial.println("frame skipepd because of overflow");
		//		}
		//		rBufferIndex = 0;
		//		overflow = false; 
		//	}
		//}
		
	}

	boolean Connected()
	{
		return ethClient.connected(); 
	}

};



extern NetworkClass Network;

NetworkClass Network;


#endif

