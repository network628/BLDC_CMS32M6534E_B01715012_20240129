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
 **			 ����EPWM������ģʽ
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
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 
 **				ClkDiv: (1)EPWM_CLK_DIV_1 
 **						(2)EPWM_CLK_DIV_2
 **						(3)EPWM_CLK_DIV_4
 **						(4)EPWM_CLK_DIV_8
 **						(5)EPWM_CLK_DIV_16
 **						(6)EPWM_CLK_DIV_32
 ** \return  none	
 ** \note 
 **	(1)����ΪCLKPSC01��ͬ����PWM0��PWM1ͨ�� �����Դ˺���ͳһ����CLKPSC01=0x01
 **     ����Ҫ�����ķ�Ƶ��������ԼĴ�����ֵ.  
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
 **			 ����EPWMͨ��������
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 
 **				Period: 16λ����ֵ
 ** \return none
 ** \note    
*****************************************************************************/
void EPWM_ConfigChannelPeriod(uint32_t EPWMn, uint32_t Period)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->PERIOD[EPWMn] = 0xffff & Period;
	EPWM->CON3 |= (0x1UL <<(EPWMn + EPWM_CON3_LOADEN0_Pos));		/*ʹ�ܼ�������&&ռ�ձ�*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelSymDuty	
 **			 ���öԳ�ģʽ�µ�EPWMͨ����ռ�ձ�
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 
 **			    Duty  : 16λ�Ƚ�ֵ
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ConfigChannelSymDuty(uint32_t EPWMn,uint32_t Duty)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->CMPDAT[EPWMn] &= 0xffff0000;
	EPWM->CMPDAT[EPWMn] |= 0xffff & Duty;	
	EPWM->CON3 |= (0x1UL <<(EPWMn +EPWM_CON3_LOADEN0_Pos));		/*ʹ�ܼ�������&&ռ�ձ�*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_ConfigChannelAsymDuty	
 **			 ���÷ǶԳ�ģʽ�µ� EPWMͨ����ռ�ձ�
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 
 **			    UpCmp 	  : 16λ���ϱȽ�ֵ
 **			    DownCmp   : 16λ���±Ƚ�ֵ
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_ConfigChannelAsymDuty(uint32_t EPWMn,uint32_t UpCmp, uint32_t DownCmp)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->CMPDAT[EPWMn] = (0xffff & UpCmp) | ((0xffff & DownCmp)<<16);	
	EPWM->CON3 |= (0x1UL <<(EPWMn +EPWM_CON3_LOADEN0_Pos));		/*ʹ�ܼ�������&&ռ�ձ�*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_EnableOneShotMode	
 **			 ʹ�ܵ���ģʽ
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 ʹ����������ģʽ
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 ���ü��ط�ʽ�Լ��жϷ�ʽ
 ** \param [in]  EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 
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
 **			 ʹ���¼���������
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
 **			 �����������
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 �رշ������
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 ��������
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 �ر�����
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 ����ͨ����ӳ��
 ** \param [in]  EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 
 **				 IPGn  : IPG0 ��IPG1 ��IPG2 ��IPG3 ��IPG4 ��IPG5 
 ** \return none
 ** \note   IPGxָ�ڲ�ͨ����EPWMxָ�ⲿ���ͨ��
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
 **			 �ر�ͨ����ӳ��
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
 **			 ����EPWMͨ�����
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 �ر�EPWMͨ�����
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 ����EPWM
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 �ر�EPWM
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
 ** \return none
 ** \note   
*****************************************************************************/
void EPWM_Stop(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CON2 &= ~(EPWMnMask);			
	EPWM->CON3 |= EPWMnMask;			/*������0*/
	EPWM->LOCK = 0x0;
}
/*****************************************************************************
 ** \brief	 EPWM_ForceStop	
 **			 ǿ�ƹر�EPWM
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
 **			 ʹ������Ԥ����
 ** \param [in] LoadMode: (1)EPWM_MASK_LOAD_EPWM0	��EPWM0 �ļ��ص������������
 **					  	  (2)EPWM_MASK_LOAD_EPWM1	��EPWM1 �ļ��ص������������
 **					  	  (3)EPWM_MASK_LOAD_EPWM2	��EPWM2 �ļ��ص������������
 **					  	  (4)EPWM_MASK_LOAD_EPWM3	��EPWM3 �ļ��ص������������
 **					  	  (5)EPWM_MASK_LOAD_EPWM4	��EPWM4 �ļ��ص������������
 **					  	  (5)EPWM_MASK_LOAD_EPWM5	��EPWM5 �ļ��ص������������
 ** \return none
 ** \note  ���� MASKNXT �Ĵ��������ݼ��ص�MASK �Ĵ����У�ͬʱ��ֹд MASK �Ĵ��� 
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
 **			 �ر�����Ԥ����
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
 **			 д����Ԥ���ػ���
 ** \param [in] MaskBuffer:  EPWM_MASK_BUF0 ��EPWM_MASK_BUF1��EPWM_MASK_BUF2��EPWM_MASK_BUF3
 **							 EPWM_MASK_BUF4��EPWM_MASK_BUF5��EPWM_MASK_BUF6��EPWM_MASK_BUF7
 **				BufferValue: 16bit
 ** \return none
 ** \note   HALLEN=0 ʱ��Ĭ�ϼ�������Ԥ�軺��0�е����ݡ�
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
 **			 ʹ���������
 ** \param [in]EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
 **				MaskLevel:  0���͵�ƽ  1���ߵ�ƽ
 ** \return none
 ** \note   �����˺���ʱ��ر�����Ԥ����
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
 **			 �ر��������
 ** \param [in]EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
 ** \return none
 ** \note   �����˺���ʱ��ر�����Ԥ����
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
 **			 ���ô����Ƚ���
 ** \param [in]CmpTgn: EPWM_CMPTG_0��EPWM_CMPTG_1
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
 **			 ����Hallģʽ���
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
 **			 �ر�Hallģʽ���
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
 **			 ��ȡHall�ӿ�״̬
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
 **			 ���Hall�ӿڴ���״̬
 ** \param [in] none
 ** \return none
 ** \note  ������ֵĴ����״̬��������ʱ��HALLST=111��HALL ��⹦��ֹͣ��
 **			�ٴμ�⿪�� HALL ״̬ʱ����Ҫ��������״̬�� 
*****************************************************************************/
void  EPWM_ClearHallErrorState(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;		
	EPWM->MASKNXT |= EPWM_MASKNXT_HALLCLR_Msk;
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** \brief	 EPWM_AllBrakeEnable
 **			 ����ɲ����ʹ��
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
 **			�ر�ɲ����ʹ��
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
 **			����ɲ��ģʽ
 ** \param [in] mode :(1)EPWM_BRK_STOP				ֹͣģʽ
 **					  (2)EPWM_BRK_SUSPEND			��ͣģʽ
 **					  (3)EPWM_BRK_RECOVER			�ָ�ģʽ
 **					  (4)EPWM_BRK_DELAY_RECOVER		��ʱ�ָ�ģʽ
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
 **			���ûָ���ʱģʽ����ʱ
 ** \param [in] time : 0~0xffff
 ** \return none
 ** \note  ��ʱʱ��= RDT��TAPBCLK
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
 **			����ɲ���ָ���
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
 **			 ����ɲ��
 ** \param [in] BrakeSource:(1)EPWM_BRK_SOFT:���ɲ��
 **							(2)EPWM_BRK_EXTLE 		�ⲿӲ����ƽɲ��
 **							(3)EPWM_BRK_EXTEE 		�ⲿӲ������ɲ��
 **							(4)EPWM_BRK_ADCBCMP0 	ADCB�Ƚ���0ɲ��
 **							(5)EPWM_BRK_ADCBCMP1 	ADCB�Ƚ���1ɲ��
 **							(6)EPWM_BRK_ADC0CMP0 	ADC0�Ƚ���0ɲ��
 **							(7)EPWM_BRK_ACMP0LE 	ģ��Ƚ���0�����ƽɲ��
 **							(8)EPWM_BRK_ACMP1LE  	ģ��Ƚ���1�����ƽɲ��
 **							(9)EPWM_BRK_ACMP0EE  	ģ��Ƚ���0�¼�ɲ��
 **							(10)EPWM_BRK_ACMP1EE  	ģ��Ƚ���1�¼�ɲ��
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
 **			 �ر�ɲ��
 ** \param [in] BrakeSource:(1)EPWM_BRK_SOFT:���ɲ��
 **							(2)EPWM_BRK_EXTLE 		�ⲿӲ����ƽɲ��
 **							(3)EPWM_BRK_EXTEE 		�ⲿӲ������ɲ��
 **							(4)EPWM_BRK_ADCBCMP0 	ADCB�Ƚ���0ɲ��
 **							(5)EPWM_BRK_ADCBCMP1 	ADCB�Ƚ���1ɲ��
 **							(6)EPWM_BRK_ADC0CMP0 	ADC0�Ƚ���0ɲ��
 **							(7)EPWM_BRK_ACMP0LE 	ģ��Ƚ���0�����ƽɲ��
 **							(8)EPWM_BRK_ACMP1LE  	ģ��Ƚ���1�����ƽɲ��
 **							(9)EPWM_BRK_ACMP0EE  	ģ��Ƚ���0�¼�ɲ��
 **							(10)EPWM_BRK_ACMP1EE  	ģ��Ƚ���1�¼�ɲ��		
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
 **			 ����EPWMnɲ����ƽ
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
 **				BrakeLevel:  0���͵�ƽ  1���ߵ�ƽ			
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
 **			 �����ⲿӲ����ƽɲ��ģʽ
 ** \param [in] BrakeMode��	(1) EPWM_EXTBK_LEVEL_HIGH		�ߵ�ƽɲ��
 **					   		(2)	EPWM_EXTBK_LEVEL_LOW		�͵�ƽɲ��
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
 **			 �����ⲿӲ������ɲ��ģʽ
 ** \param [in] BrakeMode��	(1)	EPWM_EXTBK_EDGE_FALLING 	�½���ɲ��
 **					   		(2) EPWM_EXTBK_EDGE_RISING		������ɲ��
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
 **			 �����ɲ���ź��˲�
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
 **			 �����Ƚ���0�����ƽɲ��ģʽ
 ** \param [in] BrakeMode��	(1) EPWM_ACMP0BRK_LEVEL_HIGH		�Ƚ���0����ߵ�ƽɲ��
 **					   		(2)	EPWM_ACMP0BRK_LEVEL_LOW			�Ƚ���0����͵�ƽɲ��
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
 **			 �����Ƚ���1�����ƽɲ��ģʽ
 ** \param [in] BrakeMode��	(1)	EPWM_ACMP1BRK_LEVEL_HIGH 	�Ƚ���1����ߵ�ƽɲ��
 **					   		(2) EPWM_ACMP1BRK_LEVEL_LOW		�Ƚ���1����͵�ƽɲ��
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
 **			 ʹ�����ɲ��
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
 **			 �ر����ɲ��
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
 **			 ����ɲ���ж�
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
 **			 �ر�ɲ���ж�
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
 **			��ȡɲ���ж�ʹ�ܲ������жϱ�־λ
 ** \param [in] none	
 ** \return 1���ж�ʹ�ܲ��Ҳ����ж�  0�����ж�
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetBrakeIntFlag(void)
{
	return ((EPWM->MIS&EPWM_MIS_BRKIF_Msk)? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearBrakeIntFlag
 **			���ɲ���жϱ�־λ
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
 **			��ȡɲ���źű�־λ
 ** \param [in] none	
 ** \return 1���������ϻ���ɲ���źű�����Ч  0���޹���
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetBrakeFlag(void)
{
	return ((EPWM->BRKCTL&EPWM_BRKCTL_BRKAF_Msk)? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearBrake
 **			���ɲ������λ
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
 **			 ����Hall״̬�����ж�
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
 **			 �ر�Hall״̬�����ж�
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
 **			��ȡHall״̬�����ж�ʹ�ܲ������жϱ�־λ
 ** \param [in] none	
 ** \return 1���ж�ʹ�ܲ��Ҳ����ж�  0�����ж�
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetHallErrorIntFlag(void)
{
	return ((EPWM->MIS&EPWM_MIS_HALLIF_Msk)? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearHallErrorIntFlag
 **			���Hall״̬�����жϱ�־λ
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
 **			 �������ϱȽ��ж�
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 	
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
 **			 �ر����ϱȽ��ж�
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 	
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
 **			��ȡ���ϱȽ��ж�ʹ�ܲ������жϱ�־λ
 ** \param [in]EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 	
 ** \return 1���ж�ʹ�ܲ��Ҳ����ж�  0�����ж�
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetUpCmpIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_UIFn_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearUpCmpIntFlag
 **			������ϱȽ��жϱ�־λ
 ** \param [in]EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearUpCmpIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_UIFn_Pos));
}

/*****************************************************************************
 ** \brief	 EPWM_EnableDownCmpInt
 **			 �������±Ƚ��ж�
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 �ر����±Ƚ��ж�
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			��ȡ���±Ƚ��ж�ʹ�ܲ������жϱ�־λ
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 	
 ** \return 1���ж�ʹ�ܲ��Ҳ����ж�  0�����ж�
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetDownCmpIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_DIFn_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearDownCmpIntFlag
 **			������±Ƚ��жϱ�־λ
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearDownCmpIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_DIFn_Pos));	
}


/*****************************************************************************
 ** \brief	 EPWM_EnablePeriodInt
 **			 ���������ж�
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 	
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
 **			 �ر������ж�
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 	
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
 **			��ȡ�����ж�ʹ�ܲ������жϱ�־λ
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 	
 ** \return 1���ж�ʹ�ܲ��Ҳ����ж�  0�����ж�
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetPeriodIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_PIFn_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearPeriodIntFlag
 **			��������жϱ�־λ
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearPeriodIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_PIFn_Pos));
}

/*****************************************************************************
 ** \brief	 EPWM_EnableZeroInt
 **			 ��������ж�
 ** \param [in]  EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 
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
 **			 �ر�����ж�
 ** \param [in] EPWMnMask : EPWM_CH_0_MSK ��EPWM_CH_1_MSK ��EPWM_CH_2_MSK ��
 **							 EPWM_CH_3_MSK ��EPWM_CH_4_MSK ��EPWM_CH_5_MSK 	
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
 **			��ȡ����ж�ʹ�ܲ������жϱ�־λ
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 	
 ** \return 1���ж�ʹ�ܲ��Ҳ����ж�  0�����ж�
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetZeroIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_ZIFn_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearZeroIntFlag
 **			�������жϱ�־λ
 ** \param [in] EPWMn : EPWM0 ��EPWM1 ��EPWM2 ��EPWM3 ��EPWM4 ��EPWM5 	
 ** \return none
 ** \note  
*****************************************************************************/
void  EPWM_ClearZeroIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_ZIFn_Pos));
}


/*****************************************************************************
 ** \brief	 EPWM_EnableCountCmpInt
 **			 ���������Ƚ����ж�
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
 **			 �رռ����Ƚ����ж�
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
 **			��ȡ�����Ƚ����ж�ʹ�ܲ������жϱ�־λ
 ** \param [in] EPWMCmp: EPWM_CMPTG_0 EPWM_CMPTG_1	
 ** \return 1���ж�ʹ�ܲ��Ҳ����ж�  0�����ж�
 ** \note  
*****************************************************************************/
uint32_t  EPWM_GetCountCmpIntFlag(uint32_t EPWMCmp)
{
	return((EPWM->MIS & (0x1<<(EPWMCmp+EPWM_MIS_DC0IF_Pos)))? 1:0);
}
/*****************************************************************************
 ** \brief	 EPWM_ClearCountCmp0IntFlag
 **			��������Ƚ����жϱ�־λ
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
 **			 ��������ɲ���ж��ۼƹ���
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
 **			 �رչ���ɲ���ж��ۼƹ���
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
 **			 ���ù���ɲ���ж��ۼƴ���
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
 **			 ��������ж��ۼƹ���
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
 **			 �ر�����ж��ۼƹ���
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
 **			 ��������ж��ۼƴ���
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
 **			 �жϲ��������ż��ض���
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
 **			 �жϲ�������ض����޹�
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
 **			 ʹ�ܵ�����ͣ
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
 **			 �رյ�����ͣ
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

