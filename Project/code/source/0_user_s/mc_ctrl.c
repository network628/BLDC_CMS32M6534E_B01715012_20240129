//==========================================================================//
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * @file    motro_control.c
 * @author  CMS Motor Control Team:lpj
 * @version �ڶ������ƽ̨
 * @date    2023��2��
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

volatile SystStates SYSTEM_STATE = SYS_INIT; /* ϵͳ����״̬�� */

volatile MotorStates MOTOR_STATE = MC_INIT; /* �����������״̬�� */

volatile Union_Fault u_Fault_Flag = {0}; /* �������ָʾ��־ */

volatile FaultCode MOTOR_FAULT_CODE = NOERROR; /* �������ָʾ��־ */

struct_Basic_Para Basic = {0}; /* ���������ṹ�� */

struct_Falg Flag = {0}; /* ��־λ�ṹ�� */

struct_Time Time = {0}; /* ʱ������ṹ�� */

struct_Count Count = {0}; /* ��ѧ�����ṹ�� */

struct_Fault Fault = {0}; /* ���ϲ����ṹ�� */

struct_MotCtrl_Para Para = {0}; /* ������Ʋ����ṹ�� */

struct_Config Config = {0}; /* ϵͳ����ѡ��ṹ�� */

struct_Base_Value stru_base_value = {0}; /* �����ֵ�����ṹ�� */

struct_ADC_Sample stru_Sample = {0}; /* ���������ṹ�� */

struct_FOC_Ctrl stru_FOC = {0}; /* FOC���Ʋ����ṹ�� */

struct_SpeedRamp stru_Speed_Ctrl = {0}; /* �ٶȿ��ƽṹ�� */

struct_MotorRuning Motor_Actual = {0}; /* ���ʵ�ʷ������� */

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/

//===========================================================================/
//***** definitions  end ****************************************************/
//===========================================================================/

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : User_Control
 * Description    :
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/

//===========================================================================/
// �������ʹ��
void User_Motor_On(void)
{
	Flag.MC_RunStop = 1;
}

//===========================================================================/
// ���ֹͣ
void User_Motor_Off(void)
{
	Flag.MC_RunStop = 0;
}

//===========================================================================/
// ����������
void User_Motor_FRControl(uint8_t Input_Val)
{
	if (Input_Val == FR_CW)
		Config.MotorDir = FR_CW;
	else
		Config.MotorDir = FR_CCW;
}

//===========================================================================/
// ���ٿ���
void User_MotorSpeed_Set(int32_t Input_Val)
{
//-----------------------------------------------------------------------/
// ���������ƣ�����ֵ/32767 = ��������/������--�������ֵ
#if (Config_Contorl_Mode == Current_Loop)

	if (Input_Val > stru_FOC.Curr_Iq_Max)
		Input_Val = stru_FOC.Curr_Iq_Max;
	stru_FOC.Speed_Ref = Input_Val;

#endif

//-----------------------------------------------------------------------/
// �ٶȻ�����
#if (Config_Contorl_Mode == Speed_Loop)

#if (LIMIT_SPEED_ENABLE)
	if (Input_Val > LIMIT_SPEED_VALUE)
		Input_Val = (int32_t)LIMIT_SPEED_VALUE;
#endif

	stru_FOC.Speed_Ref = FRACMPY_Q10(Input_Val, stru_Sample.G_Sp);

#endif

//-----------------------------------------------------------------------/
// ���ʻ�����
#if (Config_Contorl_Mode == Power_Loop)

// ֱ�Ӹ�������ֵ
#if 1
	if (Input_Val > (int32_t)MOTOR_POWER_BASE)
		Input_Val = (int32_t)MOTOR_POWER_BASE;

	stru_FOC.Speed_Ref = (int32_t)(Input_Val * stru_Sample.G_Pw) >> 6;

// ���յ����źű�������
#else

	stru_FOC.Speed_Ref = Input_Val;

#endif

#endif
	//-----------------------------------------------------------------------/
}

//===========================================================================/
// �ر����й���ģ��
void System_Modules_off(void)
{
	Brige_Output_Off();
	User_Motor_Off();
}

//===========================================================================/
// EPWM����ɲ����������
void EPWM_ResetFaultBrake(void)
{
	EPWM_ClearBrakeIntFlag(); /// ���ɲ���жϱ�־λ
	EPWM_ClearBrake();
	EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
}

//===========================================================================//

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : System_Control
 * Description    : ϵͳ״̬�����̿���
 * Function Call  : ��ѭ������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void System_Control(void)
{
	switch (SYSTEM_STATE)
	{
	//===================================================================//
	// ϵͳ��ʼ��
	case SYS_INIT:
	{
		DelayTime_ms(20);
		// ����ϵ��ȡADCƫ��ֵ(�����ƫ��ֵ)
		GET_ADC_REF_VAL();
		// FOC���Ʋ�����ʼ��
		FOC_ControlPara_Init();
		// ��ֵ������ʼ��
		FOC_BaseParameter_Init();
		// �������㣬��ֹ�ϵ�����
		EPWM_ResetFaultBrake();

#if (Motor_Debug_Mode == Motor_Debug_Online)
		//		DBG->DBGSTOPCR |= (0<<24);
#else
		// ����SWO���Խӿڹ��� (������¼������Ҫ��)
		//		DBG->DBGSTOPCR |= (1<<24);
#endif
		SYSTEM_STATE = SYS_RUN;

		break;
	}
	//===================================================================//
	// ϵͳ��������״̬
	case SYS_RUN:
	{
		// �����ٿ���
		if (Time.Motor_Restart == 0)
		{
			// ���ϱ���ʱ���ػ�
			if (MOTOR_FAULT_CODE || Fault_Flag)
			{
				User_Motor_Off();
			}
			// ������ٿ���
			else
			{
				User_Speed_Control();
			}
		}
		break;
	}
	//===================================================================//
	// �������״̬
	case SYS_RESTART:
	{
		// ���ػ�
		User_Motor_Off();
		// �������ʱ��
		Time.Motor_Restart = (uint32_t)((float)FAULT_RESTART_DELAY_TIME * 1000);
		// ����ʧ��������������
		if (++Count.Motor_Restart >= ((uint32_t)FAULT_RESTART_TIMES + 1))
		{
			Count.Motor_Restart = (uint32_t)FAULT_RESTART_TIMES + 1;
			SYSTEM_STATE = SYS_WAIT;
		}
		else
		{
			Fault_StartFail = 0;
			MOTOR_FAULT_CODE = NOERROR;
			SYSTEM_STATE = SYS_RUN;
		}
		// �������Ϊ����״̬
		MOTOR_STATE = MC_INIT;

		break;
	}
	//===================================================================//
	// ����ͣ��̬
	case SYS_FAULT:
	{
		// ���ػ�
		User_Motor_Off();
		Time.Motor_Restart = 0;
		SYSTEM_STATE = SYS_WAIT;

		// �������Ϊ����״̬
		MOTOR_STATE = MC_INIT;

		break;
	}
	//===================================================================//
	// �ȴ����ϱ����ָ�
	case SYS_WAIT:
	{
		// ϵͳ�ػ�
		System_Modules_off();
		// ��������źŸ�λ����
		if ((Capture.Frequency == 0) && (Capture.DutyCycle == 0))
		{
			// ���������Ϣ
			//				EPWM_ResetFaultBrake();
			//				MC_Clear_FaultMessage();
			//				Count.Motor_Restart = 0;
			//				//ϵͳ�ָ�����״̬
			//				SYSTEM_STATE = SYS_RUN;
		}
		break;
	}
	//===================================================================//
	default:
		break;
	}
	//=======================================================================//
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_PhaseLoss_Check
 * Description    : ȱ�ౣ������
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_PhaseLoss_Check(void)
{
	static uint8_t bPhaseLossCount = 0;
	static uint16_t wPhaseABCLossCount = 0;
	//-------------------------------------------------------------------------------------/
	if ((MOTOR_STATE == MC_START) || (MOTOR_STATE == MC_SW) || (MOTOR_STATE == MC_RUN))
	{
		if (++bPhaseLossCount > 50)
		{
			bPhaseLossCount = 40;
			//-------------------------------------------------------------------------------------/
			// A������ж�
			if (((Fault.Ia_max > Fault.Ib_max * 2) || (Fault.Ia_max > Fault.Ic_max * 2)) && (Fault.Ia_max > Fault.Phaseloss_Value))
			{
				Fault.PhaseA_LossCnt++;
			}
			else
			{
				Fault.PhaseA_LossCnt = 0;
			}
			//-------------------------------------------------------------------------------------/
			// B������ж�
			if (((Fault.Ib_max > Fault.Ia_max * 2) || (Fault.Ib_max > Fault.Ic_max * 2)) && (Fault.Ib_max > Fault.Phaseloss_Value))
			{
				Fault.PhaseB_LossCnt++;
			}
			else
			{
				Fault.PhaseB_LossCnt = 0;
			}
			//-------------------------------------------------------------------------------------/
			// C������ж�
			if (((Fault.Ic_max > Fault.Ia_max * 2) || (Fault.Ic_max > Fault.Ib_max * 2)) && (Fault.Ic_max > Fault.Phaseloss_Value))
			{
				Fault.PhaseC_LossCnt++;
			}
			else
			{
				Fault.PhaseC_LossCnt = 0;
			}

			if ((Fault.Ia_max < Fault.Phaseloss_Value) && (Fault.Ib_max < Fault.Phaseloss_Value) && (Fault.Ic_max < Fault.Phaseloss_Value))
			{
				wPhaseABCLossCount++;
			}
			else
			{
				wPhaseABCLossCount = 0;
			}
			//-------------------------------------------------------------------------------------/
			Fault.Ia_max = 0;
			Fault.Ib_max = 0;
			Fault.Ic_max = 0;
			//-------------------------------------------------------------------------------------/
			if ((Fault.PhaseA_LossCnt > FAULT_PHASELOSS_TIME) || (Fault.PhaseB_LossCnt > FAULT_PHASELOSS_TIME) || (Fault.PhaseC_LossCnt > FAULT_PHASELOSS_TIME) || wPhaseABCLossCount > FAULT_PHASELOSS_TIME)
			{
				bPhaseLossCount = 0;
				Fault.PhaseA_LossCnt = 0;
				Fault.PhaseB_LossCnt = 0;
				Fault.PhaseC_LossCnt = 0;
				wPhaseABCLossCount = 0;
				Fault_PhaseLoss = 1;
				MOTOR_FAULT_CODE = FAULT_PHASELOSS;
				SYSTEM_STATE = SYS_FAULT;
			}
			//-------------------------------------------------------------------------------------/
		}
	}
	else
	{
		bPhaseLossCount = 0;
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_MotorBlock_Check
 * Description    : ��ת��������
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_MotorBlock_Check(void)
{
	static uint16_t hBlockCount = 0;

	//-------------------------------------------------------------------------/
	// ת�ٳ����趨��ת�ٷ�Χ��Ϊ��ת����
	if ((MOTOR_STATE == MC_SW) || (MOTOR_STATE == MC_RUN))
	{
		if ((Basic.Mech_Speed < FAULT_SPEED_MIN) || (Basic.Mech_Speed > FAULT_SPEED_MAX) ||
			((Basic.Mech_Speed > FAULT_SPEED_LOW_GEAR) && (Basic.Target_Value == POWER_FAULT_LOW)) ||
			((Basic.Mech_Speed > FAULT_SPEED_MID_GEAR) && (Basic.Target_Value == POWER_FAULT_MID)) ||
			((Basic.Mech_Speed > FAULT_SPEED_HIGH_GEAR) && (Basic.Target_Value == POWER_FAULT_HIGH)) ||
			((Basic.Mech_Speed > (FAULT_SPEED_LOW_GEAR + Fault.Block_Fault_K1 * (Motor_Actual.Power - POWER_FAULT_LOW))) && (Basic.Target_Value <= (POWER_FAULT_MID))) ||
			((Basic.Mech_Speed > (FAULT_SPEED_MID_GEAR + Fault.Block_Fault_K2 * (Motor_Actual.Power - POWER_FAULT_MID))) && (Basic.Target_Value > (POWER_FAULT_MID))))

		{
			hBlockCount++;
		}
		else
		{
			hBlockCount = 0;
		}
		if (hBlockCount > FAULT_BLOCK_TIME)
		{
			hBlockCount = 0;
			Fault_Block = 1;
			MOTOR_FAULT_CODE = FAULT_BLOCK;
			SYSTEM_STATE = SYS_FAULT;
		}
	}
	else
	{
		hBlockCount = 0;
	}
	//-------------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_Voltage_Check
 * Description    : ��ѹ��������
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_Voltage_Check(void)
{
	static uint16_t hFaultOverCount = 0, hFaultLowerCount = 0, hFaultRecoverCount = 0,
					hFaultStartOverCount = 0, hFaultStartLowerCount = 0;
	if (MOTOR_STATE == MC_INIT)
	{
		if (Basic.Vbus_Check > Fault.StartOverVoltage_Value)
		{
			// ������ѹ����
			if (++hFaultStartOverCount > FAULT_START_OVER_VBUS_TIME)
			{
				hFaultStartOverCount = 0;
				if (0 == Fault_OverVoltage)
				{
					MOTOR_FAULT_CODE = FAULT_STARTOVERVOLTAGE;
				}
				User_Motor_Off();
				return;
			}
		}
		else if (Basic.Vbus_Check < Fault.StartLowerVoltage_Value)
		{
			// ����Ƿѹ����
			if (++hFaultStartLowerCount > FAULT_START_LOWER_VBUS_TIME)
			{
				hFaultStartLowerCount = 0;
				if (0 == Fault_UnderVoltage)
				{
					MOTOR_FAULT_CODE = FAULT_STARTUNDERVOLTAGE;
				}
				User_Motor_Off();
				return;
			}
		}
		else
		{
			hFaultStartOverCount = 0;
			hFaultStartLowerCount = 0;
			// ��Ƿѹ�����ָ�
			if (Basic.Vbus_Check >= Fault.UV_Recover_Value && Basic.Vbus_Check <= Fault.OV_Recover_Value)
			{
				if (++hFaultRecoverCount > FAULT_RECOVER_VBUS_TIME)
				{
					hFaultRecoverCount = 0;
#if (FAULT_VOLTAGE_RECOVER_ENABLE)
					if ((MOTOR_FAULT_CODE == FAULT_STARTUNDERVOLTAGE) || (MOTOR_FAULT_CODE == FAULT_STARTOVERVOLTAGE) ||
						(MOTOR_FAULT_CODE == FAULT_RUNUNDERVOLTAGE) || (MOTOR_FAULT_CODE == FAULT_RUNOVERVOLTAGE))
					{
						MOTOR_FAULT_CODE = NOERROR;
						Fault_OverVoltage = 0;
						Fault_UnderVoltage = 0;
						SYSTEM_STATE = SYS_RUN;
					}
#else
					if ((MOTOR_FAULT_CODE == FAULT_STARTUNDERVOLTAGE) || (MOTOR_FAULT_CODE == FAULT_STARTOVERVOLTAGE))
					{
						MOTOR_FAULT_CODE = NOERROR;
					}
#endif
				}
			}
			else
			{
				hFaultRecoverCount = 0;
			}
		}
	}
	else
	{
		hFaultRecoverCount = 0;
		hFaultStartLowerCount = 0;
		hFaultStartOverCount = 0;
	}

	if (MOTOR_STATE == MC_RUN || MOTOR_STATE == MC_SW || MOTOR_STATE == MC_START)
	{
		if (Basic.Vbus_Check > Fault.RunOverVoltage_Value)
		{
			// ���й�ѹ����
			if (++hFaultOverCount > FAULT_RUN_OVER_VBUS_TIME)
			{
				hFaultOverCount = 0;
				Fault_OverVoltage = 1;
				MOTOR_FAULT_CODE = FAULT_RUNOVERVOLTAGE;
				SYSTEM_STATE = SYS_FAULT;
				User_Motor_Off();
			}
		}
		else if (Basic.Vbus_Check < Fault.RunLowerVoltage_Value)
		{
			// ����Ƿѹ����
			if (++hFaultLowerCount > FAULT_RUN_LOWER_VBUS_TIME)
			{
				hFaultLowerCount = 0;
				Fault_UnderVoltage = 1;
				MOTOR_FAULT_CODE = FAULT_RUNUNDERVOLTAGE;
				SYSTEM_STATE = SYS_FAULT;
				User_Motor_Off();
			}
		}
		else
		{
			hFaultOverCount = 0;
			hFaultLowerCount = 0;
		}
	}
	else
	{
		hFaultOverCount = 0;
		hFaultLowerCount = 0;
	}
}

/*****************************************************************************
 ** \brief	MC_IBUS_Check
 **
 ** \param [in] none
 ** \return  none
 ** \note	ĸ�߹������
 *****************************************************************************/
void MC_IBUS_Check(void)
{
	static uint16_t hOverIbusCount = 0;

	if (MOTOR_STATE == MC_RUN || MOTOR_STATE == MC_SW)
	{
		if (Motor_Actual.Ibus > FAULT_OVER_IBUS * 100)
		{
			if (++hOverIbusCount > FAULT_OVER_IBUS_TIME) // ����500ms����
			{
				hOverIbusCount = 0;
				Fault_OverCurrent = 1;
				MOTOR_FAULT_CODE = FAULT_OVERIBUSCURRENT;
				SYSTEM_STATE = SYS_FAULT;
			}
		}
		else
		{
			if (hOverIbusCount > 0)
			{
				hOverIbusCount--;
			}
		}
	}
	else
	{
		hOverIbusCount = 0;
	}
}

/*****************************************************************************
 ** \brief	MC_Temperature_Check
 **
 ** \param [in] none
 ** \return  none
 ** \note	NTC�¶ȼ��
 *****************************************************************************/
void MC_Temperature_Check(void)
{
	static uint16_t hOverTemperatureCount = 0, hLowerTemperatureCount = 0, hRechOverTemperatureCount = 0;
	// ���±���
	if (Basic.VTem > Fault.Lower_Temperature_Value)
	{
		if (++hLowerTemperatureCount > FAULT_LOWER_TEMPERATURE_TIME)
		{
			hLowerTemperatureCount = FAULT_LOWER_TEMPERATURE_TIME;
			Fault_NTC_Temperature = 1;
			MOTOR_FAULT_CODE = FAULT_LOWERTEMPERATURE;
			SYSTEM_STATE = SYS_FAULT;
		}
	}
	// ���±���
	else if (Basic.VTem < Fault.Over_Temperature_Value)
	{
		if (++hOverTemperatureCount > FAULT_OVER_TEMPERATURE_TIME)
		{
			hOverTemperatureCount = FAULT_OVER_TEMPERATURE_TIME;
			Fault_NTC_Temperature = 1;
			MOTOR_FAULT_CODE = FAULT_OVERTEMPERATURE;
			SYSTEM_STATE = SYS_FAULT;
		}
	}
	else
	{
		hLowerTemperatureCount = 0;
		hOverTemperatureCount = 0;
		// NTC�����ָ�
		if (Basic.VTem >= Fault.Over_Temperature_Recover_Value && Basic.VTem <= Fault.Lower_Temperature_Recover_Value)
		{
			if (++hRechOverTemperatureCount > FAULT_RECOVER_TEMPERATURE_TIME)
			{
				hRechOverTemperatureCount = 0;
#if (FAULT_TEMPERATURE_RECOVER_ENABLE)
				if ((MOTOR_FAULT_CODE == FAULT_LOWERTEMPERATURE) || (MOTOR_FAULT_CODE == FAULT_OVERTEMPERATURE))
				{
					MOTOR_FAULT_CODE = NOERROR;
					Fault_NTC_Temperature = 0;
					SYSTEM_STATE = SYS_RUN;
				}
#endif
			}
		}
		else
		{
			hRechOverTemperatureCount = 0;
		}
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_MotorFault_Check
 * Description    : ������ϱ������
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_MotorFault_Check(void)
{
//------------------------------------------------------------------------/
// ȱ����
#if (FAULT_PHASELOSS_ENABLE)
	MC_PhaseLoss_Check();
#endif

//------------------------------------------------------------------------/
// ��ת���
#if (FAULT_BLOCK_ENABLE)
	MC_MotorBlock_Check();
#endif

//------------------------------------------------------------------------/
// ��ѹ���
#if (FAULT_VOLTAGE_ENABLE)
	MC_Voltage_Check();
#endif

//------------------------------------------------------------------------/
// ĸ�߾�ֵ�������
#if (FAULT_AVERAGE_CURRENT_ENABLE)
	MC_IBUS_Check();
#endif

//------------------------------------------------------------------------/
// ģ��NTC�¶ȼ��
#if (FAULT_TEMPERATURE_ENABLE)
	MC_Temperature_Check();
#endif
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_MotorSpeed_Calc
 * Description    : ���ת�ټ���
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_MotorSpeed_Calc(void)
{
	// ������ת��
	Basic.Mech_Speed = (stru_FOC.Elec_We * Basic.Speed_Base) / (Basic.We * Basic.ERAMP);

	// FG���ת��
	Basic.FG_Speed = Basic.Mech_Speed + SPEED_OFFSET;

	// ת�ٱ궨ֵ���㣬�����ٶȻ�
	Basic.Calb_Speed = FRACMPY_Q10(Basic.Mech_Speed, stru_Sample.G_Sp);

	// ���㷴�綯���ٶ�
	stru_bemf.Speed = (stru_bemf.DeltaTheta * Basic.Speed_Base) / Basic.We;
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_Clear_FaultMessage
 * Description    : ���������Ϣ
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_Clear_FaultMessage(void)
{
	Fault_Flag = 0;
	Fault_Block = 0;
	Fault_PhaseLoss = 0;
	Fault_OverCurrent = 0;
	Fault_StartFail = 0;
	Fault_NTC_Temperature = 0;
	Fault.Block_Cnt = 0;
	Fault.Brake_Cnt = 0;
	Fault.StartFail_Cnt = 0;
	MOTOR_FAULT_CODE = NOERROR;
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : ADC0_CALB
 * Description    : ADC0ͨ������
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void ADC_Sample(void)
{
	//----------------------------------------------------------------------/
	// �����ѹ����
	stru_Sample.ADVal[CH_VBUS] = (uint32_t)ADC->DATA[ADC_DATA_VBUS];
	// �����������
	stru_Sample.ADVal[CH_IBUS] = (uint32_t)ADC->DATA[ADC_DATA_IBUS];
	// NTC�¶Ȳ���
	stru_Sample.ADVal[CH_TEMP] = (uint32_t)ADC->DATA[ADC_DATA_TEMP];
	// ���ٵ�ѹ����
	stru_Sample.ADVal[CH_VCTR] = (uint32_t)ADC->DATA[ADC_DATA_CTRL];

//----------------------------------------------------------------------/
// Bandgap У������ֵ
#if (CONFIG_BANDGAP_MODE == BANDGAP_ENABLE)
	int32_t Aux;

	// ������ٵ�ѹ
	Aux = FRACMPY_Q10((stru_Sample.ADVal[CH_VCTR] - stru_Sample.REF[CH_VCTR]), stru_Sample.G_Vc);
	stru_Sample.ARR[CH_VCTR] = FRACMPY_Q12(Aux, stru_Sample.G_BG);
	// ����ĸ�ߵ�ѹ
	Aux = FRACMPY_Q10((stru_Sample.ADVal[CH_VBUS] - stru_Sample.REF[CH_VBUS]), stru_Sample.G_Vb);
	stru_Sample.ARR[CH_VBUS] = FRACMPY_Q12(Aux, stru_Sample.G_BG);
	// ����ĸ�ߵ���
	Aux = FRACMPY_Q10((stru_Sample.ADVal[CH_IBUS] - stru_Sample.REF[CH_IBUS]), stru_Sample.G_Ib);
	stru_Sample.ARR[CH_IBUS] = FRACMPY_Q12(Aux, stru_Sample.G_BG);

	// ����NTC�¶�
	Aux = FRACMPY_Q10((stru_Sample.ADVal[CH_TEMP] - stru_Sample.REF[CH_TEMP]), stru_Sample.G_Vc);
	stru_Sample.ARR[CH_TEMP] = FRACMPY_Q12(Aux, stru_Sample.G_BG);

#else

	// ������ٵ�ѹ
	stru_Sample.ARR[CH_VCTR] = FRACMPY_Q10((stru_Sample.ADVal[CH_VCTR] - stru_Sample.REF[CH_VCTR]), stru_Sample.G_Vc);
	// ����ĸ�ߵ�ѹ
	stru_Sample.ARR[CH_VBUS] = FRACMPY_Q10((stru_Sample.ADVal[CH_VBUS] - stru_Sample.REF[CH_VBUS]), stru_Sample.G_Vb);
	// ����ĸ�ߵ���
	stru_Sample.ARR[CH_IBUS] = FRACMPY_Q10((stru_Sample.ADVal[CH_IBUS] - stru_Sample.REF[CH_IBUS]), stru_Sample.G_Ib);
	// ����NTC�¶�
	stru_Sample.ARR[CH_TEMP] = FRACMPY_Q10((stru_Sample.ADVal[CH_TEMP] - stru_Sample.REF[CH_TEMP]), stru_Sample.G_Vc);

#endif

	// ĸ�ߵ�ѹ�����˲�
	Basic.Vbus = DFILTER(stru_Sample.ARR[CH_VBUS], Basic.Vbus, Para.Lpf.Samp_Vbus);
	// �����ź������˲�
	Basic.VCtr = DFILTER(stru_Sample.ARR[CH_VCTR], Basic.VCtr, Para.Lpf.Samp_Vctrl);
	// ĸ�ߵ��������˲�
	Basic.Ibus = DFILTER(stru_Sample.ARR[CH_IBUS], Basic.Ibus, Para.Lpf.Samp_Ibus);
	;
	if (Basic.Ibus < 0)
	{
		Basic.Ibus = 0;
	}
	// NTC�ź������˲�
	Basic.VTem = DFILTER(stru_Sample.ARR[CH_TEMP], Basic.VTem, Para.Lpf.Samp_Vctrl);

	// ĸ�ߵ�ѹ(ʵʱֵ)��������
	Basic.Vbus_Check = stru_Sample.ARR[CH_VBUS];
	//----------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : SysTick_1ms
 * Description    : 1ms��ʱ����
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
//---------------------------------------------------------------------------/
void SysTick_1ms(void)
{
	TimeFlag_1ms = 0;

	//=======================================================================//
	// ADCͨ������
	ADC_Sample();

	//=======================================================================//
	// 10ms ������
	if (++Count.TimeBase[0] >= 10) // 10ms
	{
		Count.TimeBase[0] = 0;
		//-----------------------------------------------------------------------/
		// �û�����
		FOC_User_Control();
		//-----------------------------------------------------------------------/
//-------------------------------------------------------------------//
// ���ٲ���--PWM
#if (Config_CCP_Capture == CCP_Capture_Enable)
		User_Speed_Capture();
#endif

		//-------------------------------------------------------------------//
		// �ٶȷ���--FG

#if (Config_CCP_PWM == CCP_PWM_Enable)
		User_Speed_Out();
#endif

		//-------------------------------------------------------------------//
	}

	//=======================================================================//
	// 100ms ����
	if (++Count.TimeBase[1] >= 100) // 100ms
	{
		Count.TimeBase[1] = 0;

// ˯�ߴ���
#if (Sleep_Control_Mode == Sleep_Enable)
		User_Sleep_Manage();
#endif
	}
	//=======================================================================//
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_Ramp_Control
 * Description    :
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_Ramp_Control(struct_SpeedRamp *Ramp, int32_t Ref)
{
	Ramp->RampTarget = Ref;

	if (Ramp->RampTarget > Ramp->RampOut)
	{
		if ((Ramp->RampOut + Ramp->RampInc) <= Ramp->RampTarget)
		{
			Ramp->RampOut += Ramp->RampInc;
		}
		else
		{
			Ramp->RampOut = Ramp->RampTarget;
		}
	}
	else if (Ramp->RampTarget < Ramp->RampOut)
	{
		if ((Ramp->RampOut - Ramp->RampDec) >= Ramp->RampTarget)
		{
			Ramp->RampOut -= Ramp->RampDec;
		}
		else
		{
			Ramp->RampOut = Ramp->RampTarget;
		}
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_Speed_Control
 * Description    : �ٶȻ� / ���ʻ� ����
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_Speed_Control(void)
{
	static uint8_t bSpeedLimitFlag = 0;

#if ((LIMIT_POWER_ENABLE) || (LIMIT_SPEED_ENABLE) || (LIMIT_CURRENT_ENABLE))

	// static uint16_t hSpeedLimitCount = 0;
	static uint16_t hSpeedTargetBeforeLimit;
#if (LIMIT_SPEED_ENABLE && LIMIT_CURRENT_ENABLE)
	static uint8_t bSpeedCurrentLimitFlag;
#endif
#if (LIMIT_SPEED_ENABLE)
	static uint16_t hPowerTargetBeforeLimit;
#endif
#if (LIMIT_CURRENT_ENABLE)
	static uint16_t hIbusLimitCount = 0;
	volatile int32_t wTempCalcIbus = 0;
#endif

#endif

	//=======================================================================//
	// ���ʵ�ʵ�ѹ�������ʼ���
	Motor_Actual_Calculate();

	//=========================================================================//
	// Ӳ�����ʼ���
	int32_t wTempPower = 0;

	wTempPower = FRACMPY_Q15(Basic.Vbus, Basic.Ibus);
	Basic.Power = wTempPower; // DFILTER( wTempPower, Basic.Power, Para.Lpf.Power_Val );

	//=========================================================================//
	if (MOTOR_STATE == MC_RUN)
	{
		//=======================================================================//
		Config.Mode_Svpwm = Config_SVPWM_Mode;

		//=========================================================================//

		// ���ٿ���ģʽ����ת��/��ת��/�㹦��
		if (++stru_FOC.W_Cycle >= stru_FOC.WRAMP)
		{
			stru_FOC.W_Cycle = 0;
			// �����ź����´���
			MC_Ramp_Control(&stru_Speed_Ctrl, stru_FOC.Speed_Ref);
//=======================================================================//
// ���ػ�����
#if (Config_Contorl_Mode == Current_Loop)
			{
// �޹���ʹ��,���޹��ʺ���ת��ͬʱʹ�ܣ���ִ���޹��ʿ���
#if (LIMIT_POWER_ENABLE)
				// ������״̬���жϵ�ǰ�����Ƿ񳬹��޶�����
				if (!bSpeedLimitFlag)
				{
					if (Basic.Power > stru_FOC.PowerLimit)
					{
						// ��ֹƵ���������˳�����ģʽ
						if (++hSpeedLimitCount > (500 / stru_FOC.WRAMP))
						{
							bSpeedLimitFlag = 1;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // ���浱ǰ��λ

							PID_WL.Kp = 16384; // ���ʻ�KP = _Q15(0.5), �ܿ���������
							PID_WL.Ki = 1638;  // ���ʻ�KI = _Q15(0.05), �ܿ���������
							PID_WL.KiSum = stru_Curr_dq_ref.Iq << 14;
							PID_WL.Integral = stru_Curr_dq_ref.Iq;
						}
					}
				}
				// ����״̬���ж��Ƿ���Ҫȡ������
				else
				{
					// ��λ�½�ʱȡ������
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 100))
					{
						bSpeedLimitFlag = 0;
					}

					if (!bSpeedLimitFlag)
					{
						PID_WL.Kp = Para.FOC.Wkp; // �ָ�ת�ٻ�PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}

// ����ʹ��
#elif (LIMIT_SPEED_ENABLE)
				// ������״̬���жϵ�ǰת���Ƿ񳬹��޶�ת��
				if (!bSpeedLimitFlag)
				{
					if (Basic.Mech_Speed > LIMIT_SPEED_VALUE)
					{
						// ��ֹƵ���������˳�����ģʽ
						if (++hSpeedLimitCount > (500 / stru_FOC.WRAMP))
						{
							bSpeedLimitFlag = 1;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // ���浱ǰ��λ

							PID_WL.Kp = 16384; // �ٶȻ�KP = _Q15(0.5), �ܿ���������
							PID_WL.Ki = 1638;  // �ٶȻ�KI = _Q15(0.05), �ܿ���������
							PID_WL.KiSum = stru_Curr_dq_ref.Iq << 14;
							PID_WL.Integral = stru_Curr_dq_ref.Iq;
						}
					}
				}
				// ����״̬���ж��Ƿ���Ҫȡ������
				else
				{
					// ��λ�½�ʱȡ������
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 100))
					{
						bSpeedLimitFlag = 0;
					}
					if (!bSpeedLimitFlag)
					{
						PID_WL.Kp = Para.FOC.Wkp; // �ָ����ʻ�PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}

#endif // end limit power/speed enable

				if (bSpeedLimitFlag)
				{
// �޹���ʹ��
#if (LIMIT_POWER_ENABLE)
					// �㹦��ʱ��PI����
					stru_FOC.WRAMP = TIME_POWER_LOOP;
					// �޹���ʱ�㹦�ʿ���
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.PowerLimit, Basic.Power);
// ����ʹ��
#elif (LIMIT_SPEED_ENABLE)
					// ��ת��ʱ��PI����
					stru_FOC.WRAMP = TIME_SPEED_LOOP;
					// ����ʱ��ת�ٿ���
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, Basic.Calb_SpeedLimit, Basic.Calb_Speed);
#endif
				}
				else
				{
					// ������ʱ��PI����
					stru_FOC.WRAMP = 1;
					// ����״̬�����ؿ���
					stru_FOC.Curr_Is_Ref = stru_Speed_Ctrl.RampOut;
				}
			}
//=======================================================================//
// ת�ٻ�����
#elif (Config_Contorl_Mode == Speed_Loop)
			{
// �޹���ʹ��
#if (LIMIT_POWER_ENABLE)
				// ������״̬���жϵ�ǰ�����Ƿ񳬹��޶�����
				if (!bSpeedLimitFlag)
				{
					if (Basic.Power > stru_FOC.PowerLimit)
					{
						// ��ֹƵ���������˳�����ģʽ
						if (++hSpeedLimitCount > (500 / stru_FOC.WRAMP))
						{
							bSpeedLimitFlag = 1;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // ���浱ǰ��λ

							PID_WL.Kp = 16384; // ���ʻ�KP = _Q15(0.5), �ܿ���������
							PID_WL.Ki = 1638;  // ���ʻ�KI = _Q15(0.05), �ܿ���������
						}
					}
				}
				// ����״̬���ж��Ƿ���Ҫȡ������
				else
				{
					// ��λ�½�ʱȡ������
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 100))
					{
						bSpeedLimitFlag = 0;
					}
					// ��ǰת��ֵ������ǰ��λת�ٴ�ʱ�˳���������
					//					if( Basic.Calb_Speed > hSpeedTargetBeforeLimit )
					//					{
					//						bSpeedLimitFlag = 0;
					//					}
					if (!bSpeedLimitFlag)
					{
						PID_WL.Kp = Para.FOC.Wkp; // �ָ�ת�ٻ�PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
#endif // end limit power enable

				if (bSpeedLimitFlag)
				{
					// �㹦��ʱ��PI����
					stru_FOC.WRAMP = TIME_POWER_LOOP;
					// �޹���ʱ�㹦�ʿ���
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.PowerLimit, Basic.Power);
				}
				else
				{
					// ��ת��ʱ��PI����
					stru_FOC.WRAMP = TIME_SPEED_LOOP;
					// ����״̬��ת�ٿ���
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_Speed_Ctrl.RampOut, Basic.Calb_Speed);
				}
			}
//===========================================================================//
// ���ʻ�����
#elif (Config_Contorl_Mode == Power_Loop)
			{
// ����ʹ��
#if (LIMIT_SPEED_ENABLE && LIMIT_CURRENT_ENABLE)

				if (!bSpeedLimitFlag)
				{
					// ������״̬���жϵ�ǰת���Ƿ񳬹��޶�ת��
					if (Basic.Mech_Speed > (uint32_t)LIMIT_SPEED_VALUE)
					{
						// ��ֹƵ���������˳�����ģʽ
						if ((++hSpeedLimitCount > (500 / stru_FOC.WRAMP)) && (bSpeedCurrentLimitFlag == 0))
						{
							bSpeedLimitFlag = 1;
							bSpeedCurrentLimitFlag = 0;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // ���浱ǰ��λ
							hPowerTargetBeforeLimit = Motor_Actual.Power;	   // ���浱ǰ����ֵ
						}
					}
// ����ĸ�ߵ���״̬���жϵ�ǰĸ�ߵ����Ƿ񳬹��޶�ĸ�ߵ���
#if 0
							else if( Time.Motor_Limit_Curent > 300 )
							{
								if( Motor_Actual.Ibus > stru_FOC.CurrentLimit )
								{
									// ��ֹƵ���������˳���ĸ�ߵ���ģʽ
									if( ++hSpeedLimitCount > (100 / stru_FOC.WRAMP) )
									{
										bSpeedLimitFlag = 1;
										bSpeedCurrentLimitFlag = 1;
										hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; //���浱ǰ��λ
									}
								}
							}
#else
					// FOC�ж��ж�ĸ�߾�ֵ������־��������Ӧ��
					else if (stru_FOC.CurrentLimitFlag)
					{
						bSpeedLimitFlag = 1;
						bSpeedCurrentLimitFlag = 1;
						hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // ���浱ǰ��λ
					}
#endif
				}
				else
				{
					// ����״̬���ж��Ƿ���Ҫȡ������
					if (!bSpeedCurrentLimitFlag)
					{
						// ��λ�½�ʱȡ������
						if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 200))
						{
							bSpeedLimitFlag = 0;
						}
						// ��ǰ����ֵ��Ŀ�깦��Сʱ�˳�����
						if (Motor_Actual.Power > (hPowerTargetBeforeLimit + 15))
						{
							bSpeedLimitFlag = 0;
						}

						if (!bSpeedLimitFlag)
						{
							stru_Speed_Ctrl.RampInc = SPEED_RAMP_INC;
							stru_Speed_Ctrl.RampDec = SPEED_RAMP_DEC;
							PID_WL.Kp = Para.FOC.Wkp; // �ָ����ʻ�PI
							PID_WL.Ki = Para.FOC.Wki;
						}
					}
					// ��ĸ�ߵ���״̬���ж��Ƿ���Ҫȡ����ĸ�ߵ���
					else
					{
						// ��λ�½�ʱȡ����ĸ�ߵ���
						if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 500))
						{
							bSpeedLimitFlag = 0;
						}
						// ����ĸ�ߵ����½�ʱȡ����ĸ�ߵ���
						wTempCalcIbus = Basic.Target_Value * 10000 / Motor_Actual.Vbus;

						if (wTempCalcIbus < (stru_FOC.CurrentLimit - 50))
						{
							if (++hIbusLimitCount > 400)
							{
								bSpeedLimitFlag = 0;
								hIbusLimitCount = 0;
							}
						}
						else
						{
							hIbusLimitCount = 0;
						}

						if (!bSpeedLimitFlag)
						{
							bSpeedCurrentLimitFlag = 0;
							stru_FOC.CurrentLimitFlag = 0;
							stru_Speed_Ctrl.RampInc = SPEED_RAMP_INC;
							stru_Speed_Ctrl.RampDec = SPEED_RAMP_DEC;
							PID_WL.Kp = Para.FOC.Wkp; // �ָ����ʻ�PI
							PID_WL.Ki = Para.FOC.Wki;
						}
					}
				}
#elif (LIMIT_SPEED_ENABLE)
				// ������״̬���жϵ�ǰת���Ƿ񳬹��޶�ת��
				if (!bSpeedLimitFlag)
				{
					if (Basic.Mech_Speed > (uint32_t)LIMIT_SPEED_VALUE)
					{
						// ��ֹƵ���������˳�����ģʽ
						if (++hSpeedLimitCount > (500 / stru_FOC.WRAMP))
						{
							bSpeedLimitFlag = 1;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // ���浱ǰ��λ
							hPowerTargetBeforeLimit = Motor_Actual.Power;	   // ���浱ǰ����ֵ
						}
					}
				}
				// ����״̬���ж��Ƿ���Ҫȡ������
				else
				{
					// ��λ�½�ʱȡ������
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 200))
					{
						bSpeedLimitFlag = 0;
					}
					// ��ǰ����ֵ��Ŀ�깦��Сʱ�˳�����
					if (Motor_Actual.Power > (hPowerTargetBeforeLimit + 15))
					{
						bSpeedLimitFlag = 0;
					}

					if (!bSpeedLimitFlag)
					{
						stru_Speed_Ctrl.RampInc = SPEED_RAMP_INC;
						stru_Speed_Ctrl.RampDec = SPEED_RAMP_DEC;
						PID_WL.Kp = Para.FOC.Wkp; // �ָ����ʻ�PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
#elif (LIMIT_CURRENT_ENABLE)
				// ����ĸ�ߵ���״̬���жϵ�ǰĸ�ߵ����Ƿ񳬹��޶�ĸ�ߵ���

				if (!bSpeedLimitFlag)
				{
#if 0
							if( Time.Motor_Limit_Curent > 50 )
							{
								if( Motor_Actual.Ibus > stru_FOC.CurrentLimit )
								{
									stru_Speed_Ctrl.RampInc = 20; //SPEED_RAMP_INC;
									stru_Speed_Ctrl.RampDec = 20; //SPEED_RAMP_DEC;
									// ��ֹƵ���������˳���ĸ�ߵ���ģʽ
									if( ++hSpeedLimitCount > (50 / stru_FOC.WRAMP) )
									{
										bSpeedLimitFlag = 1;
										hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; //���浱ǰ��λ
									}
								}
							}
#else
					// FOC�ж��ж�ĸ�߾�ֵ������־��������Ӧ��
					if (stru_FOC.CurrentLimitFlag)
					{
						bSpeedLimitFlag = 1;
						hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // ���浱ǰ��λ
					}
#endif
				}
				// ��ĸ�ߵ���״̬���ж��Ƿ���Ҫȡ����ĸ�ߵ���
				else
				{
					// ��λ�½�ʱȡ����ĸ�ߵ���
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 500))
					{
						bSpeedLimitFlag = 0;
					}

					// ����ĸ�ߵ����½�ʱȡ����ĸ�ߵ���
					wTempCalcIbus = Basic.Target_Value * 10000 / Motor_Actual.Vbus;

					if (wTempCalcIbus < (stru_FOC.CurrentLimit - 50))
					{
						if (++hIbusLimitCount > 400)
						{
							bSpeedLimitFlag = 0;
							hIbusLimitCount = 0;
						}
					}
					else
					{
						hIbusLimitCount = 0;
					}

					if (!bSpeedLimitFlag)
					{
						stru_FOC.CurrentLimitFlag = 0;
						stru_Speed_Ctrl.RampInc = SPEED_RAMP_INC;
						stru_Speed_Ctrl.RampDec = SPEED_RAMP_DEC;
						PID_WL.Kp = Para.FOC.Wkp; // �ָ����ʻ�PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
#endif // end limit speed enable

				if (bSpeedLimitFlag)
				{
#if (LIMIT_SPEED_ENABLE && LIMIT_CURRENT_ENABLE)

					if (!bSpeedCurrentLimitFlag)
					{
						// ��ת��ʱ��PI����
						stru_FOC.WRAMP = TIME_SPEED_LOOP;

						stru_Speed_Ctrl.RampInc = 50;
						stru_Speed_Ctrl.RampDec = 50;
						PID_WL.Kp = 6553; // �ٶȻ�KP , �ܿ���������
						PID_WL.Ki = 200;  // �ٶȻ�KI , �ܿ���������

						// ����ʱ��ת�ٿ���
						stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, Basic.Calb_SpeedLimit, Basic.Calb_Speed);
					}
					else
					{
						// ��ĸ�ߵ���ʱ��PI����
						stru_FOC.WRAMP = 1;

						stru_Speed_Ctrl.RampInc = 10; // 150
						stru_Speed_Ctrl.RampDec = 10; // 150
						PID_WL.Kp = 16384;			  // ĸ�ߵ�����KP , �ܿ���������  16384  32768
						PID_WL.Ki = 500;			  // ĸ�ߵ�����KI , �ܿ���������   127     3276

						// �޵���ʱ��ĸ�ߵ�������
						stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.CurrentLimit1, Basic.Ibus);
					}
#elif (LIMIT_SPEED_ENABLE)
					// ��ת��ʱ��PI����
					stru_FOC.WRAMP = TIME_SPEED_LOOP;

					stru_Speed_Ctrl.RampInc = 50;
					stru_Speed_Ctrl.RampDec = 50;
					PID_WL.Kp = 6553; // �ٶȻ�KP , �ܿ���������
					PID_WL.Ki = 200;  // �ٶȻ�KI , �ܿ���������

					// ����ʱ��ת�ٿ���
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, Basic.Calb_SpeedLimit, Basic.Calb_Speed);
#elif (LIMIT_CURRENT_ENABLE)
					// ��ĸ�ߵ���ʱ��PI����
					stru_FOC.WRAMP = 1;

					stru_Speed_Ctrl.RampInc = 10; // 150
					stru_Speed_Ctrl.RampDec = 10; // 150
					PID_WL.Kp = 16384;			  // ĸ�ߵ�����KP, �ܿ���������  16384  32768
					PID_WL.Ki = 500;			  // ĸ�ߵ�����KI, �ܿ���������   127     3276

					// �޵���ʱ��ĸ�ߵ�������
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.CurrentLimit1, Basic.Ibus);
#endif
				}
				else
				{
					// �㹦��ʱ��PI����
					stru_FOC.WRAMP = TIME_POWER_LOOP;
					// ����״̬�㹦�ʿ���
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_Speed_Ctrl.RampOut, Basic.Power); // PID_CALC  PID_SpeedLoop
				}
			}
#endif // end torgue and speed or power loop
	   //=======================================================================//
	   // ����IQ�������
			if (stru_FOC.Curr_Is_Ref > stru_FOC.Curr_Iq_Max)
				stru_FOC.Curr_Is_Ref = stru_FOC.Curr_Iq_Max;
			if (stru_FOC.Curr_Is_Ref < stru_FOC.Curr_Iq_Min)
				stru_FOC.Curr_Is_Ref = stru_FOC.Curr_Iq_Min;
		}
		//===========================================================================//
	}
	//===============================================================================//
	// ��������ڷ�����̬
	else
	{
		bSpeedLimitFlag = 0;
		stru_FOC.W_Cycle = 0;
		PID_WL.Kp = Para.FOC.Wkp;
		PID_WL.Ki = Para.FOC.Wki;
		stru_Speed_Ctrl.RampInc = SPEED_RAMP_INC;
		stru_Speed_Ctrl.RampDec = SPEED_RAMP_DEC;
		//		stru_Speed_Ctrl.RampOut = 0;
#if (Config_Contorl_Mode == Current_Loop)
		stru_FOC.WRAMP = 1;
#elif (Config_Contorl_Mode == Speed_Loop)
		stru_FOC.WRAMP = TIME_SPEED_LOOP;
#elif (Config_Contorl_Mode == Power_Loop)
		stru_FOC.WRAMP = TIME_POWER_LOOP;
#endif

		// #if ( (LIMIT_POWER_ENABLE) || (LIMIT_SPEED_ENABLE) || (LIMIT_CURRENT_ENABLE) )
		// 	hSpeedLimitCount = 0;
		// #endif
		Flag.Speed_Ramp = 0;
	}
	//===============================================================================//
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_MotorInit_Process
 * Description    :
 * Function Call  : 1ms���̵���
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_MotorInit_Process(void)
{
	// ��ʼ��
	if (MOTOR_STATE == MC_INIT)
	{
		FOC_MotorSpeed_Filter(stru_FOC.Elec_We);
	}
} 
 
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : UartView
 * Description    : ���ڼ������--������
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
// uint16_t hUartSum = 0;
// uint8_t hUartScopeArr[10] = {0};
// void UartView(int16_t view1, int16_t view2, int16_t view3, int16_t view4)
// {
// 	static uint8_t bUartScopeCount = 0;

// 	if (bUartScopeCount < 8)
// 		hUartSum += hUartScopeArr[bUartScopeCount];
// 	else if (bUartScopeCount == 8)
// 		hUartScopeArr[8] = (uint8_t)hUartSum;

// 	UART0->THR = hUartScopeArr[bUartScopeCount];
// 	UART0->END = 0x0;

// 	if (++bUartScopeCount >= 9)
// 	{
// 		bUartScopeCount = 0;
// 		hUartSum = 0;
// 		hUartScopeArr[0] = (uint8_t)(view1);
// 		hUartScopeArr[1] = (uint8_t)(view1 >> 8);
// 		hUartScopeArr[2] = (uint8_t)(view2);
// 		hUartScopeArr[3] = (uint8_t)(view2 >> 8);
// 		hUartScopeArr[4] = (uint8_t)(view3);
// 		hUartScopeArr[5] = (uint8_t)(view3 >> 8);
// 		hUartScopeArr[6] = (uint8_t)(view4);
// 		hUartScopeArr[7] = (uint8_t)(view4 >> 8);
// 	}
// }

/*****************************************************************************
 ** \brief Motor_Actual_Calculate
 **
 ** \param [in] none
 ** \return  none
 ** \note	���ʵ�ʲ�������(��ѹ�����������ʣ�ת��)(��ѹ������=ʵ��ֵ*100) //2021-08-02
 *****************************************************************************/
void Motor_Actual_Calculate(void)
{
	Motor_Actual.Ibus = ((Basic.Ibus * 100) / Motor_Actual.G_Ibus);
	Motor_Actual.Vbus = (Basic.Vbus * 100) / Motor_Actual.G_Vbus;
	Motor_Actual.Power = Motor_Actual.Vbus * Motor_Actual.Ibus / 10000;
	Motor_Actual.Speed = Basic.Mech_Speed;
}

//===========================================================================//
//*********END   ************************************************************//
//===========================================================================//
