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
/** \file divsqrt.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "divsqrt.h"
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
 ** \brief	DIVSQRT_Div
 **			ʵ�� "/" (ȡ��) 
 ** \param [in] x:������
 **				y:����
 ** \return  ��
 ** \note	 
*****************************************************************************/
int32_t DIVSQRT_Div(int32_t x, int32_t y)
{
	DIVSQRT->ALUA = x;
	DIVSQRT->ALUB = y;
	while(!DIVSQRT_IS_IDLE())
	{
		;
	}	
    return (DIVSQRT->RES0);   	
}
	  
/*****************************************************************************
 ** \brief	DIVSQRT_Mod
 **			ʵ�� "%" (ȡ��)
 ** \param [in] x:������
 **				y:����
 ** \return  ����
 ** \note	 
*****************************************************************************/
int32_t DIVSQRT_Mod(int32_t x, int32_t y)
{
	DIVSQRT->ALUA = x;
	DIVSQRT->ALUB = y;
	while(!DIVSQRT_IS_IDLE())
	{
		;
	}	
    return (DIVSQRT->RES1);   	  
}

/*****************************************************************************
 ** \brief	DIVSQRT_GetDiv
 **			ֱ�Ӷ����������е���
 ** \param   none
 ** \return  ��
 ** \note	 
*****************************************************************************/
int32_t DIVSQRT_GetDiv(void)
{
	 return (DIVSQRT->RES0); 
}

/*****************************************************************************
 ** \brief	DIVSQRT_GetMod
 **			ֱ�Ӷ����������е���
 ** \param   none
 ** \return  ����
 ** \note	 
*****************************************************************************/
int32_t DIVSQRT_GetMod(void)
{
	 return (DIVSQRT->RES1); 
}

/*****************************************************************************
 ** \brief	DIVSQRT_Sqrt
 **			����
 ** \param [in] x:����������		
 ** \return  none
 ** \note	 
*****************************************************************************/
uint32_t DIVSQRT_Sqrt(uint32_t x)
{
	DIVSQRT->ALUB = x;
	while(!DIVSQRT_IS_IDLE())
	{
		;
	}	
    return (0xFFFF & (DIVSQRT->RES0)); 
}

/*****************************************************************************
 ** \brief	DIVSQRT_EnableDIVMode
 **			ʹ�ܳ�������
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DIVSQRT_EnableDIVMode(void)
{
	DIVSQRT->CON &= ~(DIVSQRT_CON_MODE_Msk);	
}
/*****************************************************************************
 ** \brief	DIVSQRT_EnableSQRTMode
 **			ʹ�ܿ�������
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DIVSQRT_EnableSQRTMode(void)
{
	DIVSQRT->CON |= DIVSQRT_CON_MODE_Msk;	
}

/*****************************************************************************
 ** \brief	DIVSQRT_EnableSingedMode
 **			ʹ���з��ų���������
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DIVSQRT_EnableSingedMode(void)
{
	DIVSQRT->CON |= DIVSQRT_CON_SIGN_Msk;
}
/*****************************************************************************
 ** \brief	HWDIV_EnableUnsingedMode
 **			ʹ���޷��ų���������
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DIVSQRT_EnableUnsingedMode(void)
{
	DIVSQRT->CON &= ~(DIVSQRT_CON_SIGN_Msk);
}



