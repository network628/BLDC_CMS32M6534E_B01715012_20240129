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
/** \file uart.h
**
** History:
** 
*****************************************************************************/
#ifndef __UART_H_
#define __UART_H_

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
 **UART 字长
-----------------------------------------------------------------------------*/
#define		UART_WLS_5			(0x00UL)		/*5位字长*/
#define		UART_WLS_6			(0x01UL)		/*6位字长*/
#define		UART_WLS_7			(0x02UL)		/*7位字长*/		
#define		UART_WLS_8			(0x03UL)		/*8位字长*/
/*----------------------------------------------------------------------------
 **UART 停止位
-----------------------------------------------------------------------------*/
#define		UART_STOP_BIT_1			(0x00UL)		/*1位停止位*/
#define		UART_STOP_BIT_2			(0x01UL<<UART_LCR_SBS_Pos)		/*2位停止位*/
/*----------------------------------------------------------------------------
 **UART 奇偶校验
-----------------------------------------------------------------------------*/
#define		UART_PARITY_NONE		(0x00UL)		/*无奇偶校验*/
#define		UART_PARITY_ODD			(0x00UL|(0x1<<UART_LCR_PEN_Pos))	/*奇偶校验*/
#define		UART_PARITY_EVEN		((0x01UL<<UART_LCR_PSEL_Pos)|(0x1<<UART_LCR_PEN_Pos))	/*偶偶校验*/
#define		UART_PARITY_FORCE_1		((0x02UL<<UART_LCR_PSEL_Pos)|(0x1<<UART_LCR_PEN_Pos))		/*校验位强制为1*/
#define		UART_PARITY_FORCE_0		((0x03UL<<UART_LCR_PSEL_Pos)|(0x1<<UART_LCR_PEN_Pos))		/*校验位强制为0*/

/*----------------------------------------------------------------------------
 **UART FIFO Level
-----------------------------------------------------------------------------*/
#define 	FIFO_L0_1BYTE			(0x0UL)		
#define 	FIFO_L1_4BYTE			(0x1UL)		
#define 	FIFO_L2_8BYTE			(0x2UL)		
#define 	FIFO_L3_14BYTE			(0x3UL)		
/*----------------------------------------------------------------------------
 **UART FIFO Reset 
-----------------------------------------------------------------------------*/
#define 	FIFO_RST_EN				(0x1UL)			
#define 	FIFO_RST_DIS			(0x0UL)		

/*----------------------------------------------------------------------------
 **UART XON XOFF
-----------------------------------------------------------------------------*/
#define		UART_USE_XON1_XOFF1		(0x02)
#define		UART_USE_XON2_XOFF2		(0x01)
#define		UART_USE_BOTH_XON_XOFF		(0x03)		/*XON1&XON2,XOFF1&XOFF2*/

							
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
 ** \brief	UART_ConfigRunMode
 **			配置异步串口运行模式
 ** \param [in] uart: UART0、UART1
 **				Baudrate:波特率
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
						uint32_t Parity, uint32_t StopBit);

/*****************************************************************************
 ** \brief	UART_WriteTHR
 **			写发送保持寄存器
 ** \param [in] uart: UART0、UART1
 **				value: 8bit
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_WriteTHR(UART_T* uart, uint32_t value);

/*****************************************************************************
 ** \brief	UART_ReadRBR
 **			读接收缓存寄存器
 ** \param [in] uart: UART0、UART1
 ** \return  8bit data
 ** \note	
*****************************************************************************/
uint32_t  UART_ReadRBR(UART_T* uart);

/*****************************************************************************
 ** \brief	UART_EnableRxLineInt
 **			开启接收线状态中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableRxLineInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableRxLineInt
 **			关闭接收线状态中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableRxLineInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableTHREInt
 **			开启发送保持器空中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_EnableTHREInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableTHREInt
 **			关闭发送保持器空中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_DisableTHREInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_EnableRBRInt
 **			开启接收数据有效中断和接收定时器溢出中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_EnableRBRInt(UART_T* uart);
/*****************************************************************************
 ** \brief	UART_DisableRBRInt
 **			关闭接收数据有效中断和接收定时器溢出中断
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void  UART_DisableRBRInt(UART_T* uart);

/*****************************************************************************
 ** \brief	UART_Lock
 **			 锁定UART:在UART与其他模块切换的时候需要锁定UART
 ** \param [in] uart: UART0、UART1
 ** \return none
 ** \note	
*****************************************************************************/
void UART_Lock(UART_T* uart);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H_ */

