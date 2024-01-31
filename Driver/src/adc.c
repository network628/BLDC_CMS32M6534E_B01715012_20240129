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
/** \file ADC.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "adc.h"
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
 ** \brief	ADC_ConfigRunMode
 **			����ADC����ģʽ
 ** \param [in] RunMode: (1)ADC_MODE_HIGH
 **						 (2)ADC_MODE_LOW
 **				ConvertMode:(1)ADC1_CONVERT_SINGLE			
 **							(2)ADC1_CONVERT_CONTINUOUS				
 **				ClkDiv :(1)ADC_CLK_DIV_1
 **						(2)ADC_CLK_DIV_2
 **						(3)ADC_CLK_DIV_4
 **						(4)ADC_CLK_DIV_8
 **						(5)ADC_CLK_DIV_16
 **						(6)ADC_CLK_DIV_32
 **						(7)ADC_CLK_DIV_64
 **						(8)ADC_CLK_DIV_128
 **				Holdtime: 5~255 		
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_ConfigRunMode(uint32_t RunMode, uint32_t ConvertMode,uint32_t ClkDiv, uint32_t Holdtime)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON &= ~(ADC_CON_ADMODE10_Msk|ADC_CON_ADCMS_Msk|ADC_CON_ADCDIV_Msk | ADC_CON_ADCNSMP_Msk);
	ADC->CON |= ConvertMode |  ClkDiv | (Holdtime<<ADC_CON_ADCNSMP_Pos) | RunMode;	
	ADC->LOCK = 0x00;
}

/*****************************************************************************
 ** \brief	ADC_ConfigChannelSwitchMode
 **			����ADCͨ���л�ģʽ
 ** \param [in] 	ChSwitch:(1)ADC_SWITCH_HARDWARE	
 **							 (2)ADC_SWITCH_SOFTWARE 
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_ConfigChannelSwitchMode(uint32_t ChSwitch)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON &= ~(ADC_CON_ADCSWCHE_Msk);
	ADC->CON |= ChSwitch;
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_EnableCharge
 **			����ADC��繦��
 ** \param [in] ChargeTime:	ADC_CG_2...... ADC_CG_15
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_EnableCharge(uint32_t ChargeTime)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON &= ~(ADC_CON_ADCNDISTS_Msk);
	ADC->CON |= ADC_CON_ADCNDISEN_Msk | (ChargeTime<<ADC_CON_ADCNDISTS_Pos);
	ADC->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	ADC_EnableDischarge
 **			����ADC�ŵ繦��
 ** \param [in] ChargeTime:	ADC_CG_2...... ADC_CG_15
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_EnableDischarge(uint32_t ChargeTime)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON &= ~(ADC_CON_ADCNDISEN_Msk | ADC_CON_ADCNDISTS_Msk);
	ADC->CON |=  (ChargeTime<<ADC_CON_ADCNDISTS_Pos);
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_DisableChargeAndDischarge
 **			�ر�ADC�ŵ硢��繦��
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_DisableChargeAndDischarge(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON &= ~(ADC_CON_ADCNDISTS_Msk);
	ADC->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	ADC_ConfigVREF
 **			����ADC�Ĳο�Դ
 ** \param [in]  VrefPos:  (1)ADC_VREFP_VDD		ѡ��VDD��Ϊ���˲ο�
 **						   (2)ADC_VREFP_BG			ѡ��BG��Ϊ���˲ο�
 **						   (3)ADC_VREFP_AVREFP		ѡ���ⲿAVREFP��Ϊ���˲ο�
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_ConfigVREF(uint32_t VrefPos)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON &= ~(ADC_CON_ADCVS_Msk);
	ADC->CON |= (VrefPos);
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_ConfigSampleTime
 **			���ò���ʱ��
 ** \param [in]  sampletime: 5 ~ 255;
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_ConfigSampleTime(uint32_t sampletime)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON &= ~(ADC_CON_ADCNSMP_Msk);
	ADC->CON |= (sampletime << ADC_CON_ADCNSMP_Pos);
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_EnableSampleWait
 **			ʹ��ADC���ֲ���״̬
 ** \param [in]  none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_EnableSampleWait(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON |= ADC_CON2_ADCSMPWAIT_Msk;
	ADC->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	ADC_DisableSampleWait
 **			�ر�ADC���ֲ���״̬
 ** \param [in]  none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_DisableSampleWait(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON &= ~(ADC_CON2_ADCSMPWAIT_Msk);
	ADC->LOCK = 0x00;
}

/*****************************************************************************
 ** \brief	ADC_SelSoftwareSwitchChannel
 **			ѡ������л�ת����ͨ��
 ** \param [in] Channel: ADC_CH_0 ~ ADC_CH_30
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_SelSoftwareSwitchChannel(uint32_t Channel)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON2 &= ~(ADC_CON2_ADCSWCHS_Msk);
	ADC->CON2 |= Channel;
	ADC->LOCK = 0x00;
}

/*****************************************************************************
 ** \brief	ADC_SelAN22Source
 **			   ѡ��AN22ͨ����ת��Դ
 ** \param [in] AN22Sel : (1)ADC_AN22_SEL_TS
 **						  (2)ADC_AN22_SEL_BG
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_SelAN22Source(uint8_t AN22Sel)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON2 &= ~(ADC_CON2_BG2ADSEL_Msk);
	ADC->CON2 |= AN22Sel << ADC_CON2_BG2ADSEL_Pos;
	ADC->LOCK = 0x00;
}

/*****************************************************************************
 ** \brief	ADC_AN23Select
 **			AN23ͨ��ѡ��
 ** \param [in] AN23Sel : (1)ADC_AN23_SEL_BG
 **						  (2)ADC_AN23_SEL_ADCLDO
 **						  (3)ADC_AN23_SEL_VDDHA
 **						  (4)ADC_AN23_SEL_VDDHCVA
 **						  (5)ADC_AN23_SEL_DAC     
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_AN23Select(uint32_t AN23Sel)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON2 &= ~(ADC_CON2_ADCICHS_Msk);
	ADC->CON2 |= (AN23Sel<<ADC_CON2_ADCICHS_Pos);
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_EnableScanChannel
 **			����ADCת��ͨ��
 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_EnableScanChannel(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->SCAN |= ChannelMask;
	ADC->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	ADC_DisableScanChannel
 **			�ر�ADCת��ͨ��
 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_DisableScanChannel(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->SCAN &= ~(ChannelMask);
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_GetResult
 **			��ȡADCת�����
 ** \param [in] Channel: ADC_CH_0 ~ ADC_CH_30
 ** \return  12 bits Value
 ** \note	
*****************************************************************************/
uint32_t ADC_GetResult(uint32_t Channel)
{
	return(ADC->DATA[Channel]);
}

/*****************************************************************************
 ** \brief	ADC_EnableHardwareTrigger
 **			����Ӳ������ADCת������
 ** \param [in] TriggerSource: 
 **							(4)ADC_TG_INTNEL_ADC
 **							(5)ADC_TG_INTNEL_ACMP0
 **							(6)ADC_TG_INTNEL_ACMP1
 **							(7)ADC_TG_INTNEL_TMR0
 **							(8)ADC_TG_INTNEL_TMR1
 **							(9)ADC_TG_EPWM_CMP0
 **							(10)ADC_TG_EPWM_CMP1
 **							(11)ADC_TG_EPWM0_RISING
 **							(12)ADC_TG_EPWM0_FALLING
 **							(13)ADC_TG_EPWM0_PERIOD
 **							(14)ADC_TG_EPWM0_ZERO
 **							(15)ADC_TG_EPWM1_RISING
 **							(16)ADC_TG_EPWM1_FALLING
 **							(17)ADC_TG_EPWM1_PERIOD
 **							(18)ADC_TG_EPWM1_ZERRO
 **							(19)ADC_TG_EPWM2_RISING
 **							(20)ADC_TG_EPWM2_FALLING
 **							(21)ADC_TG_EPWM2_PERIOD
 **							(22)ADC_TG_EPWM2_ZERRO
 **							(23)ADC_TG_EPWM3_RISING
 **							(24)ADC_TG_EPWM3_FALLING
 **							(25)ADC_TG_EPWM3_PERIOD
 **							(26)ADC_TG_EPWM3_ZERRO
 **							(27)ADC_TG_EPWM4_RISING
 **							(28)ADC_TG_EPWM4_FALLING
 **							(29)ADC_TG_EPWM4_PERIOD
 **							(30)ADC_TG_EPWM4_ZERRO
 **							(31)ADC_TG_EPWM5_RISING
 **							(32)ADC_TG_EPWM5_FALLING
 **							(33)ADC_TG_EPWM5_PERIOD
 **							(34)ADC_TG_EPWM5_ZERRO 
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_EnableHardwareTrigger(uint32_t TriggerSource)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	switch(TriggerSource)
	{
		case ADC_TG_INTNEL_ADC:
		case ADC_TG_INTNEL_ACMP0:
		case ADC_TG_INTNEL_ACMP1:
		case ADC_TG_INTNEL_TMR0:
		case ADC_TG_INTNEL_TMR1:			
			ADC->HWTG &= ~(ADC_HWTG_ADCINTTGSS_Msk);
			ADC->HWTG |= TriggerSource;
			break;
		case ADC_TG_EPWM_CMP0:
		case ADC_TG_EPWM_CMP1:
			ADC->HWTG |= TriggerSource;
			break;
		case ADC_TG_EPWM0_RISING:
		case ADC_TG_EPWM0_FALLING:
		case ADC_TG_EPWM0_PERIOD:
		case ADC_TG_EPWM0_ZERO:
		case ADC_TG_EPWM1_RISING:
		case ADC_TG_EPWM1_FALLING:
		case ADC_TG_EPWM1_PERIOD:
		case ADC_TG_EPWM1_ZERO:			
		case ADC_TG_EPWM2_RISING:
		case ADC_TG_EPWM2_FALLING:
		case ADC_TG_EPWM2_PERIOD:
		case ADC_TG_EPWM2_ZERO:		
		case ADC_TG_EPWM3_RISING:
		case ADC_TG_EPWM3_FALLING:
		case ADC_TG_EPWM3_PERIOD:
		case ADC_TG_EPWM3_ZERO:		
		case ADC_TG_EPWM4_RISING:
		case ADC_TG_EPWM4_FALLING:
		case ADC_TG_EPWM4_PERIOD:
		case ADC_TG_EPWM4_ZERO:		
		case ADC_TG_EPWM5_RISING:
		case ADC_TG_EPWM5_FALLING:
		case ADC_TG_EPWM5_PERIOD:
		case ADC_TG_EPWM5_ZERO:			
			ADC->HWTG &= ~(ADC_HWTG_ADCEPWMTSS_Msk | ADC_HWTG_ADCPEWMTPS_Msk);
			ADC->HWTG |= TriggerSource;		
			break;		
		default:
			break;
	}	
	ADC->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	ADC_DisableHardwareTrigger
 **			�ر�Ӳ������ADCת������
 ** \param [in] TriggerSource: 
 **							(3)ADC_TG_INTNEL_ADCA
 **							(4)ADC_TG_INTNEL_ADC
 **							(5)ADC_TG_INTNEL_ACMP0
 **							(6)ADC_TG_INTNEL_ACMP1
 **							(7)ADC_TG_INTNEL_TMR0
 **							(8)ADC_TG_INTNEL_TMR1
 **							(9)ADC_TG_EPWM_CMP0
 **							(10)ADC_TG_EPWM_CMP1
 **							(11)ADC_TG_EPWM0_RISING
 **							(12)ADC_TG_EPWM0_FALLING
 **							(13)ADC_TG_EPWM0_PERIOD
 **							(14)ADC_TG_EPWM0_ZERRO
 **							(15)ADC_TG_EPWM1_RISING
 **							(16)ADC_TG_EPWM1_FALLING
 **							(17)ADC_TG_EPWM1_PERIOD
 **							(18)ADC_TG_EPWM1_ZERRO
 **							(19)ADC_TG_EPWM2_RISING
 **							(20)ADC_TG_EPWM2_FALLING
 **							(21)ADC_TG_EPWM2_PERIOD
 **							(22)ADC_TG_EPWM2_ZERRO
 **							(23)ADC_TG_EPWM3_RISING
 **							(24)ADC_TG_EPWM3_FALLING
 **							(25)ADC_TG_EPWM3_PERIOD
 **							(26)ADC_TG_EPWM3_ZERRO
 **							(27)ADC_TG_EPWM4_RISING
 **							(28)ADC_TG_EPWM4_FALLING
 **							(29)ADC_TG_EPWM4_PERIOD
 **							(30)ADC_TG_EPWM4_ZERRO
 **							(31)ADC_TG_EPWM5_RISING
 **							(32)ADC_TG_EPWM5_FALLING
 **							(33)ADC_TG_EPWM5_PERIOD
 **							(34)ADC_TG_EPWM5_ZERRO 	
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_DisableHardwareTrigger(uint32_t TriggerSource)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	switch(TriggerSource)
	{

		case ADC_TG_INTNEL_ADC:
		case ADC_TG_INTNEL_ACMP0:
		case ADC_TG_INTNEL_ACMP1:
		case ADC_TG_INTNEL_TMR0:
		case ADC_TG_INTNEL_TMR1:			
			ADC->HWTG &= ~(ADC_HWTG_ADCINTTGEN_Msk);
			break;
		case ADC_TG_EPWM_CMP0:
			ADC->HWTG &= ~(ADC_HWTG_ADCPTG0EN_Msk);
			break;			
		case ADC_TG_EPWM_CMP1:
			ADC->HWTG &= ~(ADC_HWTG_ADCPTG1EN_Msk);
			break;
		case ADC_TG_EPWM0_RISING:
		case ADC_TG_EPWM0_FALLING:
		case ADC_TG_EPWM0_PERIOD:
		case ADC_TG_EPWM0_ZERO:
		case ADC_TG_EPWM1_RISING:
		case ADC_TG_EPWM1_FALLING:
		case ADC_TG_EPWM1_PERIOD:
		case ADC_TG_EPWM1_ZERO:			
		case ADC_TG_EPWM2_RISING:
		case ADC_TG_EPWM2_FALLING:
		case ADC_TG_EPWM2_PERIOD:
		case ADC_TG_EPWM2_ZERO:		
		case ADC_TG_EPWM3_RISING:
		case ADC_TG_EPWM3_FALLING:
		case ADC_TG_EPWM3_PERIOD:
		case ADC_TG_EPWM3_ZERO:		
		case ADC_TG_EPWM4_RISING:
		case ADC_TG_EPWM4_FALLING:
		case ADC_TG_EPWM4_PERIOD:
		case ADC_TG_EPWM4_ZERO:		
		case ADC_TG_EPWM5_RISING:
		case ADC_TG_EPWM5_FALLING:
		case ADC_TG_EPWM5_PERIOD:
		case ADC_TG_EPWM5_ZERO:			
			ADC->HWTG &= ~(ADC_HWTG_ADCEPWMTEN_Msk);	
			break;		
		default:
			break;
	}	
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_SetAnalogSwitchDelayTime
 **			����ADC ģ�⿪�ش򿪵���ʼ������ʱ��
 ** \param [in] DelayTime: 0x00~ 0xff;
 ** \return  none
 ** \note  Time = (DelayTime+2)*ADCCLK
*****************************************************************************/
void ADC_SetAnalogSwitchDelayTime(uint32_t DelayTime)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->TEST = ((0xff & DelayTime)<< ADC_TEST_ADCSWT_Pos);	
	ADC->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	ADC_SetTriggerDelayTime
 **			����ADC EPWM��������ʱʱ��
 ** \param [in] DelayTime: 0x00~ 0x3ff;
 ** \return  none
 ** \note  Time = (DelayTime+2)*PCLK
*****************************************************************************/
void ADC_SetTriggerDelayTime(uint32_t DelayTime)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->PWMTGDLY = (0x3ff & DelayTime);	
	ADC->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	ADC_EnableEPWMTriggerChannel
 **			ʹ�� EPWM ���������ADCת��ͨ��
 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_EnableEPWMTriggerChannel(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CHEPWM |= ChannelMask;	
	ADC->LOCK = 0x00;			
}
/*****************************************************************************
 ** \brief	ADC_DisableEPWMTriggerChannel
 **			�ر� EPWM ���������ADCת��ͨ��
 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_DisableEPWMTriggerChannel(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CHEPWM &= ~(ChannelMask);	
	ADC->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	ADC_EnableEPWMCmp0TriggerChannel
 **			ʹ�� EPWM �Ƚ���0 ������ADCת��ͨ��
 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_EnableEPWMCmp0TriggerChannel(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CHPTG0 |=(ChannelMask);	
	ADC->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	ADC_DisableEPWMCmp0TriggerChannel
 **			�ر� EPWM �Ƚ���0 ������ADCת��ͨ��
 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_DisableEPWMCmp0TriggerChannel(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CHPTG0 &=~(ChannelMask);	
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_EnableEPWMCmp1TriggerChannel
 **			ʹ�� EPWM �Ƚ���1 ������ADCת��ͨ��
 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_EnableEPWMCmp1TriggerChannel(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CHPTG1 |=(ChannelMask);	
	ADC->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	ADC_DisableEPWMCmp1TriggerChannel
 **			�ر� EPWM �Ƚ���1 ������ADCת��ͨ��
 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_DisableEPWMCmp1TriggerChannel(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CHPTG1 &= ~(ChannelMask);	
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_EnableCompare
 **			����ADC�ȽϹ���
 ** \param [in] Compare : ADC_CMP0 ��ADC_CMP1
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_EnableCompare(uint8_t ACMPx, uint32_t Compare)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	if(ACMPx == ADC_CMP0)
		ADC->CMP0	|= ADC_CMP_ADCCMPEN_Msk;
	else
		ADC->CMP1	|= ADC_CMP_ADCCMPEN_Msk;
	ADC->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	ADC_DisableCompare
 **			�ر�ADC�ȽϹ���
 ** \param [in] Compare : ADC_CMP0 ��ADC_CMP1
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_DisableCompare(uint8_t ACMPx)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	if(ACMPx == ADC_CMP0)
		ADC->CMP0	&= ~(ADC_CMP_ADCCMPEN_Msk);
	else
		ADC->CMP1	&= ~(ADC_CMP_ADCCMPEN_Msk);
	ADC->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	ADC_ConfigCompare
 **			����ADC�ȽϹ���
 ** \param [in] Compare : ADC_CMP0 ��ADC_CMP1
 **				 CmpMode:(1)ADC_CMP_LESS_THAN_RESULT
 **						(2)ADC_CMP_NOT_LESS_THAN_RESULT
 **				 CmpCount: 0x00~0xf  Ԥ��Ƚϴ���
 **				 CmpChannel: ADC_CH_0 ~ ADC_CH_30
 **				 CmpData : 12 bit �Ƚ�ֵ
 ** \return  none
 ** \note	���ڲ�����������ADCCMPxMCNT+1��ֵʱ����ADC�Ƚ��¼���
*****************************************************************************/
void ADC_ConfigCompare(uint8_t ACMPx, uint32_t CmpMode, uint32_t CmpCount,  
						uint32_t CmpChannel, uint32_t CmpData)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	if(ACMPx == ADC_CMP0)
	{
		ADC->CMP0	&= ~(ADC_CMP_ADCCMPCOND_Msk | ADC_CMP_ADCCMPMCNT_Msk|
								 ADC_CMP_ADCCMPCHS_Msk | ADC_CMP_ADCCMPDATA_Msk);
		ADC->CMP0	|= CmpMode | ((0xf & CmpCount)<<ADC_CMP_ADCCMPMCNT_Pos) | ((0x1f& CmpChannel)<<ADC_CMP_ADCCMPCHS_Pos)|
								(0xfff & CmpData) ;
	}
	else
	{
		ADC->CMP1	&= ~(ADC_CMP_ADCCMPCOND_Msk | ADC_CMP_ADCCMPMCNT_Msk|
								 ADC_CMP_ADCCMPCHS_Msk | ADC_CMP_ADCCMPDATA_Msk);
		ADC->CMP1	|= CmpMode | ((0xf & CmpCount)<<ADC_CMP_ADCCMPMCNT_Pos) | ((0x1f& CmpChannel)<<ADC_CMP_ADCCMPCHS_Pos)|
								(0xfff & CmpData) ;
	}	
	ADC->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	ADC_GetCompareResult
 **			��ȡADC�Ƚ������
 ** \param [in] Compare : ADC_CMP0 ��ADC_CMP1
 ** \return  1 /0
 ** \note	
*****************************************************************************/
uint32_t ADC_GetCompareResult(uint8_t ACMPx)
{
	if(ACMPx == ADC_CMP0)
		return((ADC->CMP0 & ADC_CMP_ADCCMPO_Msk)? 1:0);
	else
		return((ADC->CMP1 & ADC_CMP_ADCCMPO_Msk)? 1:0);
}


/*****************************************************************************
 ** \brief	ADC_Start
 **			����ADC
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_Start(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->CON |= ADC_CON_ADCEN_Msk;
	ADC->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	ADC_Stop
 **			�ر�ADC
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_Stop(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->CON &= ~(ADC_CON_ADCEN_Msk);
	ADC->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	ADC_EnableReset
 **			��λADC
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_EnableReset(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->CON |= ADC_CON_ADCRST_Msk;
	ADC->LOCK = 0x00;		
}



/*****************************************************************************
 ** \brief	ADC_EnableCompareInt
 **			����ADC�Ƚ��ж�
 ** \param [in] none
 ** \return  none
 ** \note	ֻ֧�ֱȽ��� 0 
*****************************************************************************/
void ADC_EnableCompareInt(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->IMSC |= ADC_IMSC_IMSC31_Msk;
	ADC->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	ADC_DisableCompareInt
 **			�ر�ADC�Ƚ��ж�
 ** \param [in] none
 ** \return  none
 ** \note	ֻ֧�ֱȽ��� 0 
*****************************************************************************/
void ADC_DisableCompareInt(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->IMSC &= ~(ADC_IMSC_IMSC31_Msk);
	ADC->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	ADC_GetCompareIntFlag
 **			��ȡADC�Ƚ��ж�ʹ�ܲ��Ҳ����жϱ�־λ
 ** \param [in] none
 ** \return  1:�ж�ʹ�ܲ��Ҳ����ж� 0�����ж�
 ** \note	ֻ֧�ֱȽ��� 0 
*****************************************************************************/
uint32_t ADC_GetCompareIntFlag(void)
{
	return((ADC->MIS & ADC_MIS_MIS31_Msk)? 1:0);
}
/*****************************************************************************
 ** \brief	ADC_ClearCompareIntFlag
 **			���ADC�Ƚ��жϱ�־λ
 ** \param [in] none
 ** \return  none
 ** \note	ֻ֧�ֱȽ��� 0 
*****************************************************************************/
void ADC_ClearCompareIntFlag(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->ICLR |= (ADC_ICLR_ICLR31_Msk);
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_EnableChannelInt
 **			����ADCͨ��n�ж�
 ** \param [in]ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_EnableChannelInt(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->IMSC |= ChannelMask ;
	ADC->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	ADC_DisableChannelInt
 **			�ر�ADCͨ��n�ж�
 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_DisableChannelInt(uint32_t ChannelMask)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->IMSC &= ~(ChannelMask);
	ADC->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	ADC_GetChannelIntFlag
 **			��ȡADCͨ��n�ж�ʹ�ܲ��Ҳ����жϱ�־λ
 ** \param [in] Channel:ADC_CH_0 ~ ADC_CH_30
 ** \return  1:�ж�ʹ�ܲ��Ҳ����ж� 0�����ж�
 ** \note	
*****************************************************************************/
uint32_t ADC_GetChannelIntFlag(uint32_t Channel)
{
	return((ADC->MIS & (0x1UL<<Channel))? 1:0);	
}
/*****************************************************************************
 ** \brief	ADC_ClearChannelIntFlag
 **			���ADCͨ��n�жϱ�־λ
 ** \param [in] Channel:ADC_CH_0 ~ ADC_CH_30
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_ClearChannelIntFlag(uint32_t Channel)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->ICLR |= (0x01UL<< Channel);
	ADC->LOCK = 0x00;			
}


/*****************************************************************************
 ** \brief	ADC_EnableEPWMCmp0TriggerDelayTime
 **			ʹ��  EPWM �Ƚ���0 ������ADC ��ʱ
 ** \param [in] none
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_EnableEPWMCmp0TriggerDelayTime(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->HWTG &= ~(ADC_HWTG_ADCPTG0DLYEN_Msk);
	ADC->LOCK = 0x00;			
}
/*****************************************************************************
 ** \brief	ADC_DisableEPWMCmp0TriggerDelayTime
 **			�ر� EPWM �Ƚ���0 ������ADC ��ʱ
 ** \param [in] none
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_DisableEPWMCmp0TriggerDelayTime(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->HWTG |= ADC_HWTG_ADCPTG0DLYEN_Msk;
	ADC->LOCK = 0x00;	
}

/*****************************************************************************
 ** \brief	ADC_EnableEPWMCmp1TriggerDelayTime
 **			ʹ��  EPWM �Ƚ���1 ������ADC ��ʱ
 ** \param [in] none
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_EnableEPWMCmp1TriggerDelayTime(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->HWTG &= ~(ADC_HWTG_ADCPTG1DLYEN_Msk);
	ADC->LOCK = 0x00;			
}
/*****************************************************************************
 ** \brief	ADC_DisableEPWMCmp1TriggerDelayTime
 **			�ر� EPWM �Ƚ���1 ������ADC ��ʱ
 ** \param [in] none
 ** \return  none
 ** \note  
*****************************************************************************/
void ADC_DisableEPWMCmp1TriggerDelayTime(void)
{
	ADC->LOCK = ADC_LOCK_WRITE_KEY;	
	ADC->HWTG |= ADC_HWTG_ADCPTG1DLYEN_Msk;
	ADC->LOCK = 0x00;	
}





