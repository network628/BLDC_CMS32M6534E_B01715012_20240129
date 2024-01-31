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
/** \file timer.h
**
** History:
** 
*****************************************************************************/
#ifndef __TIMER_H_
#define __TIMER_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "common.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/


/*-----------------timer0/1-----------------------------------------------------------*/
/*----------------------------------------------------------------------------
 **TMR ģʽ
-----------------------------------------------------------------------------*/
#define		TMR_COUNT_CONTINUONS_MODE	(0x00UL)					/*��������ģʽ*/
#define		TMR_COUNT_PERIOD_MODE		(0x01UL<<TMR_CON_TMRMS_Pos)	/*���ڼ���ģʽ*/
	
#define		TMR_BIT_16_MODE				(0x00UL)					/*16λģʽ*/
#define		TMR_BIT_32_MODE				(0x01UL<<TMR_CON_TMRSZ_Pos)	/*32λģʽ*/

/*----------------------------------------------------------------------------
 **TMR ʱ��
-----------------------------------------------------------------------------*/
#define		TMR_CLK_DIV_1		(0x00UL)							/*1��Ƶ*/
#define		TMR_CLK_DIV_16		(0x01UL<<TMR_CON_TMRPRE_Pos)		/*16��Ƶ*/
#define		TMR_CLK_DIV_256		(0x02UL<<TMR_CON_TMRPRE_Pos)		/*256��Ƶ*/


#define		TMR_CLK_SEL_APB		(0x00UL)		/*ʱ��Դѡ��APBʱ��*/
#define		TMR_CLK_SEL_HSI		(0x01UL)		/*ʱ��Դѡ��HSIʱ��*/
#define		TMR_CLK_SEL_LSI		(0x03UL)		/*ʱ��Դѡ��LSIʱ��*/


/*****************************************************************************
 ** \brief	TMR_GET_RIS_FLAG
 **			��ȡ��ʱ����Դ��־
 ** \param [in] timer : TMR0 ��TMR1��TMR2 ��TMR3	
 ** \return  1: �����ж�  0 �����ж�
 ** \note	
*****************************************************************************/
#define		TMR_GET_RIS_FLAG(timer)		((timer->RIS & TMR_RIS_RIS_Msk)? 1:0)
							
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
 ** \brief	TMR_Start
 **			����Timer
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_Start(TMR_T * Timer);
/*****************************************************************************
 ** \brief	TMR_Stop
 **			�ر�Timer
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_Stop(TMR_T * Timer);

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
void TMR_ConfigRunMode(TMR_T * Timer, uint32_t CountMode,uint32_t Bits);


/*****************************************************************************
 ** \brief	TMR_EnableOneShotMode
 **			��������ģʽ
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_EnableOneShotMode(TMR_T * Timer);

/*****************************************************************************
 ** \brief	TMR_DisableOneShotMode
 **			�رյ���ģʽ
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_DisableOneShotMode(TMR_T * Timer);


/*****************************************************************************
 ** \brief	TMR_ConfigClk
 **			���ö�ʱ��ʱ��
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3
 **				ClkDiv :(1)TMR_CLK_DIV_1			
 **					   (2)TMR_CLK_DIV_16		
 **					   (3)TMR_CLK_DIV_256			
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_ConfigClk(TMR_T * Timer, uint32_t ClkDiv);

/*****************************************************************************
 ** \brief	TMR_SetPeriod
 **			���ö�ʱ������
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3
 **				period: (1)0x00~0xffffffff (32λģʽ)
 **						(2)0x00~0xffff (16λģʽ)		
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_SetPeriod(TMR_T * Timer, uint32_t period);


/*****************************************************************************
 ** \brief	TMR_EnableOverflowInt
 **			������ʱ������ж�
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3	
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_EnableOverflowInt(TMR_T * Timer);
/*****************************************************************************
 ** \brief	TMR_DisableOverflowInt
 **			�رն�ʱ������ж�
 ** \param [in]Timer :TMR0 ��TMR1��TMR2 ��TMR3		
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_DisableOverflowInt(TMR_T * Timer);

/*****************************************************************************
 ** \brief	TMR_GetOverflowIntFlag
 **			��ȡ��ʱ���ж�ʹ�ܲ������־
 ** \param [in] Timer :TMR0 ��TMR1��TMR2 ��TMR3		
 ** \return  1: �ж�ʹ�ܲ��Ҳ����ж�  0 �����ж�
 ** \note	
*****************************************************************************/
uint32_t TMR_GetOverflowIntFlag(TMR_T * Timer);

/*****************************************************************************
 ** \brief	TMR_ClearOverflowIntFlag
 **			�����ʱ���ж������־
 ** \param [in]Timer :TMR0 ��TMR1��TMR2 ��TMR3		
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_ClearOverflowIntFlag(TMR_T * Timer);

/*****************************************************************************
 ** \brief	TMR_GetTimingData
 **			��ȡ��ʱ������ֵ
 ** \param [in]Timer :TMR0 ��TMR1��TMR2 ��TMR3	
 ** \return  data
 ** \note	
*****************************************************************************/
uint32_t TMR_GetTimingData(TMR_T * Timer);


#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H_ */

