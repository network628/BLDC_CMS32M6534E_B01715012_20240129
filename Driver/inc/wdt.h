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
/** \file wdt.h
**
** History:
** 
*****************************************************************************/
#ifndef __WDT_H__
#define __WDT_H__

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
 **WDT 时钟
-----------------------------------------------------------------------------*/
#define		WDT_CLK_DIV_1		(0x00UL)	/*WDT clk 1 分频*/
#define		WDT_CLK_DIV_16		(0x01UL<<WDT_WDTCON_WDTPRE_Pos)	/*WDT clk 16 分频*/
#define		WDT_CLK_DIV_256		(0x02UL<<WDT_WDTCON_WDTPRE_Pos)	/*WDT clk 256 分频*/
/*----------------------------------------------------------------------------
 **WDT 写保护
-----------------------------------------------------------------------------*/
#define		WDT_ICLR_WRITE_KEY	(0x55AA55AAUL)
#define		WDT_LOCK_WRITE_KEY	(0x55AA6699UL)
#define		WDT_CON_WRITE_KEY	(0x5AUL<<WDT_WDTCON_WDTEN_Pos)

/*****************************************************************************
 ** \brief	WDT_GET_RIS_FLAG
 **			获取WDT中断源状态
 ** \param [in] none
 ** \return  1:产生WDT向下溢出中断 0：无中断
 ** \note	 
*****************************************************************************/
#define		WDT_GET_RIS_FLAG()	(((WDT->RIS &WDT_WDTRIS_WDTRIS_Msk)? 1:0))

/*****************************************************************************
 ** \brief	WDT_ENABLE_DEBUG
 **			WDT在DEBUG模式中使能
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
#define		WDT_ENABLE_DEBUG()	do{\
									WDT->LOCK = WDT_LOCK_WRITE_KEY;\
									WDT->CON |= WDT_WDTCON_DEBUG_Msk;\
									WDT->LOCK = 0x0;\
								}while(0)
/*****************************************************************************
 ** \brief	WDT_DISABLE_DEBUG
 **			WDT在DEBUG模式中关闭
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
#define		WDT_DISABLE_DEBUG()	do{\
									WDT->LOCK = WDT_LOCK_WRITE_KEY;\
									WDT->CON &= ~(WDT_WDTCON_DEBUG_Msk);\
									WDT->LOCK = 0x0;\
								}while(0)
							
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
 ** \brief	 WDT_EnableRestSystem
 **			 开启WDT溢出复位系统
 ** \param [in] none
 ** \return  none
 ** \note	(1)使能WDT 复位，当发生WDT 中断后没有清中断标志位，下次发生WDT中断时触发WDT复位.
 **			(2)当使能复位时，都会使能WDT中断.
*****************************************************************************/
void WDT_EnableRestSystem(void);
/*****************************************************************************
 ** \brief	 WDT_DisableRestSystem
 **			 关闭WDT溢出复位系统
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void WDT_DisableRestSystem(void);

/*****************************************************************************
 ** \brief	 WDT_ConfigClk
 **			 配置WDT时钟
 ** \param [in] ClkDiv: (1)WDT_CLK_DIV_1   :
 **						(2)WDT_CLK_DIV_16  :
 **						(3)WDT_CLK_DIV_256 :
 ** \return  none
 ** \note	
*****************************************************************************/
void WDT_ConfigClk(uint32_t ClkDiv);
/*****************************************************************************
 ** \brief	 WDT_ConfigPeriod
 **			 配置WDT溢出周期
 ** \param [in] Period: 32BitValue
 ** \return  none
 ** \note	 
*****************************************************************************/
void WDT_ConfigPeriod(uint32_t Period);

/*****************************************************************************
 ** \brief	 WDT_EnableOverflowInt
 **			 开启WDT溢出中断
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
void WDT_EnableOverflowInt(void);


/*****************************************************************************
 ** \brief	 WDT_DisableOverflowInt
 **			 关闭WDT溢出中断
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
void WDT_DisableOverflowInt(void);

/*****************************************************************************
 ** \brief	 WDT_GetOverflowIntFlag
 **			 获取WDT使能并产生中断标志
 ** \param [in] none
 ** \return  1:使能中断并产生中断 0：无中断
 ** \note	 
*****************************************************************************/
uint32_t WDT_GetOverflowIntFlag(void);
/*****************************************************************************
 ** \brief	 WDT_ClearOverflowIntFlag
 **			 清除WDT中断标志并重新加载初值
 ** \param [in] none
 ** \return none
 ** \note	 
*****************************************************************************/
void WDT_ClearOverflowIntFlag(void);

/*****************************************************************************
 ** \brief	 WDT_GetTimingData
 **			 获取WDT计数值
 ** \param [in] none
 ** \return 32bit value
 ** \note	 
*****************************************************************************/
uint32_t WDT_GetTimingData(void);

/*****************************************************************************
 ** \brief	 WDT_ClearWDT
 **			 喂WDT
 ** \param [in] none
 ** \return none
 ** \note	 
*****************************************************************************/
void WDT_ClearWDT(void);

/***********************************************************************************************************************
* Function Name: WDT_Restart
* @brief  This function restarts the watchdog timer.
* @param  None
* @return None
***********************************************************************************************************************/
void WDT_Restart(void);
	
#ifdef __cplusplus
}
#endif

#endif /* __WDT_H__ */

