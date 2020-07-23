#ifndef TLI493D_BUSIF_H_INCLUDED
#define TLI493D_BUSIF_H_INCLUDED

#include <Arduino.h>
#include <Wire.h>

#define TLI493D_NUM_REG		23

namespace tli493d
{

typedef struct 
{
	TwoWire *bus;
	uint8_t adress;
	uint8_t regData[TLI493D_NUM_REG];
} BusInterface_t;

}

#endif