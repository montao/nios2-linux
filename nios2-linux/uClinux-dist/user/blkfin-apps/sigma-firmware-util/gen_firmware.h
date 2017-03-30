#include <stdint.h>

#define SIGMA_MAGIC "ADISIGM"
/* 8192 may not be enough in the future */
#define MAX_LEN (8192)
#define OUTPUT_FILE "SigmaDSP_fw.bin"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

struct sigma_firmware_header {
	unsigned char magic[7];
	u8 version;
	u32 crc;
};

enum {
	SIGMA_ACTION_WRITEXBYTES = 0,
	SIGMA_ACTION_WRITESINGLE,
	SIGMA_ACTION_WRITESAFELOAD,
	SIGMA_ACTION_DELAY,
	SIGMA_ACTION_PLLWAIT,
	SIGMA_ACTION_NOOP,
	SIGMA_ACTION_END,
};

struct sigma_action {
	u8 instr;
	u8 len_hi;
	u16 len;
	u16 addr;
	unsigned char payload[];
};
