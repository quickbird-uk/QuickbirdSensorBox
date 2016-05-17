// DataStore.h

#ifndef _VARIABLES_h
#define _VARIABLES_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

const byte numberOfRelays = 4;
const byte numberOfSamples = 20;
const uint32_t networkFrequencyus = 1000000;
const uint32_t collectionFrequency = networkFrequencyus / numberOfSamples;


struct SerialNumber
{
	public: uint32_t part1;
	public: uint32_t part2;
	public: uint32_t part3;
	public: uint32_t part4;
};



#endif
