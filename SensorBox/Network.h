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
	 const uint16_t port = 1883;
	 IPAddress serverIP;
	 static const byte rBufferLength = 100; 
	 char recieveBuffer[rBufferLength];
	 byte rBufferIndex = 0; 
	 boolean overflow = false; 
	 EthernetUDP UDPsocket; 
	 EthernetClient EthClient; 
	 const uint16_t UDPPort = 44000;
	 PubSubClient _pubSubClient; 
	 bool gotAddress = false; 
	 char deviceID[49]; 

	 void tohex(unsigned char * in, size_t insz, char * out, size_t outsz)
	 {
		 unsigned char * pin = in;
		 const char * hex = "0123456789ABCDEF";
		 char * pout = out;
		 for (; pin < in + insz; pout += 3, pin++) {
			 pout[0] = hex[(*pin >> 4) & 0xF];
			 pout[1] = hex[*pin & 0xF];
			 pout[2] = ':';
			 if (pout + 3 - out > outsz) {
				 /* Better to truncate output string than overflow buffer */
				 /* it would be still better to either return a status */
				 /* or ensure the target buffer is large enough and it never happen */
				 break;
			 }
		 }
		 pout[-1] = 0;
	 }

 public:


	bool init(uint8_t* mac)
	{
		pinMode(nRST, OUTPUT);
		digitalWrite(nRST, LOW);  //Reset W5200
		delay(10);
		digitalWrite(nRST, HIGH);
		delay(200);       // wait for w5500 work

		serverIP = DataStore.getServerIP(); 
		DEBUG_PRINTLN("ServerIP:");
		DEBUG_PRINT(serverIP[0]);
		DEBUG_PRINT(serverIP[1]);
		DEBUG_PRINT(serverIP[2]);
		DEBUG_PRINTLN(serverIP[3]);

		// start the Ethernet connection:
		if (Ethernet.begin(mac) == 0) {
			DEBUG_PRINTLN("Failed to configure Ethernet using DHCP");

			return false; 
		}
		gotAddress = true;

		// print your local IP address:
		DEBUG_PRINT("My IP address: ");
		for (byte thisByte = 0; thisByte < 4; thisByte++) {
			// print the value of each byte of the IP address:
			DEBUG_PRINTDEC(Ethernet.localIP()[thisByte]);
			DEBUG_PRINT(".");
		}
		DEBUG_PRINTLN();

		//set device ID
		SerialNumber sn = DataStore.getSerial(); 

		unsigned char * dataPointer = (unsigned char *)&sn;

		tohex(dataPointer, 16, deviceID, 48);

		deviceID[48] = '\0'; //termination Char


		//Absolutely essential when using netowrk and relays together,
		//this reduces the amount of time arduino waits for conenctin attempt to complete
		w5500.setRetransmissionTime(0x03E8);
		w5500.setRetransmissionCount(3); 
		UDPsocket.begin(UDPPort); 
		_pubSubClient.setServer(serverIP, port).setClient(EthClient);

		return true; 
	}
	

	

	//If not connected, will try to connect. Reports on success
	boolean _ConnectionWorks()
	{
		boolean status = _pubSubClient.connected();

		if (!status)
		{
			_pubSubClient.disconnect(); 
			status = _pubSubClient.connect(deviceID);

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

	boolean SendData(Reading* readings)
	{
		if (_ConnectionWorks())
		{
			
			byte buffer[networkBufferSize];
			byte* dataPointer = (byte*)readings; 
			for (int i = networkBufferSize; i > 0; i--)
			{
				buffer[i] = *(dataPointer + i); 
			}
			_pubSubClient.publish("readings", buffer, networkBufferSize); 

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
		_pubSubClient.loop(); 

		int packetSize = UDPsocket.parsePacket();
		if (packetSize == 6)
		{
			IPAddress remote = UDPsocket.remoteIP();
			DEBUG_PRINTLN("PacketCame");


			char buffer[6];
			UDPsocket.read(buffer, 6);
			if (buffer[0] == 's' && buffer[3] == 'r') //sekret
			{
				DEBUG_PRINTLN("Sekret");

				if (serverIP[0] != remote[0] ||
					serverIP[1] != remote[1] ||
					serverIP[2] != remote[2] ||
					serverIP[3] != remote[3])
				{
					DEBUG_PRINTLN("IP is different");
					_pubSubClient.disconnect(); 
					_pubSubClient.setServer(remote, port); 

					if (_pubSubClient.connect(deviceID))
					{
						EthClient.println("Hello!");
						serverIP[0] = remote[0];
						serverIP[1] = remote[1];
						serverIP[2] = remote[2];
						serverIP[3] = remote[3];
						DataStore.saveServerIP(serverIP); 

						DEBUG_PRINTLN("Connected!:");
					}
					else
					{
						DEBUG_PRINTLN("Can't connect to remote");
						_pubSubClient.disconnect(); 
						_pubSubClient.setServer(serverIP, port);
						_pubSubClient.connect(deviceID);
					}
				}

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

