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
/** \file pga.h
**
** History:
** 
*****************************************************************************/
#ifndef __PGA_H_
#define __PGA_H_

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
typedef enum {
	PGA0x = 0,
	PGA1x = 1,
	PGA2x = 2,	
}PGA_TypeDef;

typedef enum {
	VrefHalf = 0,
	PGA0BG = 1,	
}PGAVref_TypeDef;

typedef enum {
	PgaSingle = 0,  //单端模式
	PgaDiffer = 1,	//差分模式
}PGAMode_TypeDef;

#define PGAUNLOCK		(0x55UL)
#define PGALOCK		  (0x00UL)
/*----------------------------------------------------------------------------
 **PGA模式
-----------------------------------------------------------------------------*/
#define PGA_MS_PGA		(0x0UL)
#define PGA_MS_OP		(0x1UL)

/*----------------------------------------------------------------------------
 **PGA	增益选择
-----------------------------------------------------------------------------*/
#define		PGA_GAIN_1			(0x00UL)					/*设置增益为1x*/
#define		PGA_GAIN_2			(0x01UL)					/*设置增益为2x*/
#define		PGA_GAIN_2d5		(0x02UL)					/*设置增益为2.5x*/
#define		PGA_GAIN_5			(0x03UL)					/*设置增益为5x*/			
#define		PGA_GAIN_7d5		(0x04UL)					/*设置增益为7.5x*/
#define		PGA_GAIN_10			(0x05UL)					/*设置增益为10x*/	
#define		PGA_GAIN_15			(0x06UL)					/*设置增益为15x*/
/*----------------------------------------------------------------------------
 **PGA	反馈电阻接地端选择
-----------------------------------------------------------------------------*/
#define		PGA_R_None			(0x00UL)					/*接输出不接电阻*/
#define		PGA_R_10K			  (0x01UL)	        /*输出串联10K电阻*/
			
/*----------------------------------------------------------------------------
 **PGA	正端
-----------------------------------------------------------------------------*/
#define		PGA_POSSEL_P0		(0x00UL)					/*接到PGA P0*/
#define		PGA_POSSEL_P1		(0x01UL<<PGA_CON_PS_Pos)	/*接到PGA P1*/
#define		PGA_POSSEL_P2		(0x02UL<<PGA_CON_PS_Pos)	/*接到PGA P2*/
#define		PGA_POSSEL_P3		(0x03UL<<PGA_CON_PS_Pos)	/*接到PGA P3*/							
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
/********************PGA PIN**************************************************
P00: A0P
P01: A0GND
P02: A0O

P24: A1P
P25: A1GND
P04: A12O

P26: A2P
P27: A2GND
P04: A12O
******************************************************************************/
/*****************************************************************************
 ** \brief	 OPA_ConfigGain
 **			 设置PGA增益
 ** \param [in] pga :PGA0 、PGA1、PGA2
 **				Gain:  (1)	PGA_GAIN_1		
 **					     (2)  PGA_GAIN_2	
 **					     (3)  PGA_GAIN_2d5
 **					     (4)  PGA_GAIN_5	
 **					     (5)  PGA_GAIN_7d5
 **					     (6)  PGA_GAIN_10	
 **					     (6)  PGA_GAIN_15	
 ** \return  none
 ** \note    
 *****************************************************************************/
void PGA_ConfigGain(PGA_TypeDef PGAUnit, uint32_t Gain);

/*****************************************************************************
 ** \brief	 PGA_ConfigResistorPAD
 **			 设置PGA反馈电阻接地端
 ** \param [in] pga :PGA0 、PGA1
 **				GndMode:	(1)PGA_R_GND
 **							(2)PGA_R_PIN
 ** \return  none	
 ** \note    
 *****************************************************************************/
void PGA0_ConfigResistorPAD(uint32_t GndMode);

/*****************************************************************************
 ** \brief	 PGA_EnableOutput
 **			 开启PGA输出
 ** \param [in] pga :PGA0 、PGA1
 ** \return  none	
 ** \note    
 *****************************************************************************/
void PGA_EnableOutput(PGA_TypeDef PGAUnit);
/*****************************************************************************
 ** \brief	 PGA_DisableOutput
 **			 关闭PGA输出
 ** \param [in] pga :PGA0 、PGA1
 ** \return  none	
 ** \note    
 *****************************************************************************/
void PGA_DisableOutput(PGA_TypeDef PGAUnit);

/*****************************************************************************
 ** \brief	 PGA0_Filter
 **			 PGA0滤波输出设置
 ** \param [in] FilterStat :
**                  ENABLE:使能
**                  DISABLE:不使能
 ** \return  none	
 ** \note    
 *****************************************************************************/
void PGA0_Filter(FunctionalState FilterStat);

/*****************************************************************************
 ** \brief	 PGA_VrefCtrl
 **			 PGA0参考电压设置
 ** \param [in] pga :PGA0x 、PGA1x、PGA2x
 ** \param [in] VrefStat :
**                  VrefHalf:VREF/2
**                  PGA0BG:BG(0.8V)
 ** \return  none	
 ** \note    
 *****************************************************************************/
void PGA_VrefCtrl(PGA_TypeDef PGAUnit,PGAVref_TypeDef VrefStat);

/*****************************************************************************
 ** \brief	 PGA_Start
 **			 开启PGA
 ** \param [in] pga :PGA0 、PGA1
 ** \return  none	
 ** \note    
 *****************************************************************************/
void PGA_Start(PGA_TypeDef PGAUnit);

/*****************************************************************************
 ** \brief	 PGA_Stop
 **			 关闭PGA
 ** \param [in] pga :PGA0 、PGA1
 ** \return  none	
 ** \note    
 *****************************************************************************/
void PGA_Stop(PGA_TypeDef PGAUnit);

/*****************************************************************************
 ** \brief	 PGA_ModeSet
 **			 PGA工作模式设置
 ** \param [in] pga :PGA0x 、PGA1x、PGA2x
 ** \param [in] PgaMode: PgaSingle、PgaDiffer
 ** \return  none	
 ** \note    
 *****************************************************************************/
void PGA_ModeSet(PGA_TypeDef PGAUnit,PGAMode_TypeDef PgaMode);
	
#ifdef __cplusplus
}
#endif

#endif /* ____ */

