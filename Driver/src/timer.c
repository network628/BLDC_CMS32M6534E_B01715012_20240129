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
/** \file timer.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "timer.h"
#include "cgc.h"

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
 ** \brief	TMR_Start
 **			开启Timer
 ** \param [in]Timer :TMR0 、TMR1、TMR2 、TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_Start(TMR_T * Timer)
{
	Timer->CON |= TMR_CON_TMREN_Msk;
}
/*****************************************************************************
 ** \brief	TMR_Stop
 **			关闭Timer
 ** \param [in] Timer :TMR0 、TMR1、TMR2 、TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_Stop(TMR_T * Timer)
{
	Timer->CON &= ~(TMR_CON_TMREN_Msk);
}

/*****************************************************************************
 ** \brief	TMR_ConfigRunMode
 **			设置定时器运行模式
 ** \param [in] Timer :TMR0 、TMR1、TMR2 、TMR3
 **				CountMode :  (1)TMR_COUNT_CONTINUONS_MODE		连续计数模式
 **					 		 (2)TMR_COUNT_PERIOD_MODE			周期计数模式
 **				Bits:  		 (1)TMR_BIT_16_MODE					16位模式
 **					   		 (2)TMR_BIT_32_MODE					32位模式
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_ConfigRunMode(TMR_T * Timer, uint32_t CountMode,uint32_t Bits)
{
	Timer->CON &= ~(TMR_CON_TMRMS_Msk |TMR_CON_TMRSZ_Msk);
	Timer->CON |= CountMode | Bits;
}

/*****************************************************************************
 ** \brief	TMR_EnableOneShotMode
 **			开启单次模式
 ** \param [in]Timer :TMR0 、TMR1、TMR2 、TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_EnableOneShotMode(TMR_T * Timer)
{
	Timer->CON |= (TMR_CON_TMROS_Msk);
}

/*****************************************************************************
 ** \brief	TMR_DisableOneShotMode
 **			关闭单次模式
 ** \param [in]Timer :TMR0 、TMR1、TMR2 、TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_DisableOneShotMode(TMR_T * Timer)
{
	Timer->CON &= ~(TMR_CON_TMROS_Msk);
}

/*****************************************************************************
 ** \brief	TMR_ConfigClk
 **			设置定时器时钟
 ** \param [in]Timer :TMR0 、TMR1、TMR2 、TMR3
 **				ClkDiv :(1)TMR_CLK_DIV_1			
 **					   (2)TMR_CLK_DIV_16		
 **					   (3)TMR_CLK_DIV_256			
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_ConfigClk(TMR_T * Timer, uint32_t ClkDiv)
{
	Timer->CON &= ~(TMR_CON_TMRPRE_Msk);
	Timer->CON |= ClkDiv;
}

/*****************************************************************************
 ** \brief	TMR_SetPeriod
 **			设置定时器周期
 ** \param [in] Timer :TMR0 、TMR1、TMR2 、TMR3
 **				period: (1)0x00~0xffffffff (32位模式)
 **						(2)0x00~0xffff (16位模式)		
 ** \return  none
 ** \note	默认使用加载模式
*****************************************************************************/
void TMR_SetPeriod(TMR_T * Timer, uint32_t period)
{
	Timer->LOAD = period;
}


/*****************************************************************************
 ** \brief	TMR_EnableOverflowInt
 **			开启定时器溢出中断
 ** \param [in] Timer :TMR0 、TMR1、TMR2 、TMR3	
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_EnableOverflowInt(TMR_T * Timer)
{
	Timer->CON |= TMR_CON_TMRIE_Msk;
}
/*****************************************************************************
 ** \brief	TMR_DisableOverflowInt
 **			关闭定时器溢出中断
 ** \param [in]Timer :TMR0 、TMR1、TMR2 、TMR3		
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_DisableOverflowInt(TMR_T * Timer)
{
	Timer->CON &= ~(TMR_CON_TMRIE_Msk);
}

/*****************************************************************************
 ** \brief	TMR_GetOverflowIntFlag
 **			获取定时器中断使能并溢出标志
 ** \param [in] Timer :TMR0 、TMR1、TMR2 、TMR3	
 ** \return  1: 中断使能并且产生中断  0 ：无中断
 ** \note	
*****************************************************************************/
uint32_t TMR_GetOverflowIntFlag(TMR_T * Timer)
{
	return((Timer->MIS & TMR_MIS_MIS_Msk)? 1:0);
}

/*****************************************************************************
 ** \brief	TMR_ClearOverflowIntFlag
 **			清除定时器中断溢出标志
 ** \param [in] Timer :TMR0 、TMR1、TMR2 、TMR3		
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_ClearOverflowIntFlag(TMR_T * Timer)
{
	Timer->ICLR = TMR_ICLR_ICLR_Msk;
}

/*****************************************************************************
 ** \brief	TMR_GetTimingData
 **			获取定时器计数值
 ** \param [in]Timer :TMR0 、TMR1、TMR2 、TMR3	
 ** \return  data
 ** \note	
*****************************************************************************/
uint32_t TMR_GetTimingData(TMR_T * Timer)
{
	return(Timer->VAL);
}
