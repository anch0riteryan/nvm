#include "Nvm.h"

Nvm *nvm;

void initNvm () {
	Nvm *p;
	
	nvm = malloc (sizeof (Nvm));
	p = nvm;
	
	p->eeprom = malloc (sizeof (Eeprom));
	p->eeprom->write = write;
	p->eeprom->read  = read;
	p->eeprom->eraseAllPages = eraseAllPages;
	
//#ifdef _SAMC21_
	MCLK->AHBMASK.bit.NVMCTRL_ = 1;
	MCLK->APBBMASK.bit.NVMCTRL_ = 1;
	
	GCLK->PCHCTRL[NVMCTRL_GCLK_ID].bit.GEN = 0;
	GCLK->PCHCTRL[NVMCTRL_GCLK_ID].bit.CHEN = 1;
	
	NVMCTRL->CTRLB.bit.MANW = 0; //AUTO-WRITE
	
	//TSEN calibration
	p->calibrationTsen.reg[0] = *((volatile uint32_t *) 0x00806030);
	p->calibrationTsen.reg[1] = *((volatile uint32_t *) 0x00806034);
	
	//Software calibration
	p->calibrationSw.reg[0] = *((volatile uint32_t *) 0x00806020);
	p->calibrationSw.reg[1] = *((volatile uint32_t *) 0x00806024);
	
	
//#elif _SAMD21_
	/*
	//Software calibration
	p->calibrationSw.reg[0] = *((volatile uint32_t *) 0x00806020);
	p->calibrationSw.reg[1] = *((volatile uint32_t *) 0x00806024);
	p->calibrationSw.reg[2] = *((volatile uint32_t *) 0x00806028);
	p->calibrationSw.reg[3] = *((volatile uint32_t *) 0x0080602C);
	*/
//#endif
	p->serial.reg[0] = *((volatile uint32_t *) 0x0080A00C);
	p->serial.reg[1] = *((volatile uint32_t *) 0x0080A040);
	p->serial.reg[2] = *((volatile uint32_t *) 0x0080A044);
	p->serial.reg[3] = *((volatile uint32_t *) 0x0080A048);
}
