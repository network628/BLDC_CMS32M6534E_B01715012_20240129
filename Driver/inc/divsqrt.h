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
/** \file divsqrt.h
**
** History:
** 
*****************************************************************************/
#ifndef __DIVSQRT_H_
#define __DIVSQRT_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "CMS32M6510.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
/*****************************************************************************
 ** \brief	DIVSQRT_DIV_IS_0
 **			�жϳ����Ƿ�Ϊ0
 ** \param [in] none
 ** \return  1������Ϊ0  0��������Ϊ0
 ** \note	 
*****************************************************************************/
#define	 DIVSQRT_DIV_IS_0()	(((DIVSQRT->CON & DIVSQRT_CON_DIVBY0_Msk)?1:0))
/*****************************************************************************
 ** \brief	DIVSQRT_IS_IDLE
 **			�жϳ������ǿ���
 ** \param [in] none
 ** \return  1������  0����������
 ** \note	 
*****************************************************************************/
#define	 DIVSQRT_IS_IDLE()	(((DIVSQRT->CON & DIVSQRT_CON_READY_Msk)?1:0))

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
 ** \brief	DIVSQRT_Div
 **			ʵ�� "/" (ȡ��)
 ** \param [in] x:������
 **				y:����
 ** \return  ����
 ** \note	 
*****************************************************************************/
int32_t DIVSQRT_Div(int32_t x, int32_t y);
	  
/*****************************************************************************
 ** \brief	DIVSQRT_Mod
 **			ʵ�� "%" (ȡ��)
 ** \param [in] x:������
 **				y:����
 ** \return  ����
 ** \note	 
*****************************************************************************/
int32_t DIVSQRT_Mod(int32_t x, int32_t y);

/*****************************************************************************
 ** \brief	DIVSQRT_GetDiv
 **			ֱ�Ӷ����������е���
 ** \param   none
 ** \return  ��
 ** \note	 
*****************************************************************************/
int32_t DIVSQRT_GetDiv(void);

/*****************************************************************************
 ** \brief	DIVSQRT_GetMod
 **			ֱ�Ӷ����������е���
 ** \param   none
 ** \return  ����
 ** \note	 
*****************************************************************************/
int32_t DIVSQRT_GetMod(void);
	
/*****************************************************************************
 ** \brief	DIVSQRT_Sqrt
 **			����
 ** \param [in] x:����������		
 ** \return  none
 ** \note	 
*****************************************************************************/
uint32_t DIVSQRT_Sqrt(uint32_t x);

/*****************************************************************************
 ** \brief	DIVSQRT_EnableDIVMode
 **			ʹ�ܳ�������
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DIVSQRT_EnableDIVMode(void);
/*****************************************************************************
 ** \brief	DIVSQRT_EnableSQRTMode
 **			ʹ�ܿ�������
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DIVSQRT_EnableSQRTMode(void);
/*****************************************************************************
 ** \brief	DIVSQRT_EnableSingedMode
 **			ʹ���з��ų���������
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DIVSQRT_EnableSingedMode(void);
/*****************************************************************************
 ** \brief	DIVSQRT_EnableUnsingedMode
 **			ʹ���޷��ų���������
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DIVSQRT_EnableUnsingedMode(void);


#ifdef __cplusplus
}
#endif

#endif /* __DIVSQRT_H_*/

