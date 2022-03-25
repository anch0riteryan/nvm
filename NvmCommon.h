#define NVMCTRL_KEY 0xA5

#define NVM_CMD_ER 0x02
#define NVM_CMD_WP 0x04
#define NVM_CMD_EAR 0x05
#define NVM_CMD_WAP 0x06
#define NVM_CMD_RWWEE_ER 0x1A
#define NVM_CMD_RWWEE_WP 0x1C
#define NVM_CMD_LR 0x40 //ROW
#define NVM_CMD_UR 0x41
#define NVM_CMD_SPRM 0x42
#define NVM_CMD_CPRM 0x43
#define NVM_CMD_PAGE_BUFFER_CLEAR 0x44
#define NVM_CMD_SET_SECURITY_BIT 0x45
#define NVM_CMD_INVALIDATE_ALL_CACHE 0x46
#define NVM_CMD_LDR 0x47
#define NVM_CMD_UDR 0x48

void nvmLockPage (uint8_t );
void nvmUnlockPage (uint8_t );
void nvmExecuteCmd (const uint8_t cmd, const uint32_t address);
