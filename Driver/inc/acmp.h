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
/** \file acmp.h
**
** History:
** 
*****************************************************************************/
#ifndef _ACMP_H_
#define _ACMP_H_

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
 **ACMP Channel
-----------------------------------------------------------------------------*/
#define ACMP0 					(0x00)			/* ACMP channel 0 number */
#define ACMP1					(0x01)			/* ACMP channel 1 number */
/*----------------------------------------------------------------------------
 **ACMP 输出滤波
-----------------------------------------------------------------------------*/
#define ACMP_NFCLK_1_TSYS		(0x00UL)		/*(0~1)*Tsys*/
#define ACMP_NFCLK_2_TSYS		(0x01UL)		/*(1~2)*Tsys*/
#define ACMP_NFCLK_3_TSYS		(0x02UL)		/*(2~3)*Tsys*/
#define ACMP_NGCLK_5_TSYS		(0x03UL)		/*(4~5)*Tsys*/
#define ACMP_NFCLK_9_TSYS		(0x04UL)		/*(8~9)*Tsys*/
#define ACMP_NFCLK_17_TSYS		(0x05UL)		/*(16~17)*Tsys*/
#define ACMP_NFCLK_33_TSYS		(0x06UL)		/*(32~33)*Tsys*/
#define ACMP_NGCLK_65_TSYS		(0x07UL)		/*(64~65)*Tsys*/
#define ACMP_NFCLK_129_TSYS		(0x08UL)		/*(128~129)*Tsys*/
#define ACMP_NFCLK_257_TSYS		(0x09UL)		/*(256~257)*Tsys*/
#define ACMP_NFCLK_513_TSYS		(0x0aUL)		/*(512~513)*Tsys*/
/*----------------------------------------------------------------------------
 **ACMP Positive Sel
-----------------------------------------------------------------------------*/
#define ACMP_POSSEL_P0			(0x00UL)		/*ACMP P0*/
#define ACMP_POSSEL_P1			(0x01UL)		/*ACMP P1*/
#define ACMP_POSSEL_P2			(0x02UL)		/*ACMP P2*/
/*For ACMP0*/
#define ACMP_POSSEL_0PGA1P		(0x03UL)		/*ACMP */
#define ACMP_POSSEL_0PGA1O		(0x04UL)		/*ACMP Sel PGA1 Out*/
#define ACMP_POSSEL_0PGA2P		(0x05UL)		/*ACMP Sel PGA1P Out*/
#define ACMP_POSSEL_0PGA2O		(0x06UL)		/*ACMP Sel PGA2 Out*/
/*For ACMP1*/
#define ACMP_POSSEL_1P3			(0x03UL)		/*ACMP P3*/
#define ACMP_POSSEL_1PGA0P		(0x03UL)		/*ACMP PGA0 Out*/
#define ACMP_POSSEL_1PGA0O		(0x04UL)		/*ACMP Sel PGA0 Out*/

/*----------------------------------------------------------------------------
 **ACMP Negative Sel
-----------------------------------------------------------------------------*/
#define ACMP_NEGSEL_BEF_N   	(0x02UL)		/*Inner signal*/ //ACMP1 has no this 
#define ACMP_NEGSEL_DAC_O		(0x01UL)		/*DAC Output*/
#define ACMP_NEGSEL_N			(0x00UL)		/*ACMP N*/

/*----------------------------------------------------------------------------
 **ACMP  事件和中断产生模式
-----------------------------------------------------------------------------*/
#define ACMP_EVENT_INT_RISING		(0x0UL)			/*ACMPx输出上升沿(0->1跳变)产生*/
#define ACMP_EVENT_INT_FALLING		(0x1UL )			/*ACMPx输出下降沿(1->0跳变)产生*/
#define ACMP_EVENT_INT_BOTH_EDGE	(0x2UL)				/*ACMPx输出从0->1的跳变或从1->0的跳变产生*/

/*----------------------------------------------------------------------------
 **ACMP 迟滞模式
------------------------------------------------------------------------------*/
#define ACMP_HYS_POS			(0x01UL)			/*正迟滞*/
#define ACMP_HYS_NEG			(0x02UL)			/*负迟滞*/
#define ACMP_HYS_BOUTH			(0x03UL)			/*双边迟滞*/

/*----------------------------------------------------------------------------
 **ACMP 迟滞配置
------------------------------------------------------------------------------*/
#define ACMP_HYS_S_00			(0x00UL)			/*0mV迟滞*/
#define ACMP_HYS_S_10			(0x01UL)			/*10mV迟滞*/
#define ACMP_HYS_S_20			(0x02UL)			/*20mV迟滞*/
#define ACMP_HYS_S_60			(0x03UL)			/*60mV迟滞*/

/*----------------------------------------------------------------------------
 **ACMP 极性配置
------------------------------------------------------------------------------*/
#define ACMP_POL_Pos			(0x00UL)			/*正常输出*/
#define ACMP_POL_Neg			(0x01UL)			/*反相输出*/

/*----------------------------------------------------------------------------
 **ACMP 写保护
------------------------------------------------------------------------------*/
#define ACMP_WRITE_KEY 			(0x55UL)
				  										 
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
/*----------------------------------------------------------------------------
 **ACMP 负端基准电压 VDD / BG 系数 K
-----------------------------------------------------------------------------*/
typedef enum V_B_Coefficient{
	V_B_COF_2 =0, 
	V_B_COF_3 =1, 
	V_B_COF_4 =2, 
	V_B_COF_5 =3, 
	V_B_COF_6 =4,
	V_B_COF_7 =5,    
	V_B_COF_8 =6, 
	V_B_COF_9 =7, 
	V_B_COF_10 =8, 
	V_B_COF_11 =9,
	V_B_COF_12 =10,  
	V_B_COF_13 =11, 
	V_B_COF_14 =12, 
	V_B_COF_15 =13, 
	V_B_COF_16 =14,
	V_B_COF_17 =15,
}V_B_Coefficient_T;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/

/******************************************************************************
 ** \brief	 ACMP_ConfigPositive
 **			 设置比较器正端输入
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \param [in] Common PositiveSource:  (1)ACMP_POSSEL_P0		:CnP0
 **								 (2)ACMP_POSSEL_P1		:CnP1
 **								 (3)ACMP_POSSEL_P2		:CnP2
 **				for ACMP0:       (4)ACMP_POSSEL_0PGA1P		
 **								 (5)ACMP_POSSEL_0PGA1O		
 **								 (6)ACMP_POSSEL_0PGA2P		
 **								 (7)ACMP_POSSEL_0PGA2O		
 **				for ACMP1:       (4)ACMP_POSSEL_1P3			
 **								 (5)ACMP_POSSEL_PGA0O	 
 ** \return  none
 ** \note   
*****************************************************************************/
void ACMP_ConfigPositive( uint32_t ACMPx, uint32_t PositiveSource);

/*****************************************************************************
 ** \brief	 ACMP_ConfigNegative
 **			 设置比较器负端输入
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 **				NegativeSource:  (1)ACMP_NEGSEL_N		:CnN  ACMP N输入端口			 
 **								 (2)ACMP_NEGSEL_DAC_O	:DAC 输出电压	
 **								 (3)ACMP_NEGSEL_BEF_N：	 比较器内部信号
 ** \return  none
 ** \note   
*****************************************************************************/
void ACMP_ConfigNegative( uint32_t ACMPx, uint32_t NegativeSource);

/*****************************************************************************
 ** \brief	 ACMP_EnableEventOut
 **			 开启比较器事件输出
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
 *****************************************************************************/
void ACMP_EnableEventOut( uint32_t ACMPx);

/*****************************************************************************
 ** \brief	 ACMP_ConfigEventAndIntMode
 **			 设置事件与中断产生模式
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 **				EventAndIntMode: 	(1)ACMP_EVENT_INT_RISING 		: ACMPx的输出上升沿触发
 **				 			 		(2)ACMP_EVENT_INT_FALLING 		: ACMPx的输出下降沿触发
 **						     		(3)ACMP_EVENT_INT_BOTH_EDGE 	: ACMPx的输出从0->1的跳变或从1->0的跳变触发
 ** \return  none
 ** \note   
 *****************************************************************************/

void ACMP_ConfigEventAndIntMode( uint32_t ACMPx, uint32_t EventAndIntMode);

/*****************************************************************************
 ** \brief	 ACMP_DisableEventOut
 **			 关闭比较器事件输出
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
 *****************************************************************************/
void ACMP_DisableEventOut( uint32_t ACMPx);

/******************************************************************************
 ** \brief	 ACMP_EnableInt
 **			 开启ACMP中断
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
 *****************************************************************************/
void ACMP_EnableInt(uint32_t ACMPx);
/*****************************************************************************
 ** \brief	 ACMP_DisableInt
 **			 关闭ACMP中断
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
*****************************************************************************/
void ACMP_DisableInt( uint32_t ACMPx);
/*****************************************************************************
 ** \brief	 ACMP_GetIntFlag
 **			 获取ACMP已使能中断标志
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  0: 无中断产生 ， 1：中断使能并产生中断
 ** \note   
******************************************************************************/
uint32_t ACMP_GetIntFlag( uint32_t ACMPx);
/*****************************************************************************
 ** \brief	 ACMP_ClearIntFlag
 **			 清ACMP中断
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
******************************************************************************/
void ACMP_ClearIntFlag( uint32_t ACMPx);

/*****************************************************************************
 ** \brief	 ACMP_Start
 **			 开启ACMP
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
******************************************************************************/
void ACMP_Start( uint32_t ACMPx);

/*****************************************************************************
 ** \brief	 ACMP_Stop
 **			 关闭ACMP
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
**********************************************************************?******/
void ACMP_Stop( uint32_t ACMPx);

/*****************************************************************************
 ** \brief	 ACMP0_CenterAliagn_Enable
 **			 开启ACMP0的中心对齐
 ** \param [in] none
 ** \return  none
 ** \note   
******************************************************************************/
void ACMP0_CenterAliagn_Enable(void);

/*****************************************************************************
 ** \brief	 ACMP0_CenterAliagn_Disable
 **			 关闭ACMP0的中心对齐
 ** \param [in] none
 ** \return  none
 ** \note   
**********************************************************************?******/
void ACMP0_CenterAliagn_Disable(void);
	
/*****************************************************************************
 ** \brief	 ACMPOut_Enable
 **			 使能ACMP输出
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
******************************************************************************/
void ACMPOut_Enable( uint32_t ACMPx);

/*****************************************************************************
 ** \brief	 ACMPOut_Disable
 **			 关闭ACMP输出
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
**********************************************************************?******/
void ACMPOut_Disable( uint32_t ACMPx);
	
/*****************************************************************************
 ** \brief	 ACMP_GetResult
 **			 获取比较器值
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  0/1
 ** \note   
 *****************************************************************************/
uint32_t ACMP_GetResult( uint32_t ACMPx);


/*****************************************************************************
 ** \brief	 ACMP_EnableHYS
 **			 配置比较器迟滞模式
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 **				HYSSelect	:(1)ACMP_HYS_POS	: 正迟滞
 **							 (2)ACMP_HYS_NEG	: 负迟滞
 **							 (3)ACMP_HYS_BOUTH	: 双边迟滞
 **				HYSValue 	:(1)ACMP_HYS_S_10	: 10mV迟滞
 **							 (2)ACMP_HYS_S_20	：20mV迟滞
 **							 (3)ACMP_HYS_S_60	：60mV迟滞 
 ** \return  none
 ** \note  
*****************************************************************************/
void ACMP_EnableHYS( uint32_t ACMPx ,uint32_t HYSSelect, uint32_t HYSValue);

/*****************************************************************************
 ** \brief	 ACMP_DisableHYS
 **			 关闭比较器迟滞模式
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note  
*****************************************************************************/
void ACMP_DisableHYS( uint32_t ACMPx);

/*****************************************************************************
 ** \brief	 ACMP_Filter_Config
 **			    设置滤波开关和滤波时间
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \param [in] FiltSta: ENABLE ,DISABLE
 ** \param [in] FlitTim: 滤波时间选择
 *                  ACMP_NFCLK_1_TSYS：(0~1)*Tsys
 *                       .......
 *                  ACMP_NFCLK_513_TSYS:(512~513)*Tsys
 ** \return  none
 ** \note  
*****************************************************************************/
void ACMP_Filter_Config( uint32_t ACMPx,FunctionalState FiltSta,uint8_t FlitTim);


/*****************************************************************************
 ** \brief	 ACMP_Polarity_Config
 **			    设置输出的极性
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \param [in] PolaritySet:  
 *               ACMP_POL_Pos：正常输出
 *               ACMP_POL_Neg：反相输出
 ** \return  none
 ** \note  
*****************************************************************************/
void ACMP_Polarity_Config( uint32_t ACMPx,uint8_t PolaritySet);
	
#ifdef __cplusplus
}
#endif

#endif /* _ACMP_H_ */

