//==========================================================================//
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * @file    user_control.c
 * @author  CMS Motor Control Team:wsh
 * @version
 * @date    2023年2月
 * @brief
 *---------------------------------------------------------------------------//
 *****************************************************************************/
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
struct_Capture Capture = {0};	   /* CCP捕获参数结构体 */
struct_Sleep Sleep = {0};		   /* 休眠参数结构体 */
struct_UserSet UserPowerSet = {0}; /* 用户功率设定参数结构体 */
volatile int32_t Set_Speed = 0;

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/

//===========================================================================/

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/
void PWM_Speed_Control(void);
void PWM_AD_Speed_Control(void);

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  :User_Speed_Control
 * Description    :调速信号控制
 * Function Call  :主函数调用
 * Input Paragram :无
 * Return Value   :无
 *-----------------------------------------------------------------------------
 ******************************************************************************/
int32_t s32_Debug_Speed = 5; // 调试时设置的转速
void User_Speed_Control(void)
{
#if (Speed_Control_Mode == FAST_Control)
	{
		Basic.Target_Value = (int32_t)TARGET_MAX;
		User_MotorSpeed_Set(Basic.Target_Value); // 上电直接运行最大功率
		User_Motor_On();
	}
#elif (Speed_Control_Mode == PWM_Control)
	{
		PWM_Speed_Control(); // PWM调速
	}
#elif (Speed_Control_Mode == PWM_AD_Control)
	{
		PWM_AD_Speed_Control(); // PWMAD调速
	}
#else//(Speed_Control_Mode == Debug_Control) // 调试用，给固定值
	{
		Basic.Target_Value = s32_Debug_Speed;
		User_MotorSpeed_Set(Basic.Target_Value); // 调试用

		// static uint8_t Printf_cnt = 200;
		// Printf_cnt--;
		// if (Printf_cnt == 0)
		// {
		// 	Printf_cnt = 200;
		// 	Printf("->FAST_Control->\r\n");
		// } 
		User_Motor_On();
	}
#endif
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  :PWM_Speed_Control
 * Description    :PWM调速控制
 * Function Call  :主函数调用
 * Input Paragram :无
 * Return Value   :无
 *-----------------------------------------------------------------------------
 ******************************************************************************/
//---------------------------------------------------------------------------//
#define PWM_DUTY_REF_M1 (450) /* 运行档位切换占空比 (0.1%) */
//---------------------------------------------------------------------------//
void PWM_Speed_Control(void)
{
	if (Capture.DutyCycle < UserPowerSet.PWMDutyStop)
	{
		User_Motor_Off();
		Basic.Target_Value = 0;
	}
	else if (Capture.DutyCycle >= UserPowerSet.PWMDutyStart)
	{
#if (1)
		{
			if (Capture.DutyCycle >= UserPowerSet.PWMDutyMax)
			{
				Basic.Target_Value = (int32_t)TARGET_MAX;
			}
			else if (Capture.DutyCycle > UserPowerSet.PWMDutyStart)
			{
				Basic.Target_Value = (Capture.DutyCycle - UserPowerSet.PWMDutyStart) * UserPowerSet.PWMPowerKe + (int32_t)TARGET_MIN;
			}
			else
			{
				Basic.Target_Value = (int32_t)TARGET_MIN;
			}
		}
#else
		{
			if (Capture.DutyCycle > PWM_DUTY_MAX)
			{
				Basic.Target_Value = TARGET_MAX;
			}
			else if (Capture.DutyCycle <= PWM_DUTY_REF_M1)
			{
				Basic.Target_Value = TARGET_MIN;
			}
		}
#endif

		if (Basic.Target_Value >= (int32_t)TARGET_MAX)
		{
			Basic.Target_Value = (int32_t)TARGET_MAX;
		}
		else if (Basic.Target_Value <= (int32_t)TARGET_MIN)
		{
			Basic.Target_Value = (int32_t)TARGET_MIN;
		}

		User_MotorSpeed_Set(Basic.Target_Value);
		User_Motor_On();
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  :PWM_AD_Speed_Control
 * Description    :PWMAD调速控制
 * Function Call  :主函数调用
 * Input Paragram :无
 * Return Value   :无
 *-----------------------------------------------------------------------------
 ******************************************************************************/
//---------------------------------------------------------------------------//
void PWM_AD_Speed_Control(void)
{
	if (Basic.VCtr > UserPowerSet.PWMADStop)
	{
		User_Motor_Off();
		Basic.Target_Value = 0;
	}
	else if (Basic.VCtr <= UserPowerSet.PWMADStart)
	{
		{
			if (Basic.VCtr <= UserPowerSet.PWMADMax)
			{
				Basic.Target_Value = (int32_t)TARGET_MAX;
			}
			else if (Basic.VCtr < UserPowerSet.PWMADStart)
			{
				Basic.Target_Value = (Basic.VCtr - UserPowerSet.PWMADStart) * UserPowerSet.PWMADPowerKe + (int32_t)TARGET_MIN;
			}
			else
			{
				Basic.Target_Value = (int32_t)TARGET_MIN;
			}
		}

		if (Basic.Target_Value >= (int32_t)TARGET_MAX)
		{
			Basic.Target_Value = (int32_t)TARGET_MAX;
		}
		else if (Basic.Target_Value <= (int32_t)TARGET_MIN)
		{
			Basic.Target_Value = (int32_t)TARGET_MIN;
		}

		User_MotorSpeed_Set(Basic.Target_Value);
		User_Motor_On();
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  :User_Speed_Capture
 * Description    :调速信号捕获
 * Function Call  :主函数调用
 * Input Paragram :无
 * Return Value   :无
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void User_Speed_Capture(void)
{
#if 0
	//===============================================================================//	
	if( ++Capture.OverFlowTime >= 10 ) // 100ms溢出计数周期
	{
		Capture.OverFlowTime = 0;
		Capture.OverFlowFlag = 1;
		Capture.CaptureCount = 0;
		Capture.CompleteFlag = 0;
		CCP_ConfigCAPMode0( CCP_CAPTURE_MODULE, CCP_CAP_MODE0_FALLING_TO_RISING ); // 下降到上升沿捕获		
		CCP_DisableRun( CCP_CAPTURE_MODULE );	
		CCP_EnableRun( CCP_CAPTURE_MODULE ); // CCP模块使能
	}
	//===============================================================================//			
	if( Capture.OverFlowFlag )
	{
		// PWM信号为高电平
		if( PORT_GetBit( PORT0,P IN4 ) )
		{
			// 判断占空比是否为100%
			if( ++Capture.HeightenTime >= 8 )	// 80ms计数消抖
			{
#if (PWM_INPUT_MODE == PWM_POSITIVE_LOGIC)
					Capture.DutyCycle = 1000;
					Capture.Frequency = 0;
#else
					Capture.DutyCycle = 0;
					Capture.Frequency = 0;
#endif
			}	

			Capture.WeakenTime = 0;
		}
		else
		{
			// 判断占空比是否为0%
			if( ++Capture.WeakenTime >= 8 ) // 80ms计数消抖
			{
#if (PWM_INPUT_MODE == PWM_POSITIVE_LOGIC)
					Capture.DutyCycle = 0;
					Capture.Frequency = 0;
#else
					Capture.DutyCycle = 1000;
					Capture.Frequency = 0;
#endif
			}
			
			Capture.HeightenTime = 0;
		}
	}
	else if( Capture.CompleteFlag )
	{
		Capture.WeakenTime = 0;
		Capture.HeightenTime = 0;
		
		//===============================================================================//
		// 脉宽很窄占空比出错的处理 2021-08-12
		if( (Capture.CaptureValue[0] >= 65000) || (Capture.CaptureValue[1] >= 65000) )
		{
			Capture.CaptureValue[0] = 0;
			Capture.CaptureValue[1] = 0;
		}
		if( (Capture.CaptureValue[2] >= 65000) || (Capture.CaptureValue[3] >= 65000) )
		{
			Capture.CaptureValue[2] = 0;
			Capture.CaptureValue[3] = 0;
		}
		//===============================================================================//		
   	// 计算捕获到的高低电平时间
		Capture.CaptureTimeH = (Capture.CaptureValue[0] + Capture.CaptureValue[1]) >> 1;
		Capture.CaptureTimeL = (Capture.CaptureValue[2] + Capture.CaptureValue[3]) >> 1;

		//===============================================================================//
		Capture.CaptureTimeCycle = Capture.CaptureTimeL + Capture.CaptureTimeH;
		
		//===============================================================================//
		// 计算捕获到频率与占空比
#if (Speed_Control_Mode == PWM_Control)
			Capture.FrequencyTemp = (uint32_t)(16000000 * 10 / Capture.CaptureTimeCycle); //Hz	1/(计数值*CCP时钟周期)
#else
			Capture.FrequencyTemp = (uint32_t)(1000000*  10 / Capture.CaptureTimeCycle); //Hz	1/(计数值*CCP时钟周期)
#endif

#if (PWM_INPUT_MODE == PWM_POSITIVE_LOGIC)
			Capture.DutyCycleTemp = (uint32_t)(Capture.CaptureTimeH * 1000 / Capture.CaptureTimeCycle) + 1;	//分辨率 1%
#else
			Capture.DutyCycleTemp = (uint32_t)(Capture.CaptureTimeL * 1000 / Capture.CaptureTimeCycle) + 1;	//分辨率 1%
#endif
		
		//===============================================================================//
		// 更新频率与占空比
		if( Capture.FrequencyTemp == Capture.FrequencyOld )						
		{
			if( ABSFUN( (int32_t)Capture.FrequencyOld - (int32_t)Capture.Frequency ) > 6 ) //偏差判断  ±6
				Capture.Frequency = Capture.FrequencyTemp; //更新数据
		}
		else 
			Capture.FrequencyOld = Capture.FrequencyTemp; //更新旧值
		if (Capture.DutyCycleTemp == Capture.DutyCycleOld)
		{
			if( ABSFUN( (int32_t)Capture.DutyCycleOld - (int32_t)Capture.DutyCycle ) > 6 ) //偏差判断	 ±6
				Capture.DutyCycle = Capture.DutyCycleTemp; //更新数据
		}
		else
			Capture.DutyCycleOld = Capture.DutyCycleTemp;				
		//===============================================================================//	
		
		Capture.CaptureCount = 0;			//重新开始采集
		Capture.CompleteFlag = 0;			//清除采集完成标志
		
		//===============================================================================//			
	}
#else

	if (++Capture.OverFlowTime >= 20) // 100ms溢出计数周期
	{
		Capture.OverFlowTime = 0;
		Capture.OverFlowFlag = 1;
		Capture.CaptureCount = 0;
		Capture.CompleteFlag = 0;

		CCP_DisableRun(CCP1);
		CCP_EnableRun(CCP1); // CCP模块使能
	}

	if (Capture.OverFlowFlag)
	{
		if (PORT_GetBit(PORT0, PIN4)) // 判断占空比是否为100%
		{
			if (++Capture.HeightenTime >= 8) // 80ms计数消抖
			{
#if (PWM_INPUT_MODE == PWM_POSITIVE_LOGIC)
				Capture.DutyCycle = 1000;
				Capture.Frequency = 0;
#else
				Capture.DutyCycle = 0;
				Capture.Frequency = 0;
#endif
			}

			Capture.WeakenTime = 0;
		}
		else
		{
			if (++Capture.WeakenTime >= 8) // 80ms计数消抖
			{
#if (PWM_INPUT_MODE == PWM_POSITIVE_LOGIC)
				Capture.DutyCycle = 0;
				Capture.Frequency = 0;
#else
				Capture.DutyCycle = 1000;
				Capture.Frequency = 0;
#endif
			}

			Capture.HeightenTime = 0;
		}
	}
	else if (Capture.CompleteFlag)
	{
		Capture.WeakenTime = 0;
		Capture.HeightenTime = 0;

		//===============================================================================//
		// 获取CCP捕获值
		Capture.CaptureTimeH = CCP_GetCAPMode2Result(CAP1_SUB_CAP2);
		Capture.CaptureTimeL = CCP_GetCAPMode2Result(CAP1_SUB_CAP3) - CCP_GetCAPMode2Result(CAP1_SUB_CAP2);
		Capture.CaptureTimeCycle = CCP_GetCAPMode2Result(CAP1_SUB_CAP3);

//===============================================================================//
// 计算捕获到频率与占空比
#if (Speed_Control_Mode == PWM_Control)
		Capture.FrequencyTemp = (uint32_t)(16000000 * 10 / Capture.CaptureTimeCycle); // 0.1Hz	1/(计数值*CCP时钟周期)
#else
		Capture.FrequencyTemp = (uint32_t)(1000000 * 10 / Capture.CaptureTimeCycle); // 0.1Hz	1/(计数值*CCP时钟周期)
#endif

#if (PWM_INPUT_MODE == PWM_POSITIVE_LOGIC)
		Capture.DutyCycleTemp = (uint32_t)(Capture.CaptureTimeH * 1000 / Capture.CaptureTimeCycle) + 1; // 分辨率 0.1%
#else
		Capture.DutyCycleTemp = (uint32_t)(Capture.CaptureTimeL * 1000 / Capture.CaptureTimeCycle) + 1; // 分辨率 0.1%
#endif

		//===============================================================================//
		// 更新频率与占空比
		if (Capture.FrequencyTemp == Capture.FrequencyOld)
		{
			if (ABSFUN((int32_t)Capture.FrequencyOld - (int32_t)Capture.Frequency) > 6) // 偏差判断  ±6
				Capture.Frequency = Capture.FrequencyTemp;								// 更新数据
		}
		else
			Capture.FrequencyOld = Capture.FrequencyTemp; // 更新旧值
		if (Capture.DutyCycleTemp == Capture.DutyCycleOld)
		{
			if (ABSFUN((int32_t)Capture.DutyCycleOld - (int32_t)Capture.DutyCycle) > 6) // 偏差判断	 ±6
				Capture.DutyCycle = Capture.DutyCycleTemp;								// 更新数据
		}
		else
			Capture.DutyCycleOld = Capture.DutyCycleTemp; // 更新旧值

		Capture.CaptureCount = 0; // 重新开始采集
		Capture.CompleteFlag = 0; // 清除采集完成标志
	}

#endif
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  :CCP_PWM_Config
 * Description    :设置CCPn模块装载值
 * Function Call  :FG输出调用
 * Input Paragram :无
 * Return Value   :无
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void CCP_PWM_Config(uint32_t CCPn, uint32_t CCPChannel, uint32_t ReloadMode, uint32_t Period, uint32_t Compare)
{
	volatile uint32_t temp;

	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if (CCP0 == CCPn)
	{
		CCP->LOAD0 = ((uint16_t)Period) | (ReloadMode << CCP_CCPLOAD0_RELOAD_Pos);

		if (CCPxA == CCPChannel)
		{
			temp = CCP->D0A;
			temp &= ~(CCP_CCPD0A_DATA_Msk);
			temp |= Compare;
			CCP->D0A = temp;
		}
		if (CCPxB == CCPChannel)
		{
			temp = CCP->D0B;
			temp &= ~(CCP_CCPD0B_DATA_Msk);
			temp |= Compare;
			CCP->D0B = temp;
		}
	}
	if (CCP1 == CCPn)
	{
		CCP->LOAD1 = ((uint16_t)Period) | (ReloadMode << CCP_CCPLOAD1_RELOAD_Pos);

		if (CCPxA == CCPChannel)
		{
			temp = CCP->D1A;
			temp &= ~(CCP_CCPD1A_DATA_Msk);
			temp |= Compare;
			CCP->D1A = temp;
		}
		if (CCPxB == CCPChannel)
		{
			temp = CCP->D1B;
			temp &= ~(CCP_CCPD1B_DATA_Msk);
			temp |= Compare;
			CCP->D1B = temp;
		}
	}
	CCP->LOCK = 0x00;
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  :User_Speed_Out
 * Description    :转速（FG）输出
 * Function Call  :主函数调用
 * Input Paragram :无
 * Return Value   :无
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void User_Speed_Out(void)
{
	if (Basic.FG_Speed >= Capture.FG_SpeedMin)
	{
		//-------------------------------------------------------------------//
		// IO口配置为CCP模式
		CCP_PWM_GPIO(0, 0);

		//-------------------------------------------------------------------//
		if (Capture.FG_SpeedOld != Basic.FG_Speed) // 与保留的旧值不一致则更新
		{
			Capture.FG_SpeedOld = Basic.FG_Speed;
			Capture.FG_Period = (uint16_t)(60 * 1000000 / (MOTOR_PAIRS * Capture.FG_SpeedOld));

			CCP_PWM_Config(CCP_PWM_MODULE, CCP_PWM_CH, CCP_RELOAD_CCPLOAD, Capture.FG_Period, Capture.FG_Period >> 1);
		}

		if (CCP_PWM_MODULE == CCP0)
		{
			if (!(CCP->CON0 & 0x40))	   // 运行标志位判断
				CCP_Start(CCP_PWM_MODULE); // 使能CCP模块
		}
		else
		{
			if (!(CCP->CON1 & 0x40))	   // 运行标志位判断
				CCP_Start(CCP_PWM_MODULE); // 使能CCP模块
		}

		if (!(CCP->RUN & (0x1 << CCP_PWM_MODULE))) // 运行标志位判断
			CCP_EnableRun(CCP_PWM_MODULE);		   // 运行CCP
	}
	else
	{
		//-------------------------------------------------------------------//
		Capture.FG_SpeedOld = 0;
		CCP_Stop(CCP_PWM_MODULE);		// 停止CCP
		CCP_DisableRun(CCP_PWM_MODULE); // 关闭CCP模块

		//-------------------------------------------------------------------//
		// 停机时，IO口配置为GPIO输出模式，并输出为低，故障产生时输出为高
		if (MOTOR_FAULT_CODE == NOERROR && 0 == Fault_Flag)
		{
			CCP_PWM_GPIO(1, 0); // 正常停机输出为低
		}
		else
		{
			CCP_PWM_GPIO(1, 1); // 故障停机输出为高
		}
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  :User_Sleep_Manage
 * Description    :睡眠管理
 * Function Call  :主函数调用
 * Input Paragram :无
 * Return Value   :无
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void User_Sleep_Manage(void)
{
	if ((MOTOR_STATE == MC_INIT) && (Basic.Target_Value == 0) && (Capture.Frequency == 0) && (Capture.DutyCycle == 0))
	{
		if (++Sleep.SleepCount >= 30) // 100*30 = 3s
		{
			Sleep.SleepCount = 0;
			// 关管
			Brige_Output_Off();
			// 清中断使能
			NVIC->ICER[0U] = 0xFFFFFFFF;
			// 特殊处理SysTick
			SysTick->CTRL = 0x00000005;

// 关闭所有模块
#if (ACMP_CH == ACMP_CH0)
			ACMP_Stop(ACMP0);
#else
			ACMP_Stop(ACMP1);
#endif

			ADC_Stop();

#if (PGA_CH == PGA_CH0)
			PGA_Stop(PGA0x);
#endif
#if (PGA_CH == PGA_CH12)
			PGA_Stop(PGA1x);
			PGA_Stop(PGA2x);
#endif
#if (PGA_CH == PGA_CH012)
			PGA_Stop(PGA0x);
			PGA_Stop(PGA1x);
			PGA_Stop(PGA2x);
#endif

			EPWM_DisableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
							   EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
			EPWM_Stop(EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
					  EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);

			// 关闭DAC
			DAC_Stop();

			// 关闭除法时钟
			CGC_PER12PeriphClockCmd(CGC_PER12Periph_DIV, ENABLE);

			//-----------------------------------------------------------------------//
			// 设置唤醒引脚
			// p04输入中断
			GPIO_Init(PORT0, PIN4, PULLDOWN_INPUT);
			GPIO_PinAFInConfig(INT3CFG, 0x04); // 设置P04为INPUT03中端口
			INTM->EGP0 &= ~(0x01 << 3);		   // 设置INPUT03 上升沿触发
			INTM->EGP0 |= 0x01 << 3;
			NVIC_EnableIRQ(INTP3_IRQn);

			//-----------------------------------------------------------------------//
			//			CGC->PMUKEY = 0x0192A;
			//			CGC->PMUKEY = 0x3E4F;
			//			CGC->PMUCTL = 1;
			SCB->SCR |= 0x04;
			__WFI();

			//-----------------------------------------------------------------------//
			// 唤醒之后
			//-----------------------------------------------------------------------//
			DelayTime_ms(2);
			// 关中断
			NVIC_DisableIRQ(INTP3_IRQn);

			GPIO_Init(PORT0, PIN4, INPUT);
			//-----------------------------------------------------------------------//
			// 恢复功能
			// 除法（关时钟之后需要重新配置）
			CGC_PER12PeriphClockCmd(CGC_PER12Periph_DIV, ENABLE);
			DIVSQRT_EnableDIVMode();
			DIVSQRT_EnableSingedMode();

// 开启所有模块
#if (Config_CCP_Capture == CCP_Capture_Enable)
			CCP_Capture_Init();
#endif
#if (Config_CCP_PWM == CCP_PWM_Enable)
			CCP_PWM_Init();
#endif

#if (PGA_CH == PGA_CH0)
			PGA_Start(PGA0x);
#endif
#if (PGA_CH == PGA_CH12)
			PGA_Start(PGA1x);
			PGA_Start(PGA2x);
#endif
#if (PGA_CH == PGA_CH012)
			PGA_Start(PGA0x);
			PGA_Start(PGA1x);
			PGA_Start(PGA2x);
#endif

			ADC_Start();

			DAC_Start();

#if (ACMP_CH == ACMP_CH0)
			ACMP_Start(ACMP0);
#else
			ACMP_Start(ACMP1);
#endif

			EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
							  EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
			EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
					   EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);

			DelayTime_ms(2);
			System_Modules_off();
			MC_Clear_FaultMessage();

			//-----------------------------------------------------------------------//
			// 恢复中断
			__EI_Set();

			SysTick->LOAD = (uint32_t)(MCU_CLK / 1000 - 1UL);
			SysTick->VAL = 0UL;
			SysTick->CTRL = 0x00000007;

			MOTOR_STATE = MC_INIT;
			SYSTEM_STATE = SYS_INIT;
		}
	}
	else
	{
		Sleep.SleepCount = 0;
	}
}

/******************************** END OF FILE *******************************/
