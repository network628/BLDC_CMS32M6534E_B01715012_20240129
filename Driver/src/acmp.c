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
/** \file acmp.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "acmp.h"
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
void ACMP_ConfigPositive( uint32_t ACMPx, uint32_t PositiveSource)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 &= ~(ACMP_C0CON0_PS_Msk);
		ACMP->C0CON0 |= (PositiveSource<<ACMP_C0CON0_PS_Pos);	
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 &= ~(ACMP_C1CON0_PS_Msk);
		ACMP->C1CON0 |= (PositiveSource<<ACMP_C1CON0_PS_Pos);					
	}
	ACMP->LOCK = 0x00;	
}



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
void ACMP_ConfigNegative( uint32_t ACMPx, uint32_t NegativeSource)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 &= ~(ACMP_C0CON0_NS_Msk);
		ACMP->C0CON0 |= NegativeSource;

	}	
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 &= ~(ACMP_C1CON0_NS_Msk);
		if((ACMP_NEGSEL_N == NegativeSource) || (ACMP_NEGSEL_DAC_O  == NegativeSource))
		{
			ACMP->C1CON0 |= NegativeSource;
		}
	}	
	

	ACMP->LOCK = 0x00;	
}



/*****************************************************************************
 ** \brief	 ACMP_EnableEventOut
 **			 开启比较器事件输出
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
 *****************************************************************************/
void ACMP_EnableEventOut( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->CEVCON |=  ACMP_CEVCON_EVE0_Msk;		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->CEVCON |=  ACMP_CEVCON_EVE1_Msk ;	
	}		
	ACMP->LOCK = 0x00;	
}

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

void ACMP_ConfigEventAndIntMode( uint32_t ACMPx, uint32_t EventAndIntMode)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->CEVCON &= ~(ACMP_CEVCON_EVS0_Msk);
		ACMP->CEVCON |=  EventAndIntMode;		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->CEVCON &= ~(ACMP_CEVCON_EVS1_Msk);
		ACMP->CEVCON |= (EventAndIntMode<< ACMP_CEVCON_EVS1_Pos);		
	}		
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	 ACMP_DisableEventOut
 **			 关闭比较器事件输出
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
 *****************************************************************************/
void ACMP_DisableEventOut( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->CEVCON &= ~(ACMP_CEVCON_EVE0_Msk);		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->CEVCON &= ~(ACMP_CEVCON_EVE1_Msk);	
	}			
	ACMP->LOCK = 0x00;	
}
/******************************************************************************
 ** \brief	 ACMP_EnableInt
 **			 开启ACMP中断
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
 *****************************************************************************/
void ACMP_EnableInt(uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	ACMP->IMSC |=(0x1<<ACMPx);				
	ACMP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	 ACMP_DisableInt
 **			 关闭ACMP中断
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
*****************************************************************************/
void ACMP_DisableInt( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	ACMP->IMSC &= ~(0x1<<ACMPx);				
	ACMP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	 ACMP_GetIntFlag
 **			 获取ACMP已使能中断标志
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  0: 无中断产生 ， 1：中断使能并产生中断
 ** \note   
******************************************************************************/
uint32_t ACMP_GetIntFlag( uint32_t ACMPx)
{
	return((ACMP->MIS &(0x1<<ACMPx))? 1:0);
}
/*****************************************************************************
 ** \brief	 ACMP_ClearIntFlag
 **			 清ACMP中断
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
******************************************************************************/
void ACMP_ClearIntFlag( uint32_t ACMPx)
{
	ACMP->ICLR |=(0x1<<ACMPx);
}

/*****************************************************************************
 ** \brief	 ACMP_Start
 **			 开启ACMP
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
******************************************************************************/
void ACMP_Start( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 |= ACMP_C0CON0_EN_Msk;		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 |= ACMP_C1CON0_EN_Msk;	
	}			
	ACMP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	 ACMP_Stop
 **			 关闭ACMP
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
**********************************************************************?******/
void ACMP_Stop( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 &= ~(ACMP_C0CON0_EN_Msk);		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 &= ~(ACMP_C1CON0_EN_Msk);	
	}			
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	 ACMP0_CenterAliagn_Enable
 **			 开启ACMP0的中心对齐
 ** \param [in] none
 ** \return  none
 ** \note   
******************************************************************************/
void ACMP0_CenterAliagn_Enable(void)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	ACMP->C0CON0 |= ACMP_C0CON0_ZXD_Msk;				
	ACMP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	 ACMP0_CenterAliagn_Disable
 **			 关闭ACMP0的中心对齐
 ** \param [in] none
 ** \return  none
 ** \note   
**********************************************************************?******/
void ACMP0_CenterAliagn_Disable(void)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	ACMP->C0CON0 &= ~(ACMP_C0CON0_ZXD_Msk);		
	ACMP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	 ACMPOut_Enable
 **			 使能ACMP输出
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
******************************************************************************/
void ACMPOut_Enable( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 |= ACMP_C0CON0_OEN_Msk;
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 |= ACMP_C1CON0_OEN_Msk;	
	}			
	ACMP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	 ACMPOut_Disable
 **			 关闭ACMP输出
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note   
**********************************************************************?******/
void ACMPOut_Disable( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 &= ~(ACMP_C0CON0_OEN_Msk);		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 &= ~(ACMP_C1CON0_OEN_Msk);	
	}			
	ACMP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	 ACMP_GetResult
 **			 获取比较器值
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  0/1
 ** \note   
 *****************************************************************************/
uint32_t ACMP_GetResult( uint32_t ACMPx)
{
	if( ACMP0 == ACMPx )
	{
		return ((ACMP->C0CON0 & ACMP_C0CON0_C0OUT_Msk)? 1:0);		
	}
	if( ACMP1 == ACMPx )
	{
		return ((ACMP->C1CON0 & ACMP_C1CON0_C1OUT_Msk)? 1:0);		
	}
	return 0;
}



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
void ACMP_EnableHYS( uint32_t ACMPx ,uint32_t HYSSelect, uint32_t HYSValue)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON1 &= ~(ACMP_C0CON1_HYSPN_Msk | ACMP_C0CON1_HYSV_Msk);	
		ACMP->C0CON1 |= (HYSSelect<< ACMP_C0CON1_HYSPN_Pos) | (HYSValue<<ACMP_C0CON1_HYSV_Pos);
	}
	if( ACMP1 == ACMPx )
	{		
		ACMP->C1CON1 &= ~(ACMP_C1CON1_HYSPN_Msk | ACMP_C1CON1_HYSV_Msk);
		ACMP->C1CON1 |= (HYSSelect<< ACMP_C1CON1_HYSPN_Pos) | (HYSValue<<ACMP_C1CON1_HYSV_Pos);			
	}			
	ACMP->LOCK = 0x00;
}

/*****************************************************************************
 ** \brief	 ACMP_DisableHYS
 **			 关闭比较器迟滞模式
 ** \param [in] ACMPx: ACMP0 ,ACMP1
 ** \return  none
 ** \note  
*****************************************************************************/
void ACMP_DisableHYS( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON1 &= ~(ACMP_C0CON1_HYSPN_Msk | ACMP_C0CON1_HYSV_Msk);	
	}
	if( ACMP1 == ACMPx )
	{		
		ACMP->C1CON1 &= ~(ACMP_C1CON1_HYSPN_Msk | ACMP_C1CON1_HYSV_Msk);		
	}			
	ACMP->LOCK = 0x00;
}

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
void ACMP_Filter_Config( uint32_t ACMPx,FunctionalState FiltSta,uint8_t FlitTim)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		if(FiltSta == ENABLE)
		{
			ACMP->C0CON1 &= ~ACMP_C0CON1_FS_Msk;
			ACMP->C0CON1 |= ACMP_C0CON1_FE_Msk | FlitTim;			
		}
		else
			ACMP->C0CON1 &= ~ACMP_C0CON1_FE_Msk;
		
	}
	if( ACMP1 == ACMPx )
	{
		if(FiltSta == ENABLE)
		{
			ACMP->C1CON1 &= ~ACMP_C1CON1_FS_Msk;
			ACMP->C1CON1 |= ACMP_C1CON1_FE_Msk | FlitTim;			
		}
		else
			ACMP->C1CON1 &= ~ACMP_C1CON1_FE_Msk;
	}
	ACMP->LOCK = 0x00;
}

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
void ACMP_Polarity_Config( uint32_t ACMPx,uint8_t PolaritySet)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON1 &= ~ACMP_C0CON1_POS_Msk;
		ACMP->C0CON1 |= PolaritySet << ACMP_C0CON1_POS_Pos;
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON1 &= ~ACMP_C1CON1_POS_Msk;
		ACMP->C1CON1 |= PolaritySet << ACMP_C1CON1_POS_Pos;
	}
	ACMP->LOCK = 0x00;
}



