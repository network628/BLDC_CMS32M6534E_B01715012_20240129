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
/** \file epwm.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "epwm.h"
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
 ** \brief	 EPWM_ConfigRunMode
 **			 设置EPWM的运行模式
 ** \param [in] EpwmRunMode: (1)EPWM_COUNT_UP_DOWN
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
void EPWM_ConfigRunMode(uint32_t EpwmRunMode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CON &= ~(EPWM_CON_MODE_Msk | EPWM_CON_GROUNPEN_Msk | EPWM_CON_ASYMEN_Msk);
	EPWM->CON |= EpwmRunMode;
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelClk	
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				ClkDiv: (1)EPWM_CLK_DIV_1 
 **						(2)EPWM_CLK_DIV_2
 **						(3)EPWM_CLK_DIV_4
 **						(4)EPWM_CLK_DIV_8
 **						(5)EPWM_CLK_DIV_16
 **						(6)EPWM_CLK_DIV_32
 ** \return  none	
 ** \note 
 **	(1)例因为CLKPSC01共同控制PWM0、PWM1通道 ，所以此函数统一设置CLKPSC01=0x01
 **     若需要其他的分频，请酌情对寄存器赋值.  
*****************************************************************************/
void EPWM_ConfigChannelClk(uint32_t EPWMn,uint32_t ClkDiv)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CLKPSC = (0x01<<EPWM_CLKPSC_CLKPSC45_Pos |
					0x01<<EPWM_CLKPSC_CLKPSC23_Pos |
					0x01<<EPWM_CLKPSC_CLKPSC01_Pos);
	EPWM->CLKDIV &= ~(0xfUL <<(EPWMn*4));
	EPWM->CLKDIV |= (ClkDiv <<(EPWMn*4));		
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelPeriod	
 **			 设置EPWM通道的周期
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				Period: 16位周期值
 ** \return none
 ** \note    
*****************************************************************************/
void EPWM_ConfigChannelPeriod(uint32_t EPWMn, uint32_t Period)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->PERIOD[EPWMn] = 0xffff & Period;
	EPWM->CON3 |= (0x1UL <<(EPWMn + EPWM_CON3_LOADEN0_Pos));		/*使能加载周期&&占空比*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelSymDuty	
 **			 设置对称模式下的EPWM通道的占空比
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **			    Duty  : 16位比较值
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ConfigChannelSymDuty(uint32_t EPWMn,uint32_t Duty)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->CMPDAT[EPWMn] &= 0xffff0000;
	EPWM->CMPDAT[EPWMn] |= 0xffff & Duty;	
	EPWM->CON3 |= (0x1UL <<(EPWMn +EPWM_CON3_LOADEN0_Pos));		/*使能加载周期&&占空比*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelAsymDuty	
 **			 设置非对称模式下的 EPWM通道的占空比
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **			    UpCmp 	  : 16位向上比较值
 **			    DownCmp   : 16位向下比较值
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ConfigChannelAsymDuty(uint32_t EPWMn,uint32_t UpCmp, uint32_t DownCmp)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->CMPDAT[EPWMn] = (0xffff & UpCmp) | ((0xffff & DownCmp)<<16);	
	EPWM->CON3 |= (0x1UL <<(EPWMn +EPWM_CON3_LOADEN0_Pos));		/*使能加载周期&&占空比*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_EnableOneShotMode	
 **			 使能单次模式
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableOneShotMode(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON &= ~(EPWMnMask); 
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_EnableAutoLoadMode	
 **			 使能连续加载模式
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableAutoLoadMode(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON |= EPWMnMask; 
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_ConfigLoadAndIntMode
 **			 配置加载方式以及中断方式
 ** \param [in]  EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				 Mode: (1)EPWM_EACH_PERIOD_ZERO
 **					   (2)EPWM_EACH_ZERO
 **					   (3)EPWM_FIRST_ZERO_NEXT_PERIOD
 **					   (4)EPWM_EVERY_TWO_ZERO
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ConfigLoadAndIntMode(uint32_t EPWMn, uint32_t Mode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON3 &= ~(0x3UL<< ((EPWMn*2) + EPWM_CON3_LOADTYP0_Pos));
	EPWM->CON3 |= Mode << ((EPWMn*2) + EPWM_CON3_LOADTYP0_Pos);	
	EPWM->LOCK = 0x0;			
}
/*****************************************************************************
 ** \brief	 EPWM_EnableEventLoad
 **			 使能事件触发加载
 ** \param [in]  Event :(1)EPWM_EVENT_HALL
 **						(2)EPWM_EVENT_ACMP1
 **						(3)EPWM_EVENT_ACMP0
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableEventLoad(uint32_t Event)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON3 |= (0x01UL<<(Event + EPWM_CON3_LETGACMP0_Pos));
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_EnableReverseOutput	
 **			 开启反相输出
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableReverseOutput(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON |= (EPWMnMask<< EPWM_CON_PINV0_Pos); 
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_DisableReverseOutput	
 **			 关闭反相输出
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableReverseOutput(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON &= ~(EPWMnMask<< EPWM_CON_PINV0_Pos); 
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_EnableDeadZone	
 **			 开启死区
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 **				DeadZone : 0x00 ~ 0x3FF;
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableDeadZone(uint32_t EPWMnMask, uint32_t DeadZone)
{
	
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	if(( EPWMnMask & EPWM_CH_0_MSK) || (EPWMnMask & EPWM_CH_1_MSK))
	{
		EPWM->CON |= EPWM_CON_ENDT01_Msk;
		EPWM->DTCTL &= ~(EPWM_DTCTL_DTI01_Msk);
		EPWM->DTCTL |= 0x3ff & DeadZone;
	}
	if(( EPWMnMask & EPWM_CH_2_MSK) || (EPWMnMask & EPWM_CH_3_MSK))
	{
		EPWM->CON |= EPWM_CON_ENDT23_Msk;
		EPWM->DTCTL &= ~(EPWM_DTCTL_DTI23_Msk);
		EPWM->DTCTL |=  ((0x3ff & DeadZone)<<EPWM_DTCTL_DTI23_Pos);
	}	
	if(( EPWMnMask & EPWM_CH_4_MSK) || (EPWMnMask & EPWM_CH_5_MSK))
	{
		EPWM->CON |= EPWM_CON_ENDT45_Msk;
		EPWM->DTCTL &= ~(EPWM_DTCTL_DTI45_Msk);
		EPWM->DTCTL |= (0x3ff & DeadZone) << EPWM_DTCTL_DTI45_Pos;
	}	
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_DisableDeadZone	
 **			 关闭死区
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableDeadZone(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	if(( EPWMnMask & EPWM_CH_0_MSK) || (EPWMnMask & EPWM_CH_1_MSK))
	{
		EPWM->CON &= ~(EPWM_CON_ENDT01_Msk);
	}
	if(( EPWMnMask & EPWM_CH_2_MSK) || (EPWMnMask & EPWM_CH_3_MSK))
	{
		EPWM->CON &= ~(EPWM_CON_ENDT23_Msk);
	}	
	if(( EPWMnMask & EPWM_CH_4_MSK) || (EPWMnMask & EPWM_CH_5_MSK))
	{
		EPWM->CON &= ~(EPWM_CON_ENDT45_Msk);
	}	
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_EnableChannelRemap	
 **			 开启通道重映射
 ** \param [in]  EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				 IPGn  : IPG0 、IPG1 、IPG2 、IPG3 、IPG4 、IPG5 
 ** \return none
 ** \note   IPGx指内部通道，EPWMx指外部输出通道
*****************************************************************************/
void EPWM_EnableChannelRemap(uint32_t EPWMn, uint32_t IPGn)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->POREMAP &= ~((EPWM_POREMAP_PWMRMEN_Msk) | (0xf<<(EPWMn * 4)));
	EPWM->POREMAP |= (EPWM_REMAP_ENABLE<< EPWM_POREMAP_PWMRMEN_Pos) |(IPGn<< (EPWMn * 4));
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_DisableChannelRemap	
 **			 关闭通道重映射
 ** \param [in]  none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableChannelRemap(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->POREMAP &= ~(EPWM_POREMAP_PWMRMEN_Msk);
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_EnableOutput
 **			 开启EPWM通道输出
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note  
*****************************************************************************/
void EPWM_EnableOutput(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->POEN |= EPWMnMask;
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_DisableOutput
 **			 关闭EPWM通道输出
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableOutput(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->POEN &= ~(EPWMnMask);
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_Start	
 **			 开启EPWM
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_Start(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON2 |= EPWMnMask;	
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_Stop	
 **			 关闭EPWM
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_Stop(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON2 &= ~(EPWMnMask);			
	EPWM->CON3 |= EPWMnMask;			/*计数清0*/
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_ForceStop	
 **			 强制关闭EPWM
 ** \param [in]  none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ForceStop(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	
	EPWM->LOCK = 0x0;	
}
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
 ** \note  允许 MASKNXT 寄存器的数据加载到MASK 寄存器中，同时禁止写 MASK 寄存器 
*****************************************************************************/
void EPWM_EnableMaskPreload(uint32_t LoadMode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->POEN &= ~(EPWM_POEN_MASKLS_Msk);
	EPWM->POEN |=  EPWM_POEN_MASKLE_Msk | LoadMode;		
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** \brief	 EPWM_DisableMaskPreload
 **			 关闭掩码预加载
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableMaskPreload(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->POEN &= ~(EPWM_POEN_MASKLE_Msk);	
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_WriteMaskBuffer
 **			 写掩码预加载缓存
 ** \param [in] MaskBuffer:  EPWM_MASK_BUF0 、EPWM_MASK_BUF1、EPWM_MASK_BUF2、EPWM_MASK_BUF3
 **							 EPWM_MASK_BUF4、EPWM_MASK_BUF5、EPWM_MASK_BUF6、EPWM_MASK_BUF7
 **				BufferValue: 16bit
 ** \return none
 ** \note   HALLEN=0 时，默认加载掩码预设缓存0中的数据。
*****************************************************************************/
void EPWM_WriteMaskBuffer(uint32_t MaskBuffer, uint32_t BufferValue)
{
	uint32_t Temp;
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	Temp = EPWM->MASKNXT;
	Temp &= ~(EPWM_MASKNXT_PMASKSEL_Msk | 0xffff);
	Temp |= (MaskBuffer<<EPWM_MASKNXT_PMASKSEL_Pos) | BufferValue;
	EPWM->MASKNXT = Temp;
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** \brief	 EPWM_EnableMaskOutPut
 **			 使能掩码输出
 ** \param [in]EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 **				MaskLevel:  0：低电平  1：高电平
 ** \return none
 ** \note   操作此函数时会关闭掩码预加载
*****************************************************************************/
void EPWM_EnableMaskOutPut(uint32_t EPWMnMask, uint32_t MaskLevel)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->POEN &= ~(EPWM_POEN_MASKLE_Msk);	
	EPWM->MASK |= (EPWMnMask <<EPWM_MASK_MASKEN0_Pos);
	if(MaskLevel)
	{
		EPWM->MASK |= EPWMnMask ;	
	}
	else
	{
		EPWM->MASK &= ~(EPWMnMask);	
	}	
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_DisableMaskOutPut
 **			 关闭掩码输出
 ** \param [in]EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note   操作此函数时会关闭掩码预加载
*****************************************************************************/
void EPWM_DisableMaskOutPut(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->POEN &= ~(EPWM_POEN_MASKLE_Msk);	
	EPWM->MASK &= ~(EPWMnMask <<EPWM_MASK_MASKEN0_Pos);	
	EPWM->LOCK = 0x0;
}

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
void EPWM_ConfigCompareTriger(uint32_t CmpTgn, uint32_t CmpTgMode,uint32_t CmpTgCounter, uint32_t CmpTgData)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CMPTGD[CmpTgn] = (CmpTgMode<<EPWM_CMPTGD_CMPTGDS_Pos)    |
						   (CmpTgCounter<<EPWM_CMPTGD_CMPPCHS_Pos) |
						   (0xffff & CmpTgData);	
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_EnableHall
 **			 开启Hall模式检测
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableHall(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->MASKNXT |= EPWM_MASKNXT_HALLEN_Msk;
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_DisableHall
 **			 关闭Hall模式检测
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableHall(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->MASKNXT &= ~(EPWM_MASKNXT_HALLEN_Msk);
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_GetHallState
 **			 获取Hall接口状态
 ** \param [in] none
 ** \return Hall State
 ** \note   
*****************************************************************************/
uint32_t  EPWM_GetHallState(void)
{
	return((EPWM->MASKNXT & EPWM_MASKNXT_HALLST_Msk)>>EPWM_MASKNXT_HALLST_Pos);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearHallErrorState
 **			 清除Hall接口错误状态
 ** \param [in] none
 ** \return none
 ** \note  如果出现的错误的状态或者序列时，HALLST=111，HALL 检测功能停止。
 **			再次检测开启 HALL 状态时，需要清除错误的状态。 
*****************************************************************************/
void  EPWM_ClearHallErrorState(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->MASKNXT |= EPWM_MASKNXT_HALLCLR_Msk;
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_AllBrakeEnable
 **			 开启刹车总使能
 ** \param [in] none
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_AllBrakeEnable(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL |= EPWM_BRKCTL_BRKEN_Msk;	
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_AllBrakeDisable
 **			关闭刹车总使能
 ** \param [in] none
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_AllBrakeDisable(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL &= ~(EPWM_BRKCTL_BRKEN_Msk);	
	EPWM->LOCK = 0x0;	
}

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
void  EPWM_ConfigBrakeMode(uint32_t mode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL &= ~(EPWM_BRKCTL_BRKMS_Msk);	
	EPWM->BRKCTL |= mode;		
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** \brief	 EPWM_ConfigBrakeRecoverDelay
 **			配置恢复延时模式的延时
 ** \param [in] time : 0~0xffff
 ** \return none
 ** \note  延时时间= RDT×TAPBCLK
*****************************************************************************/
void  EPWM_ConfigBrakeRecoverDelay(uint32_t time)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->BRKRDT &= ~(EPWM_BRKRDT_RDT_Msk);
	EPWM->BRKRDT |= time;		
	EPWM->LOCK = 0x0;
}


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
void  EPWM_ConfigBrakeRecoverLoad(uint32_t mode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL &= ~(EPWM_BRKCTL_BRKRCS_Msk);	
	EPWM->BRKCTL |= mode;		
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_EnableFaultBrake
 **			 开启刹车
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
void  EPWM_EnableFaultBrake(uint32_t BrakeSource)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL |= BrakeSource;
	EPWM->LOCK = 0x0;	
}
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
void  EPWM_DisableFaultBrake(uint32_t BrakeSource)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL &= ~(BrakeSource);
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_ConfigFaultBrakeLevel
 **			 设置EPWMn刹车电平
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 **				BrakeLevel:  0：低电平  1：高电平			
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ConfigFaultBrakeLevel(uint32_t EPWMnMask, uint32_t BrakeLevel)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	if(BrakeLevel)
	{
		EPWM->BRKCTL |= EPWMnMask;
	}
	else
	{
		EPWM->BRKCTL &= ~(EPWMnMask);		
	}
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_EnableEXTLEBrake
 **			 开启外部硬件电平刹车模式
 ** \param [in] BrakeMode：	(1) EPWM_EXTBK_LEVEL_HIGH		高电平刹车
 **					   		(2)	EPWM_EXTBK_LEVEL_LOW		低电平刹车
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableEXTLEBrake(uint32_t BrakeMode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL &= ~(EPWM_BRKCTL_EXTBRKLS_Msk);
	EPWM->BRKCTL |= BrakeMode;
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_EnableEXTEEBrake
 **			 开启外部硬件边沿刹车模式
 ** \param [in] BrakeMode：	(1)	EPWM_EXTBK_EDGE_FALLING 	下降沿刹车
 **					   		(2) EPWM_EXTBK_EDGE_RISING		上升沿刹车
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableEXTEEBrake(uint32_t BrakeMode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL &= ~(EPWM_BRKCTL_EXTBRKES_Msk);
	EPWM->BRKCTL |= BrakeMode;
	EPWM->LOCK = 0x0;	
}


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
void  EPWM_ConfigBrakeFilter(uint32_t filter)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->BRKRDT &= ~(EPWM_BRKRDT_FILS_Msk);
	EPWM->BRKRDT |= (filter << EPWM_BRKRDT_FILS_Pos);		
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_EnableACMP0LEBrake
 **			 开启比较器0输出电平刹车模式
 ** \param [in] BrakeMode：	(1) EPWM_ACMP0BRK_LEVEL_HIGH		比较器0输出高电平刹车
 **					   		(2)	EPWM_ACMP0BRK_LEVEL_LOW			比较器0输出低电平刹车
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableACMP0LEBrake(uint32_t BrakeMode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL &= ~(EPWM_BRKCTL_ACMP0BKLS_Msk);
	EPWM->BRKCTL |= BrakeMode;
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** \brief	 EPWM_EnableACMP1LEBrake
 **			 开启比较器1输出电平刹车模式
 ** \param [in] BrakeMode：	(1)	EPWM_ACMP1BRK_LEVEL_HIGH 	比较器1输出高电平刹车
 **					   		(2) EPWM_ACMP1BRK_LEVEL_LOW		比较器1输出低电平刹车
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableACMP1LEBrake(uint32_t BrakeMode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->BRKCTL &= ~(EPWM_BRKCTL_ACMP1BKLS_Msk);
	EPWM->BRKCTL |= BrakeMode;
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_EnableSoftwareBrake
 **			 使能软件刹车
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableSoftwareBrake(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->BRKCTL |=	EPWM_BRKCTL_SWBRK_Msk;
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** \brief	 EPWM_DisableSoftwareBrake
 **			 关闭软件刹车
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableSoftwareBrake(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->BRKCTL &= ~(EPWM_BRKCTL_SWBRK_Msk);
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_EnableBrakeInt
 **			 开启刹车中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableBrakeInt(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC |= EPWM_IMSC_ENBRKIF_Msk;
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_DisableBrakeInt
 **			 关闭刹车中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableBrakeInt(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC &= ~(EPWM_IMSC_ENBRKIF_Msk);
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_GetBrakeIntFlag
 **			获取刹车中断使能并产生中断标志位
 ** \param [in] none	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetBrakeIntFlag(void)
{
	return ((EPWM->MIS&EPWM_MIS_BRKIF_Msk)? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearBrakeIntFlag
 **			清除刹车中断标志位
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearBrakeIntFlag(void)
{
	EPWM->ICLR |= EPWM_ICLR_BRKIF_Msk;
}

/*****************************************************************************
 ** \brief	 EPWM_GetBrakeFlag
 **			获取刹车信号标志位
 ** \param [in] none	
 ** \return 1：发生故障或者刹车信号保持有效  0：无故障
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetBrakeFlag(void)
{
	return ((EPWM->BRKCTL&EPWM_BRKCTL_BRKAF_Msk)? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearBrake
 **			清除刹车保护位
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearBrake(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->BRKCTL |=	EPWM_BRKCTL_BRKCLR_Msk;
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** \brief	 EPWM_EnableHallErrorInt
 **			 开启Hall状态错误中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableHallErrorInt(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC |= EPWM_IMSC_ENHALLIF_Msk;
	EPWM->LOCK = 0x0;		
}
/*****************************************************************************
 ** \brief	 EPWM_DisableHallErrorInt
 **			 关闭Hall状态错误中断
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableHallErrorInt(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC &= ~(EPWM_IMSC_ENHALLIF_Msk);
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_GetHallErrorIntFlag
 **			获取Hall状态错误中断使能并产生中断标志位
 ** \param [in] none	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetHallErrorIntFlag(void)
{
	return ((EPWM->MIS&EPWM_MIS_HALLIF_Msk)? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearHallErrorIntFlag
 **			清除Hall状态错误中断标志位
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearHallErrorIntFlag(void)
{
	EPWM->ICLR |= (EPWM_ICLR_HALLIF_Msk);
}

/*****************************************************************************
 ** \brief	 EPWM_EnableUpCmpInt
 **			 开启向上比较中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableUpCmpInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC |= (EPWMnMask<<EPWM_IMSC_ENUIFn_Pos);
	EPWM->LOCK = 0x0;		
}
/*****************************************************************************
 ** \brief	 EPWM_DisableUpCmpInt
 **			 关闭向上比较中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableUpCmpInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC &= ~(EPWMnMask<<EPWM_IMSC_ENUIFn_Pos);
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_GetUpCmpIntFlag
 **			获取向上比较中断使能并产生中断标志位
 ** \param [in]EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetUpCmpIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_UIFn_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearUpCmpIntFlag
 **			清除向上比较中断标志位
 ** \param [in]EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearUpCmpIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_UIFn_Pos));
}

/*****************************************************************************
 ** \brief	 EPWM_EnableDownCmpInt
 **			 开启向下比较中断
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableDownCmpInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC |= (EPWMnMask<<EPWM_IMSC_ENDIFn_Pos);
	EPWM->LOCK = 0x0;		
}
/*****************************************************************************
 ** \brief	 EPWM_DisableDownCmpInt
 **			 关闭向下比较中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableDownCmpInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC &= ~(EPWMnMask<<EPWM_IMSC_ENDIFn_Pos);
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_GetDownCmpIntFlag
 **			获取向下比较中断使能并产生中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetDownCmpIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_DIFn_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearDownCmpIntFlag
 **			清除向下比较中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearDownCmpIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_DIFn_Pos));	
}


/*****************************************************************************
 ** \brief	 EPWM_EnablePeriodInt
 **			 开启周期中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnablePeriodInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC |= (EPWMnMask<<EPWM_IMSC_ENPIFn_Pos);
	EPWM->LOCK = 0x0;		
}
/*****************************************************************************
 ** \brief	 EPWM_DisablePeriodInt
 **			 关闭周期中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisablePeriodInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC &= ~(EPWMnMask<<EPWM_IMSC_ENPIFn_Pos);
	EPWM->LOCK = 0x0;		
}
/*****************************************************************************
 ** \brief	 EPWM_GetPeriodIntFlag
 **			获取周期中断使能并产生中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetPeriodIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_PIFn_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearPeriodIntFlag
 **			清除周期中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearPeriodIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_PIFn_Pos));
}

/*****************************************************************************
 ** \brief	 EPWM_EnableZeroInt
 **			 开启零点中断
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableZeroInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC |= (EPWMnMask<<EPWM_IMSC_ENZIFn_Pos);
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_DisableZeroInt
 **			 关闭零点中断
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableZeroInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC &= ~(EPWMnMask<<EPWM_IMSC_ENZIFn_Pos);
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_GetZeroIntFlag
 **			获取零点中断使能并产生中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetZeroIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_ZIFn_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearZeroIntFlag
 **			清除零点中断标志位
 ** \param [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearZeroIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_ZIFn_Pos));
}


/*****************************************************************************
 ** \brief	 EPWM_EnableCountCmpInt
 **			 开启计数比较器中断
 ** \param [in] EPWMCmp: EPWM_CMPTG_0 EPWM_CMPTG_1
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableCountCmpInt(uint32_t EPWMCmp)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC |= (0x1<<(EPWMCmp+EPWM_IMSC_ENDC0IF_Pos));
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_DisableCountCmp0Int
 **			 关闭计数比较器中断
 ** \param [in] EPWMCmp: EPWM_CMPTG_0 EPWM_CMPTG_1	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableCountCmpInt(uint32_t EPWMCmp)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IMSC &= ~(0x1<<(EPWMCmp+EPWM_IMSC_ENDC0IF_Pos));
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_GetCountCmpIntFlag
 **			获取计数比较器中断使能并产生中断标志位
 ** \param [in] EPWMCmp: EPWM_CMPTG_0 EPWM_CMPTG_1	
 ** \return 1：中断使能并且产生中断  0：无中断
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetCountCmpIntFlag(uint32_t EPWMCmp)
{
	return((EPWM->MIS & (0x1<<(EPWMCmp+EPWM_MIS_DC0IF_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearCountCmp0IntFlag
 **			清除计数比较器中断标志位
 ** \param [in] EPWMCmp: EPWM_CMPTG_0 EPWM_CMPTG_1	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearCountCmpIntFlag(uint32_t EPWMCmp)
{
	EPWM->ICLR |= (0x1<<(EPWMCmp+EPWM_ICLR_DC0IF_Pos));
}


/*****************************************************************************
 ** \brief	 EPWM_EnableBrakeIntSum
 **			 开启故障刹车中断累计功能
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableBrakeIntSum(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IFA |= (EPWM_IFA_BRKIFAEN_Msk);
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** \brief	 EPWM_DisableBrakeIntSum
 **			 关闭故障刹车中断累计功能
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableBrakeIntSum(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IFA &= ~(EPWM_IFA_BRKIFAEN_Msk);
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_ConfigBrakeIntSum
 **			 设置故障刹车中断累计次数
 ** \param [in] count : 0~0xf	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ConfigBrakeIntSum(uint32_t count)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IFA &= ~(EPWM_IFA_BRKIFCMP_Msk);
	EPWM->IFA |= ((0xf & count) << EPWM_IFA_BRKIFCMP_Pos);		
	EPWM->LOCK = 0x0;	
}


/*****************************************************************************
 ** \brief	 EPWM_EnableZeroIntSum
 **			 开启零点中断累计功能
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_EnableZeroIntSum(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IFA |= (EPWM_IFA_ZIFAEN_Msk);
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_DisableZeroIntSum
 **			 关闭零点中断累计功能
 ** \param [in] none	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_DisableZeroIntSum(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IFA &= ~(EPWM_IFA_ZIFAEN_Msk);
	EPWM->LOCK = 0x0;	
}
/*****************************************************************************
 ** \brief	 EPWM_ConfigZeroIntSum
 **			 设置零点中断累计次数
 ** \param [in] count : 0~0xf	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ConfigZeroIntSum(uint32_t count)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->IFA &= ~(EPWM_IFA_ZIFCMP_Msk);
	EPWM->IFA |= ((0xf & count) << EPWM_IFA_ZIFCMP_Pos);		
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** \brief	 EPWM_EnableIntAccompanyWithLoad
 **			 中断产生伴随着加载动作
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableIntAccompanyWithLoad(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON3 &= ~(EPWM_CON3_LOADNWINT_Msk);
	EPWM->LOCK = 0x0;			
}
/*****************************************************************************
 ** \brief	 EPWM_DisableIntAccompanyWithLoad
 **			 中断产生与加载动作无关
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableIntAccompanyWithLoad(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON3 |= EPWM_CON3_LOADNWINT_Msk;
	EPWM->LOCK = 0x0;		
}


/*****************************************************************************
 ** \brief	 EPWM_EnableDebugSuspend
 **			 使能调试暂停
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_EnableDebugSuspend(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON  |= (EPWM_CON_HALTMS_Msk);
	EPWM->LOCK = 0x0;			
}
/*****************************************************************************
 ** \brief	 EPWM_DisableDebugSuspend
 **			 关闭调试暂停
 ** \param [in] none
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_DisableDebugSuspend(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON  &= ~(EPWM_CON_HALTMS_Msk);
	EPWM->LOCK = 0x0;		
}

