/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/*****************************************************************************/
/** \file softinit.h
**
** 
**
** History:
** 
*****************************************************************************/
#ifndef __SOFTINIT_H__
#define __SOFTINIT_H__

#ifdef __cplusplus
extern "C"
{
#endif
	
/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include  "base_define.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
#define TimeFlag		    		Time_Flag.Byte
#define	TimeFlag_1ms				Time_Flag.Bits.Bit0
#define	TimeFlag_10ms				Time_Flag.Bits.Bit1
#define	TimeFlag_100ms			Time_Flag.Bits.Bit2
#define	TimeFlag_1s			 		Time_Flag.Bits.Bit3
#define	TimeFlag_2s			    Time_Flag.Bits.Bit4
#define	TimeFlag_1us				Time_Flag.Bits.Bit5
#define	TimeFlag_10us				Time_Flag.Bits.Bit6
#define	TimeFlag_100us			Time_Flag.Bits.Bit7

/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef	union
{
	unsigned char Byte;
	struct
	{
		unsigned char	Bit0	:1;
		unsigned char	Bit1	:1;
		unsigned char	Bit2	:1;
		unsigned char	Bit3	:1;
		unsigned char	Bit4	:1;
		unsigned char	Bit5	:1;
		unsigned char	Bit6	:1;
		unsigned char	Bit7	:1;
	}Bits;
}ByteFlag;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern ByteFlag	Time_Flag;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void  __EI_Set(void);
void  __DI_Set(void);
void  MC_Para_Init(void);
void  MC_VCRTL_SAMP(void);
void  DelayTime_ms(unsigned int delay);
void  ADC_ClearAllInt_Flag(void);
void  ADC_TGSAMP_CONFIG(void);
void	GET_ADC_REF_VAL(void);
void	GET_ADC_IBUS_REF_VAL(void);
  
#ifdef __cplusplus
}
#endif

#endif /* __HARDINIT_H__ */


