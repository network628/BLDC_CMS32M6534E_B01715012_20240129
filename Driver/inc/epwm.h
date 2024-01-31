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
/** \file epwm.h
**
** History:
** 
*****************************************************************************/
#ifndef __EPWM_H_
#define __EPWM_H_

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
 **EPWM 通道
-----------------------------------------------------------------------------*/
#define	EPWM0 			(0x00UL)
#define	EPWM1 			(0x01UL)
#define	EPWM2 			(0x02UL)
#define	EPWM3 			(0x03UL)
#define	EPWM4 			(0x04UL)
#define	EPWM5 			(0x05UL)

#define	EPWM_CH_0_MSK 		(0x01UL)
#define	EPWM_CH_1_MSK 		(0x02UL)
#define	EPWM_CH_2_MSK 		(0x04UL)
#define	EPWM_CH_3_MSK 		(0x08UL)
#define EPWM_CH_4_MSK 		(0x10UL)
#define	EPWM_CH_5_MSK		(0x20UL)

/*----------------------------------------------------------------------------
 **EPWM 重映射
-----------------------------------------------------------------------------*/
#define	IPG0			(0x00UL)
#define	IPG1			(0x01UL)
#define	IPG2			(0x02UL)
#define	IPG3			(0x03UL)
#define	IPG4			(0x04UL)
#define	IPG5			(0x05UL)
#define EPWM_REMAP_ENABLE	(0xAAUL)
/*----------------------------------------------------------------------------
 **EPWM 模式
-----------------------------------------------------------------------------*/
/*------EPWM计数模式---------------------------------------------------------*/
#define EPWM_COUNT_UP_DOWN			(0x01UL<<EPWM_CON_CNTTYPE_Pos)  	/*EPWM计数（COUNT）模式：上下计数（中心对齐）模式*/
#define EPWM_COUNT_DOWN				(0x00UL)  							/*EPWM计数（COUNT）模式：向下计数（边沿对齐）模式*/
/*------EPWM输出比较单元(OCU)模式--------------------------------------------*/
#define EPWM_OCU_ASYMMETRIC			(0x01UL<<EPWM_CON_ASYMEN_Pos)  		/*EPWM输出比较单元（OCU）模式； 非对称模式*/
#define EPWM_OCU_SYMMETRIC			(0x00UL) 					 		/*EPWM输出比较单元（OCU）模式； 对称模式*/
/*------EPWM波形发生器单元(WFG)模式------------------------------------------*/
#define EPWM_WFG_INDEPENDENT		(0x0UL)  							/*EPWM波形发生器（WFG）模式：单个（独立）模式*/
#define EPWM_WFG_COMPLEMENTARYK 	(0x1UL<<EPWM_CON_MODE_Pos) 			/*EPWM波形发生器（WFG）模式：互补模式*/
#define EPWM_WFG_SYNCHRONIZED		(0x2UL<<EPWM_CON_MODE_Pos) 			/*EPWM波形发生器（WFG）模式：同步模式*/
/*------EPWM输出控制单元(OC)模式---------------------------------------------*/
#define EPWM_OC_GROUP				(0x01UL<<EPWM_CON_GROUNPEN_Pos)  	/*EPWM输出控制（OC）模式： 组控模式*/
#define EPWM_OC_INDEPENDENT			(0x00UL) 						 	/*EPWM输出控制（OC）模式： 通道各自独立控制*/

/*----------------------------------------------------------------------------
 **EPWM 时钟
-----------------------------------------------------------------------------*/
#define 	EPWM_CLK_DIV_1				(0x5UL)		
#define 	EPWM_CLK_DIV_2				(0x4UL)		
#define	 	EPWM_CLK_DIV_4				(0x0UL)		
#define	 	EPWM_CLK_DIV_8				(0x1UL)		
#define 	EPWM_CLK_DIV_16				(0x2UL)		
#define 	EPWM_CLK_DIV_32				(0x3UL)		

/*----------------------------------------------------------------------------
 **EPWM 周期、占空比加载方式及周期中断、零点中断产生方式
-----------------------------------------------------------------------------*/
#define		EPWM_EACH_PERIOD_ZERO		(0x0UL)		/*每个零点与周期点*/
#define		EPWM_EACH_ZERO				(0x1UL)		/*每个零点*/
#define		EPWM_FIRST_ZERO_NEXT_PERIOD	(0x2UL)		/*第一个零点与下一个周期点*/
#define		EPWM_EVERY_TWO_ZERO			(0x3UL)		/*每两个零点*/

/*----------------------------------------------------------------------------
 **EPWM 事件触发加载
-----------------------------------------------------------------------------*/
#define		EPWM_EVENT_ACMP0		(0x00UL)		/*ACMP0 事件产生触发LOADENn=1*/
#define		EPWM_EVENT_ACMP1		(0x01UL)		/*ACMP1 事件产生触发LOADENn=1*/
#define		EPWM_EVENT_HALL			(0x02UL)		/*HALL 状态变化触发LOADENn=1*/ 

/*----------------------------------------------------------------------------
 **EPWM 掩码加载
-----------------------------------------------------------------------------*/
#define		EPWM_MASK_LOAD_EPWM0	(0x0UL<<EPWM_POEN_MASKLS_Pos)		/*在EPWM0 的加载点加载掩码数据*/
#define		EPWM_MASK_LOAD_EPWM1	(0x1UL<<EPWM_POEN_MASKLS_Pos)		/*在EPWM1 的加载点加载掩码数据*/
#define		EPWM_MASK_LOAD_EPWM2	(0x2UL<<EPWM_POEN_MASKLS_Pos)		/*在EPWM2 的加载点加载掩码数据*/
#define		EPWM_MASK_LOAD_EPWM3	(0x3UL<<EPWM_POEN_MASKLS_Pos)		/*在EPWM3 的加载点加载掩码数据*/
#define		EPWM_MASK_LOAD_EPWM4	(0x4UL<<EPWM_POEN_MASKLS_Pos)		/*在EPWM4 的加载点加载掩码数据*/		
#define		EPWM_MASK_LOAD_EPWM5	(0x5UL<<EPWM_POEN_MASKLS_Pos)		/*在EPWM5 的加载点加载掩码数据*/
/*----------------------------------------------------------------------------
 **EPWM 掩码数据预缓存
-----------------------------------------------------------------------------*/
#define		EPWM_MASK_BUF0			(0x0UL)		
#define		EPWM_MASK_BUF1			(0x1UL)	
#define		EPWM_MASK_BUF2			(0x2UL)	
#define		EPWM_MASK_BUF3			(0x3UL)	
#define		EPWM_MASK_BUF4			(0x4UL)	
#define		EPWM_MASK_BUF5			(0x5UL)	
#define		EPWM_MASK_BUF6			(0x6UL)	
#define		EPWM_MASK_BUF7			(0x7UL)	

/*----------------------------------------------------------------------------
 **EPWM 计数比较触发ADC采样
-----------------------------------------------------------------------------*/
#define		EPWM_CMPTG_FALLING		(0x00UL)		/*中心对称计数时：向下计数触发*/
#define		EPWM_CMPTG_RISING		(0x01UL)		/*中心对称计数时：向上计数触发*/

#define		EPWM_CMPTG_0			(0x00UL)		/*计数比较通道0 */
#define		EPWM_CMPTG_1			(0x01UL)		/*计数比较通道1 */

#define		EPWM_CMPTG_EPWM0		(0x00UL)		/*比较器选择EPWM0的计数器*/
#define		EPWM_CMPTG_EPWM1		(0x01UL)		/*比较器选择EPWM1的计数器*/
#define		EPWM_CMPTG_EPWM2		(0x02UL)		/*比较器选择EPWM2的计数器*/
#define		EPWM_CMPTG_EPWM3		(0x03UL)		/*比较器选择EPWM3的计数器*/
#define		EPWM_CMPTG_EPWM4		(0x04UL)		/*比较器选择EPWM4的计数器*/
#define		EPWM_CMPTG_EPWM5		(0x05UL)		/*比较器选择EPWM5的计数器*/

/*----------------------------------------------------------------------------
 **EPWM 故障保护（刹车）模式
-----------------------------------------------------------------------------*/
#define 	EPWM_BRK_STOP			(0x00UL<<EPWM_BRKCTL_BRKMS_Pos)			/*停止模式*/
#define 	EPWM_BRK_SUSPEND		(0x01UL<<EPWM_BRKCTL_BRKMS_Pos)			/*暂停模式*/
#define 	EPWM_BRK_RECOVER		(0x02UL<<EPWM_BRKCTL_BRKMS_Pos)			/*恢复模式*/
#define 	EPWM_BRK_DELAY_RECOVER	(0x03UL<<EPWM_BRKCTL_BRKMS_Pos)			/*延时恢复模式*/

/*----------------------------------------------------------------------------
 **EPWM  刹车
-----------------------------------------------------------------------------*/
#define 	EPWM_BRK_SOFT			(0x00)								/*软件刹车*/
#define 	EPWM_BRK_EXTLE			(EPWM_BRKCTL_EXTBRKLE_Msk)			/*外部硬件电平刹车*/
#define 	EPWM_BRK_EXTEE			(EPWM_BRKCTL_EXTBRKEE_Msk)			/*外部硬件边沿刹车*/
#define 	EPWM_BRK_ADCBCMP0		(EPWM_BRKCTL_ADCBPM0BKEN_Msk)		/*ADCB比较器0刹车*/
#define 	EPWM_BRK_ADCBCMP1		(EPWM_BRKCTL_ADCBPM1BKEN_Msk)		/*ADCB比较器1刹车*/	
#define 	EPWM_BRK_ADCACMP0		(EPWM_BRKCTL_ADCAPM0BKEN_Msk)		/*ADCA比较器0刹车*/
#define 	EPWM_BRK_ACMP0LE		(EPWM_BRKCTL_ACMP0BKLE_Msk)			/*模拟比较器0输出电平刹车*/
#define 	EPWM_BRK_ACMP1LE		(EPWM_BRKCTL_ACMP1BKLE_Msk)			/*模拟比较器1输出电平刹车*/	
#define 	EPWM_BRK_ACMP0EE		(EPWM_BRKCTL_ACMP0BKEN_Msk)			/*模拟比较器0事件刹车*/
#define 	EPWM_BRK_ACMP1EE		(EPWM_BRKCTL_ACMP1BKEN_Msk)			/*模拟比较器1事件刹车*/	


/*----------------------------------------------------------------------------
 **EPWM  刹车信号滤波
-----------------------------------------------------------------------------*/
#define     EPWM_BRK_FILCLK_1			(0x0UL)			/*Filter Clk = PCLK*(0~1)*/	
#define     EPWM_BRK_FILCLK_2			(0x1UL)			/*Filter Clk = PCLK*(1~2)*/	
#define     EPWM_BRK_FILCLK_3			(0x2UL)			/*Filter Clk = PCLK*(2~3)*/	
#define     EPWM_BRK_FILCLK_5			(0x3UL)			/*Filter Clk = PCLK*(4~5)*/	
#define     EPWM_BRK_FILCLK_9			(0x4UL)			/*Filter Clk = PCLK*(8~9)*/	
#define     EPWM_BRK_FILCLK_17			(0x5UL)			/*Filter Clk = PCLK*(16~17)*/	
#define     EPWM_BRK_FILCLK_25			(0x6UL)			/*Filter Clk = PCLK*(24~25)*/	
#define     EPWM_BRK_FILCLK_33			(0x7UL)			/*Filter Clk = PCLK*(32~33)*/	
#define     EPWM_BRK_FILCLK_49			(0x8UL)			/*Filter Clk = PCLK*(48~49)*/	
#define     EPWM_BRK_FILCLK_65			(0x9UL)			/*Filter Clk = PCLK*(64~65)*/	
#define     EPWM_BRK_FILCLK_81			(0xAUL)			/*Filter Clk = PCLK*(80~81)*/	
#define     EPWM_BRK_FILCLK_97			(0xBUL)			/*Filter Clk = PCLK*(96~97)*/	
#define     EPWM_BRK_FILCLK_113			(0xCUL)			/*Filter Clk = PCLK*(112~113)*/		


/*----------------------------------------------------------------------------
 **EPWM  外部硬件刹车
-----------------------------------------------------------------------------*/
#define		EPWM_EXTBK_LEVEL_HIGH	(0x01UL<<EPWM_BRKCTL_EXTBRKLS_Pos)		/*高电平刹车*/
#define		EPWM_EXTBK_LEVEL_LOW	(0x00<<EPWM_BRKCTL_EXTBRKLS_Pos)		/*低电平刹车*/
#define		EPWM_EXTBK_EDGE_FALLING	(0x00UL<<EPWM_BRKCTL_EXTBRKES_Pos)		/*下降沿刹车*/
#define		EPWM_EXTBK_EDGE_RISING	(0x01UL<<EPWM_BRKCTL_EXTBRKES_Pos)		/*上升沿刹车*/
/*----------------------------------------------------------------------------
 **EPWM  比较器输出电平刹车
-----------------------------------------------------------------------------*/
#define		EPWM_ACMP0BRK_LEVEL_HIGH	(0x01UL<<EPWM_BRKCTL_ACMP0BKLS_Pos)		/*比较器0输出高电平刹车*/
#define		EPWM_ACMP0BRK_LEVEL_LOW		(0x00UL<<EPWM_BRKCTL_ACMP0BKLS_Pos)		/*比较器0输出低电平刹车*/
#define		EPWM_ACMP1BRK_LEVEL_HIGH	(0x01UL<<EPWM_BRKCTL_ACMP1BKLS_Pos)		/*比较器1输出高电平刹车*/
#define		EPWM_ACMP1BRK_LEVEL_LOW		(0x00UL<<EPWM_BRKCTL_ACMP1BKLS_Pos)		/*比较器1输出低电平刹车*/

/*----------------------------------------------------------------------------
 **EPWM 故障刹车恢复加载
-----------------------------------------------------------------------------*/
#define		EPWM_BRK_LOAD_EPWM0		(0x0UL<<EPWM_BRKCTL_BRKRCS_Pos)		/*在EPWM0 的加载点*/
#define		EPWM_BRK_LOAD_EPWM1		(0x1UL<<EPWM_BRKCTL_BRKRCS_Pos)		/*在EPWM1 的加载点*/
#define		EPWM_BRK_LOAD_EPWM2		(0x2UL<<EPWM_BRKCTL_BRKRCS_Pos)		/*在EPWM2 的加载点*/
#define		EPWM_BRK_LOAD_EPWM3		(0x3UL<<EPWM_BRKCTL_BRKRCS_Pos)		/*在EPWM3 的加载点*/
#define		EPWM_BRK_LOAD_EPWM4		(0x4UL<<EPWM_BRKCTL_BRKRCS_Pos)		/*在EPWM4 的加载点*/		
#define		EPWM_BRK_LOAD_EPWM5		(0x5UL<<EPWM_BRKCTL_BRKRCS_Pos)		/*在EPWM5 的加载点*/

/*----------------------------------------------------------------------------
 **EPWM 写保护
-----------------------------------------------------------------------------*/
#define		EPWM_LOCK_P1A_WRITE_KEY		(0xAAUL)
#define		EPWM_LOCK_P1B_WRITE_KEY		(0x55UL)

/*****************************************************************************
 ** \brief	EPWM_GET_BRAKE_RIS_FLAG
 **			获取刹车中断源标志
 ** \param [in] none
 ** \return  1：产生中断  0:无中断
 ** \note	 
*****************************************************************************/
#define 	EPWM_GET_BRAKE_RIS_FLAG()		(((EPWM->RIS&EPWM_RIS_BRKIF_Msk)? 1:0))

/*****************************************************************************
 ** \brief	EPWM_GET_HALLERROR_RIS_FLAG
 **			获取Hall状态错误中断源标志
 ** \param [in] none
 ** \return  1：产生中断  0:无中断
 ** \note	 
*****************************************************************************/
#define 	EPWM_GET_HALLERROR_RIS_FLAG()		(((EPWM->RIS&EPWM_RIS_HALLIF_Msk)? 1:0))

/*****************************************************************************
 ** \brief	EPWM_GET_UPCMP_RIS_FLAG
 **			获取向上比较中断源标志
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 ** \return  1：产生中断  0:无中断
 ** \note	 
*****************************************************************************/
#define 	EPWM_GET_UPCMP_RIS_FLAG(EPWMn)		(((EPWM->RIS&(0x1<<(EPWMn+EPWM_RIS_UIFn_Pos))? 1:0)))
		
/*****************************************************************************
 ** \brief	EPWM_GET_DOWNCMP_RIS_FLAG
 **			获取向下比较中断源标志
 ** \param [in]EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 ** \return  1：产生中断  0:无中断
 ** \note	 
*****************************************************************************/
#define 	EPWM_GET_DOWNCMP_RIS_FLAG(EPWMn)		(((EPWM->RIS&(0x1<<(EPWMn+EPWM_RIS_DIFn_Pos)))? 1:0))


/*****************************************************************************
 ** \brief	EPWM_GET_PERIOD_RIS_FLAG
 **			获取周期中断源标志
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 ** \return  1：产生中断  0:无中断
 ** \note	 
*****************************************************************************/
#define 	EPWM_GET_PERIOD_RIS_FLAG(EPWMn)		(((EPWM->RIS&(0x1<<(EPWMn+EPWM_RIS_PIFn_Pos)))? 1:0))
		
/*****************************************************************************
 ** \brief	EPWM_GET_ZERO_RIS_FLAG
 **			获取零点中断源标志
 ** \param [in]EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 ** \return  1：产生中断  0:无中断
 ** \note	 
*****************************************************************************/
#define 	EPWM_GET_ZERO_RIS_FLAG(EPWMn)	(((EPWM->RIS&(0x1<<(EPWMn+EPWM_RIS_ZIFn_Pos)))? 1:0))

/*****************************************************************************
 ** \brief	EPWM_GET_COUNTCMP0_RIS_FLAG
 **			获取计数比较器0中断源标志
 ** \param [in]none
 ** \return  1：产生中断  0:无中断
 ** \note	 
*****************************************************************************/
#define 	EPWM_GET_COUNTCMP0_RIS_FLAG(EPWMn)	(((EPWM->RIS&(EPWM_RIS_DC0IF_Msk)))? 1:0)

/*****************************************************************************
 ** \brief	EPWM_GET_COUNTCMP1_RIS_FLAG
 **			获取计数比较器1中断源标志
 ** \param [in]none
 ** \return  1：产生中断  0:无中断
 ** \note	 
*****************************************************************************/
#define 	EPWM_GET_COUNTCMP1_RIS_FLAG(EPWMn)	(((EPWM->RIS&(EPWM_RIS_DC1IF_Msk))? 1:0))

									 
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
 ** \brief	 EPWM_ConfigRunMode
 **			 设置EPWM的运行模式
 ** \param [in] EpwmRunMode:   (1)EPWM_COUNT_UP_DOWN
							   (2)EPWM_COUNT_DOWN
							   (3)EPWM_OCU_ASYMMETRIC
							   (4)EPWM_OCU_SYMMETRIC
							   (5)EPWM_WFG_INDEPENDENT
							   (6)EPWM_WFG_COMPLEMENTARY
							   (7)EPWM_WFG_SYNCHRONIZED
							   (8)EPWM_OC_MODE_GROUP
							   (9)EPWM_OC_INDEPENDENT
 ** \return  none
 ** \note    
*****************************************************************************/
void EPWM_ConfigRunMode(uint32_t EpwmRunMode);

/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelClk	
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				ClkDiv: (1)EPWM_CLK_DIV_1 
 **						(2)EPWM_CLK_DIV_2
 **						(2)EPWM_CLK_DIV_4
 **						(2)EPWM_CLK_DIV_8
 **						(2)EPWM_CLK_DIV_16
 **						(2)EPWM_CLK_DIV_32
 ** \return  none	
 ** \note 
 **	(1)例因为CLKPSC01共同控制PWM0、PWM1通道 ，所以此函数统一设置CLKPSC01=0x01
 **     若需要其他的分频，请酌情对寄存器赋值. 
*****************************************************************************/
void EPWM_ConfigChannelClk(uint32_t EPWMn,uint32_t ClkDiv);

/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelPeriod	
 **			 设置EPWM通道的周期
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				Period: 16位周期值
 ** \return none
 ** \note    
*****************************************************************************/
void EPWM_ConfigChannelPeriod(uint32_t EPWMn, uint32_t Period);

/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelSymDuty	
 **			 设置对称模式下的EPWM通道的占空比
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **			    Duty  : 16位比较值
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ConfigChannelSymDuty(uint32_t EPWMn,uint32_t Duty);

/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelAsymDuty	
 **			 设置非对称模式下的 EPWM通道的占空比
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **			    UpCmp 	  : 16位向上比较值
 **			    DownCmp   : 16位向下比较值
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ConfigChannelAsymDuty(uint32_t EPWMn,uint32_t UpCmp, uint32_t DownCmp);

/*****************************************************************************
 ** \brief	 EPWM_EnableOneShotMode	
 **			 使能单次模式
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableOneShotMode(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_EnableAutoLoadMode	
 **			 使能连续加载模式
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableAutoLoadMode(uint32_t EPWMnMask);

/*****************************************************************************
 ** \brief	 EPWM_EnableReverseOutput	
 **			 开启反相输出
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableReverseOutput(uint32_t EPWMnMask);

/*****************************************************************************
 ** \brief	 EPWM_DisableReverseOutput	
 **			 关闭反相输出
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableReverseOutput(uint32_t EPWMnMask);

/*****************************************************************************
 ** \brief	 EPWM_EnableDeadZone	
 **			 开启死区
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 **				DeadZone : 0x00 ~ 0x3FF;
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableDeadZone(uint32_t EPWMnMask, uint32_t DeadZone);

/*****************************************************************************
 ** \brief	 EPWM_DisableDeadZone	
 **			 关闭死区
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableDeadZone(uint32_t EPWMnMask);

/*****************************************************************************
 ** \brief	 EPWM_EnableChannelRemap	
 **			 开启通道重映射
 ** \param [in]  EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				 IPGn  : IPG0 、IPG1 、IPG2 、IPG3 、IPG4 、IPG5 
 ** \return none
 ** \note   IPGx指内部通道，EPWMx指外部输出通道
*****************************************************************************/
void EPWM_EnableChannelRemap(uint32_t EPWMn, uint32_t IPGn);

/*****************************************************************************
 ** \brief	 EPWM_DisableChannelRemap	
 **			 关闭通道重映射
 ** \param [in]  none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableChannelRemap(void);

/*****************************************************************************
 ** \brief	 EPWM_Start	
 **			 开启EPWM
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_Start(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_Stop	
 **			 关闭EPWM
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_Stop(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_ForceStop	
 **			 强制关闭EPWM
 ** \param [in]  none
 ** \return none
 ** \note   使用掩码控制EPWM
*****************************************************************************/
void EPWM_ForceStop(void);

/*****************************************************************************
 ** \brief	 EPWM_EnableOutput
 **			 开启EPWM通道输出
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note  
*****************************************************************************/
void EPWM_EnableOutput(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_DisableOutput
 **			 关闭EPWM通道输出
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableOutput(uint32_t EPWMnMask);

/*****************************************************************************
 ** \brief	 EPWM_ConfigLoadAndIntMode
 **			 配置加载方式以及中断方式
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 **				 Mode: (1)EPWM_EACH_PERIOD_ZERO
 **					   (2)EPWM_EACH_ZERO
 **					   (3)EPWM_FIRST_ZERO_NEXT_PERIOD
 **					   (4)EPWM_EVERY_TWO_ZERO
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ConfigLoadAndIntMode(uint32_t EPWMnMask, uint32_t Mode);

/*****************************************************************************
 ** \brief	 EPWM_EnableIntAccompanyWithLoad
 **			 中断产生伴随着加载动作
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableIntAccompanyWithLoad(void);
/*****************************************************************************
 ** \brief	 EPWM_DisableIntAccompanyWithLoad
 **			 中断产生与加载动作无关
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableIntAccompanyWithLoad(void);


/*****************************************************************************
 ** \brief	 EPWM_EnableEventLoad
 **			 使能事件触发加载
 ** \param [in]  Event :(1)EPWM_EVENT_HALL
 **						(2)EPWM_EVENT_ACMP1
 **						(3)EPWM_EVENT_ACMP0
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableEventLoad(uint32_t Event);

/*****************************************************************************
 ** \brief	 EPWM_EnableMaskPreload
 **			 使能掩码预加载
 ** \param [in] LoadMode: (1)EPWM_MASK_LOAD_EPWM0	在EPWM0 的加载点加载掩码数据
 **					  	  (2)EPWM_MASK_LOAD_EPWM1	在EPWM1 的加载点加载掩码数据
 **					  	  (3)EPWM_MASK_LOAD_EPWM2	在EPWM2 的加载点加载掩码数据
 **					  	  (4)EPWM_MASK_LOAD_EPWM3	在EPWM3 的加载点加载掩码数据
 **					  	  (5)EPWM_MASK_LOAD_EPWM4	在EPWM4 的加载点加载掩码数据
 **					  	  (5)EPWM_MASK_LOAD_EPWM5	在EPWM5 的加载点加载掩码数据
 ** \return none
 ** \note   允许 MASKNXT 寄存器的数据加载到MASK 寄存器中，同时禁止写 MASK 寄存器
*****************************************************************************/
void EPWM_EnableMaskPreload(uint32_t LoadMode);

/*****************************************************************************
 ** \brief	 EPWM_DisableMaskPreload
 **			 关闭掩码预加载
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableMaskPreload(void);
/*****************************************************************************
 ** \brief	 EPWM_WriteMaskBuffer
 **			 写掩码预加载缓存
 ** \param [in] MaskBuffer:  EPWM_MASK_BUF0 、EPWM_MASK_BUF1、EPWM_MASK_BUF2、EPWM_MASK_BUF3
 **							 EPWM_MASK_BUF4、EPWM_MASK_BUF5、EPWM_MASK_BUF6、EPWM_MASK_BUF7
 **				BufferValue: 16bit
 ** \return none
 ** \note   HALLEN=0 时，默认加载掩码预设缓存0中的数据。
*****************************************************************************/
void EPWM_WriteMaskBuffer(uint32_t MaskBuffer, uint32_t BufferValue);

/*****************************************************************************
 ** \brief	 EPWM_EnableMaskOutPut
 **			 使能掩码输出
 ** \param [in]EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 **				MaskLevel:  0：低电平  1：高电平
 ** \return none
 ** \note   操作此函数时会关闭掩码预加载
*****************************************************************************/
void EPWM_EnableMaskOutPut(uint32_t EPWMnMask, uint32_t MaskLevel);
/*****************************************************************************
 ** \brief	 EPWM_DisableMaskOutPut
 **			 关闭掩码输出
 ** \param [in]EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   操作此函数时会关闭掩码预加载
*****************************************************************************/
void EPWM_DisableMaskOutPut(uint32_t EPWMnMask);

/*****************************************************************************
 ** \brief	 EPWM_ConfigCompareTriger
 **			 配置触发比较器
 ** \param [in]CmpTgn: EPWM_CMPTG_0、EPWM_CMPTG_1
 **			   CmpTgMode: (1)EPWM_CMPTG_FALLING
 **						  (2)EPWM_CMPTG_RISING
 **			   CmpTgCounter: (1)EPWM_CMPTG_EPWM0
 **							 (2)EPWM_CMPTG_EPWM1
 **							 (3)EPWM_CMPTG_EPWM2
 **							 (4)EPWM_CMPTG_EPWM3
 **							 (5)EPWM_CMPTG_EPWM4
 **							 (6)EPWM_CMPTG_EPWM5
 **			   CmpTgData: 0x00~ 0xffff;
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ConfigCompareTriger(uint32_t CmpTgn, uint32_t CmpTgMode,uint32_t CmpTgCounter, uint32_t CmpTgData);

/*****************************************************************************
 ** \brief	 EPWM_EnableHall
 **			 开启Hall模式检测
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableHall(void);
/*****************************************************************************
 ** \brief	 EPWM_DisableHall
 **			 关闭Hall模式检测
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableHall(void);
/*****************************************************************************
 ** \brief	 EPWM_GetHallState
 **			 获取Hall接口状态
 ** \param [in] none
 ** \return Hall State
 ** \note   
*****************************************************************************/
uint32_t  EPWM_GetHallState(void);
/*****************************************************************************
 ** \brief	 EPWM_ClearHallErrorState
 **			 清除Hall接口错误状态
 ** \param [in] none
 ** \return none
 ** \note  如果出现的错误的状态或者序列时，HALLST=111，HALL 检测功能停止。
 **			再次检测开启 HALL 状态时，需要清除错误的状态。 
*****************************************************************************/
void  EPWM_ClearHallErrorState(void);





/*****************************************************************************
 ** \brief	 EPWM_AllBrakeEnable
 **			 开启刹车总使能
 ** \param [in] none
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_AllBrakeEnable(void);

/*****************************************************************************
 ** \brief	 EPWM_AllBrakeDisable
 **			关闭刹车总使能
 ** \param [in] none
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_AllBrakeDisable(void);
/*****************************************************************************
 ** \brief	 EPWM_ConfigBrakeMode
 **			配置刹车模式
 ** \param [in] mode :(1)EPWM_BRK_STOP				停止模式
 **					  (2)EPWM_BRK_SUSPEND			暂停模式
 **					  (3)EPWM_BRK_RECOVER			恢复模式
 **					  (4)EPWM_BRK_DELAY_RECOVER		延时恢复模式
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ConfigBrakeMode(uint32_t mode);

/*****************************************************************************
 ** \brief	 EPWM_ConfigBrakeRecoverDelay
 **			配置刹车恢复延时模式的延时
 ** \param [in] time : 0~0xffff
 ** \return none
 ** \note  延时时间= RDT×TAPBCLK
*****************************************************************************/
void  EPWM_ConfigBrakeRecoverDelay(uint32_t time);


/*****************************************************************************
 ** \brief	 EPWM_ConfigBrakeRecoverLoad
 **			配置刹车恢复点
 ** \param [in] mode :(1)EPWM_BRK_LOAD_EPWM0
 **					  (2)EPWM_BRK_LOAD_EPWM1			
 **					  (3)EPWM_BRK_LOAD_EPWM2			
 **					  (4)EPWM_BRK_LOAD_EPWM3		
 **					  (5)EPWM_BRK_LOAD_EPWM4		
 **					  (6)EPWM_BRK_LOAD_EPWM5		
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ConfigBrakeRecoverLoad(uint32_t mode);


/*****************************************************************************
 ** \brief	 EPWM_EnableFaultBrake
 **			 开启故障刹车
 ** \param [in] BrakeSource:(1)EPWM_BRK_SOFT:软件刹车
 **							(2)EPWM_BRK_EXTLE 		外部硬件电平刹车
 **							(3)EPWM_BRK_EXTEE 		外部硬件边沿刹车
 **							(4)EPWM_BRK_ADCBCMP0 	ADCB比较器0刹车
 **							(5)EPWM_BRK_ADCBCMP1 	ADCB比较器1刹车
 **							(6)EPWM_BRK_ADC0CMP0 	ADC0比较器0刹车
 **							(7)EPWM_BRK_ACMP0LE 	模拟比较器0输出电平刹车
 **							(8)EPWM_BRK_ACMP1LE  	模拟比较器1输出电平刹车
 **							(9)EPWM_BRK_ACMP0EE  	模拟比较器0事件刹车
 **							(10)EPWM_BRK_ACMP1EE  	模拟比较器1事件刹车
 ** \return none
 ** \note  	
*****************************************************************************/
void  EPWM_EnableFaultBrake(uint32_t BrakeSource);
/*****************************************************************************
 ** \brief	 EPWM_DisableFaultBrake
 **			 关闭刹车
 ** \param [in] BrakeSource:(1)EPWM_BRK_SOFT:软件刹车
 **							(2)EPWM_BRK_EXTLE 		外部硬件电平刹车
 **							(3)EPWM_BRK_EXTEE 		外部硬件边沿刹车
 **							(4)EPWM_BRK_ADCBCMP0 	ADCB比较器0刹车
 **							(5)EPWM_BRK_ADCBCMP1 	ADCB比较器1刹车
 **							(6)EPWM_BRK_ADC0CMP0 	ADC0比较器0刹车
 **							(7)EPWM_BRK_ACMP0LE 	模拟比较器0输出电平刹车
 **							(8)EPWM_BRK_ACMP1LE  	模拟比较器1输出电平刹车
 **							(9)EPWM_BRK_ACMP0EE  	模拟比较器0事件刹车
 **							(10)EPWM_BRK_ACMP1EE  	模拟比较器1事件刹车				
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableFaultBrake(uint32_t BrakeSource);


/*****************************************************************************
 ** \brief	 EPWM_ConfigFaultBrakeLevel
 **			 设置EPWMn刹车电平
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 **				BrakeLevel:  0：低电平  1：高电平			
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ConfigFaultBrakeLevel(uint32_t EPWMnMask, uint32_t BrakeLevel);


/*****************************************************************************
 ** \brief	 EPWM_EnableEXTLEBrake
 **			 开启外部硬件电平刹车模式
 ** \param [in] BrakeMode：	(1) EPWM_EXTBK_LEVEL_HIGH		高电平刹车
 **					   		(2)	EPWM_EXTBK_LEVEL_LOW		低电平刹车
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableEXTLEBrake(uint32_t BrakeMode);
/*****************************************************************************
 ** \brief	 EPWM_EnableEXTEEBrake
 **			 开启外部硬件边沿刹车模式
 ** \param [in] BrakeMode：	(1)	EPWM_EXTBK_EDGE_FALLING 	下降沿刹车
 **					   		(2) EPWM_EXTBK_EDGE_RISING		上升沿刹车
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableEXTEEBrake(uint32_t BrakeMode);

/*****************************************************************************
 ** \brief	 EPWM_EnableACMP0LEBrake
 **			 开启比较器0输出电平刹车模式
 ** \param [in] BrakeMode：	(1) EPWM_ACMP0BRK_LEVEL_HIGH		比较器0输出高电平刹车
 **					   		(2)	EPWM_ACMP0BRK_LEVEL_LOW			比较器0输出低电平刹车
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableACMP0LEBrake(uint32_t BrakeMode);

/*****************************************************************************
 ** \brief	 EPWM_EnableACMP1LEBrake
 **			 开启比较器1输出电平刹车模式
 ** \param [in] BrakeMode：	(1)	EPWM_ACMP1BRK_LEVEL_HIGH 	比较器1输出高电平刹车
 **					   		(2) EPWM_ACMP1BRK_LEVEL_LOW		比较器1输出低电平刹车
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableACMP1LEBrake(uint32_t BrakeMode);

/*****************************************************************************
 ** \brief	 EPWM_EnableSoftwareBrake
 **			 使能软件刹车
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableSoftwareBrake(void);

/*****************************************************************************
 ** \brief	 EPWM_DisableSoftwareBrake
 **			 关闭软件刹车
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableSoftwareBrake(void);


/*****************************************************************************
 ** \brief	 EPWM_EnableBrakeInt
 **			 开启刹车中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableBrakeInt(void);
/*****************************************************************************
 ** \brief	 EPWM_DisableBrakeInt
 **			 关闭刹车中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableBrakeInt(void);
/*****************************************************************************
 ** \brief	 EPWM_GetBrakeIntFlag
 **			获取刹车中断使能并产生中断标志位
 ** \param [in] none	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetBrakeIntFlag(void);
/*****************************************************************************
 ** \brief	 EPWM_ClearBrakeIntFlag
 **			清除刹车中断标志位
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearBrakeIntFlag(void);


/*****************************************************************************
 ** \brief	 EPWM_GetBrakeFlag
 **			获取刹车信号标志位
 ** \param [in] none	
 ** \return 1：发生故障或者刹车信号保持有效  0：无故障
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetBrakeFlag(void);
/*****************************************************************************
 ** \brief	 EPWM_ClearBrake
 **			清除刹车保护位
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearBrake(void);


/*****************************************************************************
 ** \brief	 EPWM_EnableHallErrorInt
 **			 开启Hall状态错误中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableHallErrorInt(void);
/*****************************************************************************
 ** \brief	 EPWM_DisableHallErrorInt
 **			 关闭Hall状态错误中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableHallErrorInt(void);
/*****************************************************************************
 ** \brief	 EPWM_GetHallErrorIntFlag
 **			获取Hall状态错误中断使能并产生中断标志位
 ** \param [in] none	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetHallErrorIntFlag(void);
/*****************************************************************************
 ** \brief	 EPWM_ClearHallErrorIntFlag
 **			清除Hall状态错误中断标志位
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearHallErrorIntFlag(void);

/*****************************************************************************
 ** \brief	 EPWM_EnableUpCmpInt
 **			 开启向上比较中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableUpCmpInt(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_DisableUpCmpInt
 **			 关闭向上比较中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableUpCmpInt(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_GetUpCmpIntFlag
 **			获取向上比较中断使能并产生中断标志位
 ** \param [in]EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetUpCmpIntFlag(uint32_t EPWMn);
/*****************************************************************************
 ** \brief	 EPWM_ClearUpCmpIntFlag
 **			清除向上比较中断标志位
 ** \param [in]EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearUpCmpIntFlag(uint32_t EPWMn);

/*****************************************************************************
 ** \brief	 EPWM_EnableDownCmpInt
 **			 开启向下比较中断
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableDownCmpInt(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_DisableDownCmpInt
 **			 关闭向下比较中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableDownCmpInt(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_GetDownCmpIntFlag
 **			获取向下比较中断使能并产生中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetDownCmpIntFlag(uint32_t EPWMn);
/*****************************************************************************
 ** \brief	 EPWM_ClearDownCmpIntFlag
 **			清除向下比较中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearDownCmpIntFlag(uint32_t EPWMn);


/*****************************************************************************
 ** \brief	 EPWM_EnablePeriodInt
 **			 开启周期中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnablePeriodInt(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_DisablePeriodInt
 **			 关闭周期中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisablePeriodInt(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_GetPeriodIntFlag
 **			获取周期中断使能并产生中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetPeriodIntFlag(uint32_t EPWMn);
/*****************************************************************************
 ** \brief	 EPWM_ClearPeriodIntFlag
 **			清除周期中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearPeriodIntFlag(uint32_t EPWMn);

/*****************************************************************************
 ** \brief	 EPWM_EnableZeroInt
 **			 开启零点中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableZeroInt(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_DisableZeroInt
 **			 关闭零点中断
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableZeroInt(uint32_t EPWMnMask);
/*****************************************************************************
 ** \brief	 EPWM_GetZeroIntFlag
 **			获取零点中断使能并产生中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetZeroIntFlag(uint32_t EPWMn);
/*****************************************************************************
 ** \brief	 EPWM_ClearZeroIntFlag
 **			清除零点中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearZeroIntFlag(uint32_t EPWMn);

/*****************************************************************************
 ** \brief	 EPWM_EnableCountCmpInt
 **			 开启计数比较器中断
 ** \param [in] EPWMCmp: EPWM_CMPTG_0 EPWM_CMPTG_1
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableCountCmpInt(uint32_t EPWMCmp);
/*****************************************************************************
 ** \brief	 EPWM_DisableCountCmp0Int
 **			 关闭计数比较器中断
 ** \param [in] EPWMCmp: EPWM_CMPTG_0 EPWM_CMPTG_1	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableCountCmpInt(uint32_t EPWMCmp);
/*****************************************************************************
 ** \brief	 EPWM_GetCountCmpIntFlag
 **			获取计数比较器中断使能并产生中断标志位
 ** \param [in] EPWMCmp: EPWM_CMPTG_0 EPWM_CMPTG_1	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetCountCmpIntFlag(uint32_t EPWMCmp);
/*****************************************************************************
 ** \brief	 EPWM_ClearCountCmp0IntFlag
 **			清除计数比较器中断标志位
 ** \param [in] EPWMCmp: EPWM_CMPTG_0 EPWM_CMPTG_1	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearCountCmpIntFlag(uint32_t EPWMCmp);




/*****************************************************************************
 ** \brief	 EPWM_EnableCountCmp1Int
 **			 开启计数比较器1中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableCountCmp1Int(void);
/*****************************************************************************
 ** \brief	 EPWM_DisableCountCmp1Int
 **			 关闭计数比较器1中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableCountCmp1Int(void);
/*****************************************************************************
 ** \brief	 EPWM_GetCountCmp1IntFlag
 **			获取计数比较器1中断使能并产生中断标志位
 ** \param [in] none	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetCountCmp1IntFlag(void);
/*****************************************************************************
 ** \brief	 EPWM_ClearCountCmp1IntFlag
 **			清除计数比较器1中断标志位
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearCountCmp1IntFlag(void);


/*****************************************************************************
 ** \brief	 EPWM_EnableBrakeIntSum
 **			 开启故障刹车中断累计功能
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableBrakeIntSum(void);

/*****************************************************************************
 ** \brief	 EPWM_DisableBrakeIntSum
 **			 关闭故障刹车中断累计功能
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableBrakeIntSum(void);
/*****************************************************************************
 ** \brief	 EPWM_ConfigBrakeIntSum
 **			 设置故障刹车中断累计次数
 ** \param [in] count : 0~0xf	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ConfigBrakeIntSum(uint32_t count);


/*****************************************************************************
 ** \brief	 EPWM_EnableZeroIntSum
 **			 开启零点中断累计功能
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableZeroIntSum(void);

/*****************************************************************************
 ** \brief	 EPWM_DisableZeroIntSum
 **			 关闭零点中断累计功能
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableZeroIntSum(void);
/*****************************************************************************
 ** \brief	 EPWM_ConfigZeroIntSum
 **			 设置零点中断累计次数
 ** \param [in] count : 0~0xf	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ConfigZeroIntSum(uint32_t count);

/*****************************************************************************
 ** \brief	 EPWM_EnableDebugSuspend
 **			 使能调试暂停
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableDebugSuspend(void);
/*****************************************************************************
 ** \brief	 EPWM_DisableDebugSuspend
 **			 关闭调试暂停
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableDebugSuspend(void);
/*****************************************************************************
 ** \brief	 EPWM_ConfigBrakeFilter
 **			 配故障刹车信号滤波
 ** \param [in] filter	(1)	EPWM_BRK_FILCLK_1 	
 **					   		(2) EPWM_BRK_FILCLK_2	
 **					   		(3) EPWM_BRK_FILCLK_3	
 **					   		(4) EPWM_BRK_FILCLK_5	
 **					   		(5) EPWM_BRK_FILCLK_9	
 **					   		(6) EPWM_BRK_FILCLK_17	
 **					   		(7) EPWM_BRK_FILCLK_25	
 **					   		(8) EPWM_BRK_FILCLK_33	
 **					   		(9) EPWM_BRK_FILCLK_49	
 **					   		(10) EPWM_BRK_FILCLK_65	
 **					   		(11) EPWM_BRK_FILCLK_81	
 **					   		(12) EPWM_BRK_FILCLK_97	
 **					   		(13) EPWM_BRK_FILCLK_113	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ConfigBrakeFilter(uint32_t filter);
#ifdef __cplusplus
}
#endif

#endif /*__EPWM_H_ */

