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
/** \file ssp.h
**
** History:
** 
*****************************************************************************/
#ifndef __SSP_H_
#define __SSP_H_

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
 **SSP ģʽ  
-----------------------------------------------------------------------------*/
#define		SSP_FRAME_SPI			(0x00UL)
#define		SSP_FRAME_TISS			(0x01UL<<SSP_CON_FRF_Pos)
#define		SSP_FRAME_MICROWIRE		(0x02UL<<SSP_CON_FRF_Pos)	

/*----------------------------------------------------------------------------
 **SSP ʱ�ӿ��м���ѡ��
-----------------------------------------------------------------------------*/
#define		SSP_CPO_0				(0x00UL)		/*ʱ�ӿ���ʱΪ0*/
#define		SSP_CPO_1				(0x01UL<<SSP_CON_CPO_Pos)	/*ʱ�ӿ���ʱΪ1*/
/*----------------------------------------------------------------------------
 **SSP ʱ����λѡ��
-----------------------------------------------------------------------------*/
#define		SSP_CPHA_0				(0x00UL)
#define		SSP_CPHA_1				(0x01UL<<SSP_CON_CPH_Pos)
/*----------------------------------------------------------------------------
 **SSP ���ݴ��䳤��ѡ��
-----------------------------------------------------------------------------*/
#define		SSP_DAT_LENGTH_4		(0x03UL)
#define		SSP_DAT_LENGTH_5		(0x04UL)
#define		SSP_DAT_LENGTH_6		(0x05UL)
#define		SSP_DAT_LENGTH_7		(0x06UL)
#define		SSP_DAT_LENGTH_8		(0x07UL)
#define		SSP_DAT_LENGTH_9		(0x08UL)
#define		SSP_DAT_LENGTH_10		(0x09UL)
#define		SSP_DAT_LENGTH_11		(0x0AUL)
#define		SSP_DAT_LENGTH_12		(0x0BUL)
#define		SSP_DAT_LENGTH_13		(0x0CUL)
#define		SSP_DAT_LENGTH_14		(0x0DUL)
#define		SSP_DAT_LENGTH_15		(0x0EUL)
#define		SSP_DAT_LENGTH_16		(0x0FUL)

/*****************************************************************************
 ** \brief	
 **			
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/

							
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
 ** \brief	SSP_ConfigRunMode
 **			����SSP����ģʽ
 ** \param [in] FrameMode: (1)SSP_FRAME_SPI
 **							(2)SSP_FRAME_TISS
 **							(3)SSP_FRAME_MICROWIRE
 **				ClkPolarity:(1)SSP_CPO_0		
 **							(2)SSP_CPO_1
 **				ClkCPHA : (1)SSP_CPHA_0
 **						  (2)SSP_CPHA_1
 **				DataLength:(1)SSP_DAT_LENGTH_4
 **							(2)SSP_DAT_LENGTH_5
 **							(3)SSP_DAT_LENGTH_6
 **							(4)SSP_DAT_LENGTH_7
 **							(5)SSP_DAT_LENGTH_8
 **							(6)SSP_DAT_LENGTH_9
 **							(7)SSP_DAT_LENGTH_10
 **							(8)SSP_DAT_LENGTH_11
 **							(9)SSP_DAT_LENGTH_12
 **							(10)SSP_DAT_LENGTH_13
 **							(11)SSP_DAT_LENGTH_14
 **							(12)SSP_DAT_LENGTH_15
 **							(13)SSP_DAT_LENGTH_16
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_ConfigRunMode(uint32_t FrameMode, uint32_t ClkPolarity,uint32_t ClkCPHA, uint32_t DataLength);

/*****************************************************************************
 ** \brief	SSP_ConfigClk
 **			����SSP����ʱ��
 ** \param [in] Clk_M: 0x00~0xff;
 **				Clk_N : Ϊ2-254 ��ż��
 ** \return  none
 ** \note	SSPCLK = PCLK /����M+1����N��
*****************************************************************************/
void SSP_ConfigClk(uint32_t Clk_M, uint32_t Clk_N);


/*****************************************************************************
 ** \brief	SSP_EnableMasterMode
 **			ʹ������ģʽ
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_EnableMasterMode(void);
/*****************************************************************************
 ** \brief	SSP_EnableSlaveMode
 **			ʹ�ܴӻ�ģʽ
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_EnableSlaveMode(void);

/*****************************************************************************
 ** \brief	SSP_EnableMasterAutoControlCS
 **			ʹ������ģʽ��SPI�Զ�����Ƭѡ��CS���ź�
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_EnableMasterAutoControlCS(void);

/*****************************************************************************
 ** \brief	SSP_DisableMasterAutoControlCS
 **			�ر�����ģʽ��SPI�Զ�����Ƭѡ��CS���ź�
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_DisableMasterAutoControlCS(void);

/*****************************************************************************
 ** \brief	SSP_MasterSetCS
 **			����ģʽ������CS�ź�
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_MasterSetCS(void);
/*****************************************************************************
 ** \brief	SSP_MasterClearCS
 **			����ģʽ������CS�ź�
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_MasterClearCS(void);
/*****************************************************************************
 ** \brief	SSP_SendData
 **			��������
 ** \param [in] data
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_SendData(uint32_t Data);
/*****************************************************************************
 ** \brief	SSP_GetData
 **			��ȡ����
 ** \param [in] none
 ** \return  16bit data
 ** \note	
*****************************************************************************/
uint32_t SSP_GetData(void);

/*****************************************************************************
 ** \brief	SSP_Start
 **			����SSP
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_Start(void);
/*****************************************************************************
 ** \brief	SSP_Stop
 **			�ر�SSP
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_Stop(void);



/*****************************************************************************
 ** \brief	SSP_GetBusyFlag
 **			��ȡSSPæ��־λ
 ** \param [in] none
 ** \return  1��SSP���ڷ���/�������ݻ���FIFO�ǿ�  0��SSP����
 ** \note	
*****************************************************************************/
uint32_t SSP_GetBusyFlag(void);

/*****************************************************************************
 ** \brief	SSP_GetRFFFlag
 **			��ȡ����FIFO����־λ
 ** \param [in] none
 ** \return  1������FIFO����  0������FIFOδ��
 ** \note	
*****************************************************************************/
uint32_t SSP_GetRFFFlag(void);

/*****************************************************************************
 ** \brief	SSP_GetRNEFlag
 **			��ȡ����FIFO�ǿձ�־λ
 ** \param [in] none
 ** \return  1������FIFO�ǿ�  0������FIFOΪ��
 ** \note	
*****************************************************************************/
uint32_t SSP_GetRNEFlag(void);

/*****************************************************************************
 ** \brief	SSP_GetTNFFlag
 **			��ȡ����FIFO������־λ
 ** \param [in] none
 ** \return   1������FIFOδ��  0������FIFO����
 ** \note	
*****************************************************************************/
uint32_t SSP_GetTNFFlag(void);

/*****************************************************************************
 ** \brief	SSP_GetTFEFlag
 **			��ȡ����FIFO�ձ�־λ
 ** \param [in] none
 ** \return   1������FIFO�ѿ�  0������FIFO�ǿ�
 ** \note	
*****************************************************************************/
uint32_t SSP_GetTFEFlag(void);

/*****************************************************************************
 ** \brief	SSP_EnableSendHalfInt
 **			��������FIFO��գ����ٷ���4��Buffer���ж�
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_EnableSendHalfInt(void);
/*****************************************************************************
 ** \brief	SSP_DisableSendHalfInt
 **			�رշ���FIFO����ж�
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_DisableSendHalfInt(void);
/*****************************************************************************
 ** \brief	SSP_GetSendHalfIntFlag
 **			��ȡ����FIFO����жϱ�־λ
 ** \param [in] none
 ** \return   1��ʹ�ܷ���FIFO����жϣ��ҷ���FIFO���ٰ��  0��δ�����
 ** \note	
*****************************************************************************/
uint32_t SSP_GetSendHalfIntFlag(void);
/*****************************************************************************
 ** \brief	SSP_ClearSendHalfIntFlag
 **			�������FIFO����жϱ�־λ
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_ClearSendHalfIntFlag(void);

/*****************************************************************************
 ** \brief	SSP_EnableReciveHalfInt
 **			��������FIFO���������ٽ���4��Buffer���ж�
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_EnableReciveHalfInt(void);
/*****************************************************************************
 ** \brief	SSP_DisableReciveHalfInt
 **			�رս���FIFO�����ж�
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_DisableReciveHalfInt(void);
/*****************************************************************************
 ** \brief	SSP_GetReceiveHalfIntFlag
 **			��ȡ����FIFO�����жϱ�־λ
 ** \param [in] none
 ** \return   1��ʹ�ܽ���FIFO�����жϣ��ҽ���FIFO���ٰ���  0������FIFO��δ��
 ** \note	
*****************************************************************************/
uint32_t SSP_GetReceiveHalfIntFlag(void);
/*****************************************************************************
 ** \brief	SSP_ClearReceiveHalfIntFlag
 **			�������FIFO�����жϱ�־λ
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_ClearReceiveHalfIntFlag(void);

/*****************************************************************************
 ** \brief	SSP_EnableReciveTimerOverflowInt
 **			��������FIFO��ʱ������ж�
 ** \param [in] none
 ** \return   none
 ** \note	�����ʱ��Ϊ��64��SSPCLK��
*****************************************************************************/
void SSP_EnableReciveTimerOverflowInt(void);
/*****************************************************************************
 ** \brief	SSP_DisableReciveTimerOverflowInt
 **			�رս���FIFO��ʱ������ж�
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_DisableReciveTimerOverflowInt(void);
/*****************************************************************************
 ** \brief	SSP_GetReceiveHalfIntFlag
 **			��ȡ����FIFO��ʱ������жϱ�־λ
 ** \param [in] none
 ** \return   1��ʹ�ܽ���FIFO��ʱ������жϣ��ҽ���FIFO�ǿգ���ʱδ����  0������FIFO��δ��
 ** \note	
*****************************************************************************/
uint32_t SSP_GetReciveTimerOverflowIntFlag(void);
/*****************************************************************************
 ** \brief	SSP_ClearReceiveHalfIntFlag
 **			�������FIFO��ʱ������жϱ�־λ
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_ClearReciveTimerOverflowIntFlag(void);

/*****************************************************************************
 ** \brief	SSP_EnableReciveTimerOverflowInt
 **			��������FIFO��ʱ������ж�
 ** \param [in] none
 ** \return   none
 ** \note	�����ʱ��Ϊ��64��SSPCLK��
*****************************************************************************/
void SSP_EnableReciveTimerOverflowInt(void);
/*****************************************************************************
 ** \brief	SSP_DisableReciveTimerOverflowInt
 **			�رս���FIFO��ʱ������ж�
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_DisableReciveTimerOverflowInt(void);
/*****************************************************************************
 ** \brief	SSP_GetReceiveHalfIntFlag
 **			��ȡ����FIFO��ʱ������жϱ�־λ
 ** \param [in] none
 ** \return   1��ʹ�ܽ���FIFO��ʱ������жϣ��ҽ���FIFO�ǿգ���ʱδ����  0������FIFO��δ��
 ** \note	
*****************************************************************************/
uint32_t SSP_GetReciveTimerOverflowIntFlag(void);
/*****************************************************************************
 ** \brief	SSP_ClearReceiveHalfIntFlag
 **			�������FIFO��ʱ������жϱ�־λ
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_ClearReciveTimerOverflowIntFlag(void);






#ifdef __cplusplus
}
#endif

#endif /* __SSP_H_ */

