#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <samc21.h>
//#include <samd21.h>

#include "Eeprom.h"

typedef union {
	struct { //fix spec p104
		uint32_t WORD0;
		uint32_t WORD1;
		uint32_t WORD2;
		uint32_t WORD3;
	} bit;
	
	uint32_t reg[4];
} SerialNumber;

//C21 only
typedef union {
	struct {
		uint8_t TSENS_TCAL : 6;
		uint8_t TSENS_FCAL : 6;
		uint32_t TSENS_GAIN : 24;
		uint32_t TSENS_OFFSET : 24;
		uint8_t RESERVED : 4;
	} bit;
	
	uint32_t reg[2];
} TemperatureCalibration;

typedef union {
//C21
	struct {
		uint8_t ADC0_LINEARITY : 3;
		uint8_t ADC0_BIASCAL : 3;
		uint8_t ADC1_LINEARITY : 3;
		uint8_t ADC1_BIASCAL : 3;
		uint8_t OSC32K_CAL : 7;
		uint32_t CAL48M_5V : 22;
		uint32_t CAL48M_3V3: 22;
		uint32_t RESERVED : 1;
	} bit;
	uint32_t reg[2];
	
//D21
	/*
	struct {
		uint32_t RESERVED : 27;
		uint8_t ADC_LINEARITY : 8;
		uint8_t ADC_BIASCAL : 3;
		uint8_t OSC32K_CAL : 7;
		uint8_t USB_TRANSN : 5;
		uint8_t USB_TRANSP : 5;
		uint8_t USB_TRIM : 3;
		uint8_t DFLL48M_COARSE_CAL : 6;
	} bit;
	uint32_t reg[4];
	*/
} SoftwareCalibration;

typedef struct _eeprom_func {
	uint8_t (*write) (uint8_t *src, const uint32_t offset, const uint32_t size);
	uint8_t (*read) (uint8_t *dest, const uint32_t offset, const uint32_t size);
	uint8_t (*erasePage) (const uint8_t p);
	uint8_t (*eraseAllPages) (void );
} Eeprom;

typedef struct _nvm {
	Eeprom *eeprom;
	SerialNumber serial;
	SoftwareCalibration calibrationSw;
#ifdef _SAMC21_
	TemperatureCalibration calibrationTsen;
#endif
} Nvm;
extern Nvm *nvm;

void initNvm (void );
