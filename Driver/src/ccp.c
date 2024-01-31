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
/** \file ccp.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "ccp.h"
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
 ** \brief	CCP_Start
 **			����CCPģ��
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_Start(uint32_t CCPn )
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 |= (CCP_CCPCON0_CCP0EN_Msk);			
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 |= (CCP_CCPCON1_CCP1EN_Msk);		
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	CCP_Stop
 **			�ر�CCPģ��
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_Stop(uint32_t CCPn )
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0EN_Msk);			
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1EN_Msk);		
	}	
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_ConfigCLK
 **			����CCPnģ��ʱ��
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				ClkDiv:(1) CCP_CLK_DIV_1
 **					   (2) CCP_CLK_DIV_4
 **					   (3) CCP_CLK_DIV_16
 **					   (4) CCP_CLK_DIV_64
 **				ReloadMode: (1) CCP_RELOAD_0XFFFF    �������ؼ���ֵΪ0xFFFF (����ģʽ)
 **							(2) CCP_RELOAD_CCPLOAD   ���������ؼ���ֵΪCCPxLOAD
 **				Preiod: 0x00~ 0xffff; ���������ֵ��Ϊ0��		
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ConfigCLK(uint32_t CCPn, uint32_t ClkDiv ,uint32_t ReloadMode, uint32_t Period)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0PS_Msk);		
		CCP->CON0 |= (ClkDiv<< CCP_CCPCON0_CCP0PS_Pos);
		CCP->LOAD0 = ((uint16_t)Period) | (ReloadMode<< CCP_CCPLOAD0_RELOAD_Pos);		
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1PS_Msk);		
		CCP->CON1 |= (ClkDiv<< CCP_CCPCON1_CCP1PS_Pos);	
		CCP->LOAD1 = ((uint16_t)Period) | (ReloadMode<< CCP_CCPLOAD1_RELOAD_Pos);	
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	CCP_EnableRun
 **			CCPn����
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableRun(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->RUN |= (0x1<<CCPn); 					
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_DisableRun
 **			CCPn�ر�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableRun(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->RUN &= ~(0x1<<CCPn); 					
	CCP->LOCK = 0x00;
}


/*****************************************************************************
 ** \brief	CCP_EnablePWMMode
 **			����PWM����ģʽ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnablePWMMode(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 |= CCP_CCPCON0_CCP0MS_Msk;		/*ѡ��ΪPWMģʽ*/
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 |= CCP_CCPCON1_CCP1MS_Msk;		/*ѡ��ΪPWMģʽ*/
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	CCP_EnablePWMHalfCycleMode
 **			����PWM����ģʽ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB	
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnablePWMHalfCycleMode(uint32_t CCPn, uint32_t CCPChannel)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;

	if(CCP0 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->CON0 |= CCP_CCPCON0_CCP0ZAEN_Msk;		
		}
		if( CCPxB == CCPChannel)
		{
			CCP->CON0 |= CCP_CCPCON0_CCP0ZBEN_Msk;		
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->CON1 |= CCP_CCPCON1_CCP1ZAEN_Msk;		
		}
		if( CCPxB == CCPChannel)
		{
			CCP->CON1 |= CCP_CCPCON1_CCP1ZBEN_Msk;	
		}	
	}	
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_DisablePWMHalfCycleMode
 **			�ر�PWM����ģʽ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB	
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisablePWMHalfCycleMode(uint32_t CCPn, uint32_t CCPChannel)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;

	if(CCP0 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->CON0 &= ~CCP_CCPCON0_CCP0ZAEN_Msk;		
		}
		if( CCPxB == CCPChannel)
		{
			CCP->CON0 &= ~CCP_CCPCON0_CCP0ZBEN_Msk;		
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->CON1 &= ~CCP_CCPCON1_CCP1ZAEN_Msk;		
		}
		if( CCPxB == CCPChannel)
		{
			CCP->CON1 &= ~CCP_CCPCON1_CCP1ZBEN_Msk;	
		}	
	}	
	CCP->LOCK = 0x00;		
}


/*****************************************************************************
 ** \brief	CCP_ConfigCompare
 **			����CCPnģ��ͨ��n �Ƚ�ֵ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB			
 **				Compare : 0x00~ 0xffff;
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ConfigCompare(uint32_t CCPn, uint32_t CCPChannel ,uint32_t Compare)
{
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	if(CCP0 == CCPn)
	{

		if( CCPxA == CCPChannel)
		{
			CCP->D0A &= ~(CCP_CCPD0A_DATA_Msk); 
			CCP->D0A |= Compare;
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D0B &= ~(CCP_CCPD0B_DATA_Msk); 
			CCP->D0B |= Compare;
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D1A &= ~(CCP_CCPD1A_DATA_Msk); 
			CCP->D1A |= Compare;
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D1B &= ~(CCP_CCPD1B_DATA_Msk); 
			CCP->D1B |= Compare;
		}
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	CCP_EnableReverseOutput
 **			�����������
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableReverseOutput(uint32_t CCPn, uint32_t CCPChannel)
{
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D0A |= CCP_CCPD0A_PWM0AOP_Msk;
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D0B |= CCP_CCPD0B_PWM0BOP_Msk;
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D1A |= CCP_CCPD1A_PWM1AOP_Msk;
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D1B |= CCP_CCPD1B_PWM1BOP_Msk;
		}
	}	
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_DisableReverseOutput
 **			�رշ������
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableReverseOutput(uint32_t CCPn, uint32_t CCPChannel)
{
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D0A &= ~(CCP_CCPD0A_PWM0AOP_Msk);
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D0B &= ~(CCP_CCPD0B_PWM0BOP_Msk);
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D1A &= ~(CCP_CCPD1A_PWM1AOP_Msk);
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D1B &= ~(CCP_CCPD1B_PWM1BOP_Msk);
		}
	}	
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_EnableOverflowInt
 **			��������ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableOverflowInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< (CCPn+4));
	CCP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	CCP_DisableOverflowInt
 **			�ر�����ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableOverflowInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< (CCPn+4));
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_GetPWMOverflowIntFlag
 **			��ȡ����ж�ʹ�ܲ�����жϱ�־λ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  1:����ж�ʹ�ܲ��Ҳ����ж�  0:���ж�
 ** \note	
*****************************************************************************/
uint32_t CCP_GetOverflowIntFlag(uint32_t CCPn)
{
	return((CCP->MIS & (0x1UL<<(CCPn + 4)))? 1:0);
}
/*****************************************************************************
 ** \brief	CCP_ClearPWMOverflowIntFlag
 **			�������жϱ�־λ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ClearOverflowIntFlag(uint32_t CCPn)
{
	CCP->ICLR |= (0x1UL<< (CCPn+4));	
}

/*****************************************************************************
 ** \brief	CCP_EnableCompareInt
 **			����PWM�Ƚ��ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableCompareInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< CCPn);
	CCP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	CCP_DisableCompareInt
 **			�رձȽ��ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableCompareInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< CCPn);
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_GetCompareIntFlag
 **			��ȡ�Ƚ��ж�ʹ�ܲ������жϱ�־
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCompareIntFlag(uint32_t CCPn)
{
	return((CCP->MIS & (0x1UL<<CCPn))? 1:0);
}
/*****************************************************************************
 ** \brief	CCP_ClearCompareIntFlag
 **			����Ƚ��жϱ�־
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ClearCompareIntFlag(uint32_t CCPn)
{
	CCP->ICLR |= (0x1UL<< CCPn);		
}



/*****************************************************************************
 ** \brief	CCP_EnableCAPMode0
 **			��������ģʽ0
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPMode0(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP ->CAPCON &= ~(CCP_CAPCON_CAPEN_Msk);

	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0MS_Msk);		
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1MS_Msk);		
	}	
	CCP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	CCP_ConfigCAPMode0
 **			���ò���ģʽ0 �Ĳ���ʽ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��	
 **				Mode : (1)CCP_CAP_RISING
 **					   (2)CCP_CAP_FALLING
 **					   (3)CCP_CAP_RISING_TO_FALLING
 **					   (4)CCP_CAP_FALLING_TO_RISING
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_ConfigCAPMode0(uint32_t CCPn, uint32_t Mode)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0CM0ES_Msk); 
		CCP->CON0 |= Mode;
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1CM0ES_Msk); 
		CCP->CON1 |= Mode;
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	CCP_SelCAPMode0Channel
 **			ѡ�񲶻�ģʽ0 �Ĳ���ͨ��
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��	
**				Channel : CCPxA ��CCPxB 
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_SelCAPMode0Channel(uint32_t CCPn, uint32_t Channel)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0CM0CS_Msk); 
		CCP->CON0 |= (Channel<< CCP_CCPCON0_CCP0CM0CS_Pos);
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1CM0CS_Msk); 
		CCP->CON1 |= (Channel<< CCP_CCPCON1_CCP1CM0CS_Pos);
	}	
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_GetCAPMode0Result
 **			��ȡ����ģʽ0 �Ĳ���ͨ��ֵ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��	
**				Channel : CCPxA ��CCPxB 
 ** \return  16bit result
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCAPMode0Result(uint32_t CCPn, uint32_t Channel)
{
	if(CCP0 == CCPn)
	{
		if( CCPxA == Channel)
		{
			return ((uint16_t)(CCP->D0A));
		}
		if( CCPxB == Channel)
		{
			return ((uint16_t)(CCP->D0B));
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == Channel)
		{
			return ((uint16_t)(CCP->D1A));
		}
		if( CCPxB == Channel)
		{
			return ((uint16_t)(CCP->D1B));
		}
	}	
	return 0;
}

/*****************************************************************************
 ** \brief	CCP_EnableCAPMode0Int
 **			��������ģʽ�����ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableCAPMode0Int(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< CCPn);
	CCP->LOCK = 0x00;			
}

/*****************************************************************************
 ** \brief	CCP_DisableCAPMode0Int
 **			�رղ���ģʽ�����ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableCAPMode0Int(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< CCPn);
	CCP->LOCK = 0x00;
}

/*****************************************************************************
 ** \brief	CCP_GetCAPMode0IntFlag
 **			��ȡ����ģʽ�����ж�ʹ�ܲ��Ҳ����ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return 1�������ж�  0:���ж�
 ** \note	
*****************************************************************************/
uint32_t CCP_GetCAPMode0IntFlag(uint32_t CCPn)
{
	return((CCP->MIS & (0x1UL<<CCPn))? 1:0);
}

/*****************************************************************************
 ** \brief	CCP_ClearCAPMode0IntFlag
 **			�������ģʽ�����жϱ�־λ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return none
 ** \note	
*****************************************************************************/
void CCP_ClearCAPMode0IntFlag(uint32_t CCPn)
{
	CCP->ICLR |= (0x1UL<< CCPn);	
}




/*****************************************************************************
 ** \brief	CCP_EnableCAPMode1
 **			��������ģʽ1 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPMode1(void)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON |= CCP_CAPCON_CAPEN_Msk;
	CCP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode1
 **			�رղ���ģʽ1 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_DisableCAPMode1(void)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~(CCP_CAPCON_CAPEN_Msk);
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	CCP_EnableCAPnReload
 **			��������ģʽ1��CAPn���񴥷�CCP0����
 ** \param [in] CAPn: CAP0��CAP1��CAP2��CAP3 
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPnReload(uint32_t CAPn)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON |= (0x1<<(CCP_CAPCON_CAP0RLEN_Pos + CAPn));
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	CCP_DisableCAPnReload
 **			�رղ���ģʽ1��CAPn���񴥷�CCP0����
 ** \param [in] CAPn: CAP0��CAP1��CAP2��CAP3 
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_DisableCAPnReload(uint32_t CAPn)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~((0x1<<(CCP_CAPCON_CAP0RLEN_Pos + CAPn)));
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	CCP_ConfigCAPMode1
 **			���ò���ģʽ1 ͨ������ģʽ
 ** \param [in] CAPn: CAP0��CAP1��CAP2��CAP3 
 **				Mode:(1)CCP_CAP_MODE1_RISING
 **					 (2)CCP_CAP_MODE1_FALLING
 **					 (3)CCP_CAP_MODE1_BOTH_EDGE
 **					 (4)CCP_CAP_MODE1_DISABLE
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_ConfigCAPMode1(uint32_t CAPn, uint32_t Mode)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~(0x3UL<<(CAPn *2));
	CCP->CAPCON |= (Mode<<(CAPn *2)) ;
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	CCP_SelCAPMode1Channel
 **			ѡ�񲶻�ģʽ1 �Ĳ���ͨ��
 ** \param [in] CAPn: CAP0��CAP1��CAP2��CAP3 
 **				Channel :(1) ECAP00 ~ ECAP03
 **						 (2) ECAP10 ~ ECAP13
 **						 (3) CAP0_CCP0A ��CAP1_CCP0B��CAP2_CCP1A��CAP3_CCP1B
 **						 (4) CAP3_ACMP0��CAP3_ACMP1 ��  CAP3ͨ������
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_SelCAPMode1Channel(uint32_t CAPn, uint32_t Channel)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	switch(Channel)
	{
		case ECAP00:
		case ECAP01:
		case ECAP02:	
		case ECAP03:	
			CCP->CAPCHS &= ~(CCP_CAPCHS_ECAPS_Msk);
			CCP->CAPCHS &= ~(0x0FUL<<(CAPn * 4));
			CCP->CAPCHS |= Channel<<(CAPn * 4);
			break;
		case ECAP10:
		case ECAP11:
		case ECAP12:
		case ECAP13:
			CCP->CAPCHS |= CCP_CAPCHS_ECAPS_Msk;
			CCP->CAPCHS &= ~(0x0FUL<<(CAPn * 4));
			CCP->CAPCHS |= (Channel & 0xF)<<(CAPn * 4);
			break;
		case CAP0_CCP0A:
		case CAP1_CCP0B:
		case CAP2_CCP1A:
		case CAP3_CCP1B:
			CCP->CAPCHS &= ~(0x0FUL<<(CAPn * 4));
			CCP->CAPCHS |= (Channel & 0xF)<<(CAPn * 4);		
			 break;
		case CAP3_ACMP0:
		case CAP3_ACMP1:	
			CCP->CAPCHS &= ~(CCP_CAPCHS_CAP3CHS_Msk);
			CCP->CAPCHS |= (Channel & 0xF)<< CCP_CAPCHS_CAP3CHS_Pos;
			break;
		default:
			break;
	}
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	CCP_TriggerCAMMode1
 **			д�Ĵ�����������ģʽ1������
 ** \param [in] CAPn ��CAP0 ��CAP1��CAP2 ��CAP3
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_TriggerCaputer(uint32_t CAPn)
{
	uint32_t * Point;
	Point = (uint32_t *)& CCP->CAP0DAT0;
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	*(Point + CAPn) = 0x55AA0000;
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_GetCAPMode1Result
 **			��ȡ����ģʽ1 �Ĳ���ͨ��ֵ
 ** \param [in] CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  16bit result
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCAPMode1Result(uint32_t CAPn)
{
	uint32_t * Point;
	Point = (uint32_t *)&CCP->CAP0DAT0;
	return(0xFFFF & Point[CAPn]);
}

/*****************************************************************************
 ** \brief	CCP_EnableCAPMode1Int
 **			��������ģʽ1�ж�
 ** \param [in]CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_EnableCAPMode1Int(uint32_t CAPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< (CAPn+8));
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode1Int
 **			�رղ���ģʽ1�ж�
 ** \param [in]CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_DisableCAPMode1Int(uint32_t CAPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< (CAPn+8));
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_GetCAPMode1IntFlag
 **			��ȡ����ģʽ1�ж�ʹ�ܲ��Ҳ����жϱ�־λ
 ** \param [in]CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  1:�ж�ʹ�ܲ��Ҳ����ж�   0�����ж�
 ** \note	 
*****************************************************************************/
uint32_t  CCP_GetCAPMode1IntFlag(uint32_t CAPn)
{
	return((CCP->MIS & (0x1UL<<(CAPn+8)))? 1:0);
}
/*****************************************************************************
 ** \brief	CCP_ClearCAPMode1IntFlag
 **			�������ģʽ1�жϱ�־λ
 ** \param [in]CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void  CCP_ClearCAPMode1IntFlag(uint32_t CAPn)
{
	CCP->ICLR |= (0x1UL<< (CAPn+8));
}




/*****************************************************************************
 ** \brief	CCP_EnableCAPMode2
 **			��������ģʽ2 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPMode2(void)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON |= CCP_CAPCON_CAPEN2_Msk;
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode1
 **			�رղ���ģʽ2 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_DisableCAPMode2(void)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~(CCP_CAPCON_CAPEN2_Msk);
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	CCP_ConfigCAPMode2
 **			���ò���ģʽ2 ͨ������ģʽ
 ** \param [in] CAPn: CAP1��CAP2��CAP3 
 **				Mode:(1)CCP_CAP_MODE2_RISING
 **					 (2)CCP_CAP_MODE2_FALLING
 **					 (3)CCP_CAP_MODE2_BOTH_EDGE
 **					 (4)CCP_CAP_MODE2_DISABLE
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_ConfigCAPMode2(uint32_t CAPn, uint32_t Mode)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~(0x3UL<<(CAPn *2));
	CCP->CAPCON |= (Mode<<(CAPn *2)) ;
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	CCP_SelCAPMode2Channe2
 **			ѡ�񲶻�ģʽ2 �Ĳ���ͨ��
 ** \param [in] Channel :(1) ECAP00 ~ ECAP03
 **						 (2) ECAP10 ~ ECAP13
 **						 (3) CAP1_CCP0B
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_SelCAPMode2Channel(uint32_t Channel)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	switch(Channel)
	{
		case ECAP00:
		case ECAP01:
		case ECAP02:	
		case ECAP03:	
			CCP->CAPCHS &= ~(CCP_CAPCHS_ECAPS_Msk);
			CCP->CAPCHS &= ~(0x0FUL<<(CCP_CAPCHS_CAP1CHS_Pos));
			CCP->CAPCHS |= Channel<<(CCP_CAPCHS_CAP1CHS_Pos);
			break;
		case ECAP10:
		case ECAP11:
		case ECAP12:
		case ECAP13:
			CCP->CAPCHS |= CCP_CAPCHS_ECAPS_Msk;
			CCP->CAPCHS &= ~(0x0FUL<<(CCP_CAPCHS_CAP1CHS_Pos));
			CCP->CAPCHS |= (Channel & 0xF)<<(CCP_CAPCHS_CAP1CHS_Pos);
			break;
		case CAP1_CCP0B:
			CCP->CAPCHS &= ~(0x0FUL<<(CCP_CAPCHS_CAP1CHS_Pos));
			CCP->CAPCHS |= (Channel & 0xF)<<(CCP_CAPCHS_CAP1CHS_Pos);		
			 break;
		default:
			break;
	}
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	CCP_TriggerCAMMode2
 **			д�Ĵ�����������ģʽ2������
 ** \param [in] CAPn :CAP1��CAP2 ��CAP3
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_TriggerCaputer2(uint32_t CAPn)
{
	uint32_t * Point;
	Point = (uint32_t *)& CCP->CAP0DAT0;
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	*(Point + CAPn) = 0x55AA0000;
	CCP->LOCK = 0x00;
}


/*****************************************************************************
 ** \brief	CCP_GetCAPMode1Result
 **			��ȡ����ģʽ2 �Ĳ���ͨ��ֵ
 ** \param [in] CAPn ��(1)CAP1_SUB_CAP2, CAP1_SUB_CAP3
 **					   (2)CAP1��CAP2 ��CAP3	
 ** \return  16bit result
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCAPMode2Result(uint32_t CAPn)
{
	uint32_t temp;
	
	switch(CAPn)
	{
		case  CAP1_SUB_CAP2:
			temp = 0xFFFF & CCP->CAP0DAT0;
		break;
		
		case  CAP1_SUB_CAP3:
			temp = 0xFFFF & ((CCP->CAP0DAT0)>>16);			
		break;
		case  CAP1:
			temp = 0xFFFF & CCP->CAP1DAT0;			
		break;
		case  CAP2:
			temp = 0xFFFF & CCP->CAP2DAT0;		
		break;
		case  CAP3:
			temp = 0xFFFF & CCP->CAP3DAT0;			
		break;			
	}	
	
	return temp;
}


/*****************************************************************************
 ** \brief	CCP_EnableCAPMode2Int
 **			��������ģʽ2�ж�
 ** \param [in]CAPn ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_EnableCAPMode2Int(uint32_t CAPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< (CAPn+8));
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode2Int
 **			�رղ���ģʽ2�ж�
 ** \param [in]CAPn ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_DisableCAPMode2Int(uint32_t CAPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< (CAPn+8));
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_GetCAPMode2IntFlag
 **			��ȡ����ģʽ2�ж�ʹ�ܲ��Ҳ����жϱ�־λ
 ** \param [in]CAPn ��CAP1��CAP2 ��CAP3	
 ** \return  1:�ж�ʹ�ܲ��Ҳ����ж�   0�����ж�
 ** \note	 
*****************************************************************************/
uint32_t  CCP_GetCAPMode2IntFlag(uint32_t CAPn)
{
	return((CCP->MIS & (0x1UL<<(CAPn+8)))? 1:0);
}
/*****************************************************************************
 ** \brief	CCP_ClearCAPMode2IntFlag
 **			�������ģʽ2�жϱ�־λ
 ** \param [in]CAPn ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void  CCP_ClearCAPMode2IntFlag(uint32_t CAPn)
{
	CCP->ICLR |= (0x1UL<< (CAPn+8));
}


