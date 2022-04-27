#include "Nvm.h"
#include "NvmCommon.h"
#include "Eeprom.h"

uint8_t eeprom_write (uint8_t *src, const uint32_t offset, const uint32_t size) {
	uint8_t cache[NVMCTRL_ROW_SIZE]; //256 bytes
	
	uint32_t startAddress = NVMCTRL_RWW_EEPROM_ADDR + offset;
	uint32_t endAddress   = NVMCTRL_RWW_EEPROM_ADDR + offset + size;
	
	uint32_t currentRow = 0;
	uint32_t tmp = 0;
	
	if (size > NVMCTRL_RWW_EEPROM_SIZE) {
		return -1;
	}
	
	if (endAddress > (NVMCTRL_RWW_EEPROM_ADDR + NVMCTRL_RWW_EEPROM_SIZE - 1)) {
		return -1;
	}
	
#ifdef DEBUG
	printf ("---EEPROM WRITE START---\n");
	//printf ("from:0x%08X to:0x%08X\n", startAddress, endAddress);
#endif	
	
	for (uint32_t address = startAddress; address < endAddress; address++) {
		tmp = address & ~(NVMCTRL_ROW_SIZE - 1);
		if (currentRow != tmp) {
			currentRow = tmp;

			memcpy (cache, (void const *) currentRow, NVMCTRL_ROW_SIZE);
			nvmExecuteCmd (NVM_CMD_RWWEE_ER, currentRow);
#ifdef DEBUG
			//printf ("row start erase at 0x%08X\n", tmp);
#endif
		}
		
		//data copy
		*(cache + (address & (NVMCTRL_ROW_SIZE - 1))) = *(src + (address - startAddress));
		
		tmp = address & (NVMCTRL_ROW_SIZE - 1); //MASK 0-255 bytes in row
		if ((tmp == (NVMCTRL_ROW_SIZE - 1)) || (address == (endAddress - 1))) {//end of row or end of write
			uint32_t *p = (uint32_t *) cache;
			
			for (uint8_t page = 0; page < NVMCTRL_ROW_PAGES; page++) { //page offset 0-3				
				for (uint8_t index = 0; index < (NVMCTRL_PAGE_SIZE >> 2); index++) { //0-63 in dword
					while (!NVMCTRL->INTFLAG.bit.READY);
					
					*((volatile uint32_t *) (currentRow + (page * NVMCTRL_PAGE_SIZE) + (index << 2))) = *(p + index + (page * 16));
				}
				
				//when using manual write
				//nvmExecuteCmd (NVM_CMD_RWWEE_WP, (currentRow * (page * NVMCTRL_PAGE_SIZE)));
#ifdef DEBUG
				//printf ("page end at 0x%08X, PAGE WRITE\n", (currentRow + (page * NVMCTRL_PAGE_SIZE)));
#endif					
			}
		}
	} //end of for address
	
#ifdef DEBUG
	printf ("---EEPROM WRITE DONE ---\n");
#endif	
	
	return 0;
}

uint8_t eeprom_read (uint8_t *dest, const uint32_t offset, const uint32_t size) {
	if (size > NVMCTRL_RWW_EEPROM_SIZE) {
		return -1;
	}
	
	if ((NVMCTRL_RWW_EEPROM_ADDR + offset + size) > (NVMCTRL_RWW_EEPROM_ADDR + NVMCTRL_RWW_EEPROM_SIZE - 1)) {
		return -1;
	}
	
	memcpy (dest, (void const *) (NVMCTRL_RWW_EEPROM_ADDR + offset), size);
	
	return 0;
}

uint8_t erasePage (const uint8_t p) {
	
	uint32_t address = NVMCTRL_RWW_EEPROM_ADDR + (NVMCTRL_PAGE_SIZE * p);
	
	nvmExecuteCmd (NVM_CMD_RWWEE_ER, address);
	
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