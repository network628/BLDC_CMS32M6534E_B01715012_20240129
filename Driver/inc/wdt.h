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
/** \file wdt.h
**
** History:
** 
*****************************************************************************/
#ifndef __WDT_H__
#define __WDT_H__

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
 **WDT ʱ��
-----------------------------------------------------------------------------*/
#define		WDT_CLK_DIV_1		(0x00UL)	/*WDT clk 1 ��Ƶ*/
#define		WDT_CLK_DIV_16		(0x01UL<<WDT_WDTCON_WDTPRE_Pos)	/*WDT clk 16 ��Ƶ*/
#define		WDT_CLK_DIV_256		(0x02UL<<WDT_WDTCON_WDTPRE_Pos)	/*WDT clk 256 ��Ƶ*/
/*----------------------------------------------------------------------------
 **WDT д����
-----------------------------------------------------------------------------*/
#define		WDT_ICLR_WRITE_KEY	(0x55AA55AAUL)
#define		WDT_LOCK_WRITE_KEY	(0x55AA6699UL)
#define		WDT_CON_WRITE_KEY	(0x5AUL<<WDT_WDTCON_WDTEN_Pos)

/*****************************************************************************
 ** \brief	WDT_GET_RIS_FLAG
 **			��ȡWDT�ж�Դ״̬
 ** \param [in] none
 ** \return  1:����WDT��������ж� 0�����ж�
 ** \note	 
*****************************************************************************/
#define		WDT_GET_RIS_FLAG()	(((WDT->RIS &WDT_WDTRIS_WDTRIS_Msk)? 1:0))

/*****************************************************************************
 ** \brief	WDT_ENABLE_DEBUG
 **			WDT��DEBUGģʽ��ʹ��
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
#define		WDT_ENABLE_DEBUG()	do{\
									WDT->LOCK = WDT_LOCK_WRITE_KEY;\
									WDT->CON |= WDT_WDTCON_DEBUG_Msk;\
									WDT->LOCK = 0x0;\
								}while(0)
/*****************************************************************************
 ** \brief	WDT_DISABLE_DEBUG
 **			WDT��DEBUGģʽ�йر�
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
#define		WDT_DISABLE_DEBUG()	do{\
									WDT->LOCK = WDT_LOCK_WRITE_KEY;\
									WDT->CON &= ~(WDT_WDTCON_DEBUG_Msk);\
									WDT->LOCK = 0x0;\
								}while(0)
							
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
 ** \brief	 WDT_EnableRestSystem
 **			 ����WDT�����λϵͳ
 ** \param [in] none
 ** \return  none
 ** \note	(1)ʹ��WDT ��λ��������WDT �жϺ�û�����жϱ�־λ���´η���WDT�ж�ʱ����WDT��λ.
 **			(2)��ʹ�ܸ�λʱ������ʹ��WDT�ж�.
*****************************************************************************/
void WDT_EnableRestSystem(void);
/*****************************************************************************
 ** \brief	 WDT_DisableRestSystem
 **			 �ر�WDT�����λϵͳ
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void WDT_DisableRestSystem(void);

/*****************************************************************************
 ** \brief	 WDT_ConfigClk
 **			 ����WDTʱ��
 ** \param [in] ClkDiv: (1)WDT_CLK_DIV_1   :
 **						(2)WDT_CLK_DIV_16  :
 **						(3)WDT_CLK_DIV_256 :
 ** \return  none
 ** \note	
*****************************************************************************/
void WDT_ConfigClk(uint32_t ClkDiv);
/*****************************************************************************
 ** \brief	 WDT_ConfigPeriod
 **			 ����WDT�������
 ** \param [in] Period: 32BitValue
 ** \return  none
 ** \note	 
*****************************************************************************/
void WDT_ConfigPeriod(uint32_t Period);

/*****************************************************************************
 ** \brief	 WDT_EnableOverflowInt
 **			 ����WDT����ж�
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
void WDT_EnableOverflowInt(void);


/*****************************************************************************
 ** \brief	 WDT_DisableOverflowInt
 **			 �ر�WDT����ж�
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
void WDT_DisableOverflowInt(void);

/*****************************************************************************
 ** \brief	 WDT_GetOverflowIntFlag
 **			 ��ȡWDTʹ�ܲ������жϱ�־
 ** \param [in] none
 ** \return  1:ʹ���жϲ������ж� 0�����ж�
 ** \note	 
*****************************************************************************/
uint32_t WDT_GetOverflowIntFlag(void);
/*****************************************************************************
 ** \brief	 WDT_ClearOverflowIntFlag
 **			 ���WDT�жϱ�־�����¼��س�ֵ
 ** \param [in] none
 ** \return none
 ** \note	 
*****************************************************************************/
void WDT_ClearOverflowIntFlag(void);

/*****************************************************************************
 ** \brief	 WDT_GetTimingData
 **			 ��ȡWDT����ֵ
 ** \param [in] none
 ** \return 32bit value
 ** \note	 
*****************************************************************************/
uint32_t WDT_GetTimingData(void);

/*****************************************************************************
 ** \brief	 WDT_ClearWDT
 **			 ιWDT
 ** \param [in] none
 ** \return none
 ** \note	 
*****************************************************************************/
void WDT_ClearWDT(void);

/***********************************************************************************************************************
* Function Name: WDT_Restart
* @brief  This function restarts the watchdog timer.
* @param  None
* @return None
***********************************************************************************************************************/
void WDT_Restart(void);
	
#ifdef __cplusplus
}
#endif

#endif /* __WDT_H__ */

