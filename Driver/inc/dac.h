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
/** \file dac.h
**
** History:
** 
*****************************************************************************/
#ifndef __DAC_H_
#define __DAC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "common.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/	

/*----------------------------------------------------------------------------
 **DAC   写保护
-----------------------------------------------------------------------------*/
#define		DAC_LOCK_WRITE_KEY		(0x55UL)
							
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/

/*****************************************************************************
 ** \brief	DAC_ConfigInput
 **			设置DAC转换的输入
 ** \param [in] DAC: dac input value		
 ** \return  none
 ** \note	
*****************************************************************************/
void DAC_ConfigInput(uint8_t DacIn);

/*****************************************************************************
 ** \brief	DAC_Start
 **			使能DAC模块
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void DAC_Start(void);

/*****************************************************************************
 ** \brief	DAC_Stop
 **			关闭DAC模块
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void DAC_Stop(void);

#ifdef __cplusplus
}
#endif

#endif /* __DAC_H_ */

