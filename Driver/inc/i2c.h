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
 ** I2C �ӻ�ģʽ��ַ:
-----------------------------------------------------------------------------*/
#define		I2C_S_7BIT_ADDR0		(0x0UL)	
#define		I2C_S_7BIT_ADDR1		(0x1UL)
#define		I2C_S_7BIT_ADDR2		(0x2UL)
#define		I2C_S_7BIT_ADDR3		(0x3UL)
#define		I2C_S_10BIT_ADDR0		(0x4UL)

#define		I2C_S_BROADCAST_EN		(0x01UL)		/*ʹ�ܹ㲥���е�ַʶ��*/
#define		I2C_S_BROADCAST_DIS		(0x00UL)		/*��ֹ�㲥���е�ַʶ��*/

/*----------------------------------------------------------------------------
 ** I2C ����״̬
-----------------------------------------------------------------------------*/
#define		I2C_STATUS_NOTHING				(0xF8UL)		/*��ȷ�е�״̬*/
/*----------------------------------------------------------------------------
** I2C����ģʽ�µ�״̬(MS: MASTER STATUS)
-----------------------------------------------------------------------------*/
#define		I2C_MS_BUS_ERROR				(0x00UL)		/*���ߴ���ֻ������ģʽ��Ч��*/
#define		I2C_MS_START_OK					(0x08UL)		/*����λ�������*/
#define		I2C_MS_RESTART_OK				(0x10UL)		/*������λ�������*/
#define		I2C_MS_SEND_ADDR_W_ACK			(0x18UL)		/*��ַ+дλ������ɣ����յ�ACK*/
#define		I2C_MS_SEND_ADDR_W_NO_ACK		(0x20UL)		/*��ַ+дλ������ɣ�δ���յ�ACK*/
#define		I2C_MS_SEND_DAT_ACK				(0x28UL)		/*����ģʽ�����ݷ�����ɣ����յ�ACK*/
#define		I2C_MS_SEND_DAT_NO_ACK			(0x30UL)		/*����ģʽ�����ݷ�����ɣ�δ���յ�ACK*/
#define		I2C_MS_TRANS_FAIL				(0x38UL)		/*�ڵ�ַ�����ݴ��������ʧ��*/
#define		I2C_MS_SEND_ADDR_R_ACK			(0x40UL)		/*��ַ+��λ������ɣ����յ�ACK*/
#define		I2C_MS_SEND_ADDR_R_NO_ACK		(0x48UL)		/*��ַ+��λ������ɣ�δ���յ�ACK*/
#define		I2C_MS_READ_DAT_ACK				(0x50UL)		/*����ģʽ�½��յ����ݣ��ظ�ACK*/
#define		I2C_MS_READ_DAT_NO_ACK			(0x58UL)		/*����ģʽ�½��յ����ݣ����ظ�ACK*/
#define		I2C_MS_SEND_SECOND_ADDR_ACK		(0xE0UL)		/*����ģʽ�·�����ڶ�����ַ�����յ�ACK*/
#define		I2C_MS_SEND_SECOND_ADDR_NO_ACK	(0xE8UL)		/*����ģʽ�·�����ڶ�����ַ��δ���յ�ACK*/

/*----------------------------------------------------------------------------
 ** I2C �ӻ�ģʽ�µ�״̬(SS: SLAVE STATUS)
-----------------------------------------------------------------------------*/
#define		I2C_SS_RECEIVE_ADDR_W				(0x60UL)			/*�ӻ�ģʽ�½��յ���ַ+дλ���ظ�ACK*/	
#define		I2C_SS_ARL_RECEIVE_ADDR_W			(0x68UL)			/*�����ٲ�ʧ�ܣ����յ��ӻ���ַ+дλ���ظ�ACK*/	
#define		I2C_SS_RECEIVE_BORADCAST_ACK		(0x70UL)			/*���յ��㲥���е�ַ���ظ�ACK*/	
#define		I2C_SS_ARL_RECEIVE_BORADCAST_ACK	(0x78UL)		/*�����ٲ�ʧ�ܣ����յ��㲥���е�ַ���ظ�ACK*/	
#define		I2C_SS_RECEIVE_DAT_ACK				(0x80UL)			/*�ӻ���ַƥ�����յ����ݣ��ظ�ACK*/
#define		I2C_SS_RECEIVE_DAT_NO_ACK			(0x88UL)			/*�ӻ���ַƥ�����յ����ݣ����ظ�ACK*/
#define		I2C_SS_BORADCAST_RECEIVE_DAT_ACK	(0x90UL)		/*�ӻ����չ㲥���е�ַ����յ����ݣ��ظ�ACK*/
#define		I2C_SS_BORADCAST_RECEIVE_DAT_NO_ACK	(0x98UL)		/*�ӻ����չ㲥���е�ַ����յ����ݣ����ظ�ACK*/
#define		I2C_SS_RECEIVE_STOP_OR_RESTART		(0xA0UL)			/*�ӻ�ģʽ�½��յ�ֹͣ�źŻ��������ź�*/
#define		I2C_SS_RECEIVE_ADDR_R_ACK			(0xA8UL)			/*�ӻ�ģʽ�½��յ���ַ+��λ���ظ�ACK*/
#define		I2C_SS_ARL_RECEIVE_ADDR_R_ACK		(0xB0UL)			/*�����ٲ�ʧ�ܣ����յ��ӻ���ַ+��λ���ظ�ACK*/
#define		I2C_SS_SEND_DAT_ACK					(0xB8UL)			/*�ӻ�ģʽ�·������ݺ󣬽��յ�ACK*/
#define		I2C_SS_SEND_DAT_NO_ACK				(0xC0UL)			/*�ӻ�ģʽ�·������ݺ�δ���յ�ACK*/
#define		I2C_SS_SEND_LAST_DAT_ACK			(0xC8UL)		/*�ӻ�ģʽ�·��������һ�����ݣ����յ�ACK*/
#define		I2C_SS_SEND_LAST_DAT_NO_ACK			(0xD0UL)		/*�ӻ�ģʽ�·��������һ�����ݣ�δ���յ�ACK*/



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
 **			����I2C����ʱ��
 ** \param [in] CLK_M: 0x00~0x7;
 **				CLK_N: 0x00~0xf;
 ** \return  none
 ** \note	����ʱ��= PCLK/2M  �� SCLʱ��= PCLK/��2M����N+1����10��
*****************************************************************************/
void	I2C_ConfigClk(uint32_t CLK_M, uint32_t CLK_N);

/*****************************************************************************
 ** \brief	I2C_EnableOutput
 **			����I2C�ӿ�
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void	I2C_EnableOutput(void);

/*****************************************************************************
 ** \brief	I2C_DisableOutput
 **			�ر�I2C�ӿ�
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_DisableOutput(void);

/*****************************************************************************
 ** \brief	I2C_EnableInt
 **			����I2C�ж�
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_EnableInt(void);
/*****************************************************************************
 ** \brief	I2C_DisableInt
 **			�ر�I2C�ж�
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_DisableInt(void);

/*****************************************************************************
 ** \brief	I2C_GetIntFlag
 **			��ȡ�жϱ�־λ
 ** \param [in] none
 ** \return  0��I2C��������״̬δ�ı�  1��I2C��������״̬�ı�
 ** \note	
*****************************************************************************/
uint32_t I2C_GetIntFlag(void);

/*****************************************************************************
 ** \brief	I2C_ClearIntFlag
 **			��I2C�жϱ�־
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_ClearIntFlag(void);

/*****************************************************************************
 ** \brief	I2C_ClearStartFlag
 **			��I2C������־
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_ClearStartFlag(void);
/*****************************************************************************
 ** \brief	I2C_ClearACKFlag
 **			��I2CӦ���־
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_ClearACKFlag(void);

/*****************************************************************************
 ** \brief	I2C_GetStatus
 **			��ȡI2C״̬
 ** \param [in] none
 ** \return  Status
 ** \note	
*****************************************************************************/
uint32_t I2C_GetStatus(void);

/*****************************************************************************
 ** \brief	I2C_ConfigSlaveAddr
 **			���ôӻ�ģʽ��ַ
 ** \param [in] Slave: I2C_S_7BIT_ADDR0~I2C_S_10BIT_ADDR0
 **				addr: 7λ��ַ ����10λ��ַ
 **				Broadcast: I2C_S_BROADCAST_EN ��I2C_S_BROADCAST_DIS
 **				AddrMask: 7λ�������8λ����
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_ConfigSlaveModeAddr(uint32_t Slave, uint32_t Addr, uint32_t Broadcast ,uint32_t AddrMask );
/*****************************************************************************
 ** \brief	I2C_GetSlave10BitAddrFlag
 **			��ȡI2C �ӻ�10λ��ַ��־λ
 ** \param [in] none
 ** \return 0:I2C��ַ��ƥ��   1: 12C 10λ��ַƥ��
 ** \note	
*****************************************************************************/
uint32_t I2C_GetSlave10BitAddrFlag(void);
/*****************************************************************************
 ** \brief	I2C_GetSlave7BitAddrFlag
 **			��ȡI2C �ӻ�7λ��ַ��־λ
 ** \param [in] none
 ** \return 0:I2C��ַ��ƥ��   1: 12C 10λ��ַƥ��
 ** \note	
*****************************************************************************/
uint32_t I2C_GetSlave7BitAddrFlag(void);

/*****************************************************************************
 ** \brief	I2C_Reset
 **			��λI2Cģ��
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_Reset(void);

/*****************************************************************************
 ** \brief	I2C_SendData
 **			��������
 ** \param [in] I2CData
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendData(uint32_t I2CData);
/*****************************************************************************
 ** \brief	I2C_SendACK
 **			����Ӧ��λ
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendACK(void);
/*****************************************************************************
 ** \brief	I2C_NotSendACK
 **			������Ӧ��λ
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_NotSendACK(void);
/*****************************************************************************
 ** \brief	I2C_SendStart
 **			������ʼλ
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendStart(void);
/*****************************************************************************
 ** \brief	I2C_SendRestart
 **			��������λ
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendRestart(void);
/*****************************************************************************
 ** \brief	I2C_SendStop
 **			����ֹͣλ
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendStop(void);
/*****************************************************************************
 ** \brief	I2C_SendStopBeforeStart
 **			�ȷ���һ��ֹͣλ�����ŷ���һ������λ��
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void I2C_SendStopBeforeStart(void);

/*****************************************************************************
 ** \brief	I2C_GetData
 **			��ȡ����
 ** \param [in] none
 ** \return  Data
 ** \note	
*****************************************************************************/
uint32_t I2C_GetData(void);





#ifdef __cplusplus
}
#endif

#endif /* __I2C_H_ */

