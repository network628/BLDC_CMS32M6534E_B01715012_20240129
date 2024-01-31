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
/** \file ssp.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "ssp.h"
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
void SSP_ConfigRunMode(uint32_t FrameMode, uint32_t ClkPolarity,uint32_t ClkCPHA, uint32_t DataLength)
{
	SSP->CON &= ~(SSP_CON_CPH_Msk | SSP_CON_CPO_Msk |SSP_CON_FRF_Msk|SSP_CON_DSS_Msk);
	SSP->CON |= FrameMode | ClkPolarity | ClkCPHA | DataLength;
}

/*****************************************************************************
 ** \brief	SSP_ConfigClk
 **			设置SSP运行时钟
 ** \param [in] Clk_M: 0x00~0xff;
 **				Clk_N : 为2-254 的偶数
 ** \return  none
 ** \note	SSPCLK = PCLK /（（M+1）×N）
*****************************************************************************/
void SSP_ConfigClk(uint32_t Clk_M, uint32_t Clk_N)
{
	if(Clk_N & 0x1)
	{
		Clk_N += 1;
	}
	SSP->CLK = ((0xff & Clk_M)<< 8) | (0xff & Clk_N);
}
/*****************************************************************************
 ** \brief	SSP_EnableMasterMode
 **			使能主控模式
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_EnableMasterMode(void)
{
	SSP->CON &= ~(SSP_CON_MS_Msk);
}
/*****************************************************************************
 ** \brief	SSP_EnableSlaveMode
 **			使能从机模式
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_EnableSlaveMode(void)
{
	SSP->CON |= (SSP_CON_MS_Msk);
}
/*****************************************************************************
 ** \brief	SSP_Start
 **			开启SSP
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_Start(void)
{
	SSP->CON |= (SSP_CON_SSPEN_Msk);	
}
/*****************************************************************************
 ** \brief	SSP_Stop
 **			关闭SSP
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_Stop(void)
{
	SSP->CON &= ~(SSP_CON_SSPEN_Msk);
}

/*****************************************************************************
 ** \brief	SSP_EnableMasterAutoControlCS
 **			使能主控模式下SPI自动控制片选（CS）信号
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_EnableMasterAutoControlCS(void)
{
	SSP->CSCR &= ~(SSP_CSCR_SWSEL_Msk);
}	

/*****************************************************************************
 ** \brief	SSP_DisableMasterAutoControlCS
 **			关闭主控模式下SPI自动控制片选（CS）信号
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_DisableMasterAutoControlCS(void)
{
	SSP->CSCR |= (SSP_CSCR_SWSEL_Msk);
}

/*****************************************************************************
 ** \brief	SSP_MasterSetCS
 **			主控模式下拉高CS信号
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_MasterSetCS(void)
{
	SSP->CSCR |= SSP_CSCR_SWCS_Msk;
}

/*****************************************************************************
 ** \brief	SSP_MasterClearCS
 **			主控模式下拉低CS信号
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_MasterClearCS(void)
{
	SSP->CSCR &= ~(SSP_CSCR_SWCS_Msk);
}
/*****************************************************************************
 ** \brief	SSP_SendData
 **			发送数据
 ** \param [in] data
 ** \return  none
 ** \note	
*****************************************************************************/
void SSP_SendData(uint32_t Data)
{
	SSP->DAT = Data;
}
/*****************************************************************************
 ** \brief	SSP_GetData
 **			获取数据
 ** \param [in] none
 ** \return  16bit data
 ** \note	
*****************************************************************************/
uint32_t SSP_GetData(void)
{
	return (0xffff&(SSP->DAT));
}
/*****************************************************************************
 ** \brief	SSP_GetBusyFlag
 **			获取SSP忙标志位
 ** \param [in] none
 ** \return  1：SSP正在发送/接收数据或发送FIFO非空  0：SSP空闲
 ** \note	
*****************************************************************************/
uint32_t SSP_GetBusyFlag(void)
{
	return((SSP->STAT & SSP_STAT_BSY_Msk)? 1:0);
}
/*****************************************************************************
 ** \brief	SSP_GetRFFFlag
 **			获取接收FIFO满标志位
 ** \param [in] none
 ** \return  1：接收FIFO已满  0：接收FIFO未满
 ** \note	
*****************************************************************************/
uint32_t SSP_GetRFFFlag(void)
{
	return((SSP->STAT & SSP_STAT_RFF_Msk)? 1:0);
}

/*****************************************************************************
 ** \brief	SSP_GetRNEFlag
 **			获取接收FIFO非空标志位
 ** \param [in] none
 ** \return  1：接收FIFO非空  0：接收FIFO为空
 ** \note	
*****************************************************************************/
uint32_t SSP_GetRNEFlag(void)
{
	return((SSP->STAT & SSP_STAT_RNE_Msk)? 1:0);
}

/*****************************************************************************
 ** \brief	SSP_GetTNFFlag
 **			获取发送FIFO非满标志位
 ** \param [in] none
 ** \return   1：发送FIFO未满  0：发送FIFO已满
 ** \note	
*****************************************************************************/
uint32_t SSP_GetTNFFlag(void)
{
	return((SSP->STAT & SSP_STAT_TNF_Msk)? 1:0);
}

/*****************************************************************************
 ** \brief	SSP_GetTFEFlag
 **			获取发送FIFO空标志位
 ** \param [in] none
 ** \return   1：发送FIFO已空  0：发送FIFO非空
 ** \note	
*****************************************************************************/
uint32_t SSP_GetTFEFlag(void)
{
	return((SSP->STAT & SSP_STAT_TFE_Msk)? 1:0);
}

