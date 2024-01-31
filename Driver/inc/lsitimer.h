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
#ifndef __LSITIMER_H_
#define __LSITIMER_H_

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
 ** \brief	LSITIMER_ConfigInput
 **			设置LSI TIMER的比较时间
 ** \param [in] LsiTimerIn: LSI TIMER input value	(0-0xfff)	
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_ConfigInput(uint8_t LsiTimerIn);

/*****************************************************************************
 ** \brief	LSITIMER_Enable
 **			使能LSI TIMER模块
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_Enable(void);

/*****************************************************************************
 ** \brief	LSITIMER_Disable
 **			关闭LSI TIMER模块
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_Disable(void);

/*****************************************************************************
 ** \brief	LSITIMER_EnableInt
 **			关闭LSI TIMER模块中断
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_EnableInt(void);

/*****************************************************************************
 ** \brief	LSITIMER_DisableInt
 **			关闭LSI TIMER模块中断
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_DisableInt(void);
	
#ifdef __cplusplus
}
#endif

#endif /* __DAC_H_ */

