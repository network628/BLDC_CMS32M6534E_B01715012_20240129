#ifndef __FLASH_H__
#define __FLASH_H__
#include <stdint.h>
#include "common.h"
#define SECTOR_SIZE 512
#define WORD_SIZE 4

typedef enum
{
	FS_OK    =  0,
	FS_ERROR = 1,
}FLASH_STATUS;
int EraseChip (uint32_t adr);
int EraseSector (uint32_t adr);
int ProgramPage (uint32_t adr, uint32_t sz, uint8_t *buf);
FLASH_STATUS flash_write(uint32_t adr, uint32_t sz, uint8_t *buf);
void flashRead(uint8_t *read_address, uint16_t size, uint8_t *readbuf);

#endif
