//===========================================================================//
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * @file    interrupt.c
 * @author  CMS Motor Control Team:lpj
 * @version 第二代风机平台
 * @date    2023年2月
 * @brief
 *---------------------------------------------------------------------------//
 *****************************************************************************/
//===========================================================================//

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

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : EPWM_IRQHandler
 * Description    : R1:30us/64M(2~3us_week)(2us_comm)
 * Function Call  : EPWM_IRQHandler中断函数
 * Input Paragram :
 * Return Value   : 31us 单电阻
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void EPWM_IRQHandler(void)
{
	//-----------------------------------------------------------------------/
	// 清除标志
	EPWM_ClearZIFn_Flag();

	//-----------------------------------------------------------------------/
	// 电流读取
	stru_Sample.ADVal[CH_IP_A] = (uint32_t)ADC->DATA[ADC_DATA_CHA];
	stru_Sample.ADVal[CH_IP_B] = (uint32_t)ADC->DATA[ADC_DATA_CHB];
	FOC_SingleCurrent_Calc();

	//-----------------------------------------------------------------------/
	// ADC轮流触发采样 (母线电压、母线电流、温度、调速)
	static uint8_t bSampleCount = 0;

	ADC->LOCK = ADC_LOCK_WRITE_KEY;
	if (0 == bSampleCount)
	{
		ADC->CHEPWM &= ~(ADC_SCAN_IBUS | ADC_SCAN_TEMP | ADC_SCAN_CTRL);
		ADC->CHEPWM |= ADC_SCAN_VBUS;
		bSampleCount = 1;
	}
	else if (1 == bSampleCount)
	{
		ADC->CHEPWM &= ~(ADC_SCAN_VBUS | ADC_SCAN_TEMP | ADC_SCAN_CTRL);
		ADC->CHEPWM |= ADC_SCAN_IBUS;
		bSampleCount = 2;
	}
	else if (2 == bSampleCount)
	{
		ADC->CHEPWM &= ~(ADC_SCAN_VBUS | ADC_SCAN_IBUS | ADC_SCAN_TEMP);
		ADC->CHEPWM |= ADC_SCAN_CTRL;
		bSampleCount = 3;
	}
	else if (3 == bSampleCount)
	{
		ADC->CHEPWM &= ~(ADC_SCAN_VBUS | ADC_SCAN_IBUS | ADC_SCAN_CTRL);
		ADC->CHEPWM |= ADC_SCAN_TEMP;
		bSampleCount = 0;
	}
	ADC->LOCK = 0x00;

	//-----------------------------------------------------------------------/
	// FOC控制流程
	FOC_Control_Process();

	// //-----------------------------------------------------------------------/
	// // 用户进程
	// FOC_User_Control();
	// //---------------------------------- -------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : ADC_IRQHandler
 * Description    :
 * Function Call  : ADC_IRQHandler中断函数
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void ADC_IRQHandler(void)
{
	// ADC采样完成中断
	if (ADC->MIS & ADC_SCAN_CHA)
	{
		// 清除中断标志
		ADC_ClearIntFlag_CHA();

		// 电流读取
		FOC_Read_Current();

		// FOC控制流程
		FOC_Control_Process();

		// // 用户进程
		// FOC_User_Control();
	}
	// 清所有中断标志
	else
	{
		ADC_ClearAllInt_Flag();
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : ACMP_IRQHandler
 * Description    :
 * Function Call  : 故障刹车比较器中断
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void ACMP_IRQHandler(void)
{
	// 清除中断标志
	ACMP_ClearIntFlag(ACMP1);

	// 关闭输出
	System_Modules_off();

	// 短路标志置位
	Fault_ShortCircuit = 1;
	MOTOR_FAULT_CODE = FAULT_SHORTCIRCUIT;

	// 系统状态重启
	MOTOR_STATE = MC_INIT;
	SYSTEM_STATE = SYS_FAULT;

	// 参数初始化
	FOC_ControlPara_Init();
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : SysTick_Handler
 * Description    :
 * Function Call  : SysTick_Handler中断
 * Input Paragram : 140us
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void SysTick_Handler(void)
{
	//--------------------------------------------------------------------------/
	SysTick->CTRL |= SysTick_CTRL_COUNTFLAG_Msk;

	TimeFlag_1ms = 1;

//--------------------------------------------------------------------------/
// 电机故障检测
#if (Config_Debug_Mode == Debug_Disable)

	MC_MotorFault_Check();

#endif

	//--------------------------------------------------------------------------/
	// 电机初始状态任务
	MC_MotorInit_Process();

	//--------------------------------------------------------------------------/
	// 电机转速计算
	MC_MotorSpeed_Calc();

	//--------------------------------------------------------------------------/
	// 电机调速模块控制
	MC_Speed_Control();

	//--------------------------------------------------------------------------/
	// 查询捕获完成标志位
	if ((CCP->RIS & (0x1UL << (CAP3 + 8))) ? 1 : 0)
	{
		Capture.OverFlowFlag = 0;
		Capture.OverFlowTime = 0;
		CCP_ClearCAPMode2IntFlag(CAP3);
		Capture.CompleteFlag = 1; // 置捕获完成标志;
	}

	//--------------------------------------------------------------------------/
	// 时序相关处理
	if (Time.Charge_Count > 0)
		Time.Charge_Count--;
	if (Time.Motor_Restart > 0)
		Time.Motor_Restart--;
	if (Time.Motor_PowerDown > 0)
		Time.Motor_PowerDown--;

	//--------------------------------------------------------------------------/
	// 通过该变量监控各个电机状态是的耗时
	if (Flag.MC_RunStop)
	{
		Time.Motor_Runing++;

		if (Time.Motor_Runing > 100000)
		{
			Time.Motor_Runing = 0;
		}
	}
	else
	{
		Time.Motor_Runing = 0;
	}

	//--------------------------------------------------------------------------/
	if (MOTOR_STATE == MC_RUN)
	{
		// 母线均值电流限制功能计时
		if (++Time.Motor_Limit_Curent >= 3000)
			Time.Motor_Limit_Curent = 3000;
	}
	else
	{
		Time.Motor_Limit_Curent = 0;
	}

	//--------------------------------------------------------------------------/
}

/****************************************************************************
 ** \brief	TIMER1_IRQHandler
 **
 ** \param [in]  none
 ** \return none
 ** \note
 ****************************************************************************/
void TIMER1_IRQHandler(void)
{
	TMR1->ICLR = TMR_ICLR_ICLR_Msk;
	//--------------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : CCP_IRQHandler
 * Description    :
 * Function Call  : CCP中断
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void CCP_IRQHandler(void)
{
	u32 temp = CCP->LOCK; // 保存锁的状态，防止在中断外操作模块造成状态丢失

	if (CCP->MIS & (0x1UL << CCP_CAPTURE_MODULE))
	{
		Capture.OverFlowFlag = 0;
		Capture.OverFlowTime = 0;

		if (!Capture.CompleteFlag)
		{
			if (Capture.CaptureCount == 0)
			{
#if (CCP_CAPTURE_MODULE == CCP0)
#if (CCP_CAPTURE_CH == CCPxA)
				Capture.CaptureValue[0] = 65000 - ((uint16_t)CCP->D0A);
#else
				Capture.CaptureValue[0] = 65000 - ((uint16_t)CCP->D0B);
#endif
#else
#if (CCP_CAPTURE_CH == CCPxA)
				Capture.CaptureValue[0] = 65000 - ((uint16_t)CCP->D1A);
#else
				Capture.CaptureValue[0] = 65000 - ((uint16_t)CCP->D1B);
#endif
#endif
			}
			else if (Capture.CaptureCount == 1)
			{
#if (CCP_CAPTURE_MODULE == CCP0)
#if (CCP_CAPTURE_CH == CCPxA)
				Capture.CaptureValue[1] = 65000 - ((uint16_t)CCP->D0A);
#else
				Capture.CaptureValue[1] = 65000 - ((uint16_t)CCP->D0B);
#endif
#else
#if (CCP_CAPTURE_CH == CCPxA)
				Capture.CaptureValue[1] = 65000 - ((uint16_t)CCP->D1A);
#else
				Capture.CaptureValue[1] = 65000 - ((uint16_t)CCP->D1B);
#endif
#endif
// 下降到上升沿捕获
#if (CCP_CAPTURE_MODULE == CCP0)
				CCP->LOCK = CCP_P1AB_WRITE_KEY;
				CCP->CON0 &= ~(CCP_CCPCON0_CCP0CM0ES_Msk);
				CCP->CON0 |= CCP_CAP_MODE0_FALLING_TO_RISING;
				CCP->LOCK = 0x00;
#else
				CCP->LOCK = CCP_P1AB_WRITE_KEY;
				CCP->CON1 &= ~(CCP_CCPCON1_CCP1CM0ES_Msk);
				CCP->CON1 |= CCP_CAP_MODE0_FALLING_TO_RISING;
				CCP->LOCK = 0x00;
#endif
			}
			else if (Capture.CaptureCount == 2)
			{
#if (CCP_CAPTURE_MODULE == CCP0)
#if (CCP_CAPTURE_CH == CCPxA)
				Capture.CaptureValue[2] = 65000 - ((uint16_t)CCP->D0A);
#else
				Capture.CaptureValue[2] = 65000 - ((uint16_t)CCP->D0B);
#endif
#else
#if (CCP_CAPTURE_CH == CCPxA)
				Capture.CaptureValue[2] = 65000 - ((uint16_t)CCP->D1A);
#else
				Capture.CaptureValue[2] = 65000 - ((uint16_t)CCP->D1B);
#endif
#endif
			}
			else if (Capture.CaptureCount == 3)
			{
#if (CCP_CAPTURE_MODULE == CCP0)
#if (CCP_CAPTURE_CH == CCPxA)
				Capture.CaptureValue[3] = 65000 - ((uint16_t)CCP->D0A);
#else
				Capture.CaptureValue[3] = 65000 - ((uint16_t)CCP->D0B);
#endif
#else
#if (CCP_CAPTURE_CH == CCPxA)
				Capture.CaptureValue[3] = 65000 - ((uint16_t)CCP->D1A);
#else
				Capture.CaptureValue[3] = 65000 - ((uint16_t)CCP->D1B);
#endif
#endif
// 上升到下降沿捕获
#if (CCP_CAPTURE_MODULE == CCP0)
				CCP->LOCK = CCP_P1AB_WRITE_KEY;
				CCP->CON0 &= ~(CCP_CCPCON0_CCP0CM0ES_Msk);
				CCP->CON0 |= CCP_CAP_MODE0_RISING_TO_FALLING;
				CCP->LOCK = 0x00;
#else
				CCP->LOCK = CCP_P1AB_WRITE_KEY;
				CCP->CON1 &= ~(CCP_CCPCON1_CCP1CM0ES_Msk);
				CCP->CON1 |= CCP_CAP_MODE0_RISING_TO_FALLING;
				CCP->LOCK = 0x00;
#endif
			}

			if (++Capture.CaptureCount > 3)
			{
				Capture.CaptureCount = 0; // 重新开始采集
				Capture.CompleteFlag = 1; // 置捕获完成标志
			}
		}

		CCP->ICLR |= (0x1UL << CCP_CAPTURE_MODULE);

		CCP->LOCK = CCP_P1AB_WRITE_KEY;
		CCP->RUN &= ~(0x1 << CCP_CAPTURE_MODULE);
		CCP->LOCK = 0x00;

		CCP->LOCK = CCP_P1AB_WRITE_KEY;
		CCP->RUN |= (0x1 << CCP_CAPTURE_MODULE);
		CCP->LOCK = 0x00;
	}

	CCP->LOCK = temp; // 恢复锁的状态
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : UART_VIEW
 * Description    : 串口数据打印
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void UART_VIEW(void)
{ // Assuming a maximum of 12 characters for the number
	char str[60];
	{ // Convert number to string
	  // sprintf(str, "-ADVal[0]= %04d,ADVal[1]= %04d,REF[2]= %04d,Ia= %04d -\r\n", stru_Sample.ADVal[0],stru_Sample.ADVal[1],stru_Sample.REF[2],stru_Curr_abc.Ia);
	  // Printf((unsigned char *)str); // Call the existing function to send the string
	  //	UartView(stru_Sample.ADVal[0],stru_Sample.ADVal[1],stru_Sample.REF[2],stru_Curr_abc.Ia);
	}

	{ // Convert number to string
		sprintf(str, "Ref= %04d,RampOut= %04d,STATE= %04d,Power= %04d -\r\n", stru_FOC.Speed_Ref, stru_Speed_Ctrl.RampOut, MOTOR_STATE, Motor_Actual.Power);
		// Basic.Calb_Speed
		// Call the existing function to send the string
		Printf((unsigned char *)str);
		//	UartView(stru_FOC.Speed_Ref,stru_Speed_Ctrl.RampOut,MOTOR_STATE*1000,Motor_Actual.Power);
	}

	//	UartView(MOTOR_STATE*1000,stru_SingleCurr.LN_States*1000,stru_adap_est.Angle>>2,stru_Curr_abc.Ia);
	//	UartView(MOTOR_STATE*1000,stru_adap_est.Angle>>1,stru_rpd_est.Angle>>1,stru_Curr_abc.Ia);
	//	UartView(MOTOR_STATE*1000,stru_adap_est.Angle>>1,stru_rpd_est.Angle>>1,stru_Curr_abc.Ia);
	//	UartView(MOTOR_STATE*1000,stru_Volt_dq.Uq,stru_Curr_dq_ref.Iq,stru_Curr_abc.Ia);
	//	UartView(stru_adap_est.PI_Upper,Basic.Mech_Speed>>3,stru_Volt_dq.Uq,stru_adap_est.PI_Out);
	//	UartView(MOTOR_STATE*1000,Basic.Vbus_Check,stru_Curr_abc.Ia,stru_Sample.ARR[3]);
	//	UartView(MOTOR_STATE*1000,Basic.Mech_Speed>>3,stru_adap_est.PI_Kp>>1,stru_Curr_abc.Ia);
	//	UartView(MOTOR_STATE*1000,stru_bemf.Speed,stru_bemf.Angle>>1,stru_Curr_abc.Ia);
	//	UartView(MOTOR_STATE*1000,stru_Curr_abc.Ia,stru_Curr_abc.Ib,stru_Curr_abc.Ic);
	//	UartView(MOTOR_STATE*1000,stru_adap_est.PI_Kp,stru_adap_est.PI_Ki,stru_Curr_abc.Ic);
	//	UartView(MOTOR_STATE*1000,stru_SingleCurr.LN_States*1000,stru_SingleCurr.LN_Flag*1000,stru_Curr_abc.Ia);

	//	UartView(MOTOR_STATE*1000,MOTOR_FAULT_CODE*100,stru_Curr_dq_ref.Iq,stru_Curr_abc.Ia);
	//	UartView(stru_bemf.AD_Value[0],stru_bemf.AD_Value[1],stru_bemf.AD_Value[2],stru_bemf.Angle>>3);
	//	UartView(stru_bemf.Ea,stru_bemf.Angle>>3,stru_Curr_dq_ref.Iq,stru_bemf.Status*100);
	//	UartView(stru_Volt_dq.Uq,stru_Curr_abc.Ia,Basic.Mech_Speed>>3,MOTOR_STATE*1000);
	//   UartView(100,200,300,500);
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_User_Control
 * Description    : 用户操作模块
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_User_Control(void)
{
#if (UART_CONFIG_MODE == UART_ON)
	{
		UART_VIEW(); // 数据监测
	}
#endif
}

/******************************** END OF FILE *******************************/
