// DataStore.h

#ifndef _VARIABLES_h
#define _VARIABLES_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

const byte numberOfSamples = 20;
const uint32_t networkTus = 1000000;
const uint32_t AnalogueSampleT = networkTus / numberOfSamples;

struct SerialNumber
{
	public: uint32_t part1;
	public: uint32_t part2;
	public: uint32_t part3;
	public: uint32_t part4;
};

struct Reading
{
	float value;
	//in microseconds
	int32_t duration;
	//Defined in the database
	byte SensorTypeID;

	static const int size = 9; 
};



#endif
