#include "Nvm.h"
#include "NvmCommon.h"

uint8_t write (uint8_t *src, const uint32_t offset, const uint32_t size) {
	uint8_t cache[NVMCTRL_PAGE_SIZE];
	
	uint32_t startAddress = NVMCTRL_RWW_EEPROM_ADDR + offset;
	uint32_t endAddress   = NVMCTRL_RWW_EEPROM_ADDR + offset + size;
	
	uint32_t currentPage = 0;
	uint32_t tmp = 0;
	
	if (size > NVMCTRL_RWW_EEPROM_SIZE) {
		return -1;
	}
	
	if (endAddress > (NVMCTRL_RWW_EEPROM_ADDR + NVMCTRL_RWW_EEPROM_SIZE - 1)) {
		return -1;
	}
	
#ifdef DEBUG
	printf ("---EEPROM WRITE---\n");
	printf ("from:0x08%X to:0x%08X\n", startAddress, endAddress);
#endif	
	
	for (uint32_t address = startAddress; address < endAddress; address++) {
		tmp = ~(NVMCTRL_PAGE_SIZE - 1) & address;
		if (currentPage != tmp) {
			currentPage = tmp;
			printf ("page start at 0x%08X\n", tmp);
			memcpy (cache, (void const *) currentPage, NVMCTRL_PAGE_SIZE);
		}
		
		//data copy
		*(cache + (address & (NVMCTRL_PAGE_SIZE - 1))) = *(src + (address - startAddress));
		
		tmp = (NVMCTRL_PAGE_SIZE - 1) & address;
		if ((tmp == (NVMCTRL_PAGE_SIZE - 1)) || (address == (endAddress - 1))) {
			uint32_t *p = (uint32_t *)cache;
			for (uint8_t index = 0; index < (NVMCTRL_PAGE_SIZE >> 2); index++) {
				*((volatile uint32_t *) (currentPage + (index<<2))) = *(p + index);
			}
			nvmExecuteCmd (NVM_CMD_RWWEE_WP, currentPage);
			
			printf ("page end   at 0x%08X, PAGE WRITE\n", address);
		}
	}
	
	return 0;
}

uint8_t read (uint8_t *dest, const uint32_t offset, const uint32_t size) {
	if (size > NVMCTRL_RWW_EEPROM_SIZE) {
		return -1;
	}
	
	if (endAddress > (NVMCTRL_RWW_EEPROM_ADDR + NVMCTRL_RWW_EEPROM_SIZE - 1)) {
		return -1;
	}
	
	memcpy (dest, (void const *) (NVMCTRL_RWW_EEPROM_ADDR + offset), size);
	
	return 0;
}

uint8_t eraseAllPages () {
	for (uint8_t p = 0; p < NVMCTRL_RWWEE_PAGES; p++) {
		nvmExecuteCmd (
			NVM_CMD_RWWEE_ER,
			NVMCTRL_RWW_EEPROM_ADDR + (p * NVMCTRL_PAGE_SIZE)
		);
	}
	
	return 0;
}