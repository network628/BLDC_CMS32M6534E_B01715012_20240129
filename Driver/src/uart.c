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
/** \file uart.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "uart.h"
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
 ** \brief	UART_ConfigRunMode
 **			�����첽��������ģʽ
 ** \param [in] uart: UART0��UART1
 **				Baudrate:������
 **				WordLength:(1)UART_WLS_5
 **							(2)UART_WLS_6
 **							(3)UART_WLS_7
 **							(4)UART_WLS_8
 **				Parity:	(1)UART_PARITY_NONE
 **						(2)UART_PARITY_ODD
 **						(3)UART_PARITY_EVEN
 **						(4)UART_PARITY_FORCE_1
 **					 	(5)UART_PARITY_FORCE_0
 **				StopBit:(1)UART_STOP_BIT_1
 **						(2)UART_STOP_BIT_2
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_ConfigRunMode(UART_T* uart,uint32_t Baudrate,uint32_t WordLength, 
						uint32_t Parity, uint32_t StopBit)
{
	uart->LCR &= ~(UART_LCR_WLS_Msk | UART_LCR_SBS_Msk|UART_LCR_PEN_Msk|UART_LCR_PSEL_Msk);
	uart->LCR |= WordLength | Parity | StopBit;
	uart->DLR = SystemCoreClock /16/Baudrate;

}

/*****************************************************************************
 ** \brief	UART_WriteTHR
 **			д���ͱ��ּĴ���
 ** \param [in] uart: UART0��UART1
 **				value: 8bit
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_WriteTHR(UART_T* uart, uint32_t value)
{
	uart->THR = value;
}
/*****************************************************************************
 ** \brief	UART_ReadRBR
 **			�����ջ���Ĵ���
 ** \param [in] uart: UART0��UART1
 ** \return  8bit data
 ** \note	
*****************************************************************************/
uint32_t  UART_ReadRBR(UART_T* uart)
{
	return (uart->RBR);
}

/*****************************************************************************
 ** \brief	UART_EnableRxLineInt
 **			����������״̬�ж�
 ** \param [in] uart: UART0��UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableRxLineInt(UART_T* uart)
{
	uart->IER |= UART_IER_RLSIE_Msk;
//	uart->END = 0;
}

/*****************************************************************************
 ** \brief	UART_DisableRxLineInt
 **			�رս�����״̬�ж�
 ** \param [in] uart: UART0��UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableRxLineInt(UART_T* uart)
{
	uart->IER &= ~(UART_IER_RLSIE_Msk);
}

/*****************************************************************************
 ** \brief	UART_EnableTHREInt
 **			�������ͱ��������ж�
 ** \param [in] uart: UART0��UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableTHREInt(UART_T* uart)
{
	uart->IER |=UART_IER_THREIE_Msk;
}
/*****************************************************************************
 ** \brief	UART_DisableTHREInt
 **			�رշ��ͱ��������ж�
 ** \param [in] uart: UART0��UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableTHREInt(UART_T* uart)
{
	uart->IER &= ~(UART_IER_THREIE_Msk);
}
/*****************************************************************************
 ** \brief	UART_EnableRBRInt
 **			��������������Ч�жϺͽ��ն�ʱ������ж�
 ** \param [in] uart: UART0��UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_EnableRBRInt(UART_T* uart)
{
	uart->IER |= UART_IER_RBREIE_Msk;
	uart->END = 0;
}
/*****************************************************************************
 ** \brief	UART_DisableRBRInt
 **			�رս���������Ч�жϺͽ��ն�ʱ������ж�
 ** \param [in] uart: UART0��UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_DisableRBRInt(UART_T* uart)
{
	uart->IER &= ~(UART_IER_RBREIE_Msk);	
}

/*****************************************************************************
 ** \brief	UART_Lock
**			 ����UART:��UART������ģ���л���ʱ����Ҫ����UART
 ** \param [in] uart: UART0��UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_Lock(UART_T* uart)
{
	uart->END = 0;
}
