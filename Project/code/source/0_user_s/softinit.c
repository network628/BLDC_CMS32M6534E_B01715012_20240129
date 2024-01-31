//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    softinit.c
* @author  CMS Motor Control Team:lpj
* @version 第二代风机平台
* @date    2023年2月
* @brief   
*---------------------------------------------------------------------------//
**********************************************1******************************/
//==========================================================================//

//---------------------------------------------------------------------------/
//	include files
//---------------------------------------------------------------------------/
#include "mcu_driver.h"
#include "motor_driver.h"
#include "user.h"

//---------------------------------------------------------------------------/
//	Local pre-processor symbols/macros('#define')
//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
//	Local variable  definitions
//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/
ByteFlag Time_Flag;

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/

/*****************************************************************************
 ** \brief	MC_SampPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_SampPara_Init(void)
{
	stru_Sample.G_Ip     = _Q10( SW_GAIN_IPHASE );
	stru_Sample.G_Vb     = _Q10( SW_GAIN_VBUS   );
	stru_Sample.G_Ib     = _Q10( SW_GAIN_IBUS   );	
	stru_Sample.G_Sp     = _Q10( SW_GAIN_SPEED  );
	stru_Sample.G_Vc     = _Q10( SW_GAIN_VCTRL  );
	stru_Sample.G_Vbemf  = _Q10( SW_GAIN_VBEMF  );
	stru_Sample.G_Pw     = _Q15( (((float)HW_VBUS_SDR*SW_GAIN_VBUS)/HW_ADC_REF)*((HW_IBUS_RES*HW_AMP_GAIN_IBUS*SW_GAIN_IBUS)/HW_ADC_REF) );

	stru_Sample.IP_CH    = (uint8_t)IP_SAMP_CH;
	stru_Sample.G_Power  = (int32_t)SW_GAIN_POWER;	
	Motor_Actual.G_Vbus  = Calc_Coeff_Vbus;  
	Motor_Actual.G_Ibus  = Calc_Coeff_Ibus;
	Motor_Actual.G_Power = FRACMPY_Q12( Calc_Coeff_Vbus, Calc_Coeff_Ibus );
	
}

/*****************************************************************************
 ** \brief	MC_FaultPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_FaultPara_Init(void)
{		
	Fault.Over_Temperature_Value          = FAULT_OVER_TEMPERATURE;
	Fault.Over_Temperature_Recover_Value  = FAULT_OVER_TEMPERATURE_RECOVER;	
	Fault.Lower_Temperature_Value         = FAULT_LOWER_TEMPERATURE;	
	Fault.Lower_Temperature_Recover_Value = FAULT_LOWER_TEMPERATURE_RECOVER;		

	Fault.StartOverVoltage_Value          = Calc_Vbus_Value( FAULT_VBUS_START_OVOT );
	Fault.StartLowerVoltage_Value         = Calc_Vbus_Value( FAULT_VBUS_START_UVOT );
	Fault.RunOverVoltage_Value            = Calc_Vbus_Value( FAULT_VBUS_RUN_OVOT );
	Fault.RunLowerVoltage_Value           = Calc_Vbus_Value( FAULT_VBUS_RUN_UVOT );
	Fault.OV_Recover_Value                = Calc_Vbus_Value( FAULT_VBUS_OV_REV );
	Fault.UV_Recover_Value                = Calc_Vbus_Value( FAULT_VBUS_UV_REV );
	
	Fault.Phaseloss_Value                 = FAULT_PHASELOSS_CURR;	
	Fault.OverCurrent_Value               = FAULT_OVER_CURRENT;
	
	Fault.Block_Fault_K1                  = FAULT_SPEED_K1;
	Fault.Block_Fault_K2                  = FAULT_SPEED_K2;
	
}

/*****************************************************************************
 ** \brief	MC_CtrltPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_CtrltPara_Init(void)
{	
	Config.Mode_Control = (uint8_t)Config_Contorl_Mode;
	Config.Mode_IPD     = (uint8_t)Config_IPD_Mode;
	Config.Mode_Shunt   = (uint8_t)Config_Shunt_Mode;
	Config.Mode_Svpwm   = (uint8_t)Config_SVPWM_Mode;
	Config.Mode_Wind    = (uint8_t)Config_Wind_Mode;
	Config.MotorDir     = (uint8_t)Config_Motor_FR;
	Config.Mode_Weeken  = (uint8_t)Config_Weak_Mode;	
	Config.Mode_Start   = 1;
}

/*****************************************************************************
 ** \brief	MC_BasePara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_BasePara_Init(void)
{
	Basic.Speed_Close     = (int32_t)MOTOR_SPEED_CLOSE;
	Basic.Speed_Stop      = (int32_t)MOTOR_SPEED_STOP;
	Basic.Speed_Base      = (int32_t)MOTOR_SPEED_BASE;

	Basic.Speed_Brake     = (int32_t)Motor_Brake_Speed;	
	Basic.Freq_EPWM       = EPWM_FREQ;
	Basic.Tpwm_us         = (2 * 1000000) / Basic.Freq_EPWM;
	Basic.MotorVolt       = (float)MOTOR_VOLT_BASE;
	Basic.MotorCurr       = (float)((HW_ADC_REF - HW_AMP_REF_IP) / (HW_AMP_GAIN_IP*HW_PHASE_RES));	
	Basic.Ke              = (float)MOTOR_Ke;
	Basic.We_Ke           = (int32_t)(MOTOR_Ke_K * MOTOR_WE_BASE);
	Basic.We              = _Q15((float)(MOTOR_SPEED_BASE) / (((float)EPWM_FREQ * 60) / ((float)FOC_ERAMP * 2 * MOTOR_PAIRS)));
	Basic.Calb_Vbus       = (int32_t)((float)HW_VBUS_VOLT * Calc_Coeff_Vbus);
	Basic.Vbus            = Basic.Calb_Vbus;
	Basic.Vbus_Check      = Basic.Vbus;
	
	Basic.Calb_SpeedLimit = FRACMPY_Q10( LIMIT_SPEED_VALUE, stru_Sample.G_Sp );
	Basic.Calb_Speed_Min  = FRACMPY_Q10( FOC_RUN_SPEED_MIN, stru_Sample.G_Sp );	
	Basic.Calb_Speed_Max  = FRACMPY_Q10( FOC_RUN_SPEED_MAX, stru_Sample.G_Sp );
	
	#if ( Speed_Control_Mode == PWM_AD_Control )
	Basic.VCtr            = (int32_t)_Q15_VAL;
	#else
	Basic.VCtr            = 0;
	#endif
	Basic.VTem            = (uint32_t)FAULT_OVER_TEMPERATURE_RECOVER;
  
}

/*****************************************************************************
 ** \brief	MC_FOCPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_FOCPara_Init(void)
{	
	Para.Start.run_current_init  = START_CURRENT_INIT;
	Para.Start.run_current_final = START_CURRENT_FINAL;
	Para.Start.Dkp               = START_DKP;
	Para.Start.Dki               = START_DKI;
	Para.Start.Dout_max          = START_DOUT_MAX;
	Para.Start.Dout_min          = START_DOUT_MIN;	
	Para.Start.Qkp               = START_QKP;
	Para.Start.Qki               = START_QKI;
	Para.Start.Qout_max          = START_QOUT_MAX;
	Para.Start.Qout_min          = START_QOUT_MIN;	
	Para.Start.Ekp               = 10000;
	Para.Start.Eki               = 0;
	Para.Start.Eout_min          = 5;
	Para.Start.Hold_Time         = (uint32_t)((TIME_START_HOLD * EPWM_FREQ) / 1000);
	
	// FOC PARA
	Para.FOC.Dkp                 = FOC_DKP;
	Para.FOC.Dki                 = FOC_DKI;
	Para.FOC.Dout_max            = FOC_DOUT_MAX;
	Para.FOC.Dout_min            = FOC_DOUT_MIN;	
	Para.FOC.Qkp                 = FOC_QKP;
	Para.FOC.Qki                 = FOC_QKI;
	Para.FOC.Qout_max            = FOC_QOUT_MAX;
	Para.FOC.Qout_min            = FOC_QOUT_MIN;	
	Para.FOC.Wkp                 = FOC_WKP;
	Para.FOC.Wki                 = FOC_WKI;
	Para.FOC.Wkd                 = FOC_WKD;	
	Para.FOC.Wout_max            = FOC_WOUT_MAX;
	Para.FOC.Wout_min            = FOC_WOUT_MIN;
	Para.FOC.Ekp                 = FOC_EKP;
	Para.FOC.Eki                 = FOC_EKI;
	Para.FOC.Fc_LPF              = FOC_FC_LPF;	
	Para.FOC.OffsetTheta         = FOC_OFFSET_ANGLE * 2;
	
	stru_FOC.SRAMP               = 1;	
	stru_FOC.IRAMP               = (uint16_t)START_IRAMP;
	stru_FOC.FRAMP               = (uint16_t)FOC_FRAMP;
	stru_FOC.ERAMP               = (uint16_t)FOC_ERAMP;
	stru_FOC.Vs_Max              = _Q15(MAX_VOLTAGE_VECTOR);
	stru_FOC.Vs_Square_Max       = _Q15(MAX_VOLTAGE_VECTOR*MAX_VOLTAGE_VECTOR);
	stru_FOC.EPWM_Period         = (uint16_t)EPWM_PERIOD;
	
	stru_FOC.EPWM_SAMPDLY        = (uint16_t)((SAMPLE_TGDLY + 0.05)*EPWM_Tus);
	stru_FOC.TSHIFT              = (uint16_t)((SAMPLE_TGDLY + 0.25)*EPWM_Tus);
	stru_FOC.TSAM                = (uint16_t)(((SAMPLE_TGDLY + 0.5)*EPWM_Tus * 32767) / EPWM_PERIOD);
	stru_FOC.TADVANCE            = 30;

	stru_FOC.KBW                 = SMO_EBW_FCH;
	stru_FOC.Kslf_Bemf           = _2PI() * TPWM_SAMP*MOTOR_PAIRS / 60;
	stru_FOC.Kslf                = (32767 * 2 * FOC_ERAMP) / (_2PI());
	stru_FOC.Kslf_L              = _Q15(((stru_FOC.Kslf_Bemf * MOTOR_SPEED_BASE * SMO_EBW_FCL ) / (1 + stru_FOC.Kslf_Bemf * MOTOR_SPEED_BASE * SMO_EBW_FCL)));
	stru_FOC.Kslf_H              = _Q15(((stru_FOC.Kslf_Bemf * MOTOR_SPEED_BASE * stru_FOC.KBW) / (1 + stru_FOC.Kslf_Bemf * MOTOR_SPEED_BASE * stru_FOC.KBW)));
	stru_FOC.Ksilde              = SMO_KSLIDE;
	stru_FOC.Ke                  = SMO_KE;

	stru_FOC.Weakening           = (int32_t)(WEAKENING_RANGE * stru_FOC.Vs_Max);
	stru_FOC.Curr_Iq_Max         = FOC_RUN_CURRENT_MAX;
	stru_FOC.Curr_Iq_Min         = FOC_RUN_CURRENT_MIN;
	stru_FOC.Speed_Ref_Min       = ((250 * stru_Sample.G_Sp) >> 10);
	
	stru_FOC.PowerLimit          = (int32_t)((float)LIMIT_POWER_VALUE * stru_Sample.G_Pw) >> 6;
		
	/* 2022-06-14母线电流限制值 */
	stru_FOC.CurrentLimit1       = (int32_t)(_Q12((HW_IBUS_RES * HW_AMP_GAIN_IBUS * SW_GAIN_IBUS * LIMIT_CURRENT_VALUE) / HW_ADC_REF));
	stru_FOC.CurrentLimit        = (int32_t)((float)LIMIT_CURRENT_VALUE * 100);
}

/*****************************************************************************
 ** \brief	MC_LPFPara_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_LPFPara_Init(void)
{
	// Lpf PARA
	Para.Lpf.Samp_Ibus  = LPF_SAMP_IBUS;
	Para.Lpf.Samp_Vbus  = LPF_SAMP_VBUS;
	Para.Lpf.Samp_Vctrl = LPF_SAMP_VCTRL;
	Para.Lpf.Samp_Vtem  = LPF_SAMP_TEMP;
	Para.Lpf.Speed_Val  = LPF_SPEED_VAL;
	Para.Lpf.Power_Val  = LPF_POWER_VAL;	
}

/*****************************************************************************
 ** \brief	SpeedRampInit
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_SpeedRamp_Init(void)
{
	stru_Speed_Ctrl.RampInc    = (int32_t)SPEED_RAMP_INC;
	stru_Speed_Ctrl.RampDec    = (int32_t)SPEED_RAMP_DEC;
	stru_Speed_Ctrl.RampOut    = 0;
	stru_Speed_Ctrl.RampTarget = 0;
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  : MC_BaseValue_Init
* Description    : 
* Function Call  : 初始化调用
* Input Paragram :
* Return Value   : 
*-----------------------------------------------------------------------------
******************************************************************************/
void MC_BaseValue_Init(void)
{
	//------------------------------------------------------------------------/
	stru_base_value.Us 		= HW_VBUS_VOLT * 2;
	stru_base_value.Rs 		= MOTOR_RS;
	stru_base_value.Ls 		= MOTOR_LS / 1000.0;
	stru_base_value.Ts 		= 1.0 / EPWM_FREQ;
	stru_base_value.Poles	= MOTOR_PAIRS;
	
	//------------------------------------------------------------------------/	
	stru_base_value.Ub 		= stru_base_value.Us / 1.732;
	stru_base_value.Ib 		= (float)((HW_ADC_REF - HW_AMP_REF_IP) / (HW_AMP_GAIN_IP * HW_PHASE_RES));	
	stru_base_value.Fb 		= (float)((stru_base_value.Poles * MOTOR_SPEED_BASE) / 60);
	stru_base_value.Wb		= _2PI() * stru_base_value.Fb;
	
	//------------------------------------------------------------------------/
}

/*****************************************************************************
 ** \brief	MC_OpenLoop_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_OpenLoop_Init(void)
{	
	stru_OpenLoop.RevUp_T1 = START_REVUP_TIME1;
	stru_OpenLoop.RevUp_T2 = START_REVUP_TIME2;
	stru_OpenLoop.RevUp_T3 = START_REVUP_TIME3;	
	stru_OpenLoop.RevUp_I1 = START_REVUP_CURRENT1;
	stru_OpenLoop.RevUp_I2 = START_REVUP_CURRENT2;
	stru_OpenLoop.RevUp_I3 = START_REVUP_CURRENT3;
	stru_OpenLoop.We_K1    = START_REVUP_WE_K1;
	stru_OpenLoop.We_K2    = START_REVUP_WE_K2;
	stru_OpenLoop.We_K3    = START_REVUP_WE_K3;
	stru_OpenLoop.We_Min   = START_REVUP_WEMIN;
	stru_OpenLoop.We_Max   = START_REVUP_WEMAX;
}

/*****************************************************************************
 ** \brief	MC_UserPowerSet_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_UserPowerSet_Init(void)
{
	UserPowerSet.PWMDutyStop  = (int32_t)PWM_DUTY_STOP;
	UserPowerSet.PWMDutyStart = (int32_t)PWM_DUTY_START;
	UserPowerSet.PWMDutyMax   = (int32_t)PWM_DUTY_MAX;
	UserPowerSet.PWMADStop    = (int32_t)((float)(PWM_AD_STOP / HW_ADC_REF) * _Q15_VAL);
	UserPowerSet.PWMADStart   = ((float)(PWM_AD_START / HW_ADC_REF) * _Q15_VAL);
	UserPowerSet.PWMADMax     = ((float)(PWM_AD_MAX / HW_ADC_REF) * _Q15_VAL);
	UserPowerSet.PWMPowerKe   = ((float)(TARGET_MAX - TARGET_MIN) / (float)(UserPowerSet.PWMDutyMax - UserPowerSet.PWMDutyStart));
	UserPowerSet.PWMADPowerKe	= ((float)(TARGET_MAX - TARGET_MIN) / (float)(UserPowerSet.PWMADMax - UserPowerSet.PWMADStart));
}

/*****************************************************************************
 ** \brief	MC_Para_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void MC_Para_Init(void)
{
	MC_CtrltPara_Init();	
	MC_SampPara_Init();	
	MC_FaultPara_Init();
	MC_BasePara_Init();		
	MC_FOCPara_Init();	
	MC_LPFPara_Init();		
	MC_SpeedRamp_Init();
	MC_BaseValue_Init();
	MC_OpenLoop_Init();	
	MC_UserPowerSet_Init();
		
	Time.Motor_Stophold     = (uint32_t)((TIME_STOP_HOLD * EPWM_FREQ) / 1000);
	Time.Motor_Brake        = (uint32_t)((Motor_Brake_Time * EPWM_FREQ) / 1000);
	Time.Motor_Block        = (uint32_t)((FAULT_BLOCK_TIME * EPWM_FREQ) / 1000);
	Time.Motor_StartFail    = (uint32_t)((float)FAULT_START_FAIL_TIME * EPWM_FREQ);	
	Time.Motor_StartSuccess = (uint32_t)((MOTOR_SPEED_CLOSE_TIME * EPWM_FREQ) / 1000);
	wObseFs                 = 32768 - (uint32_t)_Q15(stru_base_value.Rs * (float)OBSE_FS_FACTOR / (stru_base_value.Ls * Basic.Freq_EPWM));
	wObseGs                 = (uint32_t)_Q15((Basic.MotorVolt) / (stru_base_value.Ls * Basic.MotorCurr * Basic.Freq_EPWM));	
	stru_bemf.Speed_Time    = (Basic.Freq_EPWM * 600 ) / (BEMF_SPEED_START * MOTOR_PAIRS);
	
}

/*****************************************************************************
 ** \brief	__EI
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void __EI_Set(void)
{
	/* 开启EPWM中断 */	
	#if ( Config_Shunt_Mode == Single_Shunt )
	{
		EPWM->ICLR |= 0xFFFFFFFF;
		EPWM_EnableZeroInt( EPWM_CH_0_MSK );
		NVIC_EnableIRQ( EPWM_IRQn );
		NVIC_SetPriority( EPWM_IRQn, 1 );	
	}
	#endif	

	/* 开启ADC中断 */	
	#if ( Config_Shunt_Mode == Double_Shunt )
	{
		ADC_EnableChannelInt( ADC_SCAN_CHA );
		NVIC_EnableIRQ( ADC_IRQn);
		NVIC_SetPriority( ADC_IRQn, 1 );
	}
	#endif
	
	/* 开启TMR0中断 */
	#if ( 0 )
	{	
		TMR_EnableOverflowInt( TMR0 );
		TMR_ClearOverflowIntFlag( TMR0 );
		NVIC_EnableIRQ( TIMER0_IRQn );	
		NVIC_SetPriority( TIMER0_IRQn, 2 );
	}
	#endif

	#if ( Config_CCP_Capture == CCP_Capture_Enable && 0)	
	CCP_EnableCAPMode0Int( CCP_CAPTURE_MODULE ); /*开启CCP模块捕获模式0中断*/
	NVIC_EnableIRQ( CCP_IRQn );		
	NVIC_SetPriority( CCP_IRQn , 0 ); /*优先级0~3， 0最高、3最低*/			
	#endif	
	
	/* 开启SysTick中断 */	
	NVIC_EnableIRQ( SysTick_IRQn );
	NVIC_SetPriority( SysTick_IRQn, 3 );
	SysTick->CTRL  = 0x00000007;
	
	/* 开启ACMP1中断 */
	NVIC_EnableIRQ( ACMP_IRQn );
	NVIC_SetPriority( ACMP_IRQn, 0 );
	
	#if (ACMP_CH == ACMP0)
  {
    ACMP_ClearIntFlag( ACMP0 );
    ACMP_EnableInt( ACMP0);
  }
	#else
  {
    ACMP_ClearIntFlag( ACMP1 );
    ACMP_EnableInt( ACMP1 );	
  }
	#endif

}

/*****************************************************************************
 ** \brief	DI
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void __DI_Set(void)
{
	NVIC_DisableIRQ( EPWM_IRQn );
	NVIC_DisableIRQ( ADC_IRQn );	
	NVIC_DisableIRQ( SysTick_IRQn );	
	NVIC_DisableIRQ( UART0_IRQn );	
	NVIC_DisableIRQ( UART0_IRQn );
	NVIC_DisableIRQ( TIMER0_IRQn );	
	NVIC_DisableIRQ( TIMER1_IRQn );
	NVIC_DisableIRQ( LVI_IRQn );		
	NVIC_DisableIRQ( ACMP_IRQn );
	NVIC_DisableIRQ( CCP_IRQn );		
	NVIC_DisableIRQ( INTP0_IRQn );
	NVIC_DisableIRQ( INTP1_IRQn );
	NVIC_DisableIRQ( INTP2_IRQn );
	NVIC_DisableIRQ( INTP3_IRQn );
}

/*****************************************************************************
 ** \brief	
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DelayTime(uint32_t delay)
{
	for( ; delay > 0; delay -- ) 
	{
		__nop();
	}
}

/*****************************************************************************
 ** \brief	
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DelayTime_ms(uint32_t delay)
{
	uint32_t i,j;
	
	for( i = 0; i < delay; i ++ )
	{
		for( j = 0; j < 10000; j++ ) //1ms,64M
		{
			;
		}
	}
}

/*****************************************************************************
 ** \brief	void ADC_ClearAllInt_Flag(void)
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_ClearAllInt_Flag(void)
{
	ADC->LOCK  = ADC_LOCK_WRITE_KEY;	
	ADC->ICLR |= 0xFFFFFFFF;
	ADC->LOCK  = 0x00;
}

/*****************************************************************************
 ** \brief	ADC_TGSAMP_CONFIG
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_TGSAMP_CONFIG(void)
{
	/* 双电阻ADC1触发采样模式配置 */
	#if ( Config_Shunt_Mode == Double_Shunt )	
	{
		ADC_DisableEPWMCmp0TriggerChannel( 0X0FFFFFF );
		ADC_DisableEPWMCmp1TriggerChannel( 0X0FFFFFF );
		ADC_EnableEPWMCmp0TriggerChannel( ADC_SCAN_CHA | ADC_SCAN_CHB );
		ADC_EnableEPWMCmp1TriggerChannel( ADC_SCAN_VBUS | ADC_SCAN_IBUS );
		ADC_EnableHardwareTrigger( ADC_TG_EPWM_CMP0 );
		ADC_EnableHardwareTrigger( ADC_TG_EPWM_CMP1 );	
	}
	/* 单电阻ADC1触发采样模式配置 */
	#else
	{	
		/* 计数比较器0/1触发转换通道配置 */
		ADC_DisableEPWMCmp0TriggerChannel( 0X0FFFFFF );
		ADC_DisableEPWMCmp1TriggerChannel( 0X0FFFFFF );
		ADC_EnableEPWMCmp0TriggerChannel( ADC_SCAN_CHA );
		ADC_EnableEPWMCmp1TriggerChannel( ADC_SCAN_CHB );
		ADC_EnableHardwareTrigger( ADC_TG_EPWM_CMP0 );
		ADC_EnableHardwareTrigger( ADC_TG_EPWM_CMP1 );

		ADC_EnableEPWMTriggerChannel( ADC_SCAN_VBUS | ADC_SCAN_IBUS | ADC_SCAN_TEMP ); // ADC_SCAN_IBUS
//    ADC_EnableEPWMTriggerChannel( ADC_SCAN_IBUS );
		ADC_EnableHardwareTrigger( ADC_TG_EPWM0_ZERO ); //ADC_TG_EPWM0_PERIOD    ADC_TG_EPWM0_ZERO
    //EPWM_DisableIntAccompanyWithLoad();
	}
	#endif
}

/*****************************************************************************
 ** \brief	GET_ADC_IBUS_REF_VAL
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void GET_ADC_IBUS_REF_VAL(void)
{
	uint8_t Count, j;
	uint16_t SampValue[66] = {0};
	int32_t MinValue = 0, MaxValue = 0, SumValue = 0, REFValue = 0;
	
	for( Count = 0; Count < 66; Count ++ )
	{										
		SampValue[Count] = (uint32_t)ADC->DATA[ADC_DATA_IBUS];
		DelayTime(10);
	}

  MinValue = (int32_t)SampValue[0];
  MaxValue = (int32_t)SampValue[0];
  SumValue = 0;
  REFValue = 0;
  
  for( j = 0;j < 66; j ++ )	
  {
    if( MinValue > (int32_t)SampValue[j] )
      MinValue = (int32_t)SampValue[j];
    if( MaxValue < (int32_t)SampValue[j] )
      MaxValue = (int32_t)SampValue[j];	
    SumValue  += (int32_t)SampValue[j];
  }	
		
	REFValue = (SumValue - MinValue - MaxValue) >> 6;
	stru_Sample.REF[CH_IBUS] = REFValue;  
}

/*****************************************************************************
 ** \brief	GET_ADC_BG_REF_VAL
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
uint16_t SampValue[66] = {0};
extern void ADC_Init(void);
void GET_ADC_BG_REF_VAL(void)
{
	uint8_t Count, j;
//	uint16_t SampValue[66] = {0};
	int32_t MinValue = 0, MaxValue = 0, SumValue = 0, REFValue = 0;

	ADC_ConfigRunMode( ADC_MODE_HIGH, ADC_CONVERT_SINGLE, ADC_CLK_DIV_128, 30 );
	ADC_ConfigChannelSwitchMode( ADC_SWITCH_SOFTWARE ); /*软件自动切换*/  

//	ADC->LOCK = ADC_LOCK_WRITE_KEY;
//	ADC->CON2 &= ~(ADC_CON2_BG2ADSEL_Msk);
//	ADC->CON2 |= (ADC_AN22_SEL_BG<<ADC_CON2_BG2ADSEL_Pos);
//	ADC->LOCK = 0x00;
  ADC_AN23Select( ADC_AN23_SEL_BG );

	for( Count = 0; Count < 66; Count ++ )
	{
    ADC_SelSoftwareSwitchChannel( ADC_DATA_BG );
    ADC_EnableScanChannel( ADC_SCAN_BG );
		if(!ADC_IS_BUSY()) 
		{
			ADC_Go();
			while(ADC_IS_BUSY());									
			SampValue[Count] = (uint32_t)ADC->DATA[ADC_DATA_BG];
      DelayTime(10);
		}	
	}

  MinValue = (int32_t)SampValue[0];
  MaxValue = (int32_t)SampValue[0];
  SumValue = 0;
  REFValue = 0;
  
  for( j = 0;j < 66; j ++ )	
  {
    if( MinValue > (int32_t)SampValue[j] )
      MinValue = (int32_t)SampValue[j];
    if( MaxValue < (int32_t)SampValue[j] )
      MaxValue = (int32_t)SampValue[j];	
    SumValue += (int32_t)SampValue[j];
  }	
		
  REFValue = (SumValue - MinValue - MaxValue) >> 6;
  
  stru_Sample.G_BG = (800 * 4095) / REFValue; // 1450  800
  stru_Sample.G_BG = (stru_Sample.G_BG * 4095) / ((int32_t)(HW_ADC_REF * 1000));
  
  ADC_DisableScanChannel( ADC_SCAN_BG );
	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	ADC->CON2 &= ~(ADC_CON2_ADCSWCHS_Msk);
	ADC->LOCK = 0x00;
  ADC_Stop();
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_ADCEN, DISABLE );
  DelayTime(10);
  
  ADC_Init();

}

/*****************************************************************************
 ** \brief	GET_ADC_REF_VAL
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void GET_ADC_REF_VAL(void)
{
	uint8_t Channel, Count, j;
	uint16_t SampValue[3][66] = {0};
	int32_t MinValue[3] = {0}, MaxValue[3] = {0}, SumValue[3] = {0}, REFValue[3] = {0};
	
	static int32_t Aux_OffsetMin, Aux_OffsetMax;
	
	for( Count = 0; Count < 66; Count ++ )
	{										
		SampValue[0][Count] = (uint32_t)ADC->DATA[ADC_DATA_CHA];
		SampValue[1][Count] = (uint32_t)ADC->DATA[ADC_DATA_CHB];	
		SampValue[2][Count] = (uint32_t)ADC->DATA[ADC_DATA_IBUS];
		DelayTime(10);
	}

	for( Channel = 0;Channel < 3; Channel ++ )
	{
		MinValue[Channel] = (int32_t)SampValue[Channel][0];
		MaxValue[Channel] = (int32_t)SampValue[Channel][0];
		SumValue[Channel] = 0;
		REFValue[Channel] = 0;
		
		for( j = 0;j < 66; j ++ )	
		{
			if( MinValue[Channel] > (int32_t)SampValue[Channel][j] )
				MinValue[Channel] = (int32_t)SampValue[Channel][j];
			if( MaxValue[Channel] < (int32_t)SampValue[Channel][j] )
				MaxValue[Channel] = (int32_t)SampValue[Channel][j];	
			SumValue[Channel]  += (int32_t)SampValue[Channel][j];
		}	
		
		REFValue[Channel] = (SumValue[Channel] - MinValue[Channel] - MaxValue[Channel]) >> 6;
 	}
	
	stru_Sample.REF[CH_IP_A] = REFValue[0]+20;
	stru_Sample.REF[CH_IP_B] = REFValue[1]+20;
	stru_Sample.REF[CH_IBUS] = REFValue[2];

		
	//判断ADC采集偏置值是否在正常范围内
	
	Aux_OffsetMin = (int32_t)((HW_AMP_REF_IP - 0.25) / HW_ADC_REF * 4095);
	Aux_OffsetMax = (int32_t)((HW_AMP_REF_IP + 0.25) / HW_ADC_REF * 4095);
	
	if( stru_Sample.REF[CH_IP_A] < Aux_OffsetMin || stru_Sample.REF[CH_IP_A] > Aux_OffsetMax || stru_Sample.REF[CH_IP_B] < Aux_OffsetMin || stru_Sample.REF[CH_IP_B] > Aux_OffsetMax )
	{
		MOTOR_FAULT_CODE = FAULT_SAMPLEREF;
	}

	Aux_OffsetMin = (int32_t)((HW_AMP_REF_IBUS - 0.25) / HW_ADC_REF * 4095);
	Aux_OffsetMax = (int32_t)((HW_AMP_REF_IBUS + 0.25) / HW_ADC_REF * 4095);
	
	if( stru_Sample.REF[CH_IBUS] < Aux_OffsetMin || stru_Sample.REF[CH_IBUS] > Aux_OffsetMax )
	{
		MOTOR_FAULT_CODE = FAULT_SAMPLEREF;
	}	
}

/******************************** END OF FILE *******************************/
