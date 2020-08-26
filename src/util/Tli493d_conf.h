#ifndef TLI493D_CONF_H_INCLUDED
#define TLI493D_CONF_H_INCLUDED

#include "RegMask.h"

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

//master contrlled mode should be used in combination with power down mode
#define TLI493D_DEFAULTMODE			MASTERCONTROLLEDMODE

#define TLI493D_STARTUPDELAY		60
#define TLI493D_RESETDELAY			30

#define TLI493D_NUM_OF_REGMASKS		51
#define TLI493D_NUM_OF_ACCMODES		4
#define TLI493D_MSB_MASK			0x07F1
#define TLI493D_LSB_MASK			0x07
#define TLI493D_MAX_THRESHOLD		1023
#define TLI493D_MEASUREMENT_READOUT	7

#define TLI493D_B_MULT_FULL			1.0/7.7
#define TLI493D_B_MULT_X2			1.0/15.4
#define TLI493D_B_MULT_X4			1.0/30.8
#define TLI493D_B_MULT_FULL_LOW		2.08 //for 8 bit resolution
#define TLI493D_TEMP_MULT 			0.24 //range 0.21 to 0.27
#define TLI493D_TEMP_MULT_LOW 		3.84 //for 8 bit resolution
#define TLI493D_TEMP_OFFSET 		1180 //range 1000 to 1360
#define TLI493D_TEMP_25				25 	 //room temperature offset

namespace tli493d
{
/**
 * @enum Registers_e
 * names of register fields
 */
enum Registers_e
{
	BX1 = 0,
	BX2,
	BY1,
	BY2,
	BZ1,
	BZ2,
	TEMP1,
	TEMP2,
	ID, 							//diagnosis
	P, FF, CF, T, PD3, PD0, FRM, 	//diagnosis (Diag)
	XL, 							//wake up threshold MSBs
	XH,
	YL,
	YH,
	ZL,
	ZH,
	WA, WU, XH2, XL2, 				//wake up (WU) & wake up threshold LSBs
	TST, YH2, YL2,					//TMode
	PH, ZH2, ZL2,					//TPhase
	DT, AM, TRIG, X2, TL_mag, CP,	//Config
	FP, IICadr, PR, CA, INT, MODE,	//MOD1
	Res12,
	PRD, Res13,						//MOD2
	Res14, X4,						//Config2
	Res15, 
	Ver,
};

enum RegisterAddr_e
{
	WAKEUP_REGISTER = 0x0D,
	CONFIG_REGISTER = 0x10,
	MOD1_REGISTER = 0x11,
	MOD2_REGISTER = 0x13,
	CONFIG2_REGISTER = 0x14
};

const RegMask_t regMasks[] = {
	{ REGMASK_READ, 0, 0xFF, 0 },		// R_BX1
	{ REGMASK_READ, 4, 0xF0, 4 },		// R_BX2
	{ REGMASK_READ, 1, 0xFF, 0 },		// R_BY1
	{ REGMASK_READ, 4, 0x0F, 0 },		// R_BY2
	{ REGMASK_READ, 2, 0xFF, 0 },		// R_BZ1
	{ REGMASK_READ, 5, 0x0F, 0 },		// R_BZ2
	{ REGMASK_READ, 3, 0xFF, 0 },		// R_TEMP1
	{ REGMASK_READ, 5, 0xC0, 6 },		// R_TEMP2
	{ REGMASK_READ, 5, 0x30, 4 }, 		// ID
	
	{ REGMASK_READ, 6, 0x80, 7},		// P
	{ REGMASK_READ, 6, 0x40, 6},		// FF 
	{ REGMASK_READ, 6, 0x20, 5},		// CF 
	{ REGMASK_READ, 6, 0x10, 4},		// T 
	{ REGMASK_READ, 6, 0x08, 3},		// PD3 
	{ REGMASK_READ, 6, 0x04, 2},		// PD0
	{ REGMASK_READ, 6, 0x03, 0},		// FRM
	
	{ REGMASK_WRITE, 7, 0xFF, 0},		// XL (MSB)
	{ REGMASK_WRITE, 8, 0xFF, 0},		// XH 
	{ REGMASK_WRITE, 9, 0xFF, 0},		// YL 
	{ REGMASK_WRITE, 10, 0xFF, 0},		// YH 
	{ REGMASK_WRITE, 11, 0xFF, 0},		// ZL 
	{ REGMASK_WRITE, 12, 0xFF, 0},		// ZH 
	
	{ REGMASK_READ, 13, 0x80, 7},		// WA
	{ REGMASK_WRITE, 13, 0x40, 6},		// WU
	{ REGMASK_WRITE, 13, 0x38, 3},		// XH2 (LSB)
	{ REGMASK_WRITE, 13, 0x07, 0},		// XL2
	
	{ REGMASK_WRITE, 14, 0xA0, 6 }, 	// TST 
	{ REGMASK_WRITE, 14, 0x38, 3 }, 	// YH2
	{ REGMASK_WRITE, 14, 0x07, 0 }, 	// YL2 
	
	{ REGMASK_WRITE, 15, 0xA0, 6 }, 	// PH 
	{ REGMASK_WRITE, 15, 0x38, 3 }, 	// ZH2 
	{ REGMASK_WRITE, 15, 0x07, 0 }, 	// ZL2 
	
	// CONFIG
	{ REGMASK_WRITE, 16, 0x80, 7}, 		// DT
	{ REGMASK_WRITE, 16, 0x40, 6}, 		// AM 
	{ REGMASK_WRITE, 16, 0x30, 4}, 		// TRIG 
	{ REGMASK_WRITE, 16, 0x04, 3}, 		// X2
	{ REGMASK_WRITE, 16, 0x03, 1}, 		// TL_mag
	{ REGMASK_WRITE, 16, 0x01, 0}, 		// CP 
	
	// MOD1
	{ REGMASK_WRITE, 17, 0x80, 7}, 		// FP 
	{ REGMASK_WRITE, 17, 0x60, 5}, 		// IICadr
	{ REGMASK_WRITE, 17, 0x10, 4}, 		// PR
	{ REGMASK_WRITE, 17, 0x08, 3}, 		// CA
	{ REGMASK_WRITE, 17, 0x04, 2}, 		// INT
	{ REGMASK_WRITE, 17, 0x03, 0}, 		// MODE
	
	{ REGMASK_WRITE, 18, 0xFF, 0 }, 	// Res12
	{ REGMASK_WRITE, 19, 0xE0, 5 }, 	// PRD 
	{ REGMASK_WRITE, 19, 0x1F, 0 }, 	// RES13
	
	// CONFIG2
	{ REGMASK_WRITE, 20, 0xFE, 0 }, 	// Res14
	{ REGMASK_WRITE, 20, 0x01, 0 },		// X4
	
	{ REGMASK_WRITE, 21, 0xFF, 0 }, 	// Res15
	
	{ REGMASK_READ, 22, 0xFF, 0 }, 		// Version 
	
};

const uint8_t resetValues[] = {
	//register 05h, 11h uses different reset values for different types
	//12h 14h 15h are reserved and initialized to 0
	//version register (16h) can be initialized with C9h, D9h or E9h
	0x80,
	0x80, 0x80, 0x80, 0x00, 0x00,
	0x60, 0x80, 0x7F, 0x80, 0x7F,
	0x80, 0x7F, 0x38, 0x38, 0x38,
	0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xC9,
};
}
#endif
