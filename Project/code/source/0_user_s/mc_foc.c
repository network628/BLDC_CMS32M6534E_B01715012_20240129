//==========================================================================//
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * @file    mc_foc.c
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
PID_Components PID_ID = {0}; /* D������� */

PID_Components PID_IQ = {0}; /* Q�������*/

PID_Components PID_WL = {0}; /* �ٶ�/�����⻷*/

struct_SinCos stru_SinCos = {0}; /* SinCos����ṹ�� */

struct_Curr_abc stru_Curr_abc = {0}; /* ���abc������� */

struct_Clark stru_Curr_alphabeta = {0}; /* alpha��beta����ϵ���� */

struct_Park stru_Curr_dq = {0}; /* d��q����ϵ���� */

struct_Park stru_Curr_dq_ref = {0}; /* d��q����ϵ�ο����� */

struct_RevPark stru_Volt_alphabeta = {0}; /* alpha��beta����ϵ��ѹ */

struct_Volt_dq stru_Volt_dq = {0}; /* d��q����ϵ��ѹ */

struct_Arctan stru_Arctan = {0}; /* �����м��� */

struct_SVPWM stru_SVM_Calc = {0}; /* �ռ�ʸ���ṹ�� */

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/
volatile uint16_t hBrakeDuty = 0; /* ɲ��ռ�ձ� */
volatile uint32_t wFOCBrakeCount = 0;
volatile uint32_t wSwitchHoldTime = 0;
uint8_t bFristFlag = 0;
uint32_t wMotorLockSwitchTime = 0;
int32_t wObseFs = 0;
int32_t wObseGs = 0;
uint8_t bBLDCSwitchFlag = 0;
uint8_t bBLDCFristFlag = 0;

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/
extern const uint16_t Q15Sqrt_Table[512];
extern const int16_t Sin_Cos_Table[256];

//===========================================================================/
//***** definitions  end ****************************************************/
//===========================================================================/

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Start_Init
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Start_Init(void)
{
	//------------------------------------------------------------------------/
	// ����������������ʼ��
	stru_smo_est.Fs = wObseFs;
	stru_smo_est.Gs = (wObseGs * (uint16_t)OBSE_GS_FACTOR) >> 10;
	stru_smo_est.Ealpha = 0;
	stru_smo_est.Ebeta = 0;
	stru_smo_est.Est_Ialpha = 0;
	stru_smo_est.Est_Ibeta = 0;
	stru_smo_est.Ialpha = 0;
	stru_smo_est.Ibeta = 0;
	stru_smo_est.IalphaError = 0;
	stru_smo_est.IbetaError = 0;
	stru_smo_est.Zalpha = 0;
	stru_smo_est.Zbeta = 0;
	stru_smo_est.We = 0;
	stru_smo_est.Angle = 0;
	stru_smo_est.GWe = 0;
	stru_smo_est.PI_Ui = 0;
	stru_smo_est.PI_Out = 0;
	stru_smo_est.Kslide = stru_FOC.Ksilde;
	stru_smo_est.E0 = stru_FOC.Ke;
	stru_smo_est.Kslf = (uint16_t)OBSE_KSLF;
	stru_smo_est.PI_Kp = (uint16_t)OBSE_START_EKP;
	stru_smo_est.PI_Ki = (uint16_t)OBSE_START_EKI;
	stru_smo_est.PI_Lower = (uint16_t)OBSE_START_OMGEA;
	stru_smo_est.PI_Upper = 16384;
	stru_smo_est.OffsetAngle = (uint16_t)OBSE_OFFSET_ANGLE;
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_ControlPara_Init
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_ControlPara_Init(void)
{
	//------------------------------------------------------------------------/
	// ������ʼ��
	stru_Curr_alphabeta.Alpha = 0;
	stru_Curr_alphabeta.Beta = 0;
	stru_Curr_dq.Id = 0;
	stru_Curr_dq.Iq = 0;
	stru_Volt_dq.Ud = 0;
	stru_Volt_dq.Uq = 0;
	stru_Curr_abc.Ia = 0;
	stru_Curr_abc.Ib = 0;
	stru_Curr_abc.Ic = 0;
	stru_Volt_alphabeta.Alpha = 0;
	stru_Volt_alphabeta.Beta = 0;
	stru_SinCos.Sin = 0;
	stru_SinCos.Cos = 0;
	stru_Curr_dq_ref.Id = 0;
	stru_Curr_dq_ref.Iq = Para.Start.run_current_init;

	//------------------------------------------------------------------------/
	// ������ʼ��
	Fault.PhaseA_LossCnt = 0;
	Fault.PhaseB_LossCnt = 0;
	Fault.PhaseC_LossCnt = 0;

	//------------------------------------------------------------------------/
	// ������ʼ��
	Flag.Charge = 0;
	Flag.IPD = 0;
	Flag.MotorBack = 0;
	Flag.MotorFR = Config.MotorDir;
	Flag.MotorFR_Pre = Flag.MotorFR;

	//------------------------------------------------------------------------/
	// ������ʼ��
	stru_FOC.Elec_Angle = 0;
	stru_FOC.MotorRunTime = 0;
	stru_FOC.Offset_Angle = 0;
	stru_FOC.Elec_We = 0;
	stru_FOC.I_Cycle = 0;
	stru_FOC.IPD_Angle = ERRORRETURN;
	stru_Speed_Ctrl.RampOut = 0;

	stru_FOC.CurrentLimitCount = 0;
	stru_FOC.CurrentLimitFlag = 0;

	Config.Mode_Svpwm = Segment_7;
	Basic.ERAMP = stru_FOC.SRAMP;
	hBrakeDuty = 0;
	bFristFlag = 0;
	wSwitchHoldTime = Para.Start.Hold_Time;

	//------------------------------------------------------------------------/
	// �����۲���������ʼ��
	FOC_Start_Init();

	// PID������ʼ��
	FOC_PID_Parameter_Init();

	// λ�ù۲�����ʼ��
	FOC_RotorPosition_Init();

	// ���綯�Ʋ�����ʼ��
	FOC_BEMF_Init();

	// �������ʼ��
	FOC_SingleShunt_Init();

	// ����������ʼ��
	FOC_OpenLoop_Init();

	//------------------------------------------------------------------------/
	Brige_Output_Off(); // �ر��ű����
	//------------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Read_Current
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Read_Current(void)
{
	int32_t wTempA, wTempB, wTempC;
	//-----------------------------------------------------------------------/
	// ��ȡ��������ֵ
	stru_Sample.ADVal[CH_IP_A] = (uint32_t)ADC->DATA[ADC_DATA_CHA];
	stru_Sample.ADVal[CH_IP_B] = (uint32_t)ADC->DATA[ADC_DATA_CHB];

	//---------------------------------------------------------------------/
	// ˫�����������
	stru_Sample.ARR[CH_IP_A] = FRACMPY_Q10((stru_Sample.ADVal[CH_IP_A] - stru_Sample.REF[CH_IP_A]), stru_Sample.G_Ip);
	stru_Sample.ARR[CH_IP_B] = FRACMPY_Q10((stru_Sample.ADVal[CH_IP_B] - stru_Sample.REF[CH_IP_B]), stru_Sample.G_Ip);

	//---------------------------------------------------------------------/
	switch (stru_Sample.IP_CH)
	{
	case IP_UV:
		wTempA = -stru_Sample.ARR[CH_IP_A];
		wTempB = -stru_Sample.ARR[CH_IP_B];
		wTempC = stru_Sample.ARR[CH_IP_A] + stru_Sample.ARR[CH_IP_B];
		break;
	case IP_UW:
		wTempA = -stru_Sample.ARR[CH_IP_A];
		wTempC = -stru_Sample.ARR[CH_IP_B];
		wTempB = stru_Sample.ARR[CH_IP_A] + stru_Sample.ARR[CH_IP_B];
		break;
	case IP_VU:
		wTempB = -stru_Sample.ARR[CH_IP_A];
		wTempA = -stru_Sample.ARR[CH_IP_B];
		wTempC = stru_Sample.ARR[CH_IP_A] + stru_Sample.ARR[CH_IP_B];
		break;
	case IP_VW:
		wTempB = -stru_Sample.ARR[CH_IP_A];
		wTempC = -stru_Sample.ARR[CH_IP_B];
		wTempA = stru_Sample.ARR[CH_IP_A] + stru_Sample.ARR[CH_IP_B];
		break;
	case IP_WU:
		wTempC = -stru_Sample.ARR[CH_IP_A];
		wTempA = -stru_Sample.ARR[CH_IP_B];
		wTempB = stru_Sample.ARR[CH_IP_A] + stru_Sample.ARR[CH_IP_B];
		break;
	case IP_WV:
		wTempC = -stru_Sample.ARR[CH_IP_A];
		wTempB = -stru_Sample.ARR[CH_IP_B];
		wTempA = stru_Sample.ARR[CH_IP_A] + stru_Sample.ARR[CH_IP_B];
		break;
	default:
		break;
	}
	//---------------------------------------------------------------------/
	// ��������ƥ��
	if (FR_CW == Flag.MotorFR)
	{
		stru_Curr_abc.Ia = wTempA;
		stru_Curr_abc.Ib = wTempB;
		stru_Curr_abc.Ic = wTempC;
	}
	else
	{
		stru_Curr_abc.Ib = wTempA;
		stru_Curr_abc.Ia = wTempB;
		stru_Curr_abc.Ic = wTempC;
	}
	//---------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Overcurrent_Check
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Overcurrent_Check(void)
{
	volatile int32_t wABSIa, wABSIb, wABSIc;

	//-----------------------------------------------------------------------/
	// �����ľ���ֵ
	wABSIa = ABSFUN(stru_Curr_abc.Ia);
	wABSIb = ABSFUN(stru_Curr_abc.Ib);
	wABSIc = ABSFUN(stru_Curr_abc.Ic);

	//-----------------------------------------------------------------------/
	// �������ֵ����
	if (MOTOR_STATE == MC_RUN)
	{
		if ((wABSIa > Fault.OverCurrent_Value) || (wABSIb > Fault.OverCurrent_Value) || (wABSIc > Fault.OverCurrent_Value))
		{
			Fault_OverCurrent = 1;
			SYSTEM_STATE = SYS_FAULT;
			MOTOR_FAULT_CODE = FAULT_OVERCURRENT;
		}
	}

	//-----------------------------------------------------------------------/
	// ȱ����--�������ֵ
	if (Fault.Ia_max < wABSIa)
	{
		Fault.Ia_max = wABSIa;
	}
	if (Fault.Ib_max < wABSIb)
	{
		Fault.Ib_max = wABSIb;
	}
	if (Fault.Ic_max < wABSIc)
	{
		Fault.Ic_max = wABSIc;
	}

	//-----------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Brake_Control
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Brake_Control(void)
{
	//-----------------------------------------------------------------------/
	// ɲ��ռ�ձ�
	if (hBrakeDuty < 32767)
		hBrakeDuty += 100;
	if (hBrakeDuty > 32767)
		hBrakeDuty = 32767;

	//-----------------------------------------------------------------------/
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	// ����duty
	EPWM->CMPDAT[0] = ((hBrakeDuty * stru_FOC.EPWM_Period) >> 15);
	EPWM->CMPDAT[2] = ((hBrakeDuty * stru_FOC.EPWM_Period) >> 15);
	EPWM->CMPDAT[4] = ((hBrakeDuty * stru_FOC.EPWM_Period) >> 15);
	// ʹ�ܼ�������&&ռ�ձ�
	EPWM->CON3 |= 0x00001500;
	EPWM->MASK = 0x00001500;
	EPWM->LOCK = 0x0;

	//-----------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Start_Check
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Start_Check(void)
{
	//=======================================================================/
	// ����״̬���
	volatile static uint16_t hStartCount = 0;
	volatile static uint32_t wStartFailCount = 0;

	//-----------------------------------------------------------------------/
	// ת�ٴ����趨ֵ�����ɹ�
	if (Basic.Mech_Speed > Basic.Speed_Close)
	{
		hStartCount++;
	}
	else
	{
		hStartCount = 0;
		wStartFailCount++;
	}
	//-----------------------------------------------------------------------/
	// �����趨ʱ��ת��δ�ﵽ�趨ֵ��Ϊ����ʧ��
	if (hStartCount > 3200)
	{
		hStartCount = 0;
		wStartFailCount = 0;
		stru_FOC.Curr_Is_Ref = stru_Curr_dq_ref.Iq;
		MOTOR_STATE = MC_SW;
	}
	//-----------------------------------------------------------------------/
	if (wStartFailCount > Time.Motor_StartFail)
	{
		hStartCount = 0;
		wStartFailCount = 0;
		Fault_StartFail = 1;
		SYSTEM_STATE = SYS_RESTART;
	}

	//-----------------------------------------------------------------------/
	// ͣ��������ת�л�
	if (Flag.MC_RunStop == 0)
	{
		hStartCount = 0;
		wStartFailCount = 0;
		MOTOR_STATE = MC_INIT;
	}
	//=======================================================================/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Stop_Check
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Stop_Check(void)
{
	volatile static uint32_t wMotorStopholdCount = 0;
// ����ɲ��
#if (BRAKE_STOP_MODE == BRAKE_STOP_ENABLE)
	{
		// û�й��ϲ�������ɲ��״̬����������ʼ̬
		if (MOTOR_FAULT_CODE == NOERROR)
		{
			// ͣ�������н��յ������������˳�������ж�,�������ɲ��״̬
			if ((Flag.MC_RunStop == 1) && (Flag.MotorFR_Pre == Config.MotorDir))
			{
				// ת�ٴ����趨ת�ٿ�ֱ�ӻظ�RUN̬����������ʼ̬���з��綯���ж�
				if (Basic.Mech_Speed > (uint32_t)(BRAKE_STOP_SPEED + 10000))
				{
					PID_WL.KiSum = stru_Curr_dq_ref.Iq << 14;
					PID_WL.Integral = stru_Curr_dq_ref.Iq;
					stru_Curr_dq_ref.Id = 0;
					wMotorStopholdCount = 0;
					MOTOR_STATE = MC_RUN;
				}
				else
				{
					wMotorStopholdCount = 0;
					;
					Time.Motor_PowerDown = BRAKE_STOP_TIME;
					MOTOR_STATE = MC_BRAKE;
				}
			}
			else if (Basic.Mech_Speed < (uint32_t)BRAKE_STOP_SPEED)
			{
				wMotorStopholdCount = 0;
				Time.Motor_PowerDown = BRAKE_STOP_TIME;
				MOTOR_STATE = MC_BRAKE;
			}
		}
		else
		{
			Brige_Output_Off();
			wMotorStopholdCount = 0;
			Basic.Mech_Speed = 0;
			MOTOR_STATE = MC_INIT;
		}
	}
#else
	{
#if (0)
		{
			// ͣ�������н��յ������������˳�������ж�,�������INIT״̬
			if ((Flag.MC_RunStop == 1) && (Flag.MotorFR_Pre == Config.MotorDir))
			{
				// ת�ٴ����趨ת�ٿ�ֱ�ӻظ�RUN̬����������ʼ̬���з��綯���ж�
				if (Basic.Mech_Speed > (uint32_t)(BRAKE_STOP_SPEED + 10000))
				{
					PID_WL.KiSum = stru_Curr_dq_ref.Iq << 14;
					PID_WL.Integral = stru_Curr_dq_ref.Iq;
					stru_Curr_dq_ref.Id = 0;
					wMotorStopholdCount = 0;
					MOTOR_STATE = MC_RUN;
				}
				else
				{
					Brige_Output_Off();
					wMotorStopholdCount = 0;
					Basic.Mech_Speed = 0;
					MOTOR_STATE = MC_INIT;
				}
			}
			else if (Basic.Mech_Speed < (uint32_t)MOTOR_SPEED_STOP)
			{
				Brige_Output_Off();
				wMotorStopholdCount = 0;
				Basic.Mech_Speed = 0;
				MOTOR_STATE = MC_INIT;
			}
		}
#else
		{
			Brige_Output_Off();
			wMotorStopholdCount = 0;
			Basic.Mech_Speed = 0;
			MOTOR_STATE = MC_INIT;
		}
#endif
	}
#endif
	// ��ʱǿ��ͣ��
	if ((++wMotorStopholdCount > Time.Motor_Stophold))
	{
		wMotorStopholdCount = 0;
		Basic.Mech_Speed = 0;
		MOTOR_STATE = MC_INIT;
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Init
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Init(void)
{
	//-----------------------------------------------------------------------//
	// ������ʼ��
	FOC_ControlPara_Init();

//-----------------------------------------------------------------------//
// ����
#if (FAULT_VOLTAGE_ENABLE && (Config_Debug_Mode == Debug_Disable))
	if (Flag.MC_RunStop && (Basic.Vbus_Check < Fault.StartOverVoltage_Value) && (Basic.Vbus_Check > Fault.StartLowerVoltage_Value))
#else
	if (Flag.MC_RunStop)
#endif
	{
		MOTOR_STATE = MC_CHARGE;
	}
	//-----------------------------------------------------------------------//
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Charge
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Charge(void)
{
//=======================================================================//
// ��磬˫N����Ԥ��磬PN����������
#if (CHARGE_ENABLE == CHARGE_MODE)

	if (Flag.Charge == 0)
	{
		Flag.Charge = 1;
		Time.Charge_Count = CHARGE_TIME;
	}
	//=======================================================================//
	if (Time.Charge_Count == CHARGE_TIME)
	{
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;

		if (Double_Shunt == Config.Mode_Shunt)
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD);
		}
		else
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
		}

		EPWM->CON3 |= (0x1UL << (EPWM0 + EPWM_CON3_LOADEN0_Pos));
		EPWM->MASK = 0x00003D00; // U�����
		EPWM->LOCK = 0x0;
	}
	//=======================================================================//
	else if (Time.Charge_Count == (CHARGE_TIME << 1) / 3)
	{
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;

		if (Double_Shunt == Config.Mode_Shunt)
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD);
			EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD);
		}
		else
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
			EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
		}

		EPWM->CON3 |= (0x1UL << (EPWM0 + EPWM_CON3_LOADEN0_Pos));
		EPWM->CON3 |= (0x1UL << (EPWM2 + EPWM_CON3_LOADEN0_Pos));
		EPWM->MASK = 0x00003500; // U��V�����
		EPWM->LOCK = 0x0;
	}
	//=======================================================================//
	else if (Time.Charge_Count == CHARGE_TIME / 3)
	{
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
		if (Double_Shunt == Config.Mode_Shunt)
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD);
			EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD);
			EPWM->CMPDAT[EPWM4] = (0xffff & EPWM_HALFPERIOD);
		}
		else
		{
			EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
			EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
			EPWM->CMPDAT[EPWM4] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
		}

		EPWM->CON3 |= (0x1UL << (EPWM0 + EPWM_CON3_LOADEN0_Pos));
		EPWM->CON3 |= (0x1UL << (EPWM2 + EPWM_CON3_LOADEN0_Pos));
		EPWM->CON3 |= (0x1UL << (EPWM4 + EPWM_CON3_LOADEN0_Pos));
		EPWM->MASK = 0x00001500; // U��V��W�����
		EPWM->LOCK = 0x0;
	}
	Brige_Output_On();

	//=======================================================================//
	if (Time.Charge_Count == 0)
	{
#if (Config_Wind_Mode == Start_Wind)
		{
			MOTOR_STATE = MC_WIND;
		}
#else
		{
			stru_Curr_dq_ref.Id = 0;
			stru_Curr_dq_ref.Iq = Para.Start.run_current_init;
			stru_FOC.Curr_Is_Ref = Para.Start.run_current_final;
			MOTOR_STATE = MC_START;
		}
#endif
	}

//=======================================================================//
#else

//---------------------------------------------------------------------/
#if (Config_Wind_Mode == Start_Wind)
	{
		Brige_Output_Off();
		Basic.ERAMP = stru_FOC.ERAMP;
		MOTOR_STATE = MC_WIND;
	}
#else
	{
		Brige_Output_On();
		stru_Curr_dq_ref.Id = 0;
		stru_Curr_dq_ref.Iq = Para.Start.run_current_init;
		stru_FOC.Curr_Is_Ref = Para.Start.run_current_final;
		stru_FOC.Elec_Angle = DEG_30;
		Basic.ERAMP = stru_FOC.SRAMP;
		MOTOR_STATE = MC_START;
	}
#endif
	//---------------------------------------------------------------------/
#endif
	//=======================================================================//
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Wind_Check
 * Description    : ˳�����
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Wind_Check(void)
{
	//-----------------------------------------------------------------------/
	// ����Alpha��Beta����
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// ����d��q����
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// �޸�λ�ù�����
	FOC_RotorPosition_Detection();

	//-----------------------------------------------------------------------/
	// ����Ƕ�
	stru_FOC.Elec_Angle = stru_rpd_est.Angle;
	stru_SinCos = FOC_SinCos_Calc(stru_FOC.Elec_Angle);

	//----------------------------------------------------------------------/
	// Park��任
	stru_Volt_dq.Ud = 0;
	stru_Volt_dq.Uq = 0;

	//----------------------------------------------------------------------/
	// Park��任
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

	//----------------------------------------------------------------------/
	// ����ռ�ʸ��
	if (Single_Shunt == Config.Mode_Shunt)
	{
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
	else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}

	//----------------------------------------------------------------------/
	if (Flag.MC_RunStop == 0)
	{
		MOTOR_STATE = MC_INIT;
	}

	//----------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Wind
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Wind(void)
{
#if 1
	stru_SingleCurr.LN_States = 0;
	stru_SingleCurr.LN_Flag = 1;
#endif
	//-----------------------------------------------------------------------/
	// ���綯��ADC������������
	if (!bFristFlag)
	{
		bFristFlag = 1;
		EPWM_ConfigCompareTriger(EPWM_CMPTG_0, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM0, EPWM_HALFPERIOD - 128);
		EPWM_ConfigCompareTriger(EPWM_CMPTG_1, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM2, EPWM_HALFPERIOD + 128);
		ADC_EnableEPWMTriggerChannel(ADC_SCAN_UBEMF | ADC_SCAN_VBEMF | ADC_SCAN_WBEMF);
		ADC_EnableHardwareTrigger(ADC_TG_EPWM0_ZERO);
	}
	//-----------------------------------------------------------------------/
	// MOS��ȫ��
	Brige_Output_Off();

	// ��ȡ���綯�Ƶ�ѹ
	stru_bemf.AD_Value[0] = (int32_t)ADC->DATA[ADC_DATA_UBEMF];
	stru_bemf.AD_Value[1] = (int32_t)ADC->DATA[ADC_DATA_VBEMF];
	stru_bemf.AD_Value[2] = (int32_t)ADC->DATA[ADC_DATA_WBEMF];

	//-----------------------------------------------------------------------/
	// ���㷴�綯��
	FOC_BEMF_Detection();

	//-----------------------------------------------------------------------/
	static uint16_t v_Status_Count = 0;

	// �жϵ�����綯��ת��
	if (stru_bemf.Speed > BEMF_SPEED_MAX)
	{
		stru_bemf.Status = 3; // �ȴ��������
		v_Status_Count = 0;
	}
	else if ((stru_bemf.Speed <= BEMF_SPEED_MAX) && (stru_bemf.Speed > BEMF_SPEED_MIN))
	{
		if (++v_Status_Count > 50)
			stru_bemf.Status = 2; // ˳������ ����SW
	}
	else if ((stru_bemf.Speed <= BEMF_SPEED_MIN) && (stru_bemf.Speed > BEMF_SPEED_START))
	{
		stru_bemf.Status = 1; // �ȴ��������
		v_Status_Count = 0;
	}
	else
	{
		stru_bemf.Status = 0; // ��ֹ����
		v_Status_Count = 0;
	}
#if 1
	//---------------------------------------------------------------------------/
	// ����������������ʱ��

	if ((++stru_bemf.Time >= BEMF_CHECK_TIME))
	{
		stru_bemf.Time = BEMF_CHECK_TIME;

		//-----------------------------------------------------------------------/
		switch (stru_bemf.Status)
		{
		//-------------------------------------------------------------------/
		// ��ֹ����
		case 0:

			if ((stru_bemf.Angle < 1000) || (stru_bemf.Speed < 3000))
			{
				// ����ǰ�Ĳ�����ʼ��
				stru_Curr_dq_ref.Id = 0;
				stru_Curr_dq_ref.Iq = Para.Start.run_current_init;
				stru_FOC.Curr_Is_Ref = Para.Start.run_current_final;

				stru_bemf.Time = 0;
				bFristFlag = 0;
				//------------------------------------------------------------------------------/
				// �رղ���
				ADC_DisableEPWMTriggerChannel(ADC_SCAN_UBEMF | ADC_SCAN_VBEMF | ADC_SCAN_WBEMF);

				// �����Ĵ���
				EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
// ������ʸ��
#if (Config_Shunt_Mode == Double_Shunt)
				{
					EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD);
					EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD);
					EPWM->CMPDAT[EPWM4] = (0xffff & EPWM_HALFPERIOD);
				}
#else
				{
					EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
					EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
					EPWM->CMPDAT[EPWM4] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
				}

#endif
				// ʹ�ܼ�������&&ռ�ձ�
				EPWM->CON3 |= 0x00001500;
				EPWM->LOCK = 0x0;
				// ʹ������
				Brige_Output_On();

				//------------------------------------------------------------------------------/
				// ��������״̬
				MOTOR_STATE = MC_START;
			}

			break;

		//-------------------------------------------------------------------/
		// �ȴ��������
		case 1:

			break;

		//-------------------------------------------------------------------/
		// ˳������
		case 2:

			if (stru_bemf.Angle < 3000)
			{
				//-------------------------------------------------------------/
				v_Status_Count = 0;
				stru_bemf.Time = 0;
				//									stru_bemf.Status			= 0;
				stru_bemf.Flag = 1;
				bFristFlag = 0;
				//-------------------------------------------------------------/
				// ���в����Ƕ�
				stru_rpd_est.Angle = stru_bemf.Angle + (uint32_t)BEMF_OFFSET_ANGLE; //+ DEG_120;

				if (stru_rpd_est.Angle >= 65535)
					stru_rpd_est.Angle = stru_rpd_est.Angle - 65535;
				else if (stru_rpd_est.Angle < 0)
					stru_rpd_est.Angle = stru_rpd_est.Angle + 65535;

				stru_rpd_est.PI_Ui = HWDIV_Div((stru_bemf.DeltaTheta * 32768), stru_rpd_est.Tc);
				stru_rpd_est.PI_Lower = (stru_rpd_est.PI_Ui * 26214) >> 15;

				//-------------------------------------------------------------/
				// Dq������PI
				PID_ID.Kp = 32768;
				PID_ID.Ki = 3276;
				PID_ID.Integral = stru_bemf.Ud >> 1;

				PID_IQ.Kp = 32768;
				PID_IQ.Ki = 3276;
				PID_IQ.Integral = stru_bemf.Uq >> 1;

				//-------------------------------------------------------------/
				// ����ǰ�Ĳ�����ʼ��
				stru_Curr_dq_ref.Id = 0;
				stru_Curr_dq_ref.Iq = Para.Start.run_current_init;
				stru_FOC.Curr_Is_Ref = Para.Start.run_current_init; // Para.Start.run_current_final;

				//------------------------------------------------------------------------------/
				// �����Ĵ���
				EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
// ������ʸ��
#if (Config_Shunt_Mode == Double_Shunt)
				{
					EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD);
					EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD);
					EPWM->CMPDAT[EPWM4] = (0xffff & EPWM_HALFPERIOD);
				}
#else
				{
					EPWM->CMPDAT[EPWM0] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
					EPWM->CMPDAT[EPWM2] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
					EPWM->CMPDAT[EPWM4] = (0xffff & EPWM_HALFPERIOD) | ((0xffff & EPWM_HALFPERIOD) << 16);
				}
#endif

				// ʹ�ܼ�������&&ռ�ձ�
				EPWM->CON3 |= 0x00001500;
				EPWM->LOCK = 0x0;

				//------------------------------------------------------------------------------/
				// �رղ���
				ADC_DisableEPWMTriggerChannel(ADC_SCAN_UBEMF | ADC_SCAN_VBEMF | ADC_SCAN_WBEMF);

				// ʹ������
				Brige_Output_On();

				//------------------------------------------------------------------------------/
				stru_SingleCurr.LN_States = 0;
				stru_SingleCurr.LN_Flag = 1;

				wSwitchHoldTime = Para.Start.Hold_Time * 2;
				//------------------------------------------------------------------------------/
#if (Config_Debug_Mode == Debug_Enable)
				MOTOR_STATE = MC_START;
#else
				MOTOR_STATE = MC_SW;
#endif
			}

		//---------------------------------------------------------------/
		// �ȴ�����
		case 3:

			break;

		//---------------------------------------------------------------/
		default:
			break;
			//---------------------------------------------------------------/
		}
	}
#endif
	//=======================================================================/
	// ˳��״̬���
	if (Flag.MC_RunStop == 0)
	{
		MOTOR_STATE = MC_INIT;
	}
	//=======================================================================/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Align
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Align(void)
{
	//-----------------------------------------------------------------------/
	// ����Alpha��Beta����
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// ����d��q����
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// ��һ�۲���
	FOC_Adaption_Angle_Calc(&stru_adap_est);

	//-----------------------------------------------------------------------/
	// ����Ƕ�
	stru_FOC.Elec_We = 1;

	// stru_FOC.Elec_Angle += 	stru_FOC.Elec_We;

	if (stru_FOC.Elec_Angle < 0)
		stru_FOC.Elec_Angle += 65535;
	else if (stru_FOC.Elec_Angle > 65535)
		stru_FOC.Elec_Angle -= 65535;

	//-----------------------------------------------------------------------/
	// ����Ƕ�������
	stru_SinCos = FOC_SinCos_Calc(stru_FOC.Elec_Angle);

	//-----------------------------------------------------------------------/
	// Q���������
	if (stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq--;
	if (stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq += 1;

	PID_ID.Upper_Output = 10000;
	PID_ID.Upper_Integral = 10000;

	//-----------------------------------------------------------------------/
	// DQ����������
	FOC_DQCurrent_PIRegulator();

	//----------------------------------------------------------------------/
	// Park��任
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

	//----------------------------------------------------------------------/
	// �����ѹռ�ձ�
	if (Single_Shunt == Config.Mode_Shunt)
	{
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
	else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}

	//=======================================================================/
	// Ԥ��λ���

	volatile static u16 hStartCount = 0;

	//-----------------------------------------------------------------------/
	// Ԥ��λʱ��
	if (++hStartCount > 20000)
	{
		hStartCount = 0;

		stru_FOC.Curr_Is_Ref = Para.Start.run_current_final;

		stru_adap_est.Angle = stru_FOC.Elec_Angle;

		if (stru_adap_est.Angle < 0)
			stru_adap_est.Angle += 65535;
		else if (stru_adap_est.Angle > 65535)
			stru_adap_est.Angle -= 65535;

		PID_ID.Kp = Para.Start.Dkp;
		PID_ID.Ki = Para.Start.Dki;

		PID_IQ.Kp = Para.Start.Qkp;
		PID_IQ.Ki = Para.Start.Qki;

		PID_ID.Upper_Output = 0;
		PID_ID.Upper_Integral = 0;

		MOTOR_STATE = MC_START;
	}

	//-----------------------------------------------------------------------/
	// ͣ��
	if (Flag.MC_RunStop == 0)
	{
		hStartCount = 0;
		MOTOR_STATE = MC_INIT;
	}

	//=======================================================================/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Start
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Start(void)
{
	stru_SingleCurr.LN_States = 0;
	stru_SingleCurr.LN_Flag = 1;
	stru_bemf.Flag = 0;

	//=======================================================================/
	volatile static uint32_t wKpAddCount = 0;
	// �������
	volatile int32_t wABSIa, wABSIb, wABSIc;

	//-----------------------------------------------------------------------/
	// ���������ֵ
	wABSIa = ABSFUN(stru_Curr_abc.Ia);
	wABSIb = ABSFUN(stru_Curr_abc.Ib);
	wABSIc = ABSFUN(stru_Curr_abc.Ic);

	//-----------------------------------------------------------------------/
	// ȱ����--�������ֵ
	if (Fault.Ia_max < wABSIa)
	{
		Fault.Ia_max = wABSIa;
	}
	if (Fault.Ib_max < wABSIb)
	{
		Fault.Ib_max = wABSIb;
	}
	if (Fault.Ic_max < wABSIc)
	{
		Fault.Ic_max = wABSIc;
	}

	//=======================================================================/
	// ����Alpha��Beta����
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// ����DQ����
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// �޸�λ�ù�����
	FOC_RotorPosition_Detection();

	//=======================================================================/
	// ����Ƕ�������
	volatile uint16_t bIndex;

	bIndex = (uint16_t)(stru_FOC.Elec_Angle + 32768) >> 6;

	switch (bIndex & 0x0300)
	{
	/* 0~90�� */
	case 0x0200:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 90~180�� */
	case 0x0300:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	/* 180~270�� */
	case 0x0000:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 270~360�� */
	case 0x0100:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	default:
		break;
	}

	//=======================================================================/
	// Q������ο�ֵ
	if (stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq--;
	if (stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq += 10;

	//=======================================================================/
	// DQ����������
	//-----------------------------------------------------------------------/
	// ��ʱ����
	int32_t wPIError = 0;
	int32_t wVqLimit = 0;

	//----------------------------------------------------------------------/
	// D�����������

	// �������
	wPIError = (stru_Curr_dq_ref.Id - stru_Curr_dq.Id);

	// �������
	PID_ID.Integral += (((PID_ID.Ki >> 2) * wPIError) >> 13);

	// ���ֱ���
	if (PID_ID.Integral > PID_ID.Upper_Integral)
		PID_ID.Integral = PID_ID.Upper_Integral;
	if (PID_ID.Integral < PID_ID.Lower_Integral)
		PID_ID.Integral = PID_ID.Lower_Integral;

	// �������
	PID_ID.Out = (((PID_ID.Kp >> 3) * wPIError) >> 12) + PID_ID.Integral;

	// �����ֵ
	if (PID_ID.Out > PID_ID.Upper_Output)
		PID_ID.Out = PID_ID.Upper_Output;
	if (PID_ID.Out < PID_ID.Lower_Output)
		PID_ID.Out = PID_ID.Lower_Output;

	// d���ѹ
	stru_Volt_dq.Ud = PID_ID.Out;

	//----------------------------------------------------------------------/
	// ��ѹԲ����
	wVqLimit = (stru_Volt_dq.Ud * stru_Volt_dq.Ud) >> 15;
	wVqLimit = (stru_FOC.Vs_Square_Max - wVqLimit) >> 6;

	// ����Uq��ѹ����
	PID_IQ.Upper_Output = (int32_t)Q15Sqrt_Table[wVqLimit];
	PID_IQ.Upper_Integral = PID_IQ.Upper_Output;

	//----------------------------------------------------------------------/
	// Q�����������

	// �������
	wPIError = (stru_Curr_dq_ref.Iq - stru_Curr_dq.Iq);

	// �������
	PID_IQ.Integral += (((PID_IQ.Ki >> 2) * wPIError) >> 13);

	// ���ֱ���
	if (PID_IQ.Integral > PID_IQ.Upper_Integral)
		PID_IQ.Integral = PID_IQ.Upper_Integral;
	if (PID_IQ.Integral < PID_IQ.Lower_Integral)
		PID_IQ.Integral = PID_IQ.Lower_Integral;

	// �������
	PID_IQ.Out = (((PID_IQ.Kp >> 3) * wPIError) >> 12) + PID_IQ.Integral;

	// �����ֵ
	if (PID_IQ.Out > PID_IQ.Upper_Output)
		PID_IQ.Out = PID_IQ.Upper_Output;
	if (PID_IQ.Out < PID_IQ.Lower_Output)
		PID_IQ.Out = PID_IQ.Lower_Output;

	// q���ѹ
	stru_Volt_dq.Uq = PID_IQ.Out;

	//=======================================================================/
	// Park��任
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

//=======================================================================/
// ����ռ�ʸ��
#if (Single_Shunt == Config_Shunt_Mode)
	{
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
#else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
#endif

	//=======================================================================/
	// ����״̬���
	volatile static u16 hStartCount = 0;
	volatile static u32 wStartFailCount = 0;

	//-----------------------------------------------------------------------/
	// ת�ٴ����趨ֵ�����ɹ�
	if (Basic.Mech_Speed > Basic.Speed_Close)
	{
		hStartCount++;
	}
	else
	{
		hStartCount = 0;
		wStartFailCount++;
	}
#if (Config_Debug_Mode == Debug_Disable)
	//-----------------------------------------------------------------------/
	// �����趨ʱ��ת��δ�ﵽ�趨ֵ��Ϊ����ʧ��
	// �����ɹ��ж�
	if (hStartCount > Time.Motor_StartSuccess)
	{
		hStartCount = 0;
		wStartFailCount = 0;
		wKpAddCount = 0;
		stru_FOC.Curr_Is_Ref = stru_Curr_dq_ref.Iq;

		// �л���һ״̬
		MOTOR_STATE = MC_SW;
	}

	//-----------------------------------------------------------------------/
	// ����ʧ���ж�
	if (wStartFailCount > Time.Motor_StartFail)
	{
		hStartCount = 0;
		wStartFailCount = 0;
		wKpAddCount = 0;
		Fault_StartFail = 1;
		MOTOR_FAULT_CODE = FAULT_STARTFAIL;
		SYSTEM_STATE = SYS_RESTART;
	}
#endif

	if (Basic.Mech_Speed > Basic.Speed_Close)
	{
		stru_SingleCurr.LN_States = 0;
		stru_SingleCurr.LN_Flag = 1;
	}

	//-----------------------------------------------------------------------/
	// ͣ��������ת�л�
	if (Flag.MC_RunStop == 0)
	{
		hStartCount = 0;
		wStartFailCount = 0;
		wKpAddCount = 0;
		MOTOR_STATE = MC_INIT;
	}

	//=======================================================================/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Switch
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Switch(void)
{
	//=======================================================================/
	// �������
	volatile int32_t wABSIa, wABSIb, wABSIc;

	//-----------------------------------------------------------------------/
	// ���������ֵ
	wABSIa = ABSFUN(stru_Curr_abc.Ia);
	wABSIb = ABSFUN(stru_Curr_abc.Ib);
	wABSIc = ABSFUN(stru_Curr_abc.Ic);

	//-----------------------------------------------------------------------/
	// ȱ����--�������ֵ
	if (Fault.Ia_max < wABSIa)
	{
		Fault.Ia_max = wABSIa;
	}
	if (Fault.Ib_max < wABSIb)
	{
		Fault.Ib_max = wABSIb;
	}
	if (Fault.Ic_max < wABSIc)
	{
		Fault.Ic_max = wABSIc;
	}

	//=======================================================================/
	// ����Alpha,Beta����
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// ����DQ����
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// �޸�λ�ù�����
	FOC_RotorPosition_Detection();

	//=======================================================================/
	// ����Ƕ�������
	volatile uint16_t bIndex;

	bIndex = (uint16_t)(stru_FOC.Elec_Angle + 32768) >> 6;

	switch (bIndex & 0x0300)
	{
	/* 0~90�� */
	case 0x0200:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 90~180�� */
	case 0x0300:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	/* 180~270�� */
	case 0x0000:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 270~360�� */
	case 0x0100:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	default:
		break;
	}

	//=======================================================================/
	// D���������
	if (stru_Curr_dq_ref.Id > 0)
		stru_Curr_dq_ref.Id -= 2;
	if (stru_Curr_dq_ref.Id < 0)
		stru_Curr_dq_ref.Id++;

	// Q���������
	if (stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq--;
	if (stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq++;

	//=======================================================================/
	// DQ����������
	//-----------------------------------------------------------------------/
	// ��ʱ����
	int32_t wPIError = 0;
	int32_t wVqLimit = 0;

	//----------------------------------------------------------------------/
	// D�����������

	// �������
	wPIError = (stru_Curr_dq_ref.Id - stru_Curr_dq.Id);

	// �������
	PID_ID.Integral += (((PID_ID.Ki >> 2) * wPIError) >> 13);

	// ���ֱ���
	if (PID_ID.Integral > PID_ID.Upper_Integral)
		PID_ID.Integral = PID_ID.Upper_Integral;
	if (PID_ID.Integral < PID_ID.Lower_Integral)
		PID_ID.Integral = PID_ID.Lower_Integral;

	// �������
	PID_ID.Out = (((PID_ID.Kp >> 3) * wPIError) >> 12) + PID_ID.Integral;

	// �����ֵ
	if (PID_ID.Out > PID_ID.Upper_Output)
		PID_ID.Out = PID_ID.Upper_Output;
	if (PID_ID.Out < PID_ID.Lower_Output)
		PID_ID.Out = PID_ID.Lower_Output;

	// d���ѹ
	stru_Volt_dq.Ud = PID_ID.Out;

	//----------------------------------------------------------------------/
	// ��ѹԲ����
	wVqLimit = (stru_Volt_dq.Ud * stru_Volt_dq.Ud) >> 15;
	wVqLimit = (stru_FOC.Vs_Square_Max - wVqLimit) >> 6;

	// ����Uq��ѹ����
	PID_IQ.Upper_Output = (int32_t)Q15Sqrt_Table[wVqLimit];
	PID_IQ.Upper_Integral = PID_IQ.Upper_Output;

	//----------------------------------------------------------------------/
	// Q�����������

	// �������
	wPIError = (stru_Curr_dq_ref.Iq - stru_Curr_dq.Iq);

	// �������
	PID_IQ.Integral += (((PID_IQ.Ki >> 2) * wPIError) >> 13);

	// ���ֱ���
	if (PID_IQ.Integral > PID_IQ.Upper_Integral)
		PID_IQ.Integral = PID_IQ.Upper_Integral;
	if (PID_IQ.Integral < PID_IQ.Lower_Integral)
		PID_IQ.Integral = PID_IQ.Lower_Integral;

	// �������
	PID_IQ.Out = (((PID_IQ.Kp >> 3) * wPIError) >> 12) + PID_IQ.Integral;

	// �����ֵ
	if (PID_IQ.Out > PID_IQ.Upper_Output)
		PID_IQ.Out = PID_IQ.Upper_Output;
	if (PID_IQ.Out < PID_IQ.Lower_Output)
		PID_IQ.Out = PID_IQ.Lower_Output;

	// q���ѹ
	stru_Volt_dq.Uq = PID_IQ.Out;

	//========================================================================/
	// Park��任
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

//=======================================================================/
// ����ռ�ʸ��
#if (Single_Shunt == Config_Shunt_Mode)
	{
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
#else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
#endif

	//=======================================================================/
	// ����������̬�Ĺ��ɽ׶�
	if (++stru_FOC.MotorRunTime >= wSwitchHoldTime)
	{

		// �л�̬����ʱ��
		stru_FOC.MotorRunTime = wSwitchHoldTime;

		// ������
		stru_FOC.Offset_Angle = Para.FOC.OffsetTheta;

		// dq�������л�
		PID_ID.Kp = Para.FOC.Dkp;
		PID_ID.Ki = Para.FOC.Dki;
		PID_IQ.Kp = Para.FOC.Qkp;
		PID_IQ.Ki = Para.FOC.Qki;

		PID_WL.KiSum = stru_Curr_dq_ref.Iq << 14;
		PID_WL.Integral = stru_Curr_dq_ref.Iq;
		stru_FOC.Curr_Is_Ref = stru_Curr_dq_ref.Iq;

// �ٶȻ�����
#if (Speed_Loop == Config_Contorl_Mode)
		{
			stru_Speed_Ctrl.RampOut = Basic.Calb_Speed;
		}
// ���ʻ�����
#elif (Power_Loop == Config_Contorl_Mode)
		{
			stru_Speed_Ctrl.RampOut = Basic.Power;
		}
// ����������
#else
		{
			stru_Speed_Ctrl.RampOut = stru_Curr_dq_ref.Iq;
		}
#endif

		// ������ʼ��
		stru_SingleCurr.LN_States = 0;
		stru_SingleCurr.LN_Flag = 1;

		// ����л�
		MOTOR_STATE = MC_RUN;
	}

	//=======================================================================/
	// ͣ��
	if (Flag.MC_RunStop == 0)
	{
		MOTOR_STATE = MC_INIT; // MC_STOP;
	}

	//=======================================================================/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Run
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Run(void)
{
	//=======================================================================/
	// �������
	volatile int32_t wABSIa, wABSIb, wABSIc;

	//-----------------------------------------------------------------------/
	// ���������ֵ
	wABSIa = ABSFUN(stru_Curr_abc.Ia);
	wABSIb = ABSFUN(stru_Curr_abc.Ib);
	wABSIc = ABSFUN(stru_Curr_abc.Ic);

	//-----------------------------------------------------------------------/
	/****-----�������ֵ����-----****/
	if ((wABSIa > Fault.OverCurrent_Value) || (wABSIb > Fault.OverCurrent_Value) || (wABSIc > Fault.OverCurrent_Value))
	{
		Fault_OverCurrent = 1;
		MOTOR_FAULT_CODE = FAULT_OVERCURRENT;
		SYSTEM_STATE = SYS_FAULT;
	}

	//-----------------------------------------------------------------------/
	// ȱ����--�������ֵ
	if (Fault.Ia_max < wABSIa)
	{
		Fault.Ia_max = wABSIa;
	}
	if (Fault.Ib_max < wABSIb)
	{
		Fault.Ib_max = wABSIb;
	}
	if (Fault.Ic_max < wABSIc)
	{
		Fault.Ic_max = wABSIc;
	}

	//=======================================================================/
	// ����Alpha��Beta����
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// ����D��Q����
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// �޸�λ�ù�����
	FOC_RotorPosition_Detection();

	//=======================================================================/
	// ����Ƕ�������
	volatile uint16_t bIndex;

	bIndex = (uint16_t)(stru_FOC.Elec_Angle + 32768) >> 6;

	switch (bIndex & 0x0300)
	{
	/* 0~90�� */
	case 0x0200:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 90~180�� */
	case 0x0300:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	/* 180~270�� */
	case 0x0000:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 270~360�� */
	case 0x0100:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	default:
		break;
	}

//=======================================================================/
// DQ��������
#if (Weaken_Disable == Config_Weak_Mode)
	{
		stru_Curr_dq_ref.Id = 0;

		if (stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)
			stru_Curr_dq_ref.Iq--;
		if (stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)
			stru_Curr_dq_ref.Iq++;
	}
#else
	{
		FOC_Control_Weakening();
	}
#endif

	//=======================================================================/
	// DQ����������
	//-----------------------------------------------------------------------/
	// ��ʱ����
	int32_t wPIError = 0;
	int32_t wVqLimit = 0;

	//----------------------------------------------------------------------/
	// D�����������

	// �������
	wPIError = (stru_Curr_dq_ref.Id - stru_Curr_dq.Id);

	// �������
	PID_ID.Integral += (((PID_ID.Ki >> 2) * wPIError) >> 13);

	// ���ֱ���
	if (PID_ID.Integral > PID_ID.Upper_Integral)
		PID_ID.Integral = PID_ID.Upper_Integral;
	if (PID_ID.Integral < PID_ID.Lower_Integral)
		PID_ID.Integral = PID_ID.Lower_Integral;

	// �������
	PID_ID.Out = (((PID_ID.Kp >> 3) * wPIError) >> 12) + PID_ID.Integral;

	// �����ֵ
	if (PID_ID.Out > PID_ID.Upper_Output)
		PID_ID.Out = PID_ID.Upper_Output;
	if (PID_ID.Out < PID_ID.Lower_Output)
		PID_ID.Out = PID_ID.Lower_Output;

	// d���ѹ
	stru_Volt_dq.Ud = PID_ID.Out;

	//----------------------------------------------------------------------/
	// ��ѹԲ����
	wVqLimit = (stru_Volt_dq.Ud * stru_Volt_dq.Ud) >> 15;
	wVqLimit = (stru_FOC.Vs_Square_Max - wVqLimit) >> 6;

	// ����Uq��ѹ����
	PID_IQ.Upper_Output = (int32_t)Q15Sqrt_Table[wVqLimit];
	PID_IQ.Upper_Integral = PID_IQ.Upper_Output;

	//----------------------------------------------------------------------/
	// Q�����������

	// �������
	wPIError = (stru_Curr_dq_ref.Iq - stru_Curr_dq.Iq);

	// �������
	PID_IQ.Integral += (((PID_IQ.Ki >> 2) * wPIError) >> 13);

	// ���ֱ���
	if (PID_IQ.Integral > PID_IQ.Upper_Integral)
		PID_IQ.Integral = PID_IQ.Upper_Integral;
	if (PID_IQ.Integral < PID_IQ.Lower_Integral)
		PID_IQ.Integral = PID_IQ.Lower_Integral;

	// �������
	PID_IQ.Out = (((PID_IQ.Kp >> 3) * wPIError) >> 12) + PID_IQ.Integral;

	// �����ֵ
	if (PID_IQ.Out > PID_IQ.Upper_Output)
		PID_IQ.Out = PID_IQ.Upper_Output;
	if (PID_IQ.Out < PID_IQ.Lower_Output)
		PID_IQ.Out = PID_IQ.Lower_Output;

	// q���ѹ
	stru_Volt_dq.Uq = PID_IQ.Out;

	//=======================================================================/
	// Park��任
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

//=======================================================================/
// ����ռ�ʸ��
#if (Single_Shunt == Config_Shunt_Mode)
	{
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
#else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
#endif

	//=======================================================================/
	// ����״̬���
	if (Flag.MC_RunStop == 0)
	{
		MOTOR_STATE = MC_STOP;
	}
	//----------------------------------------------------------------------/
	// �������⴦��
	if ((stru_Volt_dq.Uq > 31500) && (Basic.Vbus_Check < (Fault.RunLowerVoltage_Value - 300)))
	{
		MOTOR_STATE = MC_FAULT;
		Time.Motor_PowerDown = 100;
	}
	//----------------------------------------------------------------------/
	// ��ĸ�߾�ֵ�����ж�
	if ((Basic.Ibus > stru_FOC.CurrentLimit1) && (0 == stru_FOC.CurrentLimitFlag))
	{
		if (++stru_FOC.CurrentLimitCount > 50)
		{
			stru_FOC.CurrentLimitCount = 0;
			stru_FOC.CurrentLimitFlag = 1;
		}
	}
	else
	{
		stru_FOC.CurrentLimitCount = 0;
	}
	//=======================================================================/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Stop
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Stop(void)
{
	//-----------------------------------------------------------------------/
	// ����Alpha��Beta����
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// ����D��Q����
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// �޸�λ�ù�����
	FOC_RotorPosition_Detection();

	//=======================================================================/
	// ����Ƕ�������
	volatile uint16_t bIndex;

	bIndex = (uint16_t)(stru_FOC.Elec_Angle + 32768) >> 6;

	switch (bIndex & 0x0300)
	{
	/* 0~90�� */
	case 0x0200:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 90~180�� */
	case 0x0300:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	/* 180~270�� */
	case 0x0000:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 270~360�� */
	case 0x0100:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	default:
		break;
	}

	//----------------------------------------------------------------------/
	// DQ����Ϊ0
	stru_FOC.Curr_Is_Ref = 0;

	if (stru_Curr_dq_ref.Id > 0)
		stru_Curr_dq_ref.Id -= 1;
	if (stru_Curr_dq_ref.Id < 0)
		stru_Curr_dq_ref.Id += 1;
	if (stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq -= 1;
	if (stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq += 1;

	//----------------------------------------------------------------------/
	// DQ��ѹΪ0
	if (stru_Volt_dq.Uq < FOC_STOP_UQ)
	{
		/*voltage Vds =0*/
		if (stru_Volt_dq.Ud < 0)
			stru_Volt_dq.Ud++;
		else if (stru_Volt_dq.Ud > 0)
			stru_Volt_dq.Ud--;
		else
			stru_Volt_dq.Ud = 0;

		/*voltage Vqs =0*/
		if (stru_Volt_dq.Uq < 0)
			stru_Volt_dq.Uq++;
		else if (stru_Volt_dq.Uq > 0)
			stru_Volt_dq.Uq--;
		else
			stru_Volt_dq.Uq = 0;
	}
	//----------------------------------------------------------------------/
	else
	{
		//----------------------------------------------------------------------/
		// DQ����������
		//-----------------------------------------------------------------------/
		// ��ʱ����
		int32_t wPIError = 0;
		int32_t wVqLimit = 0;

		//----------------------------------------------------------------------/
		// D�����������

		// �������
		wPIError = (stru_Curr_dq_ref.Id - stru_Curr_dq.Id);

		// �������
		PID_ID.Integral += (((PID_ID.Ki >> 2) * wPIError) >> 13);

		// ���ֱ���
		if (PID_ID.Integral > PID_ID.Upper_Integral)
			PID_ID.Integral = PID_ID.Upper_Integral;
		if (PID_ID.Integral < PID_ID.Lower_Integral)
			PID_ID.Integral = PID_ID.Lower_Integral;

		// �������
		PID_ID.Out = (((PID_ID.Kp >> 3) * wPIError) >> 12) + PID_ID.Integral;

		// �����ֵ
		if (PID_ID.Out > PID_ID.Upper_Output)
			PID_ID.Out = PID_ID.Upper_Output;
		if (PID_ID.Out < PID_ID.Lower_Output)
			PID_ID.Out = PID_ID.Lower_Output;

		// d���ѹ
		stru_Volt_dq.Ud = PID_ID.Out;

		//----------------------------------------------------------------------/
		// ��ѹԲ����
		wVqLimit = (stru_Volt_dq.Ud * stru_Volt_dq.Ud) >> 15;
		wVqLimit = (stru_FOC.Vs_Square_Max - wVqLimit) >> 6;

		// ����Uq��ѹ����
		PID_IQ.Upper_Output = (int32_t)Q15Sqrt_Table[wVqLimit];
		PID_IQ.Upper_Integral = PID_IQ.Upper_Output;

		//----------------------------------------------------------------------/
		// Q�����������

		// �������
		wPIError = (stru_Curr_dq_ref.Iq - stru_Curr_dq.Iq);

		// �������
		PID_IQ.Integral += (((PID_IQ.Ki >> 2) * wPIError) >> 13);

		// ���ֱ���
		if (PID_IQ.Integral > PID_IQ.Upper_Integral)
			PID_IQ.Integral = PID_IQ.Upper_Integral;
		if (PID_IQ.Integral < PID_IQ.Lower_Integral)
			PID_IQ.Integral = PID_IQ.Lower_Integral;

		// �������
		PID_IQ.Out = (((PID_IQ.Kp >> 3) * wPIError) >> 12) + PID_IQ.Integral;

		// �����ֵ
		if (PID_IQ.Out > PID_IQ.Upper_Output)
			PID_IQ.Out = PID_IQ.Upper_Output;
		if (PID_IQ.Out < PID_IQ.Lower_Output)
			PID_IQ.Out = PID_IQ.Lower_Output;

		// q���ѹ
		stru_Volt_dq.Uq = PID_IQ.Out;
	}

	//----------------------------------------------------------------------/
	// Park��任
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

//----------------------------------------------------------------------/
// ����ռ�ʸ��
#if (Single_Shunt == Config_Shunt_Mode)
	{
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
#else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
#endif

	//----------------------------------------------------------------------/
	// ͣ��״̬���
	FOC_Stop_Check();
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Brake
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Brake(void)
{
	// ���¹�ɲ��
	Brige_Output_Down();
	if (stru_FOC.Elec_We > 0)
	{
		stru_FOC.Elec_We--;
	}
	else
	{
		stru_FOC.Elec_We = 0;
	}
	// ɲ������
	if (Time.Motor_PowerDown == 0)
	{
		stru_FOC.Elec_We = 0;
		MOTOR_STATE = MC_INIT;
	}
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Process
 * Description    :
 * Function Call  : EPWM_IRQHandler�жϺ�������
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Process(void)
{
	//=======================================================================//
	// �������״̬��
	switch (MOTOR_STATE)
	{
	//-------------------------------------------------------------------/
	// ����״̬
	case MC_INIT:

		FOC_Control_Init();

		break;

	//-------------------------------------------------------------------/
	// ���״̬
	case MC_CHARGE:

		FOC_Control_Charge();

		break;

	//-------------------------------------------------------------------/
	// ˳���״̬
	case MC_WIND:

		FOC_Control_Wind();

		break;

	//-------------------------------------------------------------------/
	// Ԥ��λ״̬
	case MC_ALIGN:

		FOC_Control_Align();

		break;

	//-------------------------------------------------------------------/
	// ����״̬
	case MC_START:

		FOC_Control_Start();

		break;

	//-------------------------------------------------------------------/
	// �л�״̬
	case MC_SW:

		FOC_Control_Switch();

		break;

	//-------------------------------------------------------------------/
	// ����״̬
	case MC_RUN:

		FOC_Control_Run();

		break;

	//-------------------------------------------------------------------/
	// ͣ��״̬
	case MC_STOP:

		FOC_Control_Stop();

		break;

	//-------------------------------------------------------------------/
	// ɲ��״̬
	case MC_BRAKE:

		FOC_Control_Brake();

		break;

	//-------------------------------------------------------------------/
	// ����״̬
	case MC_FAULT:

		Brige_Output_Off();
		if (Time.Motor_PowerDown == 0)
		{
			Basic.Mech_Speed = 0;
			MOTOR_FAULT_CODE = FAULT_STARTUNDERVOLTAGE;
			MOTOR_STATE = MC_INIT;
		}

		break;

		//-------------------------------------------------------------------/

	default:
		break;
	}
	//=======================================================================//
}

//===========================================================================//
//*********END   ************************************************************//
//===========================================================================//
