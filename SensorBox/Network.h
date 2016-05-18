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
#include "Variables.h"
#include "PubSubClient\PubSubClient.h"


#define nRST  11  //Reset Pin
#define sdCardPin  4  //Self-explanotary





class NetworkClass
{
 protected:
	 // Update these with values suitable for your network.
	 const uint16_t port = 1337;
	 uint8_t serverIP[4] = { 192, 168, 1, 53};	 
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
			DEBUG_PRINTLN("Failed to configure Ethernet using DHCP");

			return false; 
		}
		// print your local IP address:
		DEBUG_PRINT("My IP address: ");
		for (byte thisByte = 0; thisByte < 4; thisByte++) {
			// print the value of each byte of the IP address:
			DEBUG_PRINTDEC(Ethernet.localIP()[thisByte]);
			DEBUG_PRINT(".");
		}
		DEBUG_PRINTLN();


		//Absolutely essential when using netowrk and relays together,
		//this reduces the amount of time arduino waits for conenctin attempt to complete
		w5500.setRetransmissionTime(0x03E8);
		w5500.setRetransmissionCount(3); 
		UDPsocket.begin(UDPPort); 
		return true; 
	}
	

	

	//If not connected, will try to connect. Reports on success
	boolean _ConnectionWorks()
	{
		boolean status = EthClient.connected();

		if (!status)
		{
			EthClient.stop(); 
			status = EthClient.connect(serverIP, port);

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

	boolean SendData(Reading* readings, const byte number)
	{
		if (EthClient.connected())
		{
			EthClient.println(readings[1].value); 
			//const int buffersize = Reading::size * number; 
			//byte[buffersize] buffer;

		}

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
			DEBUG_PRINTLN("PacketCame");


			char buffer[6];
			UDPsocket.read(buffer, 6);
			if (buffer[0] == 's' && buffer[3] == 'r') //sekret
			{

				EthClient.stop();

				if (EthClient.connect(remote, port) == 1)
				{
					EthClient.println("Hello! ");
					serverIP[0] = remote[0]; 
					serverIP[1] = remote[1];
					serverIP[2] = remote[2];
					serverIP[3] = remote[3];

					DEBUG_PRINTLN("Contents:");
					DEBUG_PRINTLN(buffer);
				}

				//PubSubClient client(EthClient);
				//client.connect("Bafoon"); 
				//client.publish("soasdasdasdasd", "asdadasdasdasda");
			}
			else {

				//we recieved soem strange packet
				DEBUG_PRINTLN("size is :");
				DEBUG_PRINTLN(packetSize);
			}
		}
		
	}


};



extern NetworkClass Network;

NetworkClass Network;


#endif

