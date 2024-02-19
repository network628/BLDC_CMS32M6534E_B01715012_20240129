//===========================================================================//
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * @file    interrupt.c
 * @author  CMS Motor Control Team:lpj
 * @version �ڶ������ƽ̨
 * @date    2023��2��
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
 * Function Call  : EPWM_IRQHandler�жϺ���
 * Input Paragram :
 * Return Value   : 31us ������
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void EPWM_IRQHandler(void)
{
	//-----------------------------------------------------------------------/
	// �����־
	EPWM_ClearZIFn_Flag();

	//-----------------------------------------------------------------------/
	// ������ȡ
	stru_Sample.ADVal[CH_IP_A] = (uint32_t)ADC->DATA[ADC_DATA_CHA];
	stru_Sample.ADVal[CH_IP_B] = (uint32_t)ADC->DATA[ADC_DATA_CHB];
	FOC_SingleCurrent_Calc();

	//-----------------------------------------------------------------------/
	// ADC������������ (ĸ�ߵ�ѹ��ĸ�ߵ������¶ȡ�����)
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
	// FOC��������
	FOC_Control_Process();

	// //-----------------------------------------------------------------------/
	// // �û�����
	// FOC_User_Control();
	// //---------------------------------- -------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : ADC_IRQHandler
 * Description    :
 * Function Call  : ADC_IRQHandler�жϺ���
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void ADC_IRQHandler(void)
{
	// ADC��������ж�
	if (ADC->MIS & ADC_SCAN_CHA)
	{
		// ����жϱ�־
		ADC_ClearIntFlag_CHA();

		// ������ȡ
		FOC_Read_Current();

		// FOC��������
		FOC_Control_Process();

		// // �û�����
		// FOC_User_Control();
	}
	// �������жϱ�־
	else
	{
		ADC_ClearAllInt_Flag();
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : ACMP_IRQHandler
 * Description    :
 * Function Call  : ����ɲ���Ƚ����ж�
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void ACMP_IRQHandler(void)
{
	// ����жϱ�־
	ACMP_ClearIntFlag(ACMP1);

	// �ر����
	System_Modules_off();

	// ��·��־��λ
	Fault_ShortCircuit = 1;
	MOTOR_FAULT_CODE = FAULT_SHORTCIRCUIT;

	// ϵͳ״̬����
	MOTOR_STATE = MC_INIT;
	SYSTEM_STATE = SYS_FAULT;

	// ������ʼ��
	FOC_ControlPara_Init();
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : SysTick_Handler
 * Description    :
 * Function Call  : SysTick_Handler�ж�
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
// ������ϼ��
#if (Config_Debug_Mode == Debug_Disable)

	MC_MotorFault_Check();

#endif

	//--------------------------------------------------------------------------/
	// �����ʼ״̬����
	MC_MotorInit_Process();

	//--------------------------------------------------------------------------/
	// ���ת�ټ���
	MC_MotorSpeed_Calc();

	//--------------------------------------------------------------------------/
	// �������ģ�����
	MC_Speed_Control();

	//--------------------------------------------------------------------------/
	// ��ѯ������ɱ�־λ
	if ((CCP->RIS & (0x1UL << (CAP3 + 8))) ? 1 : 0)
	{
		Capture.OverFlowFlag = 0;
		Capture.OverFlowTime = 0;
		CCP_ClearCAPMode2IntFlag(CAP3);
		Capture.CompleteFlag = 1; // �ò�����ɱ�־;
	}

	//--------------------------------------------------------------------------/
	// ʱ����ش���
	if (Time.Charge_Count > 0)
		Time.Charge_Count--;
	if (Time.Motor_Restart > 0)
		Time.Motor_Restart--;
	if (Time.Motor_PowerDown > 0)
		Time.Motor_PowerDown--;

	//--------------------------------------------------------------------------/
	// ͨ���ñ�����ظ������״̬�ǵĺ�ʱ
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
		// ĸ�߾�ֵ�������ƹ��ܼ�ʱ
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
 * Function Call  : CCP�ж�
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void CCP_IRQHandler(void)
{
	u32 temp = CCP->LOCK; // ��������״̬����ֹ���ж������ģ�����״̬��ʧ

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
// �½��������ز���
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
// �������½��ز���
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
				Capture.CaptureCount = 0; // ���¿�ʼ�ɼ�
				Capture.CompleteFlag = 1; // �ò�����ɱ�־
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

	CCP->LOCK = temp; // �ָ�����״̬
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : UART_VIEW
 * Description    : �������ݴ�ӡ
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
 * Description    : �û�����ģ��
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_User_Control(void)
{
#if (UART_CONFIG_MODE == UART_ON)
	{
		UART_VIEW(); // ���ݼ��
	}
#endif
}

/******************************** END OF FILE *******************************/
