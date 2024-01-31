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
/** \file i2c.h
**
** History:
** 
*****************************************************************************/
#ifndef __I2C_H_
#define __I2C_H_

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
 ** I2C 从机模式地址:
-----------------------------------------------------------------------------*/
#define		I2C_S_7BIT_ADDR0		(0x0UL)	
#define		I2C_S_7BIT_ADDR1		(0x1UL)
#define		I2C_S_7BIT_ADDR2		(0x2UL)
#define		I2C_S_7BIT_ADDR3		(0x3UL)
#define		I2C_S_10BIT_ADDR0		(0x4UL)

#define		I2C_S_BROADCAST_EN		(0x01UL)		/*使能广播呼叫地址识别*/
#define		I2C_S_BROADCAST_DIS		(0x00UL)		/*禁止广播呼叫地址识别*/

/*----------------------------------------------------------------------------
 ** I2C 总线状态
-----------------------------------------------------------------------------*/
#define		I2C_STATUS_NOTHING				(0xF8UL)		/*不确切的状态*/
/*----------------------------------------------------------------------------
** I2C主控模式下的状态(MS: MASTER STATUS)
-----------------------------------------------------------------------------*/
#define		I2C_MS_BUS_ERROR				(0x00UL)		/*总线错误（只在主机模式有效）*/
#define		I2C_MS_START_OK					(0x08UL)		/*启动位发送完成*/
#define		I2C_MS_RESTART_OK				(0x10UL)		/*重启动位发送完成*/
#define		I2C_MS_SEND_ADDR_W_ACK			(0x18UL)		/*地址+写位发送完成，接收到ACK*/
#define		I2C_MS_SEND_ADDR_W_NO_ACK		(0x20UL)		/*地址+写位发送完成，未接收到ACK*/
#define		I2C_MS_SEND_DAT_ACK				(0x28UL)		/*主机模式下数据发送完成，接收到ACK*/
#define		I2C_MS_SEND_DAT_NO_ACK			(0x30UL)		/*主机模式下数据发送完成，未接收到ACK*/
#define		I2C_MS_TRANS_FAIL				(0x38UL)		/*在地址或数据传输过程中失败*/
#define		I2C_MS_SEND_ADDR_R_ACK			(0x40UL)		/*地址+读位发送完成，接收到ACK*/
#define		I2C_MS_SEND_ADDR_R_NO_ACK		(0x48UL)		/*地址+读位发送完成，未接收到ACK*/
#define		I2C_MS_READ_DAT_ACK				(0x50UL)		/*主机模式下接收到数据，回复ACK*/
#define		I2C_MS_READ_DAT_NO_ACK			(0x58UL)		/*主机模式下接收到数据，不回复ACK*/
#define		I2C_MS_SEND_SECOND_ADDR_ACK		(0xE0UL)		/*主机模式下发送完第二个地址，接收到ACK*/
#define		I2C_MS_SEND_SECOND_ADDR_NO_ACK	(0xE8UL)		/*主机模式下发送完第二个地址，未接收到ACK*/

/*----------------------------------------------------------------------------
 ** I2C 从机模式下的状态(SS: SLAVE STATUS)
-----------------------------------------------------------------------------*/
#define		I2C_SS_RECEIVE_ADDR_W				(0x60UL)			/*从机模式下接收到地址+写位，回复ACK*/	
#define		I2C_SS_ARL_RECEIVE_ADDR_W			(0x68UL)			/*主机仲裁失败，接收到从机地址+写位，回复ACK*/	
#define		I2C_SS_RECEIVE_BORADCAST_ACK		(0x70UL)			/*接收到广播呼叫地址，回复ACK*/	
#define		I2C_SS_ARL_RECEIVE_BORADCAST_ACK	(0x78UL)		/*主机仲裁失败，接收到广播呼叫地址，回复ACK*/	
#define		I2C_SS_RECEIVE_DAT_ACK				(0x80UL)			/*从机地址匹配后接收到数据，回复ACK*/
#define		I2C_SS_RECEIVE_DAT_NO_ACK			(0x88UL)			/*从机地址匹配后接收到数据，不回复ACK*/
#define		I2C_SS_BORADCAST_RECEIVE_DAT_ACK	(0x90UL)		/*从机接收广播呼叫地址后接收到数据，回复ACK*/
#define		I2C_SS_BORADCAST_RECEIVE_DAT_NO_ACK	(0x98UL)		/*从机接收广播呼叫地址后接收到数据，不回复ACK*/
#define		I2C_SS_RECEIVE_STOP_OR_RESTART		(0xA0UL)			/*从机模式下接收到停止信号或重启动信号*/
#define		I2C_SS_RECEIVE_ADDR_R_ACK			(0xA8UL)			/*从机模式下接收到地址+读位，回复ACK*/
#define		I2C_SS_ARL_RECEIVE_ADDR_R_ACK		(0xB0UL)			/*主机仲裁失败，接收到从机地址+读位，回复ACK*/
#define		I2C_SS_SEND_DAT_ACK					(0xB8UL)			/*从机模式下发送数据后，接收到ACK*/
#define		I2C_SS_SEND_DAT_NO_ACK				(0xC0UL)			/*从机模式下发送数据后，未接收到ACK*/
#define		I2C_SS_SEND_LAST_DAT_ACK			(0xC8UL)		/*从机模式下发送完最后一个数据，接收到ACK*/
#define		I2C_SS_SEND_LAST_DAT_NO_ACK			(0xD0UL)		/*从机模式下发送完最后一个数据，未接收到ACK*/



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
 ** \brief	I2C_ConfigClk
 **			设置I2C运行时钟
 ** \param [in] CLK_M: 0x00~0x7;
 **				CLK_N: 0x00~0xf;
 ** \return  none
 ** \note	采样时钟= PCLK/2M  ； SCL时钟= PCLK/（2M×（N+1）×10）
*****************************************************************************/
void	I2C_ConfigClk(uint32_t CLK_M, uint32_t CLK_N);

/*****************************************************************************
 ** \brief	I2C_EnableOutput
 **			开启I2C接口
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void	I2C_EnableOutput(void);

/*****************************************************************************
 ** \brief	I2C_DisableOutput
 **			关闭I2C接口
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_DisableOutput(void);

/*****************************************************************************
 ** \brief	I2C_EnableInt
 **			开启I2C中断
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_EnableInt(void);
/*****************************************************************************
 ** \brief	I2C_DisableInt
 **			关闭I2C中断
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_DisableInt(void);

/*****************************************************************************
 ** \brief	I2C_GetIntFlag
 **			获取中断标志位
 ** \param [in] none
 ** \return  0：I2C发生总线状态未改变  1：I2C发生总线状态改变
 ** \note	
*****************************************************************************/
uint32_t I2C_GetIntFlag(void);

/*****************************************************************************
 ** \brief	I2C_ClearIntFlag
 **			清I2C中断标志
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_ClearIntFlag(void);

/*****************************************************************************
 ** \brief	I2C_ClearStartFlag
 **			清I2C启动标志
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_ClearStartFlag(void);
/*****************************************************************************
 ** \brief	I2C_ClearACKFlag
 **			清I2C应答标志
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_ClearACKFlag(void);

/*****************************************************************************
 ** \brief	I2C_GetStatus
 **			获取I2C状态
 ** \param [in] none
 ** \return  Status
 ** \note	
*****************************************************************************/
uint32_t I2C_GetStatus(void);

/*****************************************************************************
 ** \brief	I2C_ConfigSlaveAddr
 **			设置从机模式地址
 ** \param [in] Slave: I2C_S_7BIT_ADDR0~I2C_S_10BIT_ADDR0
 **				addr: 7位地址 或者10位地址
 **				Broadcast: I2C_S_BROADCAST_EN 、I2C_S_BROADCAST_DIS
 **				AddrMask: 7位掩码或者8位掩码
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_ConfigSlaveModeAddr(uint32_t Slave, uint32_t Addr, uint32_t Broadcast ,uint32_t AddrMask );
/*****************************************************************************
 ** \brief	I2C_GetSlave10BitAddrFlag
 **			获取I2C 从机10位地址标志位
 ** \param [in] none
 ** \return 0:I2C地址不匹配   1: 12C 10位地址匹配
 ** \note	
*****************************************************************************/
uint32_t I2C_GetSlave10BitAddrFlag(void);
/*****************************************************************************
 ** \brief	I2C_GetSlave7BitAddrFlag
 **			获取I2C 从机7位地址标志位
 ** \param [in] none
 ** \return 0:I2C地址不匹配   1: 12C 10位地址匹配
 ** \note	
*****************************************************************************/
uint32_t I2C_GetSlave7BitAddrFlag(void);

/*****************************************************************************
 ** \brief	I2C_Reset
 **			复位I2C模块
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_Reset(void);

/*****************************************************************************
 ** \brief	I2C_SendData
 **			发送数据
 ** \param [in] I2CData
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendData(uint32_t I2CData);
/*****************************************************************************
 ** \brief	I2C_SendACK
 **			发送应答位
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendACK(void);
/*****************************************************************************
 ** \brief	I2C_NotSendACK
 **			不发送应答位
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_NotSendACK(void);
/*****************************************************************************
 ** \brief	I2C_SendStart
 **			发送启始位
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendStart(void);
/*****************************************************************************
 ** \brief	I2C_SendRestart
 **			发送重启位
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendRestart(void);
/*****************************************************************************
 ** \brief	I2C_SendStop
 **			发送停止位
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendStop(void);
/*****************************************************************************
 ** \brief	I2C_SendStopBeforeStart
 **			先发送一个停止位，接着发送一个启动位。
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendStopBeforeStart(void);

/*****************************************************************************
 ** \brief	I2C_GetData
 **			获取数据
 ** \param [in] none
 ** \return  Data
 ** \note	
*****************************************************************************/
uint32_t I2C_GetData(void);





#ifdef __cplusplus
}
#endif

#endif /* __I2C_H_ */

