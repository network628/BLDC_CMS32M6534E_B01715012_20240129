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

/****************************************************************************/
/** \file DAC.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "dac.h"
/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
*****************************************************************************/

/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/


/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/

/*****************************************************************************
 ** \brief	DAC_ConfigInput
 **			设置DAC转换的输入
 ** \param [in] DAC: dac input value	(0-0xff)	
 ** \return  none
 ** \note	
*****************************************************************************/
void DAC_ConfigInput(uint8_t DacIn)
{
	DAC->LOCK = DAC_LOCK_WRITE_KEY;
	DAC->CON0 = DacIn;	
	DAC->LOCK = 0x00;
}

/*****************************************************************************
 ** \brief	DAC_Start
 **			使能DAC模块
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void DAC_Start(void)
{
	DAC->LOCK = DAC_LOCK_WRITE_KEY;
	DAC->CON0 |= DAC_CON0_EN_Msk;
	DAC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	DAC_Stop
 **			关闭DAC模块
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void DAC_Stop(void)
{
	DAC->LOCK = DAC_LOCK_WRITE_KEY;
	DAC->CON0 &= ~DAC_CON0_EN_Msk;
	DAC->LOCK = 0x00;	
}





