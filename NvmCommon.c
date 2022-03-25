#include "Nvm.h"
#include "NvmCommon.h"

void nvmLockPage (uint8_t p) {
}

void nvmUnlockPage (uint8_t p) {
}

void nvmExecuteCmd (const uint8_t cmd, const uint32_t address) {
	while (!NVMCTRL->INTFLAG.bit.READY);
	
	NVMCTRL->ADDR.reg = address;
	NVMCTRL->CTRLA.reg = ((NVMCTRL_KEY << 8) | (cmd & 0x7F));
}