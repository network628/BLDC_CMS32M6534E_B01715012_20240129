/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    flash.c
* @brief   This file implements flash sector erase and program.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "flash.h"

#if defined (__CC_ARM)
#pragma arm section code = "ram_fetch_code"   // Arm Compiler 5
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION > 6010050)
#pragma clang section text = "ram_fetch_code" // Arm Compiler 6
#endif

/***********************************************************************************************************************
* Function Name: EraseChip
* @brief  Chip Erase Flash
* @param  adr - Any address of user code flash 
* @return None
***********************************************************************************************************************/
int EraseChip (uint32_t adr)
{
    FMC->FLERMD = 0x08;
    FMC->FLPROT = 0xF1;
    FMC->FLOPMD1 = 0x55;
    FMC->FLOPMD2 = 0xAA;  
    // Write data to start address of sector to trigger Erase Operation
    *(uint32_t *) adr = 0xFFFFFFFF;
    
    // polling OVER Flag
    while((FMC->FLSTS & FMC_FLSTS_OVF_Msk) == 0);
    FMC->FLSTS |= FMC_FLSTS_OVF_Msk;
    FMC->FLERMD = 0x00;

    return(0);
}

/***********************************************************************************************************************
* Function Name: EraseSector
* @brief  Sector Erase Flash
* @param  adr - sector address of user code flash
* @return status: 0 - OK, 1 - Failed
***********************************************************************************************************************/
int EraseSector (uint32_t adr)
{
    FMC->FLERMD = 0x10;
    FMC->FLPROT = 0xF1;
    FMC->FLOPMD1 = 0x55;
    FMC->FLOPMD2 = 0xAA;  
    // Write data to start address of sector to trigger Erase Operation
    *(uint32_t *) adr = 0xFFFFFFFF;
    
    // polling Erase Over Flag
    while((FMC->FLSTS & FMC_FLSTS_OVF_Msk) == 0);
    FMC->FLSTS |= FMC_FLSTS_OVF_Msk;
    FMC->FLERMD = 0x00;
    FMC->FLPROT = 0x00;

    if(FMC->FLSTS & FMC_FLSTS_EVF_Msk)
    {
        return(1);  /* verify ng */
    }
    else
    {
        return(0);  /* verify ok */
    }
}

/***********************************************************************************************************************
* Function Name: ProgramPage
* @brief  Write data to Flash
* @param  adr - Page Start Address 
* @param  sz - Page Size 
* @param  buf - Page Data 
* @return None
***********************************************************************************************************************/
int ProgramPage (uint32_t adr, uint32_t sz, uint8_t *buf)
{
    uint32_t i;
    uint8_t *ptr;
    
    ptr = (uint8_t *) adr;
    
    FMC->FLPROT = 0xF1;
    
    for(i=0; i<sz; i++) 
    {
        FMC->FLOPMD1 = 0xAA;
        FMC->FLOPMD2 = 0x55;  
        *ptr++ = *buf++;    
        // polling OVER Flag
        while((FMC->FLSTS & FMC_FLSTS_OVF_Msk) == 0);
        FMC->FLSTS |= FMC_FLSTS_OVF_Msk;
    }

    FMC->FLPROT = 0x00;
    
    return (0);
}

/***********************************************************************************************************************
* Function Name: flash_write
* @brief  Write data to Flash area. Automatically determines if erasure is needed.
*         If the destination area is blank, omits Erase and Pragram directly.
*         Otherwise, save data of flash to ram, erase flash, and then write flash.
* @param  adr - Page Start Address 
* @param  sz - Page Size 
* @param  buf - Page Data 
* @return status - MD_OK or MD_ERROR
* @note   sz must be less then or equal to SECTOR_SIZE, if not, return MD_ERROR and don't change flash contents.
***********************************************************************************************************************/
FLASH_STATUS flash_write(uint32_t adr, uint32_t sz, uint8_t *buf)
{
    FLASH_STATUS status = FS_OK;
    uint8_t dat, tmp[SECTOR_SIZE*2];
    uint8_t  blank = 1;
    uint8_t  cross;
    uint8_t *ptr;
    uint8_t *ptr_base;      /* sector base address */
    uint16_t offset;
    uint16_t i;
    uint32_t sector_num;

    if(sz > SECTOR_SIZE) 
	{
       status = FS_ERROR; 
       return(status);
    }

    ptr = (uint8_t *)adr;

    /* Determine if the target area is blank */
    for(i=0; i<sz; i++) 
	{
        dat = *ptr++;
        if(dat != 0xFFU)
        {
            blank = 0;  /* Not blank */
        }
    }

    /* if it is blank, omits erase and program directlly. */
    if(blank)
	{
        /* write data to flash data */
        ProgramPage(adr, sz, buf);
    /* if it isn't blank, erase and then program */
    } 
	else 
	{
        sector_num = (adr & ~(SECTOR_SIZE-1)) >> 9;
        if((adr+sz) > ((sector_num+1) << 9)) 
		{
            cross = 1;  /* write area cross sectors */
        } 
		else
		{
            cross = 0;
        }
        ptr_base = (uint8_t *)(adr & ~(SECTOR_SIZE-1));    /* get sector base address: Each sector is 512 bytes (i.e. 128 words) */
        offset   = adr & (SECTOR_SIZE-1);                  /* get offset */
        
        /* Save the Flash data temporarily */
        if(cross) 
		{
            /* read two sectors */
            for(i = 0; i < SECTOR_SIZE * 2; i++)
            {
                tmp[i] = *ptr_base++;
            }
        }
		else
		{
            /* read one sector */
            for(i = 0; i < SECTOR_SIZE; i++)
            {
                tmp[i] = *ptr_base++;
            }
        }

        /* replace flash data with write data */
        for(i = 0; i < sz; i++)
		{
            tmp[offset+i] = *buf++;
        }

        /* write data to flash data */
        if(cross) 
		{
            EraseSector((sector_num + 0 ) << 9);
            EraseSector((sector_num + 1 ) << 9);
            ProgramPage(sector_num << 9, SECTOR_SIZE * 2, tmp);
        } 
		else
		{
            EraseSector((sector_num + 0 ) << 9);
            ProgramPage(sector_num << 9, SECTOR_SIZE * 1, tmp);
        }
    }
    
    return (status);
}

void flashRead(uint8_t *read_address, uint16_t size, uint8_t *readbuf)
{
    uint8_t *w_ptr;
    uint32_t i;
    w_ptr = (uint8_t *)read_address;

    for (i = 0; i < size; i++)
    {
        *readbuf = *w_ptr++;
		readbuf++;
    }
}
#if defined (__CC_ARM)
#pragma arm section code        // Arm Compiler 5
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION > 6010050)
#pragma clang section text = "" // Arm Compiler 6
#endif
