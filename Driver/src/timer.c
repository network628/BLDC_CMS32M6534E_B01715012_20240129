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
 **			����Timer
 ** \param [in]Timer :TMR0 ��TMR1��TMR2 ��TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_Start(TMR_T * Timer)
{
	Timer->CON |= TMR_CON_TMREN_Msk;
}
/*****************************************************************************
 ** \brief	TMR_Stop
 **			�ر�Timer
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_Stop(TMR_T * Timer)
{
	Timer->CON &= ~(TMR_CON_TMREN_Msk);
}

/*****************************************************************************
 ** \brief	TMR_ConfigRunMode
 **			���ö�ʱ������ģʽ
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3
 **				CountMode :  (1)TMR_COUNT_CONTINUONS_MODE		��������ģʽ
 **					 		 (2)TMR_COUNT_PERIOD_MODE			���ڼ���ģʽ
 **				Bits:  		 (1)TMR_BIT_16_MODE					16λģʽ
 **					   		 (2)TMR_BIT_32_MODE					32λģʽ
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
 **			��������ģʽ
 ** \param [in]Timer :TMR0 ��TMR1��TMR2 ��TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_EnableOneShotMode(TMR_T * Timer)
{
	Timer->CON |= (TMR_CON_TMROS_Msk);
}

/*****************************************************************************
 ** \brief	TMR_DisableOneShotMode
 **			�رյ���ģʽ
 ** \param [in]Timer :TMR0 ��TMR1��TMR2 ��TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_DisableOneShotMode(TMR_T * Timer)
{
	Timer->CON &= ~(TMR_CON_TMROS_Msk);
}

/*****************************************************************************
 ** \brief	TMR_ConfigClk
 **			���ö�ʱ��ʱ��
 ** \param [in]Timer :TMR0 ��TMR1��TMR2 ��TMR3
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
 **			���ö�ʱ������
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3
 **				period: (1)0x00~0xffffffff (32λģʽ)
 **						(2)0x00~0xffff (16λģʽ)		
 ** \return  none
 ** \note	Ĭ��ʹ�ü���ģʽ
*****************************************************************************/
void TMR_SetPeriod(TMR_T * Timer, uint32_t period)
{
	Timer->LOAD = period;
}


/*****************************************************************************
 ** \brief	TMR_EnableOverflowInt
 **			������ʱ������ж�
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3	
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_EnableOverflowInt(TMR_T * Timer)
{
	Timer->CON |= TMR_CON_TMRIE_Msk;
}
/*****************************************************************************
 ** \brief	TMR_DisableOverflowInt
 **			�رն�ʱ������ж�
 ** \param [in]Timer :TMR0 ��TMR1��TMR2 ��TMR3		
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_DisableOverflowInt(TMR_T * Timer)
{
	Timer->CON &= ~(TMR_CON_TMRIE_Msk);
}

/*****************************************************************************
 ** \brief	TMR_GetOverflowIntFlag
 **			��ȡ��ʱ���ж�ʹ�ܲ������־
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3	
 ** \return  1: �ж�ʹ�ܲ��Ҳ����ж�  0 �����ж�
 ** \note	
*****************************************************************************/
uint32_t TMR_GetOverflowIntFlag(TMR_T * Timer)
{
	return((Timer->MIS & TMR_MIS_MIS_Msk)? 1:0);
}

/*****************************************************************************
 ** \brief	TMR_ClearOverflowIntFlag
 **			�����ʱ���ж������־
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3		
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_ClearOverflowIntFlag(TMR_T * Timer)
{
	Timer->ICLR = TMR_ICLR_ICLR_Msk;
}

/*****************************************************************************
 ** \brief	TMR_GetTimingData
 **			��ȡ��ʱ������ֵ
 ** \param [in]Timer :TMR0 ��TMR1��TMR2 ��TMR3	
 ** \return  data
 ** \note	
*****************************************************************************/
uint32_t TMR_GetTimingData(TMR_T * Timer)
{
	return(Timer->VAL);
}
