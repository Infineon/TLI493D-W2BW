/** @file Tli493d.cpp
 *  @brief Arduino library to control Infineon's Magnetic 3D Sensor TLI493D families
 *	@author Yuxi Sun
 *	@author Florian Schmidt
 *	@author Severin Neuner
 *	@bug reset sequence freezes the W2B6 sensor, fast mode not working
 *	@bug User manual recommands INT=0 in fast mode however only disabling INT works
 *	@bug wake up mode not configured correctly (WA bit = 0)
 */

#include "Tli493d.h"
#include "./util/RegMask.h"
#include "./util/BusInterface2.h"
#include <math.h>

Tli493d::Tli493d(AccessMode_e mode, TypeAddress_e productType, int powerPin, bool powerLevel) : mMode(mode), mProductType(productType), mPowerPin(powerPin), mPowerLevel(powerLevel)
{
	mXdata = 0;
	mYdata = 0;
	mZdata = 0;
	mTempdata = 0;
}

Tli493d::Tli493d(int powerPin, bool powerLevel, AccessMode_e mode, TypeAddress_e productType) : mMode(mode), mProductType(productType), mPowerPin(powerPin), mPowerLevel(powerLevel)
{
	mXdata = 0;
	mYdata = 0;
	mZdata = 0;
	mTempdata = 0;
}

Tli493d::~Tli493d(void)
{
	mInterface.bus->end();
}

void Tli493d::begin(void)
{
	begin(Wire, mProductType, FALSE, 1);
}

void Tli493d::begin(bool reset)
{
	begin(Wire, mProductType, reset, 1);
}

void Tli493d::begin(TwoWire &bus, TypeAddress_e slaveAddress, bool reset, uint8_t oneByteRead)
{
	if(mPowerPin != NO_POWER_PIN)
	{
		//TURN ON THE SENSOR
		pinMode(mPowerPin, OUTPUT);
		digitalWrite(mPowerPin, mPowerLevel);
	}
	delay(100);
	initInterface(&mInterface, &bus, slaveAddress, tli493d::resetValues);
	
	//correct reset values for other product types
	switch (mProductType)
	{
	case TLI493D_A1:
		setRegBits(tli493d::IICadr, 0x01);
		break;
	case TLI493D_A2:
		setRegBits(tli493d::IICadr, 0x10);
		break;
	case TLI493D_A3:
		setRegBits(tli493d::IICadr, 0x11);
		break;
	default:
		break;
	}
	
	mInterface.bus->begin();
	if (reset)
	{
		resetSensor();
	}

	//1-byte protocol -> PR = 1
	setRegBits(tli493d::PR, oneByteRead);
	 //disable interrupt
	//setRegBits(tli493d::INT, 1);
	calcParity(tli493d::FP);
	tli493d::writeOut(&mInterface, tli493d::MOD1_REGISTER);
	tli493d::writeOut(&mInterface, tli493d::MOD1_REGISTER);

	// get all register data from sensor
	tli493d::readOut(&mInterface);

	// default: master controlled mode
	setAccessMode(mMode);
	calcParity(tli493d::CP);
	calcParity(tli493d::FP);

	//write out the configuration register
	tli493d::writeOut(&mInterface, tli493d::CONFIG_REGISTER);
	//write out MOD1 register
	tli493d::writeOut(&mInterface, tli493d::MOD1_REGISTER);

	// make sure the correct setting is written 
	tli493d::writeOut(&mInterface, tli493d::CONFIG_REGISTER);
	tli493d::writeOut(&mInterface, tli493d::MOD1_REGISTER);
	delay(TLI493D_STARTUPDELAY);
}

bool Tli493d::setAccessMode(AccessMode_e mode)
{
	bool ret = BUS_OK;

	switch (mode)
	{
	case MASTERCONTROLLEDMODE:
		setRegBits(tli493d::MODE, MASTERCONTROLLEDMODE);
		// in master cotrolled mode TRIG has to be set
		// if PR = 0 (2-byte read protocol these bits have no effect)
		setRegBits(tli493d::TRIG, 1); //trigger on read of address 00h, set to 2 or 3 to trigger on read of 06h
		break;
	case FASTMODE:
		setRegBits(tli493d::MODE, FASTMODE);
		setRegBits(tli493d::WA, 1);
		setRegBits(tli493d::WU, 1);
		setRegBits(tli493d::XH2, 0x7);
		setRegBits(tli493d::XL2, 0x0);
		tli493d::writeOut(&mInterface, tli493d::WAKEUP_REGISTER);

		//User manual recommands INT=0 in fast mode however only disabling INT works
		setRegBits(tli493d::INT, 1);
		//no clock stretching (INT=1 & CA=0) in fast mode
		setRegBits(tli493d::CA, 1);
		setRegBits(tli493d::TRIG, 0);

		break;

	case LOWPOWERMODE:
		//for wake up mode T must be 0, CP must be odd, and CF = 1
		setRegBits(tli493d::WA, 0x1);
		setRegBits(tli493d::WU, 0x1);
		setRegBits(tli493d::XH2, 0x7);
		setRegBits(tli493d::XL2, 0x0);
		tli493d::writeOut(&mInterface, tli493d::WAKEUP_REGISTER);

		//set update rate: fastest is 000b, slowest 111b
		setRegBits(tli493d::PRD, 0);
		tli493d::writeOut(&mInterface, tli493d::MOD2_REGISTER);

		//INT must be disabled
		setRegBits(tli493d::CA, 0);
		setRegBits(tli493d::INT, 1);
		setRegBits(tli493d::MODE, LOWPOWERMODE);
		break;
	}

	if (ret != BUS_ERROR)
	{
		mMode = mode;
	}
	return ret;
}

void Tli493d::enableInterrupt(void)
{
	setRegBits(tli493d::INT, 0);
	calcParity(tli493d::FP);
	tli493d::writeOut(&mInterface, tli493d::MOD1_REGISTER);
}

void Tli493d::disableInterrupt(void)
{
	setRegBits(tli493d::INT, 1);
	calcParity(tli493d::FP);
	tli493d::writeOut(&mInterface, tli493d::MOD1_REGISTER);
}

void Tli493d::enableCollisionAvoidance(void)
{
	setRegBits(tli493d::CA, 0);
	calcParity(tli493d::FP);
	tli493d::writeOut(&mInterface, tli493d::MOD1_REGISTER);
}

void Tli493d::disableCollisionAvoidance(void)
{
	setRegBits(tli493d::CA, 1);
	calcParity(tli493d::FP);
	tli493d::writeOut(&mInterface, tli493d::MOD1_REGISTER);
}

void Tli493d::enableTemp(void)
{
	setRegBits(tli493d::DT, 0);
	calcParity(tli493d::CP);
	tli493d::writeOut(&mInterface, tli493d::CONFIG_REGISTER);
}

void Tli493d::disableTemp(void)
{
	setRegBits(tli493d::DT, 1);
	calcParity(tli493d::CP);
	tli493d::writeOut(&mInterface, tli493d::CONFIG_REGISTER);
}

void Tli493d::setWakeUpThreshold(float xh_th, float xl_th, float yh_th, float yl_th, float zh_th, float zl_th){
	//all thresholds should be within [-1,1], upper thresholds should be greater than lower thresholds
	if(xh_th>1 || xl_th<-1 || xl_th>xh_th ||
		yh_th>1 || yl_th<-1 || yl_th>yh_th||
		zh_th>1 || zl_th<-1 || zl_th>zh_th)
		return;

	int16_t xh = TLI493D_MAX_THRESHOLD * xh_th; int16_t xl = TLI493D_MAX_THRESHOLD * xl_th;
	int16_t yh = TLI493D_MAX_THRESHOLD * yh_th; int16_t yl = TLI493D_MAX_THRESHOLD * yl_th;
	int16_t zh = TLI493D_MAX_THRESHOLD * zh_th; int16_t zl = TLI493D_MAX_THRESHOLD * zl_th;

	setRegBits(tli493d::XL, (xl&TLI493D_MSB_MASK) >> 3); setRegBits(tli493d::XL2, xl&TLI493D_LSB_MASK);
	setRegBits(tli493d::XH, (xh&TLI493D_MSB_MASK) >> 3); setRegBits(tli493d::XH2, xh&TLI493D_LSB_MASK);

	setRegBits(tli493d::YL, (yl&TLI493D_MSB_MASK) >> 3); setRegBits(tli493d::YL2, yl&TLI493D_LSB_MASK);
	setRegBits(tli493d::YH, (yh&TLI493D_MSB_MASK) >> 3); setRegBits(tli493d::YH2, yh&TLI493D_LSB_MASK);

	setRegBits(tli493d::ZL, (zl&TLI493D_MSB_MASK) >> 3); setRegBits(tli493d::ZL2, zl&TLI493D_LSB_MASK);
	setRegBits(tli493d::ZH, (zh&TLI493D_MSB_MASK) >> 3); setRegBits(tli493d::ZH2, zh&TLI493D_LSB_MASK);

	//write out registers
	for(uint8_t i = tli493d::regMasks[tli493d::XL].byteAdress; i<=tli493d::regMasks[tli493d::ZL2].byteAdress; i++)
		tli493d::writeOut(&mInterface, i);
}

bool Tli493d::wakeUpEnabled(void){
	//TODO not returning the correct value even when wake up is enabled
	return (bool)getRegBits(tli493d::WA);
}


void Tli493d::setUpdateRate(uint8_t updateRate){
	if(updateRate>7) updateRate = 7;
	setRegBits(tli493d::PRD, updateRate);
	tli493d::writeOut(&mInterface, 0x13);
}

Tli493d_Error_t Tli493d::updateData(void)
{
	Tli493d_Error_t ret = TLI493D_NO_ERROR;

	if (readOut(&mInterface, TLI493D_MEASUREMENT_READOUT) != BUS_OK)
	{
		ret = TLI493D_BUS_ERROR;
	}
	//no concatenation for 8 bit resolution
	mXdata = concatResults(getRegBits(tli493d::BX1), getRegBits(tli493d::BX2), true);
	mYdata = concatResults(getRegBits(tli493d::BY1), getRegBits(tli493d::BY2), true);
	mZdata = concatResults(getRegBits(tli493d::BZ1), getRegBits(tli493d::BZ2), true);
	mTempdata = concatResults(getRegBits(tli493d::TEMP1), getRegBits(tli493d::TEMP2), false);

	return ret;
}

float Tli493d::getX(void)
{
	return static_cast<float>(mXdata) * TLI493D_B_MULT;
}

float Tli493d::getY(void)
{
	return static_cast<float>(mYdata) * TLI493D_B_MULT;
}

float Tli493d::getZ(void)
{
	return static_cast<float>(mZdata) * TLI493D_B_MULT;
}

float Tli493d::getTemp(void)
{
	return static_cast<float>(mTempdata - TLI493D_TEMP_OFFSET) * TLI493D_TEMP_MULT + TLI493D_TEMP_25;
}

float Tli493d::getNorm(void)
{
	return TLI493D_B_MULT * sqrt(pow(static_cast<float>(mXdata), 2) + pow(static_cast<float>(mYdata), 2) + pow(static_cast<float>(mZdata), 2));
}

float Tli493d::getAzimuth(void)
{
	return atan2(static_cast<float>(mYdata), static_cast<float>(mXdata));
}

float Tli493d::getPolar(void)
{
	return atan2(static_cast<float>(mZdata), sqrt(pow(static_cast<float>(mXdata), 2) + pow(static_cast<float>(mYdata), 2)));
}

/* CAUTION: If the microcontroller is reset, the communication with the sensor may be corrupted, possibly causing the
	sensor to enter an incorrect state. After a reset, the sensor must be reconfigured to the desired settings.
*/
void Tli493d::resetSensor()
{
	if(mPowerPin == NO_POWER_PIN)
	{
		Wire.requestFrom(0xFF, 0);
		Wire.requestFrom(0xFF, 0);
		Wire.beginTransmission(0x00);
		Wire.endTransmission();
		Wire.beginTransmission(0x00);
		Wire.endTransmission();
		//If the uC has problems with this sequence: reset Wire-module.
		Wire.end();
		Wire.begin();
	}
	else
	{
		pinMode(mPowerPin, OUTPUT);
		digitalWrite(mPowerPin, !mPowerLevel);	//Switch Sensor off
		delay(100);
		digitalWrite(mPowerPin, mPowerLevel);	//Switch Sensor on
		//If the uC has problems with this sequence: reset Wire-module.
		Wire.end();
		Wire.begin();
	}

	delayMicroseconds(TLI493D_RESETDELAY);
}

void Tli493d::readDiagnosis(uint8_t (&diag)[7])
{
	//P, FF, CF, T, PD3, PD0, FRM
	diag[0] = getRegBits(tli493d::CP);
	diag[1] = getRegBits(tli493d::FF);
	diag[2] = getRegBits(tli493d::CF);
	diag[3] = getRegBits(tli493d::T);
	diag[4] = getRegBits(tli493d::PD3);
	diag[5] = getRegBits(tli493d::PD0);
	diag[6] = getRegBits(tli493d::FRM);
}
void Tli493d::setRegBits(uint8_t regMaskIndex, uint8_t data)
{
	if (regMaskIndex < TLI493D_NUM_OF_REGMASKS)
	{
		tli493d::setToRegs(&(tli493d::regMasks[regMaskIndex]), mInterface.regData, data);
	}
}

uint8_t Tli493d::getRegBits(uint8_t regMaskIndex)
{
	if (regMaskIndex < TLI493D_NUM_OF_REGMASKS)
	{
		const tli493d::RegMask_t *mask = &(tli493d::regMasks[regMaskIndex]);
		return tli493d::getFromRegs(mask, mInterface.regData);
	}
	else
		return 0;
}

void Tli493d::calcParity(uint8_t regMaskIndex)
{
	// regMaskIndex should be FP or CP, both odd parity
	// FP: parity of register 11 and the upper 3 bits (PRD) of 13
	// CP: registers 7-10 without WA, TST and PH bit. Affects CF bit in registre 6, thus CP has to be corrected
	//     after startup or reset. If CP is incorrect during a write cycle wake up is disabled

	if (regMaskIndex != tli493d::CP && regMaskIndex != tli493d::FP)
		return;

	uint8_t y = 0x00;
	// set parity bit to 1
	// algorithm will calculate an even parity and replace this bit,
	// so parity becomes odd
	setRegBits(regMaskIndex, 1);

	if (regMaskIndex == tli493d::FP)
	{
		y ^= mInterface.regData[17];
		y ^= (mInterface.regData[19] >> 5); //upper 3 bits
	}
	else if (regMaskIndex == tli493d::CP)
	{
		uint8_t i;
		for (i = 7; i <= 12; i++)
		{
			// combine XL through ZH
			y ^= mInterface.regData[i];
		}
		y ^= (mInterface.regData[13] & 0x7F); //ignoring WA
		y ^= (mInterface.regData[14] & 0x3F); //ignoring TST
		y ^= (mInterface.regData[15] & 0x3F); //ignoring PH
		y ^= mInterface.regData[16];
	}
	// combine all bits of this byte (assuming each register is one byte)
	y = y ^ (y >> 1);
	y = y ^ (y >> 2);
	y = y ^ (y >> 4);
	// parity is in the LSB of y
	setRegBits(regMaskIndex, y & 0x01);
}

int16_t Tli493d::concatResults(uint8_t upperByte, uint8_t lowerByte, bool isB)
{
	//this function is register specific
	int16_t value = 0x0000; //16-bit signed integer for 12-bit values of sensor
	if (isB)
	{
		value = (uint16_t)upperByte << 8;
		value |= ((uint16_t)lowerByte & 0x0F) << 4;
	}
	else
	{
		//temperature measurement has 2 LSB
		value = (uint16_t)upperByte << 8;
		value |= ((uint16_t)lowerByte & 0xC0) << 6;
		value |= 0x06; //append bit 1 and 0
	}
	value >>= 4; //right shift of 2's complement fills MSB with 1's
	return value;
}