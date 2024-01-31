//==========================================================================//
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * @file    mc_foc.c
 * @author  CMS Motor Control Team:lpj
 * @version 第二代风机平台
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
PID_Components PID_ID = {0}; /* D轴电流环 */

PID_Components PID_IQ = {0}; /* Q轴电流环*/

PID_Components PID_WL = {0}; /* 速度/功率外环*/

struct_SinCos stru_SinCos = {0}; /* SinCos计算结构体 */

struct_Curr_abc stru_Curr_abc = {0}; /* 马达abc三相电流 */

struct_Clark stru_Curr_alphabeta = {0}; /* alpha、beta坐标系电流 */

struct_Park stru_Curr_dq = {0}; /* d、q坐标系电流 */

struct_Park stru_Curr_dq_ref = {0}; /* d、q坐标系参考电流 */

struct_RevPark stru_Volt_alphabeta = {0}; /* alpha、beta坐标系电压 */

struct_Volt_dq stru_Volt_dq = {0}; /* d、q坐标系电压 */

struct_Arctan stru_Arctan = {0}; /* 反正切计算 */

struct_SVPWM stru_SVM_Calc = {0}; /* 空间矢量结构体 */

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/
volatile uint16_t hBrakeDuty = 0; /* 刹车占空比 */
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Start_Init(void)
{
	//------------------------------------------------------------------------/
	// 自收敛启动参数初始化
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_ControlPara_Init(void)
{
	//------------------------------------------------------------------------/
	// 变量初始化
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
	// 变量初始化
	Fault.PhaseA_LossCnt = 0;
	Fault.PhaseB_LossCnt = 0;
	Fault.PhaseC_LossCnt = 0;

	//------------------------------------------------------------------------/
	// 变量初始化
	Flag.Charge = 0;
	Flag.IPD = 0;
	Flag.MotorBack = 0;
	Flag.MotorFR = Config.MotorDir;
	Flag.MotorFR_Pre = Flag.MotorFR;

	//------------------------------------------------------------------------/
	// 变量初始化
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
	// 启动观测器参数初始化
	FOC_Start_Init();

	// PID参数初始化
	FOC_PID_Parameter_Init();

	// 位置观测器初始化
	FOC_RotorPosition_Init();

	// 反电动势参数初始化
	FOC_BEMF_Init();

	// 单电阻初始化
	FOC_SingleShunt_Init();

	// 开环参数初始化
	FOC_OpenLoop_Init();

	//------------------------------------------------------------------------/
	Brige_Output_Off(); // 关闭桥臂输出
	//------------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Read_Current
 * Description    :
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Read_Current(void)
{
	int32_t wTempA, wTempB, wTempC;
	//-----------------------------------------------------------------------/
	// 读取电流采样值
	stru_Sample.ADVal[CH_IP_A] = (uint32_t)ADC->DATA[ADC_DATA_CHA];
	stru_Sample.ADVal[CH_IP_B] = (uint32_t)ADC->DATA[ADC_DATA_CHB];

	//---------------------------------------------------------------------/
	// 双电阻电流计算
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
	// 电流相序匹配
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Overcurrent_Check(void)
{
	volatile int32_t wABSIa, wABSIb, wABSIc;

	//-----------------------------------------------------------------------/
	// 电流的绝对值
	wABSIa = ABSFUN(stru_Curr_abc.Ia);
	wABSIb = ABSFUN(stru_Curr_abc.Ib);
	wABSIc = ABSFUN(stru_Curr_abc.Ic);

	//-----------------------------------------------------------------------/
	// 相电流峰值保护
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
	// 缺相检测--电流最大值
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Brake_Control(void)
{
	//-----------------------------------------------------------------------/
	// 刹车占空比
	if (hBrakeDuty < 32767)
		hBrakeDuty += 100;
	if (hBrakeDuty > 32767)
		hBrakeDuty = 32767;

	//-----------------------------------------------------------------------/
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	// 计算duty
	EPWM->CMPDAT[0] = ((hBrakeDuty * stru_FOC.EPWM_Period) >> 15);
	EPWM->CMPDAT[2] = ((hBrakeDuty * stru_FOC.EPWM_Period) >> 15);
	EPWM->CMPDAT[4] = ((hBrakeDuty * stru_FOC.EPWM_Period) >> 15);
	// 使能加载周期&&占空比
	EPWM->CON3 |= 0x00001500;
	EPWM->MASK = 0x00001500;
	EPWM->LOCK = 0x0;

	//-----------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Start_Check
 * Description    :
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Start_Check(void)
{
	//=======================================================================/
	// 启动状态检测
	volatile static uint16_t hStartCount = 0;
	volatile static uint32_t wStartFailCount = 0;

	//-----------------------------------------------------------------------/
	// 转速大于设定值启动成功
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
	// 超过设定时间转速未达到设定值认为启动失败
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
	// 停机或正反转切换
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Stop_Check(void)
{
	volatile static uint32_t wMotorStopholdCount = 0;
// 开启刹车
#if (BRAKE_STOP_MODE == BRAKE_STOP_ENABLE)
	{
		// 没有故障产生进入刹车状态，否则进入初始态
		if (MOTOR_FAULT_CODE == NOERROR)
		{
			// 停机过程中接收到重启命令进入顺风启动判定,否则进入刹车状态
			if ((Flag.MC_RunStop == 1) && (Flag.MotorFR_Pre == Config.MotorDir))
			{
				// 转速大于设定转速可直接回复RUN态，否则进入初始态进行反电动势判定
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
			// 停机过程中接收到重启命令进入顺风启动判定,否则进入INIT状态
			if ((Flag.MC_RunStop == 1) && (Flag.MotorFR_Pre == Config.MotorDir))
			{
				// 转速大于设定转速可直接回复RUN态，否则进入初始态进行反电动势判定
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
	// 超时强制停机
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Init(void)
{
	//-----------------------------------------------------------------------//
	// 参数初始化
	FOC_ControlPara_Init();

//-----------------------------------------------------------------------//
// 启动
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Charge(void)
{
//=======================================================================//
// 充电，双N控制预充电，PN控制无需充电
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
		EPWM->MASK = 0x00003D00; // U相输出
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
		EPWM->MASK = 0x00003500; // U、V相输出
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
		EPWM->MASK = 0x00001500; // U、V、W相输出
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
 * Description    : 顺逆风检测
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Wind_Check(void)
{
	//-----------------------------------------------------------------------/
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// 计算d、q电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();

	//-----------------------------------------------------------------------/
	// 计算角度
	stru_FOC.Elec_Angle = stru_rpd_est.Angle;
	stru_SinCos = FOC_SinCos_Calc(stru_FOC.Elec_Angle);

	//----------------------------------------------------------------------/
	// Park逆变换
	stru_Volt_dq.Ud = 0;
	stru_Volt_dq.Uq = 0;

	//----------------------------------------------------------------------/
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

	//----------------------------------------------------------------------/
	// 计算空间矢量
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
 * Function Call  : EPWM_IRQHandler中断函数调用
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
	// 反电动势ADC触发采样配置
	if (!bFristFlag)
	{
		bFristFlag = 1;
		EPWM_ConfigCompareTriger(EPWM_CMPTG_0, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM0, EPWM_HALFPERIOD - 128);
		EPWM_ConfigCompareTriger(EPWM_CMPTG_1, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM2, EPWM_HALFPERIOD + 128);
		ADC_EnableEPWMTriggerChannel(ADC_SCAN_UBEMF | ADC_SCAN_VBEMF | ADC_SCAN_WBEMF);
		ADC_EnableHardwareTrigger(ADC_TG_EPWM0_ZERO);
	}
	//-----------------------------------------------------------------------/
	// MOS管全封
	Brige_Output_Off();

	// 读取反电动势电压
	stru_bemf.AD_Value[0] = (int32_t)ADC->DATA[ADC_DATA_UBEMF];
	stru_bemf.AD_Value[1] = (int32_t)ADC->DATA[ADC_DATA_VBEMF];
	stru_bemf.AD_Value[2] = (int32_t)ADC->DATA[ADC_DATA_WBEMF];

	//-----------------------------------------------------------------------/
	// 计算反电动势
	FOC_BEMF_Detection();

	//-----------------------------------------------------------------------/
	static uint16_t v_Status_Count = 0;

	// 判断电机反电动势转速
	if (stru_bemf.Speed > BEMF_SPEED_MAX)
	{
		stru_bemf.Status = 3; // 等待电机减速
		v_Status_Count = 0;
	}
	else if ((stru_bemf.Speed <= BEMF_SPEED_MAX) && (stru_bemf.Speed > BEMF_SPEED_MIN))
	{
		if (++v_Status_Count > 50)
			stru_bemf.Status = 2; // 顺分启动 切入SW
	}
	else if ((stru_bemf.Speed <= BEMF_SPEED_MIN) && (stru_bemf.Speed > BEMF_SPEED_START))
	{
		stru_bemf.Status = 1; // 等待电机减速
		v_Status_Count = 0;
	}
	else
	{
		stru_bemf.Status = 0; // 静止启动
		v_Status_Count = 0;
	}
#if 1
	//---------------------------------------------------------------------------/
	// 反电势启动检测持续时间

	if ((++stru_bemf.Time >= BEMF_CHECK_TIME))
	{
		stru_bemf.Time = BEMF_CHECK_TIME;

		//-----------------------------------------------------------------------/
		switch (stru_bemf.Status)
		{
		//-------------------------------------------------------------------/
		// 静止启动
		case 0:

			if ((stru_bemf.Angle < 1000) || (stru_bemf.Speed < 3000))
			{
				// 启动前的参数初始化
				stru_Curr_dq_ref.Id = 0;
				stru_Curr_dq_ref.Iq = Para.Start.run_current_init;
				stru_FOC.Curr_Is_Ref = Para.Start.run_current_final;

				stru_bemf.Time = 0;
				bFristFlag = 0;
				//------------------------------------------------------------------------------/
				// 关闭采样
				ADC_DisableEPWMTriggerChannel(ADC_SCAN_UBEMF | ADC_SCAN_VBEMF | ADC_SCAN_WBEMF);

				// 解锁寄存器
				EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
// 给定零矢量
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
				// 使能加载周期&&占空比
				EPWM->CON3 |= 0x00001500;
				EPWM->LOCK = 0x0;
				// 使能驱动
				Brige_Output_On();

				//------------------------------------------------------------------------------/
				// 进入启动状态
				MOTOR_STATE = MC_START;
			}

			break;

		//-------------------------------------------------------------------/
		// 等待电机减速
		case 1:

			break;

		//-------------------------------------------------------------------/
		// 顺风启动
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
				// 滑行补偿角度
				stru_rpd_est.Angle = stru_bemf.Angle + (uint32_t)BEMF_OFFSET_ANGLE; //+ DEG_120;

				if (stru_rpd_est.Angle >= 65535)
					stru_rpd_est.Angle = stru_rpd_est.Angle - 65535;
				else if (stru_rpd_est.Angle < 0)
					stru_rpd_est.Angle = stru_rpd_est.Angle + 65535;

				stru_rpd_est.PI_Ui = HWDIV_Div((stru_bemf.DeltaTheta * 32768), stru_rpd_est.Tc);
				stru_rpd_est.PI_Lower = (stru_rpd_est.PI_Ui * 26214) >> 15;

				//-------------------------------------------------------------/
				// Dq电流环PI
				PID_ID.Kp = 32768;
				PID_ID.Ki = 3276;
				PID_ID.Integral = stru_bemf.Ud >> 1;

				PID_IQ.Kp = 32768;
				PID_IQ.Ki = 3276;
				PID_IQ.Integral = stru_bemf.Uq >> 1;

				//-------------------------------------------------------------/
				// 启动前的参数初始化
				stru_Curr_dq_ref.Id = 0;
				stru_Curr_dq_ref.Iq = Para.Start.run_current_init;
				stru_FOC.Curr_Is_Ref = Para.Start.run_current_init; // Para.Start.run_current_final;

				//------------------------------------------------------------------------------/
				// 解锁寄存器
				EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
// 给定零矢量
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

				// 使能加载周期&&占空比
				EPWM->CON3 |= 0x00001500;
				EPWM->LOCK = 0x0;

				//------------------------------------------------------------------------------/
				// 关闭采样
				ADC_DisableEPWMTriggerChannel(ADC_SCAN_UBEMF | ADC_SCAN_VBEMF | ADC_SCAN_WBEMF);

				// 使能驱动
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
		// 等待降速
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
	// 顺风状态检测
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Align(void)
{
	//-----------------------------------------------------------------------/
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// 计算d、q电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// 第一观测器
	FOC_Adaption_Angle_Calc(&stru_adap_est);

	//-----------------------------------------------------------------------/
	// 计算角度
	stru_FOC.Elec_We = 1;

	// stru_FOC.Elec_Angle += 	stru_FOC.Elec_We;

	if (stru_FOC.Elec_Angle < 0)
		stru_FOC.Elec_Angle += 65535;
	else if (stru_FOC.Elec_Angle > 65535)
		stru_FOC.Elec_Angle -= 65535;

	//-----------------------------------------------------------------------/
	// 计算角度正余弦
	stru_SinCos = FOC_SinCos_Calc(stru_FOC.Elec_Angle);

	//-----------------------------------------------------------------------/
	// Q轴电流给定
	if (stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq--;
	if (stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq += 1;

	PID_ID.Upper_Output = 10000;
	PID_ID.Upper_Integral = 10000;

	//-----------------------------------------------------------------------/
	// DQ电流环计算
	FOC_DQCurrent_PIRegulator();

	//----------------------------------------------------------------------/
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

	//----------------------------------------------------------------------/
	// 计算电压占空比
	if (Single_Shunt == Config.Mode_Shunt)
	{
		FOC_SingleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}
	else
	{
		FOC_DoubleShunt_Calc(&stru_SVM_Calc, stru_Volt_alphabeta);
	}

	//=======================================================================/
	// 预定位检测

	volatile static u16 hStartCount = 0;

	//-----------------------------------------------------------------------/
	// 预定位时间
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
	// 停机
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
 * Function Call  : EPWM_IRQHandler中断函数调用
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
	// 过流检测
	volatile int32_t wABSIa, wABSIb, wABSIc;

	//-----------------------------------------------------------------------/
	// 电流的最大值
	wABSIa = ABSFUN(stru_Curr_abc.Ia);
	wABSIb = ABSFUN(stru_Curr_abc.Ib);
	wABSIc = ABSFUN(stru_Curr_abc.Ic);

	//-----------------------------------------------------------------------/
	// 缺相检测--电流最大值
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
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// 计算DQ电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();

	//=======================================================================/
	// 计算角度正余弦
	volatile uint16_t bIndex;

	bIndex = (uint16_t)(stru_FOC.Elec_Angle + 32768) >> 6;

	switch (bIndex & 0x0300)
	{
	/* 0~90度 */
	case 0x0200:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 90~180度 */
	case 0x0300:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	/* 180~270度 */
	case 0x0000:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 270~360度 */
	case 0x0100:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	default:
		break;
	}

	//=======================================================================/
	// Q轴电流参考值
	if (stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq--;
	if (stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq += 10;

	//=======================================================================/
	// DQ电流环计算
	//-----------------------------------------------------------------------/
	// 临时变量
	int32_t wPIError = 0;
	int32_t wVqLimit = 0;

	//----------------------------------------------------------------------/
	// D轴电流环计算

	// 计算误差
	wPIError = (stru_Curr_dq_ref.Id - stru_Curr_dq.Id);

	// 计算积分
	PID_ID.Integral += (((PID_ID.Ki >> 2) * wPIError) >> 13);

	// 积分饱和
	if (PID_ID.Integral > PID_ID.Upper_Integral)
		PID_ID.Integral = PID_ID.Upper_Integral;
	if (PID_ID.Integral < PID_ID.Lower_Integral)
		PID_ID.Integral = PID_ID.Lower_Integral;

	// 计算输出
	PID_ID.Out = (((PID_ID.Kp >> 3) * wPIError) >> 12) + PID_ID.Integral;

	// 输出限值
	if (PID_ID.Out > PID_ID.Upper_Output)
		PID_ID.Out = PID_ID.Upper_Output;
	if (PID_ID.Out < PID_ID.Lower_Output)
		PID_ID.Out = PID_ID.Lower_Output;

	// d轴电压
	stru_Volt_dq.Ud = PID_ID.Out;

	//----------------------------------------------------------------------/
	// 电压圆限制
	wVqLimit = (stru_Volt_dq.Ud * stru_Volt_dq.Ud) >> 15;
	wVqLimit = (stru_FOC.Vs_Square_Max - wVqLimit) >> 6;

	// 计算Uq电压极限
	PID_IQ.Upper_Output = (int32_t)Q15Sqrt_Table[wVqLimit];
	PID_IQ.Upper_Integral = PID_IQ.Upper_Output;

	//----------------------------------------------------------------------/
	// Q轴电流环计算

	// 计算误差
	wPIError = (stru_Curr_dq_ref.Iq - stru_Curr_dq.Iq);

	// 计算积分
	PID_IQ.Integral += (((PID_IQ.Ki >> 2) * wPIError) >> 13);

	// 积分饱和
	if (PID_IQ.Integral > PID_IQ.Upper_Integral)
		PID_IQ.Integral = PID_IQ.Upper_Integral;
	if (PID_IQ.Integral < PID_IQ.Lower_Integral)
		PID_IQ.Integral = PID_IQ.Lower_Integral;

	// 计算输出
	PID_IQ.Out = (((PID_IQ.Kp >> 3) * wPIError) >> 12) + PID_IQ.Integral;

	// 输出限值
	if (PID_IQ.Out > PID_IQ.Upper_Output)
		PID_IQ.Out = PID_IQ.Upper_Output;
	if (PID_IQ.Out < PID_IQ.Lower_Output)
		PID_IQ.Out = PID_IQ.Lower_Output;

	// q轴电压
	stru_Volt_dq.Uq = PID_IQ.Out;

	//=======================================================================/
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

//=======================================================================/
// 计算空间矢量
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
	// 启动状态检测
	volatile static u16 hStartCount = 0;
	volatile static u32 wStartFailCount = 0;

	//-----------------------------------------------------------------------/
	// 转速大于设定值启动成功
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
	// 超过设定时间转速未达到设定值认为启动失败
	// 启动成功判定
	if (hStartCount > Time.Motor_StartSuccess)
	{
		hStartCount = 0;
		wStartFailCount = 0;
		wKpAddCount = 0;
		stru_FOC.Curr_Is_Ref = stru_Curr_dq_ref.Iq;

		// 切换下一状态
		MOTOR_STATE = MC_SW;
	}

	//-----------------------------------------------------------------------/
	// 启动失败判定
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
	// 停机或正反转切换
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Switch(void)
{
	//=======================================================================/
	// 过流检测
	volatile int32_t wABSIa, wABSIb, wABSIc;

	//-----------------------------------------------------------------------/
	// 电流的最大值
	wABSIa = ABSFUN(stru_Curr_abc.Ia);
	wABSIb = ABSFUN(stru_Curr_abc.Ib);
	wABSIc = ABSFUN(stru_Curr_abc.Ic);

	//-----------------------------------------------------------------------/
	// 缺相检测--电流最大值
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
	// 计算Alpha,Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// 计算DQ电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();

	//=======================================================================/
	// 计算角度正余弦
	volatile uint16_t bIndex;

	bIndex = (uint16_t)(stru_FOC.Elec_Angle + 32768) >> 6;

	switch (bIndex & 0x0300)
	{
	/* 0~90度 */
	case 0x0200:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 90~180度 */
	case 0x0300:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	/* 180~270度 */
	case 0x0000:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 270~360度 */
	case 0x0100:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	default:
		break;
	}

	//=======================================================================/
	// D轴电流给定
	if (stru_Curr_dq_ref.Id > 0)
		stru_Curr_dq_ref.Id -= 2;
	if (stru_Curr_dq_ref.Id < 0)
		stru_Curr_dq_ref.Id++;

	// Q轴电流给定
	if (stru_Curr_dq_ref.Iq > stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq--;
	if (stru_Curr_dq_ref.Iq < stru_FOC.Curr_Is_Ref)
		stru_Curr_dq_ref.Iq++;

	//=======================================================================/
	// DQ电流环计算
	//-----------------------------------------------------------------------/
	// 临时变量
	int32_t wPIError = 0;
	int32_t wVqLimit = 0;

	//----------------------------------------------------------------------/
	// D轴电流环计算

	// 计算误差
	wPIError = (stru_Curr_dq_ref.Id - stru_Curr_dq.Id);

	// 计算积分
	PID_ID.Integral += (((PID_ID.Ki >> 2) * wPIError) >> 13);

	// 积分饱和
	if (PID_ID.Integral > PID_ID.Upper_Integral)
		PID_ID.Integral = PID_ID.Upper_Integral;
	if (PID_ID.Integral < PID_ID.Lower_Integral)
		PID_ID.Integral = PID_ID.Lower_Integral;

	// 计算输出
	PID_ID.Out = (((PID_ID.Kp >> 3) * wPIError) >> 12) + PID_ID.Integral;

	// 输出限值
	if (PID_ID.Out > PID_ID.Upper_Output)
		PID_ID.Out = PID_ID.Upper_Output;
	if (PID_ID.Out < PID_ID.Lower_Output)
		PID_ID.Out = PID_ID.Lower_Output;

	// d轴电压
	stru_Volt_dq.Ud = PID_ID.Out;

	//----------------------------------------------------------------------/
	// 电压圆限制
	wVqLimit = (stru_Volt_dq.Ud * stru_Volt_dq.Ud) >> 15;
	wVqLimit = (stru_FOC.Vs_Square_Max - wVqLimit) >> 6;

	// 计算Uq电压极限
	PID_IQ.Upper_Output = (int32_t)Q15Sqrt_Table[wVqLimit];
	PID_IQ.Upper_Integral = PID_IQ.Upper_Output;

	//----------------------------------------------------------------------/
	// Q轴电流环计算

	// 计算误差
	wPIError = (stru_Curr_dq_ref.Iq - stru_Curr_dq.Iq);

	// 计算积分
	PID_IQ.Integral += (((PID_IQ.Ki >> 2) * wPIError) >> 13);

	// 积分饱和
	if (PID_IQ.Integral > PID_IQ.Upper_Integral)
		PID_IQ.Integral = PID_IQ.Upper_Integral;
	if (PID_IQ.Integral < PID_IQ.Lower_Integral)
		PID_IQ.Integral = PID_IQ.Lower_Integral;

	// 计算输出
	PID_IQ.Out = (((PID_IQ.Kp >> 3) * wPIError) >> 12) + PID_IQ.Integral;

	// 输出限值
	if (PID_IQ.Out > PID_IQ.Upper_Output)
		PID_IQ.Out = PID_IQ.Upper_Output;
	if (PID_IQ.Out < PID_IQ.Lower_Output)
		PID_IQ.Out = PID_IQ.Lower_Output;

	// q轴电压
	stru_Volt_dq.Uq = PID_IQ.Out;

	//========================================================================/
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

//=======================================================================/
// 计算空间矢量
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
	// 启动到运行态的过渡阶段
	if (++stru_FOC.MotorRunTime >= wSwitchHoldTime)
	{

		// 切换态保持时间
		stru_FOC.MotorRunTime = wSwitchHoldTime;

		// 补偿角
		stru_FOC.Offset_Angle = Para.FOC.OffsetTheta;

		// dq环参数切换
		PID_ID.Kp = Para.FOC.Dkp;
		PID_ID.Ki = Para.FOC.Dki;
		PID_IQ.Kp = Para.FOC.Qkp;
		PID_IQ.Ki = Para.FOC.Qki;

		PID_WL.KiSum = stru_Curr_dq_ref.Iq << 14;
		PID_WL.Integral = stru_Curr_dq_ref.Iq;
		stru_FOC.Curr_Is_Ref = stru_Curr_dq_ref.Iq;

// 速度环参数
#if (Speed_Loop == Config_Contorl_Mode)
		{
			stru_Speed_Ctrl.RampOut = Basic.Calb_Speed;
		}
// 功率环参数
#elif (Power_Loop == Config_Contorl_Mode)
		{
			stru_Speed_Ctrl.RampOut = Basic.Power;
		}
// 电流环参数
#else
		{
			stru_Speed_Ctrl.RampOut = stru_Curr_dq_ref.Iq;
		}
#endif

		// 参数初始化
		stru_SingleCurr.LN_States = 0;
		stru_SingleCurr.LN_Flag = 1;

		// 完成切换
		MOTOR_STATE = MC_RUN;
	}

	//=======================================================================/
	// 停机
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Run(void)
{
	//=======================================================================/
	// 过流检测
	volatile int32_t wABSIa, wABSIb, wABSIc;

	//-----------------------------------------------------------------------/
	// 电流的最大值
	wABSIa = ABSFUN(stru_Curr_abc.Ia);
	wABSIb = ABSFUN(stru_Curr_abc.Ib);
	wABSIc = ABSFUN(stru_Curr_abc.Ic);

	//-----------------------------------------------------------------------/
	/****-----相电流峰值保护-----****/
	if ((wABSIa > Fault.OverCurrent_Value) || (wABSIb > Fault.OverCurrent_Value) || (wABSIc > Fault.OverCurrent_Value))
	{
		Fault_OverCurrent = 1;
		MOTOR_FAULT_CODE = FAULT_OVERCURRENT;
		SYSTEM_STATE = SYS_FAULT;
	}

	//-----------------------------------------------------------------------/
	// 缺相检测--电流最大值
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
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// 计算D、Q电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();

	//=======================================================================/
	// 计算角度正余弦
	volatile uint16_t bIndex;

	bIndex = (uint16_t)(stru_FOC.Elec_Angle + 32768) >> 6;

	switch (bIndex & 0x0300)
	{
	/* 0~90度 */
	case 0x0200:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 90~180度 */
	case 0x0300:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	/* 180~270度 */
	case 0x0000:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 270~360度 */
	case 0x0100:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	default:
		break;
	}

//=======================================================================/
// DQ电流给定
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
	// DQ电流环计算
	//-----------------------------------------------------------------------/
	// 临时变量
	int32_t wPIError = 0;
	int32_t wVqLimit = 0;

	//----------------------------------------------------------------------/
	// D轴电流环计算

	// 计算误差
	wPIError = (stru_Curr_dq_ref.Id - stru_Curr_dq.Id);

	// 计算积分
	PID_ID.Integral += (((PID_ID.Ki >> 2) * wPIError) >> 13);

	// 积分饱和
	if (PID_ID.Integral > PID_ID.Upper_Integral)
		PID_ID.Integral = PID_ID.Upper_Integral;
	if (PID_ID.Integral < PID_ID.Lower_Integral)
		PID_ID.Integral = PID_ID.Lower_Integral;

	// 计算输出
	PID_ID.Out = (((PID_ID.Kp >> 3) * wPIError) >> 12) + PID_ID.Integral;

	// 输出限值
	if (PID_ID.Out > PID_ID.Upper_Output)
		PID_ID.Out = PID_ID.Upper_Output;
	if (PID_ID.Out < PID_ID.Lower_Output)
		PID_ID.Out = PID_ID.Lower_Output;

	// d轴电压
	stru_Volt_dq.Ud = PID_ID.Out;

	//----------------------------------------------------------------------/
	// 电压圆限制
	wVqLimit = (stru_Volt_dq.Ud * stru_Volt_dq.Ud) >> 15;
	wVqLimit = (stru_FOC.Vs_Square_Max - wVqLimit) >> 6;

	// 计算Uq电压极限
	PID_IQ.Upper_Output = (int32_t)Q15Sqrt_Table[wVqLimit];
	PID_IQ.Upper_Integral = PID_IQ.Upper_Output;

	//----------------------------------------------------------------------/
	// Q轴电流环计算

	// 计算误差
	wPIError = (stru_Curr_dq_ref.Iq - stru_Curr_dq.Iq);

	// 计算积分
	PID_IQ.Integral += (((PID_IQ.Ki >> 2) * wPIError) >> 13);

	// 积分饱和
	if (PID_IQ.Integral > PID_IQ.Upper_Integral)
		PID_IQ.Integral = PID_IQ.Upper_Integral;
	if (PID_IQ.Integral < PID_IQ.Lower_Integral)
		PID_IQ.Integral = PID_IQ.Lower_Integral;

	// 计算输出
	PID_IQ.Out = (((PID_IQ.Kp >> 3) * wPIError) >> 12) + PID_IQ.Integral;

	// 输出限值
	if (PID_IQ.Out > PID_IQ.Upper_Output)
		PID_IQ.Out = PID_IQ.Upper_Output;
	if (PID_IQ.Out < PID_IQ.Lower_Output)
		PID_IQ.Out = PID_IQ.Lower_Output;

	// q轴电压
	stru_Volt_dq.Uq = PID_IQ.Out;

	//=======================================================================/
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

//=======================================================================/
// 计算空间矢量
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
	// 运行状态检测
	if (Flag.MC_RunStop == 0)
	{
		MOTOR_STATE = MC_STOP;
	}
	//----------------------------------------------------------------------/
	// 掉电特殊处理
	if ((stru_Volt_dq.Uq > 31500) && (Basic.Vbus_Check < (Fault.RunLowerVoltage_Value - 300)))
	{
		MOTOR_STATE = MC_FAULT;
		Time.Motor_PowerDown = 100;
	}
	//----------------------------------------------------------------------/
	// 限母线均值电流判定
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Stop(void)
{
	//-----------------------------------------------------------------------/
	// 计算Alpha、Beta电流
	stru_Curr_alphabeta.Alpha = stru_Curr_abc.Ia;
	stru_Curr_alphabeta.Beta = (9459 * (stru_Curr_abc.Ia + stru_Curr_abc.Ib * 2) >> 14);

	// 计算D、Q电流
	stru_Curr_dq.Id = ((stru_SinCos.Cos * stru_Curr_alphabeta.Alpha) >> 15) + ((stru_SinCos.Sin * stru_Curr_alphabeta.Beta) >> 15);
	stru_Curr_dq.Iq = ((stru_SinCos.Cos * stru_Curr_alphabeta.Beta) >> 15) - ((stru_SinCos.Sin * stru_Curr_alphabeta.Alpha) >> 15);

	//-----------------------------------------------------------------------/
	// 无感位置估算器
	FOC_RotorPosition_Detection();

	//=======================================================================/
	// 计算角度正余弦
	volatile uint16_t bIndex;

	bIndex = (uint16_t)(stru_FOC.Elec_Angle + 32768) >> 6;

	switch (bIndex & 0x0300)
	{
	/* 0~90度 */
	case 0x0200:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 90~180度 */
	case 0x0300:
		stru_SinCos.Sin = Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	/* 180~270度 */
	case 0x0000:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(bIndex)];
		stru_SinCos.Cos = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		break;

	/* 270~360度 */
	case 0x0100:
		stru_SinCos.Sin = -Sin_Cos_Table[(uint8_t)(0xFF - (uint8_t)(bIndex))];
		stru_SinCos.Cos = Sin_Cos_Table[(uint8_t)(bIndex)];
		break;

	default:
		break;
	}

	//----------------------------------------------------------------------/
	// DQ电流为0
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
	// DQ电压为0
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
		// DQ电流环计算
		//-----------------------------------------------------------------------/
		// 临时变量
		int32_t wPIError = 0;
		int32_t wVqLimit = 0;

		//----------------------------------------------------------------------/
		// D轴电流环计算

		// 计算误差
		wPIError = (stru_Curr_dq_ref.Id - stru_Curr_dq.Id);

		// 计算积分
		PID_ID.Integral += (((PID_ID.Ki >> 2) * wPIError) >> 13);

		// 积分饱和
		if (PID_ID.Integral > PID_ID.Upper_Integral)
			PID_ID.Integral = PID_ID.Upper_Integral;
		if (PID_ID.Integral < PID_ID.Lower_Integral)
			PID_ID.Integral = PID_ID.Lower_Integral;

		// 计算输出
		PID_ID.Out = (((PID_ID.Kp >> 3) * wPIError) >> 12) + PID_ID.Integral;

		// 输出限值
		if (PID_ID.Out > PID_ID.Upper_Output)
			PID_ID.Out = PID_ID.Upper_Output;
		if (PID_ID.Out < PID_ID.Lower_Output)
			PID_ID.Out = PID_ID.Lower_Output;

		// d轴电压
		stru_Volt_dq.Ud = PID_ID.Out;

		//----------------------------------------------------------------------/
		// 电压圆限制
		wVqLimit = (stru_Volt_dq.Ud * stru_Volt_dq.Ud) >> 15;
		wVqLimit = (stru_FOC.Vs_Square_Max - wVqLimit) >> 6;

		// 计算Uq电压极限
		PID_IQ.Upper_Output = (int32_t)Q15Sqrt_Table[wVqLimit];
		PID_IQ.Upper_Integral = PID_IQ.Upper_Output;

		//----------------------------------------------------------------------/
		// Q轴电流环计算

		// 计算误差
		wPIError = (stru_Curr_dq_ref.Iq - stru_Curr_dq.Iq);

		// 计算积分
		PID_IQ.Integral += (((PID_IQ.Ki >> 2) * wPIError) >> 13);

		// 积分饱和
		if (PID_IQ.Integral > PID_IQ.Upper_Integral)
			PID_IQ.Integral = PID_IQ.Upper_Integral;
		if (PID_IQ.Integral < PID_IQ.Lower_Integral)
			PID_IQ.Integral = PID_IQ.Lower_Integral;

		// 计算输出
		PID_IQ.Out = (((PID_IQ.Kp >> 3) * wPIError) >> 12) + PID_IQ.Integral;

		// 输出限值
		if (PID_IQ.Out > PID_IQ.Upper_Output)
			PID_IQ.Out = PID_IQ.Upper_Output;
		if (PID_IQ.Out < PID_IQ.Lower_Output)
			PID_IQ.Out = PID_IQ.Lower_Output;

		// q轴电压
		stru_Volt_dq.Uq = PID_IQ.Out;
	}

	//----------------------------------------------------------------------/
	// Park逆变换
	stru_Volt_alphabeta.Alpha = ((stru_SinCos.Cos * stru_Volt_dq.Ud) >> 15) - ((stru_SinCos.Sin * stru_Volt_dq.Uq) >> 15);
	stru_Volt_alphabeta.Beta = ((stru_SinCos.Sin * stru_Volt_dq.Ud) >> 15) + ((stru_SinCos.Cos * stru_Volt_dq.Uq) >> 15);

//----------------------------------------------------------------------/
// 计算空间矢量
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
	// 停机状态检测
	FOC_Stop_Check();
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : FOC_Control_Brake
 * Description    :
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Brake(void)
{
	// 开下管刹车
	Brige_Output_Down();
	if (stru_FOC.Elec_We > 0)
	{
		stru_FOC.Elec_We--;
	}
	else
	{
		stru_FOC.Elec_We = 0;
	}
	// 刹车结束
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
 * Function Call  : EPWM_IRQHandler中断函数调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void FOC_Control_Process(void)
{
	//=======================================================================//
	// 电机控制状态机
	switch (MOTOR_STATE)
	{
	//-------------------------------------------------------------------/
	// 待机状态
	case MC_INIT:

		FOC_Control_Init();

		break;

	//-------------------------------------------------------------------/
	// 充电状态
	case MC_CHARGE:

		FOC_Control_Charge();

		break;

	//-------------------------------------------------------------------/
	// 顺逆风状态
	case MC_WIND:

		FOC_Control_Wind();

		break;

	//-------------------------------------------------------------------/
	// 预定位状态
	case MC_ALIGN:

		FOC_Control_Align();

		break;

	//-------------------------------------------------------------------/
	// 启动状态
	case MC_START:

		FOC_Control_Start();

		break;

	//-------------------------------------------------------------------/
	// 切换状态
	case MC_SW:

		FOC_Control_Switch();

		break;

	//-------------------------------------------------------------------/
	// 运行状态
	case MC_RUN:

		FOC_Control_Run();

		break;

	//-------------------------------------------------------------------/
	// 停机状态
	case MC_STOP:

		FOC_Control_Stop();

		break;

	//-------------------------------------------------------------------/
	// 刹车状态
	case MC_BRAKE:

		FOC_Control_Brake();

		break;

	//-------------------------------------------------------------------/
	// 故障状态
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
