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
 **SSP 模式  
-----------------------------------------------------------------------------*/
#define		SSP_FRAME_SPI			(0x00UL)
#define		SSP_FRAME_TISS			(0x01UL<<SSP_CON_FRF_Pos)
#define		SSP_FRAME_MICROWIRE		(0x02UL<<SSP_CON_FRF_Pos)	

/*----------------------------------------------------------------------------
 **SSP 时钟空闲极性选择
-----------------------------------------------------------------------------*/
#define		SSP_CPO_0				(0x00UL)		/*时钟空闲时为0*/
#define		SSP_CPO_1				(0x01UL<<SSP_CON_CPO_Pos)	/*时钟空闲时为1*/
/*----------------------------------------------------------------------------
 **SSP 时钟相位选择
-----------------------------------------------------------------------------*/
#define		SSP_CPHA_0				(0x00UL)
#define		SSP_CPHA_1				(0x01UL<<SSP_CON_CPH_Pos)
/*----------------------------------------------------------------------------
 **SSP 数据传输长度选择
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
 **			设置SSP运行模式
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
 **			设置SSP运行时钟
 ** \param [in] Clk_M: 0x00~0xff;
 **				Clk_N : 为2-254 的偶数
 ** \return  none
 ** \note	SSPCLK = PCLK /（（M+1）×N）
*****************************************************************************/
void SSP_ConfigClk(uint32_t Clk_M, uint32_t Clk_N);


/*****************************************************************************
 ** \brief	SSP_EnableMasterMode
 **			使能主控模式
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_EnableMasterMode(void);
/*****************************************************************************
 ** \brief	SSP_EnableSlaveMode
 **			使能从机模式
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_EnableSlaveMode(void);

/*****************************************************************************
 ** \brief	SSP_EnableMasterAutoControlCS
 **			使能主控模式下SPI自动控制片选（CS）信号
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_EnableMasterAutoControlCS(void);

/*****************************************************************************
 ** \brief	SSP_DisableMasterAutoControlCS
 **			关闭主控模式下SPI自动控制片选（CS）信号
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_DisableMasterAutoControlCS(void);

/*****************************************************************************
 ** \brief	SSP_MasterSetCS
 **			主控模式下拉高CS信号
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_MasterSetCS(void);
/*****************************************************************************
 ** \brief	SSP_MasterClearCS
 **			主控模式下拉低CS信号
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_MasterClearCS(void);
/*****************************************************************************
 ** \brief	SSP_SendData
 **			发送数据
 ** \param [in] data
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_SendData(uint32_t Data);
/*****************************************************************************
 ** \brief	SSP_GetData
 **			获取数据
 ** \param [in] none
 ** \return  16bit data
 ** \note	
*****************************************************************************/
uint32_t SSP_GetData(void);

/*****************************************************************************
 ** \brief	SSP_Start
 **			开启SSP
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_Start(void);
/*****************************************************************************
 ** \brief	SSP_Stop
 **			关闭SSP
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_Stop(void);



/*****************************************************************************
 ** \brief	SSP_GetBusyFlag
 **			获取SSP忙标志位
 ** \param [in] none
 ** \return  1：SSP正在发送/接收数据或发送FIFO非空  0：SSP空闲
 ** \note	
*****************************************************************************/
uint32_t SSP_GetBusyFlag(void);

/*****************************************************************************
 ** \brief	SSP_GetRFFFlag
 **			获取接收FIFO满标志位
 ** \param [in] none
 ** \return  1：接收FIFO已满  0：接收FIFO未满
 ** \note	
*****************************************************************************/
uint32_t SSP_GetRFFFlag(void);

/*****************************************************************************
 ** \brief	SSP_GetRNEFlag
 **			获取接收FIFO非空标志位
 ** \param [in] none
 ** \return  1：接收FIFO非空  0：接收FIFO为空
 ** \note	
*****************************************************************************/
uint32_t SSP_GetRNEFlag(void);

/*****************************************************************************
 ** \brief	SSP_GetTNFFlag
 **			获取发送FIFO非满标志位
 ** \param [in] none
 ** \return   1：发送FIFO未满  0：发送FIFO已满
 ** \note	
*****************************************************************************/
uint32_t SSP_GetTNFFlag(void);

/*****************************************************************************
 ** \brief	SSP_GetTFEFlag
 **			获取发送FIFO空标志位
 ** \param [in] none
 ** \return   1：发送FIFO已空  0：发送FIFO非空
 ** \note	
*****************************************************************************/
uint32_t SSP_GetTFEFlag(void);

/*****************************************************************************
 ** \brief	SSP_EnableSendHalfInt
 **			开启发送FIFO半空（至少发送4个Buffer）中断
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_EnableSendHalfInt(void);
/*****************************************************************************
 ** \brief	SSP_DisableSendHalfInt
 **			关闭发送FIFO半空中断
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_DisableSendHalfInt(void);
/*****************************************************************************
 ** \brief	SSP_GetSendHalfIntFlag
 **			获取发送FIFO半空中断标志位
 ** \param [in] none
 ** \return   1：使能发送FIFO半空中断，且发送FIFO至少半空  0：未到半空
 ** \note	
*****************************************************************************/
uint32_t SSP_GetSendHalfIntFlag(void);
/*****************************************************************************
 ** \brief	SSP_ClearSendHalfIntFlag
 **			清除发送FIFO半空中断标志位
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_ClearSendHalfIntFlag(void);

/*****************************************************************************
 ** \brief	SSP_EnableReciveHalfInt
 **			开启接收FIFO半满（至少接收4个Buffer）中断
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_EnableReciveHalfInt(void);
/*****************************************************************************
 ** \brief	SSP_DisableReciveHalfInt
 **			关闭接收FIFO半满中断
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_DisableReciveHalfInt(void);
/*****************************************************************************
 ** \brief	SSP_GetReceiveHalfIntFlag
 **			获取接收FIFO半满中断标志位
 ** \param [in] none
 ** \return   1：使能接收FIFO半满中断，且接收FIFO至少半满  0：接收FIFO半未满
 ** \note	
*****************************************************************************/
uint32_t SSP_GetReceiveHalfIntFlag(void);
/*****************************************************************************
 ** \brief	SSP_ClearReceiveHalfIntFlag
 **			清除接收FIFO半满中断标志位
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_ClearReceiveHalfIntFlag(void);

/*****************************************************************************
 ** \brief	SSP_EnableReciveTimerOverflowInt
 **			开启接收FIFO定时器溢出中断
 ** \param [in] none
 ** \return   none
 ** \note	（溢出时间为：64×SSPCLK）
*****************************************************************************/
void SSP_EnableReciveTimerOverflowInt(void);
/*****************************************************************************
 ** \brief	SSP_DisableReciveTimerOverflowInt
 **			关闭接收FIFO定时器溢出中断
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_DisableReciveTimerOverflowInt(void);
/*****************************************************************************
 ** \brief	SSP_GetReceiveHalfIntFlag
 **			获取接收FIFO定时器溢出中断标志位
 ** \param [in] none
 ** \return   1：使能接收FIFO定时器溢出中断，且接收FIFO非空，超时未被读  0：接收FIFO半未满
 ** \note	
*****************************************************************************/
uint32_t SSP_GetReciveTimerOverflowIntFlag(void);
/*****************************************************************************
 ** \brief	SSP_ClearReceiveHalfIntFlag
 **			清除接收FIFO定时器溢出中断标志位
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_ClearReciveTimerOverflowIntFlag(void);

/*****************************************************************************
 ** \brief	SSP_EnableReciveTimerOverflowInt
 **			开启接收FIFO定时器溢出中断
 ** \param [in] none
 ** \return   none
 ** \note	（溢出时间为：64×SSPCLK）
*****************************************************************************/
void SSP_EnableReciveTimerOverflowInt(void);
/*****************************************************************************
 ** \brief	SSP_DisableReciveTimerOverflowInt
 **			关闭接收FIFO定时器溢出中断
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_DisableReciveTimerOverflowInt(void);
/*****************************************************************************
 ** \brief	SSP_GetReceiveHalfIntFlag
 **			获取接收FIFO定时器溢出中断标志位
 ** \param [in] none
 ** \return   1：使能接收FIFO定时器溢出中断，且接收FIFO非空，超时未被读  0：接收FIFO半未满
 ** \note	
*****************************************************************************/
uint32_t SSP_GetReciveTimerOverflowIntFlag(void);
/*****************************************************************************
 ** \brief	SSP_ClearReceiveHalfIntFlag
 **			清除接收FIFO定时器溢出中断标志位
 ** \param [in] none
 ** \return   none
 ** \note	
*****************************************************************************/
void SSP_ClearReciveTimerOverflowIntFlag(void);






#ifdef __cplusplus
}
#endif

#endif /* __SSP_H_ */

