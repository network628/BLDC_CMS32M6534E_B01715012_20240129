//==========================================================================//
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * @file    motro_control.c
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

volatile SystStates SYSTEM_STATE = SYS_INIT; /* 系统流程状态机 */

volatile MotorStates MOTOR_STATE = MC_INIT; /* 电机控制流程状态机 */

volatile Union_Fault u_Fault_Flag = {0}; /* 电机故障指示标志 */

volatile FaultCode MOTOR_FAULT_CODE = NOERROR; /* 电机故障指示标志 */

struct_Basic_Para Basic = {0}; /* 基本参数结构体 */

struct_Falg Flag = {0}; /* 标志位结构体 */

struct_Time Time = {0}; /* 时间计数结构体 */

struct_Count Count = {0}; /* 数学计数结构体 */

struct_Fault Fault = {0}; /* 故障参数结构体 */

struct_MotCtrl_Para Para = {0}; /* 电机控制参数结构体 */

struct_Config Config = {0}; /* 系统配置选项结构体 */

struct_Base_Value stru_base_value = {0}; /* 电机基值参数结构体 */

struct_ADC_Sample stru_Sample = {0}; /* 采样参数结构体 */

struct_FOC_Ctrl stru_FOC = {0}; /* FOC控制参数结构体 */

struct_SpeedRamp stru_Speed_Ctrl = {0}; /* 速度控制结构体 */

struct_MotorRuning Motor_Actual = {0}; /* 电机实际反馈参数 */

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
// 电机运行使能
void User_Motor_On(void)
{
	Flag.MC_RunStop = 1;
}

//===========================================================================/
// 电机停止
void User_Motor_Off(void)
{
	Flag.MC_RunStop = 0;
}

//===========================================================================/
// 电机方向控制
void User_Motor_FRControl(uint8_t Input_Val)
{
	if (Input_Val == FR_CW)
		Config.MotorDir = FR_CW;
	else
		Config.MotorDir = FR_CCW;
}

//===========================================================================/
// 调速控制
void User_MotorSpeed_Set(int32_t Input_Val)
{
//-----------------------------------------------------------------------/
// 电流环控制，给定值/32767 = 给定电流/最大电流--相电流峰值
#if (Config_Contorl_Mode == Current_Loop)

	if (Input_Val > stru_FOC.Curr_Iq_Max)
		Input_Val = stru_FOC.Curr_Iq_Max;
	stru_FOC.Speed_Ref = Input_Val;

#endif

//-----------------------------------------------------------------------/
// 速度环控制
#if (Config_Contorl_Mode == Speed_Loop)

#if (LIMIT_SPEED_ENABLE)
	if (Input_Val > LIMIT_SPEED_VALUE)
		Input_Val = (int32_t)LIMIT_SPEED_VALUE;
#endif

	stru_FOC.Speed_Ref = FRACMPY_Q10(Input_Val, stru_Sample.G_Sp);

#endif

//-----------------------------------------------------------------------/
// 功率环控制
#if (Config_Contorl_Mode == Power_Loop)

// 直接给定功率值
#if 1
	if (Input_Val > (int32_t)MOTOR_POWER_BASE)
		Input_Val = (int32_t)MOTOR_POWER_BASE;

	stru_FOC.Speed_Ref = (int32_t)(Input_Val * stru_Sample.G_Pw) >> 6;

// 按照调速信号比例给定
#else

	stru_FOC.Speed_Ref = Input_Val;

#endif

#endif
	//-----------------------------------------------------------------------/
}

//===========================================================================/
// 关闭所有功能模块
void System_Modules_off(void)
{
	Brige_Output_Off();
	User_Motor_Off();
}

//===========================================================================/
// EPWM故障刹车保护重启
void EPWM_ResetFaultBrake(void)
{
	EPWM_ClearBrakeIntFlag(); /// 清除刹车中断标志位
	EPWM_ClearBrake();
	EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
}

//===========================================================================//

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : System_Control
 * Description    : 系统状态机流程控制
 * Function Call  : 主循环调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void System_Control(void)
{
	switch (SYSTEM_STATE)
	{
	//===================================================================//
	// 系统初始化
	case SYS_INIT:
	{
		DelayTime_ms(20);
		// 电机上电获取ADC偏置值(相电流偏置值)
		GET_ADC_REF_VAL();
		// FOC控制参数初始化
		FOC_ControlPara_Init();
		// 基值参数初始化
		FOC_BaseParameter_Init();
		// 故障清零，防止上电误动作
		EPWM_ResetFaultBrake();

#if (Motor_Debug_Mode == Motor_Debug_Online)
		//		DBG->DBGSTOPCR |= (0<<24);
#else
		// 禁用SWO调试接口功能 (复用烧录口线需要打开)
		//		DBG->DBGSTOPCR |= (1<<24);
#endif
		SYSTEM_STATE = SYS_RUN;

		break;
	}
	//===================================================================//
	// 系统正常工作状态
	case SYS_RUN:
	{
		// 马达调速控制
		if (Time.Motor_Restart == 0)
		{
			// 故障保护时马达关机
			if (MOTOR_FAULT_CODE || Fault_Flag)
			{
				User_Motor_Off();
			}
			// 进入调速控制
			else
			{
				User_Speed_Control();
			}
		}
		break;
	}
	//===================================================================//
	// 马达重启状态
	case SYS_RESTART:
	{
		// 马达关机
		User_Motor_Off();
		// 重启间隔时间
		Time.Motor_Restart = (uint32_t)((float)FAULT_RESTART_DELAY_TIME * 1000);
		// 启动失败允许重启次数
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
		// 马达设置为待机状态
		MOTOR_STATE = MC_INIT;

		break;
	}
	//===================================================================//
	// 故障停机态
	case SYS_FAULT:
	{
		// 马达关机
		User_Motor_Off();
		Time.Motor_Restart = 0;
		SYSTEM_STATE = SYS_WAIT;

		// 马达设置为待机状态
		MOTOR_STATE = MC_INIT;

		break;
	}
	//===================================================================//
	// 等待故障保护恢复
	case SYS_WAIT:
	{
		// 系统关机
		System_Modules_off();
		// 允许调速信号复位重启
		if ((Capture.Frequency == 0) && (Capture.DutyCycle == 0))
		{
			// 清除故障信息
			//				EPWM_ResetFaultBrake();
			//				MC_Clear_FaultMessage();
			//				Count.Motor_Restart = 0;
			//				//系统恢复正常状态
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
 * Description    : 缺相保护功能
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
			// A相电流判断
			if (((Fault.Ia_max > Fault.Ib_max * 2) || (Fault.Ia_max > Fault.Ic_max * 2)) && (Fault.Ia_max > Fault.Phaseloss_Value))
			{
				Fault.PhaseA_LossCnt++;
			}
			else
			{
				Fault.PhaseA_LossCnt = 0;
			}
			//-------------------------------------------------------------------------------------/
			// B相电流判断
			if (((Fault.Ib_max > Fault.Ia_max * 2) || (Fault.Ib_max > Fault.Ic_max * 2)) && (Fault.Ib_max > Fault.Phaseloss_Value))
			{
				Fault.PhaseB_LossCnt++;
			}
			else
			{
				Fault.PhaseB_LossCnt = 0;
			}
			//-------------------------------------------------------------------------------------/
			// C相电流判断
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
 * Description    : 堵转保护功能
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_MotorBlock_Check(void)
{
	static uint16_t hBlockCount = 0;

	//-------------------------------------------------------------------------/
	// 转速超过设定的转速范围认为堵转保护
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
 * Description    : 电压保护功能
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
			// 启动过压保护
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
			// 启动欠压保护
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
			// 过欠压保护恢复
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
			// 运行过压保护
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
			// 运行欠压保护
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
 ** \note	母线过流检测
 *****************************************************************************/
void MC_IBUS_Check(void)
{
	static uint16_t hOverIbusCount = 0;

	if (MOTOR_STATE == MC_RUN || MOTOR_STATE == MC_SW)
	{
		if (Motor_Actual.Ibus > FAULT_OVER_IBUS * 100)
		{
			if (++hOverIbusCount > FAULT_OVER_IBUS_TIME) // 大于500ms保护
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
 ** \note	NTC温度检测
 *****************************************************************************/
void MC_Temperature_Check(void)
{
	static uint16_t hOverTemperatureCount = 0, hLowerTemperatureCount = 0, hRechOverTemperatureCount = 0;
	// 低温保护
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
	// 过温保护
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
		// NTC保护恢复
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
 * Description    : 电机故障保护检测
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_MotorFault_Check(void)
{
//------------------------------------------------------------------------/
// 缺相检测
#if (FAULT_PHASELOSS_ENABLE)
	MC_PhaseLoss_Check();
#endif

//------------------------------------------------------------------------/
// 堵转检测
#if (FAULT_BLOCK_ENABLE)
	MC_MotorBlock_Check();
#endif

//------------------------------------------------------------------------/
// 电压检测
#if (FAULT_VOLTAGE_ENABLE)
	MC_Voltage_Check();
#endif

//------------------------------------------------------------------------/
// 母线均值电流检测
#if (FAULT_AVERAGE_CURRENT_ENABLE)
	MC_IBUS_Check();
#endif

//------------------------------------------------------------------------/
// 模块NTC温度检测
#if (FAULT_TEMPERATURE_ENABLE)
	MC_Temperature_Check();
#endif
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_MotorSpeed_Calc
 * Description    : 电机转速计算
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_MotorSpeed_Calc(void)
{
	// 计算电机转速
	Basic.Mech_Speed = (stru_FOC.Elec_We * Basic.Speed_Base) / (Basic.We * Basic.ERAMP);

	// FG输出转速
	Basic.FG_Speed = Basic.Mech_Speed + SPEED_OFFSET;

	// 转速标定值计算，用于速度环
	Basic.Calb_Speed = FRACMPY_Q10(Basic.Mech_Speed, stru_Sample.G_Sp);

	// 计算反电动势速度
	stru_bemf.Speed = (stru_bemf.DeltaTheta * Basic.Speed_Base) / Basic.We;
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : MC_Clear_FaultMessage
 * Description    : 清除故障信息
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
 * Description    : ADC0通道采样
 * Function Call  :
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void ADC_Sample(void)
{
	//----------------------------------------------------------------------/
	// 输入电压采样
	stru_Sample.ADVal[CH_VBUS] = (uint32_t)ADC->DATA[ADC_DATA_VBUS];
	// 输入电流采样
	stru_Sample.ADVal[CH_IBUS] = (uint32_t)ADC->DATA[ADC_DATA_IBUS];
	// NTC温度采样
	stru_Sample.ADVal[CH_TEMP] = (uint32_t)ADC->DATA[ADC_DATA_TEMP];
	// 调速电压采样
	stru_Sample.ADVal[CH_VCTR] = (uint32_t)ADC->DATA[ADC_DATA_CTRL];

//----------------------------------------------------------------------/
// Bandgap 校正采样值
#if (CONFIG_BANDGAP_MODE == BANDGAP_ENABLE)
	int32_t Aux;

	// 计算调速电压
	Aux = FRACMPY_Q10((stru_Sample.ADVal[CH_VCTR] - stru_Sample.REF[CH_VCTR]), stru_Sample.G_Vc);
	stru_Sample.ARR[CH_VCTR] = FRACMPY_Q12(Aux, stru_Sample.G_BG);
	// 计算母线电压
	Aux = FRACMPY_Q10((stru_Sample.ADVal[CH_VBUS] - stru_Sample.REF[CH_VBUS]), stru_Sample.G_Vb);
	stru_Sample.ARR[CH_VBUS] = FRACMPY_Q12(Aux, stru_Sample.G_BG);
	// 计算母线电流
	Aux = FRACMPY_Q10((stru_Sample.ADVal[CH_IBUS] - stru_Sample.REF[CH_IBUS]), stru_Sample.G_Ib);
	stru_Sample.ARR[CH_IBUS] = FRACMPY_Q12(Aux, stru_Sample.G_BG);

	// 计算NTC温度
	Aux = FRACMPY_Q10((stru_Sample.ADVal[CH_TEMP] - stru_Sample.REF[CH_TEMP]), stru_Sample.G_Vc);
	stru_Sample.ARR[CH_TEMP] = FRACMPY_Q12(Aux, stru_Sample.G_BG);

#else

	// 计算调速电压
	stru_Sample.ARR[CH_VCTR] = FRACMPY_Q10((stru_Sample.ADVal[CH_VCTR] - stru_Sample.REF[CH_VCTR]), stru_Sample.G_Vc);
	// 计算母线电压
	stru_Sample.ARR[CH_VBUS] = FRACMPY_Q10((stru_Sample.ADVal[CH_VBUS] - stru_Sample.REF[CH_VBUS]), stru_Sample.G_Vb);
	// 计算母线电流
	stru_Sample.ARR[CH_IBUS] = FRACMPY_Q10((stru_Sample.ADVal[CH_IBUS] - stru_Sample.REF[CH_IBUS]), stru_Sample.G_Ib);
	// 计算NTC温度
	stru_Sample.ARR[CH_TEMP] = FRACMPY_Q10((stru_Sample.ADVal[CH_TEMP] - stru_Sample.REF[CH_TEMP]), stru_Sample.G_Vc);

#endif

	// 母线电压数字滤波
	Basic.Vbus = DFILTER(stru_Sample.ARR[CH_VBUS], Basic.Vbus, Para.Lpf.Samp_Vbus);
	// 调速信号数字滤波
	Basic.VCtr = DFILTER(stru_Sample.ARR[CH_VCTR], Basic.VCtr, Para.Lpf.Samp_Vctrl);
	// 母线电流数字滤波
	Basic.Ibus = DFILTER(stru_Sample.ARR[CH_IBUS], Basic.Ibus, Para.Lpf.Samp_Ibus);
	;
	if (Basic.Ibus < 0)
	{
		Basic.Ibus = 0;
	}
	// NTC信号数字滤波
	Basic.VTem = DFILTER(stru_Sample.ARR[CH_TEMP], Basic.VTem, Para.Lpf.Samp_Vctrl);

	// 母线电压(实时值)用作保护
	Basic.Vbus_Check = stru_Sample.ARR[CH_VBUS];
	//----------------------------------------------------------------------/
}

/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : SysTick_1ms
 * Description    : 1ms定时任务
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
	// ADC通道采样
	ADC_Sample();

	//=======================================================================//
	// 10ms 任务处理
	if (++Count.TimeBase[0] >= 10) // 10ms
	{
		Count.TimeBase[0] = 0;
		//-----------------------------------------------------------------------/
		// 用户进程
		FOC_User_Control();
		//-----------------------------------------------------------------------/
//-------------------------------------------------------------------//
// 调速捕获--PWM
#if (Config_CCP_Capture == CCP_Capture_Enable)
		User_Speed_Capture();
#endif

		//-------------------------------------------------------------------//
		// 速度反馈--FG

#if (Config_CCP_PWM == CCP_PWM_Enable)
		User_Speed_Out();
#endif

		//-------------------------------------------------------------------//
	}

	//=======================================================================//
	// 100ms 处理
	if (++Count.TimeBase[1] >= 100) // 100ms
	{
		Count.TimeBase[1] = 0;

// 睡眠处理
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
 * Description    : 速度环 / 功率环 功能
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
	// 电机实际电压电流功率计算
	Motor_Actual_Calculate();

	//=========================================================================//
	// 硬件功率计算
	int32_t wTempPower = 0;

	wTempPower = FRACMPY_Q15(Basic.Vbus, Basic.Ibus);
	Basic.Power = wTempPower; // DFILTER( wTempPower, Basic.Power, Para.Lpf.Power_Val );

	//=========================================================================//
	if (MOTOR_STATE == MC_RUN)
	{
		//=======================================================================//
		Config.Mode_Svpwm = Config_SVPWM_Mode;

		//=========================================================================//

		// 调速控制模式，恒转矩/恒转速/恒功率
		if (++stru_FOC.W_Cycle >= stru_FOC.WRAMP)
		{
			stru_FOC.W_Cycle = 0;
			// 调速信号爬坡处理
			MC_Ramp_Control(&stru_Speed_Ctrl, stru_FOC.Speed_Ref);
//=======================================================================//
// 力矩环控制
#if (Config_Contorl_Mode == Current_Loop)
			{
// 限功率使能,若限功率和限转速同时使能，则执行限功率控制
#if (LIMIT_POWER_ENABLE)
				// 非限制状态下判断当前功率是否超过限定功率
				if (!bSpeedLimitFlag)
				{
					if (Basic.Power > stru_FOC.PowerLimit)
					{
						// 防止频繁进入与退出限速模式
						if (++hSpeedLimitCount > (500 / stru_FOC.WRAMP))
						{
							bSpeedLimitFlag = 1;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // 保存当前档位

							PID_WL.Kp = 16384; // 功率环KP = _Q15(0.5), 避开浮点运算
							PID_WL.Ki = 1638;  // 功率环KI = _Q15(0.05), 避开浮点运算
							PID_WL.KiSum = stru_Curr_dq_ref.Iq << 14;
							PID_WL.Integral = stru_Curr_dq_ref.Iq;
						}
					}
				}
				// 限制状态下判断是否需要取消限制
				else
				{
					// 档位下降时取消限制
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 100))
					{
						bSpeedLimitFlag = 0;
					}

					if (!bSpeedLimitFlag)
					{
						PID_WL.Kp = Para.FOC.Wkp; // 恢复转速环PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}

// 限速使能
#elif (LIMIT_SPEED_ENABLE)
				// 非限速状态下判断当前转速是否超过限定转速
				if (!bSpeedLimitFlag)
				{
					if (Basic.Mech_Speed > LIMIT_SPEED_VALUE)
					{
						// 防止频繁进入与退出限速模式
						if (++hSpeedLimitCount > (500 / stru_FOC.WRAMP))
						{
							bSpeedLimitFlag = 1;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // 保存当前档位

							PID_WL.Kp = 16384; // 速度环KP = _Q15(0.5), 避开浮点运算
							PID_WL.Ki = 1638;  // 速度环KI = _Q15(0.05), 避开浮点运算
							PID_WL.KiSum = stru_Curr_dq_ref.Iq << 14;
							PID_WL.Integral = stru_Curr_dq_ref.Iq;
						}
					}
				}
				// 限速状态下判断是否需要取消限速
				else
				{
					// 档位下降时取消限速
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 100))
					{
						bSpeedLimitFlag = 0;
					}
					if (!bSpeedLimitFlag)
					{
						PID_WL.Kp = Para.FOC.Wkp; // 恢复功率环PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}

#endif // end limit power/speed enable

				if (bSpeedLimitFlag)
				{
// 限功率使能
#if (LIMIT_POWER_ENABLE)
					// 恒功率时的PI周期
					stru_FOC.WRAMP = TIME_POWER_LOOP;
					// 限功率时恒功率控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.PowerLimit, Basic.Power);
// 限速使能
#elif (LIMIT_SPEED_ENABLE)
					// 恒转速时的PI周期
					stru_FOC.WRAMP = TIME_SPEED_LOOP;
					// 限速时恒转速控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, Basic.Calb_SpeedLimit, Basic.Calb_Speed);
#endif
				}
				else
				{
					// 恒力矩时的PI周期
					stru_FOC.WRAMP = 1;
					// 开机状态恒力矩控制
					stru_FOC.Curr_Is_Ref = stru_Speed_Ctrl.RampOut;
				}
			}
//=======================================================================//
// 转速环控制
#elif (Config_Contorl_Mode == Speed_Loop)
			{
// 限功率使能
#if (LIMIT_POWER_ENABLE)
				// 非限制状态下判断当前功率是否超过限定功率
				if (!bSpeedLimitFlag)
				{
					if (Basic.Power > stru_FOC.PowerLimit)
					{
						// 防止频繁进入与退出限速模式
						if (++hSpeedLimitCount > (500 / stru_FOC.WRAMP))
						{
							bSpeedLimitFlag = 1;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // 保存当前档位

							PID_WL.Kp = 16384; // 功率环KP = _Q15(0.5), 避开浮点运算
							PID_WL.Ki = 1638;  // 功率环KI = _Q15(0.05), 避开浮点运算
						}
					}
				}
				// 限制状态下判断是否需要取消限制
				else
				{
					// 档位下降时取消限制
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 100))
					{
						bSpeedLimitFlag = 0;
					}
					// 当前转速值比限速前档位转速大时退出功率限制
					//					if( Basic.Calb_Speed > hSpeedTargetBeforeLimit )
					//					{
					//						bSpeedLimitFlag = 0;
					//					}
					if (!bSpeedLimitFlag)
					{
						PID_WL.Kp = Para.FOC.Wkp; // 恢复转速环PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
#endif // end limit power enable

				if (bSpeedLimitFlag)
				{
					// 恒功率时的PI周期
					stru_FOC.WRAMP = TIME_POWER_LOOP;
					// 限功率时恒功率控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.PowerLimit, Basic.Power);
				}
				else
				{
					// 恒转速时的PI周期
					stru_FOC.WRAMP = TIME_SPEED_LOOP;
					// 开机状态恒转速控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_Speed_Ctrl.RampOut, Basic.Calb_Speed);
				}
			}
//===========================================================================//
// 功率环控制
#elif (Config_Contorl_Mode == Power_Loop)
			{
// 限速使能
#if (LIMIT_SPEED_ENABLE && LIMIT_CURRENT_ENABLE)

				if (!bSpeedLimitFlag)
				{
					// 非限速状态下判断当前转速是否超过限定转速
					if (Basic.Mech_Speed > (uint32_t)LIMIT_SPEED_VALUE)
					{
						// 防止频繁进入与退出限速模式
						if ((++hSpeedLimitCount > (500 / stru_FOC.WRAMP)) && (bSpeedCurrentLimitFlag == 0))
						{
							bSpeedLimitFlag = 1;
							bSpeedCurrentLimitFlag = 0;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // 保存当前档位
							hPowerTargetBeforeLimit = Motor_Actual.Power;	   // 保存当前功率值
						}
					}
// 非限母线电流状态下判断当前母线电流是否超过限定母线电流
#if 0
							else if( Time.Motor_Limit_Curent > 300 )
							{
								if( Motor_Actual.Ibus > stru_FOC.CurrentLimit )
								{
									// 防止频繁进入与退出限母线电流模式
									if( ++hSpeedLimitCount > (100 / stru_FOC.WRAMP) )
									{
										bSpeedLimitFlag = 1;
										bSpeedCurrentLimitFlag = 1;
										hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; //保存当前档位
									}
								}
							}
#else
					// FOC中断判定母线均值限流标志（快速响应）
					else if (stru_FOC.CurrentLimitFlag)
					{
						bSpeedLimitFlag = 1;
						bSpeedCurrentLimitFlag = 1;
						hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // 保存当前档位
					}
#endif
				}
				else
				{
					// 限速状态下判断是否需要取消限速
					if (!bSpeedCurrentLimitFlag)
					{
						// 档位下降时取消限速
						if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 200))
						{
							bSpeedLimitFlag = 0;
						}
						// 当前功率值比目标功率小时退出限速
						if (Motor_Actual.Power > (hPowerTargetBeforeLimit + 15))
						{
							bSpeedLimitFlag = 0;
						}

						if (!bSpeedLimitFlag)
						{
							stru_Speed_Ctrl.RampInc = SPEED_RAMP_INC;
							stru_Speed_Ctrl.RampDec = SPEED_RAMP_DEC;
							PID_WL.Kp = Para.FOC.Wkp; // 恢复功率环PI
							PID_WL.Ki = Para.FOC.Wki;
						}
					}
					// 限母线电流状态下判断是否需要取消限母线电流
					else
					{
						// 档位下降时取消限母线电流
						if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 500))
						{
							bSpeedLimitFlag = 0;
						}
						// 计算母线电流下降时取消限母线电流
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
							PID_WL.Kp = Para.FOC.Wkp; // 恢复功率环PI
							PID_WL.Ki = Para.FOC.Wki;
						}
					}
				}
#elif (LIMIT_SPEED_ENABLE)
				// 非限速状态下判断当前转速是否超过限定转速
				if (!bSpeedLimitFlag)
				{
					if (Basic.Mech_Speed > (uint32_t)LIMIT_SPEED_VALUE)
					{
						// 防止频繁进入与退出限速模式
						if (++hSpeedLimitCount > (500 / stru_FOC.WRAMP))
						{
							bSpeedLimitFlag = 1;
							hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // 保存当前档位
							hPowerTargetBeforeLimit = Motor_Actual.Power;	   // 保存当前功率值
						}
					}
				}
				// 限速状态下判断是否需要取消限速
				else
				{
					// 档位下降时取消限速
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 200))
					{
						bSpeedLimitFlag = 0;
					}
					// 当前功率值比目标功率小时退出限速
					if (Motor_Actual.Power > (hPowerTargetBeforeLimit + 15))
					{
						bSpeedLimitFlag = 0;
					}

					if (!bSpeedLimitFlag)
					{
						stru_Speed_Ctrl.RampInc = SPEED_RAMP_INC;
						stru_Speed_Ctrl.RampDec = SPEED_RAMP_DEC;
						PID_WL.Kp = Para.FOC.Wkp; // 恢复功率环PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
#elif (LIMIT_CURRENT_ENABLE)
				// 非限母线电流状态下判断当前母线电流是否超过限定母线电流

				if (!bSpeedLimitFlag)
				{
#if 0
							if( Time.Motor_Limit_Curent > 50 )
							{
								if( Motor_Actual.Ibus > stru_FOC.CurrentLimit )
								{
									stru_Speed_Ctrl.RampInc = 20; //SPEED_RAMP_INC;
									stru_Speed_Ctrl.RampDec = 20; //SPEED_RAMP_DEC;
									// 防止频繁进入与退出限母线电流模式
									if( ++hSpeedLimitCount > (50 / stru_FOC.WRAMP) )
									{
										bSpeedLimitFlag = 1;
										hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; //保存当前档位
									}
								}
							}
#else
					// FOC中断判定母线均值限流标志（快速响应）
					if (stru_FOC.CurrentLimitFlag)
					{
						bSpeedLimitFlag = 1;
						hSpeedTargetBeforeLimit = stru_Speed_Ctrl.RampOut; // 保存当前档位
					}
#endif
				}
				// 限母线电流状态下判断是否需要取消限母线电流
				else
				{
					// 档位下降时取消限母线电流
					if (stru_Speed_Ctrl.RampOut < (hSpeedTargetBeforeLimit - 500))
					{
						bSpeedLimitFlag = 0;
					}

					// 计算母线电流下降时取消限母线电流
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
						PID_WL.Kp = Para.FOC.Wkp; // 恢复功率环PI
						PID_WL.Ki = Para.FOC.Wki;
					}
				}
#endif // end limit speed enable

				if (bSpeedLimitFlag)
				{
#if (LIMIT_SPEED_ENABLE && LIMIT_CURRENT_ENABLE)

					if (!bSpeedCurrentLimitFlag)
					{
						// 恒转速时的PI周期
						stru_FOC.WRAMP = TIME_SPEED_LOOP;

						stru_Speed_Ctrl.RampInc = 50;
						stru_Speed_Ctrl.RampDec = 50;
						PID_WL.Kp = 6553; // 速度环KP , 避开浮点运算
						PID_WL.Ki = 200;  // 速度环KI , 避开浮点运算

						// 限速时恒转速控制
						stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, Basic.Calb_SpeedLimit, Basic.Calb_Speed);
					}
					else
					{
						// 恒母线电流时的PI周期
						stru_FOC.WRAMP = 1;

						stru_Speed_Ctrl.RampInc = 10; // 150
						stru_Speed_Ctrl.RampDec = 10; // 150
						PID_WL.Kp = 16384;			  // 母线电流环KP , 避开浮点运算  16384  32768
						PID_WL.Ki = 500;			  // 母线电流环KI , 避开浮点运算   127     3276

						// 限电流时恒母线电流控制
						stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.CurrentLimit1, Basic.Ibus);
					}
#elif (LIMIT_SPEED_ENABLE)
					// 恒转速时的PI周期
					stru_FOC.WRAMP = TIME_SPEED_LOOP;

					stru_Speed_Ctrl.RampInc = 50;
					stru_Speed_Ctrl.RampDec = 50;
					PID_WL.Kp = 6553; // 速度环KP , 避开浮点运算
					PID_WL.Ki = 200;  // 速度环KI , 避开浮点运算

					// 限速时恒转速控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, Basic.Calb_SpeedLimit, Basic.Calb_Speed);
#elif (LIMIT_CURRENT_ENABLE)
					// 恒母线电流时的PI周期
					stru_FOC.WRAMP = 1;

					stru_Speed_Ctrl.RampInc = 10; // 150
					stru_Speed_Ctrl.RampDec = 10; // 150
					PID_WL.Kp = 16384;			  // 母线电流环KP, 避开浮点运算  16384  32768
					PID_WL.Ki = 500;			  // 母线电流环KI, 避开浮点运算   127     3276

					// 限电流时恒母线电流控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_FOC.CurrentLimit1, Basic.Ibus);
#endif
				}
				else
				{
					// 恒功率时的PI周期
					stru_FOC.WRAMP = TIME_POWER_LOOP;
					// 开机状态恒功率控制
					stru_FOC.Curr_Is_Ref = PID_CALC(&PID_WL, stru_Speed_Ctrl.RampOut, Basic.Power); // PID_CALC  PID_SpeedLoop
				}
			}
#endif // end torgue and speed or power loop
	   //=======================================================================//
	   // 限制IQ电流输出
			if (stru_FOC.Curr_Is_Ref > stru_FOC.Curr_Iq_Max)
				stru_FOC.Curr_Is_Ref = stru_FOC.Curr_Iq_Max;
			if (stru_FOC.Curr_Is_Ref < stru_FOC.Curr_Iq_Min)
				stru_FOC.Curr_Is_Ref = stru_FOC.Curr_Iq_Min;
		}
		//===========================================================================//
	}
	//===============================================================================//
	// 电机工作在非运行态
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
 * Function Call  : 1ms进程调用
 * Input Paragram :
 * Return Value   :
 *-----------------------------------------------------------------------------
 ******************************************************************************/
void MC_MotorInit_Process(void)
{
	// 初始化
	if (MOTOR_STATE == MC_INIT)
	{
		FOC_MotorSpeed_Filter(stru_FOC.Elec_We);
	}
} 
 
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * Function Name  : UartView
 * Description    : 串口监测数据--调试用
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
 ** \note	电机实际参数计算(电压，电流，功率，转速)(电压、电流=实际值*100) //2021-08-02
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
