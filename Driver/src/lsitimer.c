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
#include "lsitimer.h"
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
 ** \brief	LSITIMER_ConfigInput
 **			����LSI TIMER�ıȽ�ʱ��
 ** \param [in] LsiTimerIn: LSI TIMER input value	(0-0xfff)	
 *      �Ƚ�ʱ����㹫ʽ��1/15[kHz]*(LsiTimerIn+1)
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_ConfigInput(uint8_t LsiTimerIn)
{
	uint16_t Temp=0;
	
	__nop(); 							//1��ʱ�ӵȴ�ģ���ȶ�
	Temp = LSITIMER->CON0;
	Temp &= ~LSITIMER_CON0_ITCMP_Msk;
	Temp |= LsiTimerIn;
	
	LSITIMER->CON0 = Temp;	
	
}

/*****************************************************************************
 ** \brief	LSITIMER_Enable
 **			ʹ��LSI TIMERģ��
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_Enable(void)
{
//	NVIC_ClearPendingIRQ(LSITIMER_IRQn);			//����жϱ�־
	LSITIMER->CON0 |= LSITIMER_CON0_RINTE_Msk;	
}

/*****************************************************************************
 ** \brief	LSITIMER_Disable
 **			�ر�LSI TIMERģ��
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_Disable(void)
{
//	NVIC_DisableIRQ(LSITIMER_IRQn);			//�ر��ж�
//	NVIC->ICER[0] |= 1 << LSITIMER_IRQn;						//�ر��ж�
	LSITIMER_DisableInt();
	LSITIMER->CON0 &= ~LSITIMER_CON0_RINTE_Msk;	
}

/*****************************************************************************
 ** \brief	LSITIMER_EnableInt
 **			�ر�LSI TIMERģ���ж�
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_EnableInt(void)
{
	
	NVIC->ISER[0] |= 1 << LSITIMER_IRQn;						//�ر��ж�

}

/*****************************************************************************
 ** \brief	LSITIMER_DisableInt
 **			�ر�LSI TIMERģ���ж�
 ** \param   none	 
 ** \return  none
 ** \note	
*****************************************************************************/
void LSITIMER_DisableInt(void)
{
	
	NVIC->ISER[0] |= 1 << LSITIMER_IRQn;						//�ر��ж�

}




