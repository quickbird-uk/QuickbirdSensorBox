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
#include "LightSensor.h"
#include "SensorDS18B20.h"
#include "PubSubClient\PubSubClient.h"


#define nRST  11  //Reset Pin
#define sdCardPin  4  //Self-explanotary
#include <SHT1x.h>

SHT1x sht1x(12, 13);


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
	 EthernetClient EthClient; 
	 const uint16_t UDPPort = 44000;



 public:


	bool init(uint8_t* mac)
	{
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
		if (packetSize == 6)
		{
			IPAddress remote = UDPsocket.remoteIP();
			Serial.println("PacketCame");


			char buffer[6];
			UDPsocket.read(buffer, 6);
			if (buffer[0] == 's' && buffer[3] == 'r') //sekret
			{
				if (EthClient.connected())
				{
					EthClient.stop();

				}
				EthClient.connect(remote, port);
				uint16_t light =  LightSensor.getLight(); 
				EthClient.println(light); 
				EthClient.println(SensorDS18B20.GetReading());
				EthClient.println(sht1x.readTemperatureC());
				EthClient.println(sht1x.readHumidity());
				Serial.println("Contents:");
				Serial.println(buffer);
				PubSubClient client(EthClient);
				client.connect("Bafoon"); 
				client.publish("soasdasdasdasd", "asdadasdasdasda");
			}
			else {

				//we recieved soem strange packet
				Serial.println("size is :");
				Serial.println(packetSize);
			}
		}
		
	}


};



extern NetworkClass Network;

NetworkClass Network;


#endif

