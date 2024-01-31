//==========================================================================//
/*****************************************************************************
 *-----------------------------------------------------------------------------
 * @file    mc_ctrl.c
 * @author  CMS Motor Control Team
 * @version V3.0.0
 * @date    2021年3月
 * @brief
 *---------------------------------------------------------------------------//
 *****************************************************************************/
//==========================================================================//

#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include <stdint.h>
#include "base_define.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
#define Fault_Flag u_Fault_Flag.Byte
#define Fault_ShortCircuit u_Fault_Flag.Bits.Bit0
#define Fault_UnderVoltage u_Fault_Flag.Bits.Bit7
#define Fault_StartFail u_Fault_Flag.Bits.Bit6
#define Fault_OverVoltage u_Fault_Flag.Bits.Bit5
#define Fault_NTC_Temperature u_Fault_Flag.Bits.Bit4
#define Fault_OverCurrent u_Fault_Flag.Bits.Bit3
#define Fault_PhaseLoss u_Fault_Flag.Bits.Bit2
#define Fault_Block u_Fault_Flag.Bits.Bit1

/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/

//---------------------------------------------------------------------------/

typedef enum
{
	SYS_INIT = 0,
	SYS_RESTART = 1,
	SYS_RUN = 2,
	SYS_FAULT = 3,
	SYS_STOP = 4,
	SYS_WAIT = 5
} SystStates;

//---------------------------------------------------------------------------/
typedef enum
{
	MC_INIT = 0,
	MC_CHARGE = 1,
	MC_WIND = 2,
	MC_ALIGN = 3,
	MC_START = 4,
	MC_SW = 5,
	MC_RUN = 6,
	MC_STOP = 7,
	MC_BRAKE = 8,
	MC_FAULT = 9
} MotorStates;

//---------------------------------------------------------------------------/
typedef struct
{
	//----------------------------------------------------------------------/
	float Ub;
	float Ib;
	float Rb;
	float Lb;
	float Fb;
	float Tb;
	float Wb;

	//----------------------------------------------------------------------/
	float Us;
	float Rs;
	float Ls;
	float Ts;
	float Poles;

	//----------------------------------------------------------------------/
	int32_t Rp;
	int32_t Lp;
	int32_t Tp;
	int32_t Fp;

	//----------------------------------------------------------------------/
	int32_t B;
	int32_t C;
	int32_t G;

	//----------------------------------------------------------------------/
	int32_t Fs;
	int32_t Gs;

} struct_Base_Value;

//---------------------------------------------------------------------------/
typedef struct
{
	int32_t RampInc;
	int32_t RampDec;
	int32_t RampTarget;
	int32_t RampOut;
	int32_t RampBack;
	int32_t RampRef;
} struct_SpeedRamp;

//---------------------------------------------------------------------------/
typedef struct
{
	uint8_t Mode_Control;
	uint8_t Mode_Shunt;
	uint8_t Mode_Svpwm;
	uint8_t Mode_Wind;
	uint8_t MotorDir;
	uint8_t Mode_IPD;
	uint8_t Mode_Weeken;
	uint8_t Mode_Start;

} struct_Config;

//---------------------------------------------------------------------------/
typedef struct
{
	int32_t Vbus;
	int32_t Ibus;
	int32_t VCtr;
	int32_t Power;
	int32_t dq_power;
	int32_t Speed_Close;
	int32_t Speed_Min;
	int32_t Speed_Base;
	int32_t Speed_Stop;
	int32_t Speed_Brake;
	int32_t Calb_Speed;
	int32_t Calb_SpeedLimit;
	int32_t Calb_Speed_Min;
	int32_t Calb_Speed_Max;
	int32_t Calb_Vbus;
	int32_t Mech_Speed;
	int32_t FG_Speed;
	int32_t We;
	int32_t Freq_EPWM;
	int32_t Tpwm_us;
	int32_t We_Ke;
	int32_t Speed_Ke;
	float MotorVolt;
	float MotorCurr;
	float Ke;
	uint16_t ERAMP;

	int32_t Vbus_Check; // 过欠压保护使用（未滤波）
	int32_t VTem;
	int32_t Target_Value;
} struct_Basic_Para;

//---------------------------------------------------------------------------/
typedef union
{
	unsigned char Byte;
	struct
	{
		unsigned char Bit0 : 1;
		unsigned char Bit1 : 1;
		unsigned char Bit2 : 1;
		unsigned char Bit3 : 1;
		unsigned char Bit4 : 1;
		unsigned char Bit5 : 1;
		unsigned char Bit6 : 1;
		unsigned char Bit7 : 1;
	} Bits;
} Union_Fault;

//---------------------------------------------------------------------------/
typedef struct
{
	int32_t Speed_Ref;
	int32_t Speed_Ref_Min;
	int32_t Curr_Is_Ref;
	int32_t Curr_Iq_Max;
	int32_t Curr_Iq_Min;
	int32_t Elec_Angle;
	uint32_t MotorRunTime;
	int32_t Is_beta;
	int32_t IPD_Angle;
	uint16_t W_Cycle;
	uint16_t I_Cycle;
	uint16_t WRAMP;
	uint16_t IRAMP;
	uint16_t FRAMP;
	uint16_t SRAMP;
	uint16_t ERAMP;
	int32_t Vs_Max;
	int32_t Vs_Square_Max;
	uint16_t EPWM_Period;
	uint16_t EPWM_SAMPDLY;
	uint16_t TSAM;
	uint16_t TSHIFT;
	uint16_t TADVANCE;

	int32_t Ksilde;
	int32_t Ke;
	int32_t Kslf;
	int32_t Kslf_L;
	int32_t Kslf_H;
	int32_t KBW;
	float Kslf_Bemf;

	int32_t Weakening;
	int32_t Vout;
	int32_t PowerLimit;
	int32_t Elec_We;
	int32_t Offset_Angle;
	int32_t Volt_Uq_Ref;	   // Vq模式
	int32_t CurrentLimit;	   // 2022-06-27
	int32_t CurrentLimit1;	   // 2022-10-18
	int32_t CurrentLimitFlag;  // 2022-10-18
	int32_t CurrentLimitCount; // 2022-10-18
} struct_FOC_Ctrl;

//---------------------------------------------------------------------------/
typedef struct
{
	uint8_t IP_CH;
	int32_t ADVal[10];
	int32_t ARR[10];
	int32_t REF[10];
	int32_t G_Ip;
	int32_t G_Ib;
	int32_t G_Vb;
	int32_t G_Vc;
	int32_t G_Sp;
	int32_t G_Pw;
	int32_t G_Power;
	int32_t G_BG;
	int32_t G_Vbemf;
} struct_ADC_Sample;

//---------------------------------------------------------------------------/
typedef struct
{
	uint8_t MC_RunStop : 1;
	uint8_t Speed_Ramp : 1;
	uint8_t IPD : 1;
	uint8_t Charge : 1;
	uint8_t MotorBack : 1;
	uint8_t MotorFR;
	uint8_t MotorFR_Pre;

} struct_Falg;

//---------------------------------------------------------------------------/
typedef struct
{
	uint8_t Sample_Chenal;
	uint16_t Charge_Count;
	uint16_t Motor_Restart;
	uint16_t FG_Count;
	uint32_t Motor_Stophold;
	uint32_t Motor_StartFail;
	uint32_t Motor_Brake;
	uint32_t Motor_Block;
	uint32_t Motor_PowerDown;
	uint32_t Motor_Runing;
	uint32_t Motor_Limit_Curent;
	uint32_t Motor_StartSuccess;
} struct_Time;

//---------------------------------------------------------------------------/
typedef struct
{
	uint16_t Charge_Count;
	uint32_t Motor_Restart;

	uint16_t TimeBase[5];

} struct_Count;

//---------------------------------------------------------------------------/
typedef struct
{
	uint8_t Block_Cnt;
	uint8_t PhaseLoss_Cnt;
	uint8_t StartFail_Cnt;
	uint8_t Brake_Cnt;
	uint8_t TorgueLoss_Cnt;
	uint8_t Brake;
	uint8_t Braking;
	uint16_t PhaseA_LossCnt;
	uint16_t PhaseB_LossCnt;
	uint16_t PhaseC_LossCnt;
	uint16_t Phaseloss_Value;
	uint32_t StartOverVoltage_Value;
	uint32_t StartLowerVoltage_Value;
	uint32_t RunOverVoltage_Value;
	uint32_t RunLowerVoltage_Value;
	uint32_t OV_Recover_Value;
	uint32_t UV_Recover_Value;
	int32_t OverCurrent_Value;

	int32_t Ia_max;
	int32_t Ib_max;
	int32_t Ic_max;

	float Block_Fault_K1;
	float Block_Fault_K2;

	int32_t Over_Temperature_Value;
	int32_t Over_Temperature_Recover_Value;
	int32_t Lower_Temperature_Value;
	int32_t Lower_Temperature_Recover_Value;

} struct_Fault;

//---------------------------------------------------------------------------/
typedef struct
{
	uint8_t CompleteFlag : 1; // 捕获完成标志
	uint8_t OverFlowFlag : 1; // 计数溢出标志
	uint16_t CaptureCount;	  // 采集顺序
	uint16_t WeakenTime;	  // 溢出电平拉低消抖计时
	uint16_t HeightenTime;	  // 溢出电平拉高消抖计时
	uint32_t CaptureValue[4]; // 捕获的值
	uint32_t OverFlowTime;	  // 溢出时间计数
	uint32_t CaptureTimeL;	  // 捕获的低电平时间
	uint32_t CaptureTimeH;	  // 捕获的高电平时间

	uint32_t Frequency;		// 频率
	uint32_t DutyCycle;		// 占空比
	uint32_t FrequencyOld;	// 频率旧值
	uint32_t DutyCycleOld;	// 占空比旧值
	uint32_t FrequencyTemp; // 频率缓冲
	uint32_t DutyCycleTemp; // 占空比缓冲

	uint32_t FG_SpeedOld;	   // 速度旧值
	uint16_t FG_SpeedMin;	   // FG输出速度最小值
	uint16_t FG_Period;		   // FG输出周期
	uint32_t CaptureTimeCycle; // 捕获的高低电平时间
} struct_Capture;

//---------------------------------------------------------------------------/
typedef struct
{
	uint8_t SleepFlag : 1; //
	uint16_t SleepCount;   //
} struct_Sleep;

//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
typedef struct
{
	uint8_t mode;
	uint16_t rpm_init;
	uint16_t rpm_final;
	int32_t align_current;
	uint32_t align_time;
	uint32_t align_addtime;
	uint32_t align_holdtime;
	uint32_t align_add_k;
	int32_t run_current_init;
	int32_t run_current_final;
	uint32_t run_time;
	int32_t Dkp;
	int32_t Dki;
	int32_t Dout_max;
	int32_t Dout_min;
	int32_t Qkp;
	int32_t Qki;
	int32_t Qout_max;
	int32_t Qout_min;
	int32_t Ekp;
	int32_t Eki;
	int32_t Eout_max;
	int32_t Eout_min;
	int32_t EInt_max;
	int32_t EInt_min;
	int32_t Etheta;
	uint32_t Hold_Time;
} STARTPARA;

//---------------------------------------------------------------------------/
typedef struct
{
	int32_t Dkp;
	int32_t Dki;
	int32_t Dout_max;
	int32_t Dout_min;
	int32_t Qkp;
	int32_t Qki;
	int32_t Qout_max;
	int32_t Qout_min;
	int32_t Wkp;
	int32_t Wki;
	int32_t Wkd;
	int32_t Wout_max;
	int32_t Wout_min;
	int32_t OffsetTheta;
	int32_t Ekp;
	int32_t Eki;
	int32_t SMO_Gs;
	int32_t SMO_Fs;
	int32_t Fc_LPF;
} FOCPARA;

//---------------------------------------------------------------------------/
typedef struct
{

	int32_t Samp_Ibus;
	int32_t Samp_Vbus;
	int32_t Samp_Vctrl;
	int32_t Samp_Vtem;
	int32_t Speed_Val;
	int32_t Power_Val;
	int32_t PhaseK;
} LPFPARA;

//---------------------------------------------------------------------------/
typedef struct
{
	STARTPARA Start;
	FOCPARA FOC;
	LPFPARA Lpf;

} struct_MotCtrl_Para;

//---------------------------------------------------------------------------/

typedef enum
{
	NOERROR = 0,			 // 无故障
	FAULT_SHORTCIRCUIT,		 // 1 硬件过流故障(刹车保护)
	FAULT_BLOCK,			 // 2 堵转故障
	FAULT_PHASELOSS,		 // 3 缺相故障
	FAULT_OVERCURRENT,		 // 4 相电流峰值过流故障
	FAULT_OVERIBUSCURRENT,	 // 5 母线过流故障
	FAULT_OVERTEMPERATURE,	 // 6 NTC过温故障
	FAULT_LOWERTEMPERATURE,	 // 7 NTC低温故障
	FAULT_RUNOVERVOLTAGE,	 // 8 母线电压运行过压故障
	FAULT_STARTFAIL,		 // 9 启动失败故障
	FAULT_RUNUNDERVOLTAGE,	 // 10 母线电压运行欠压故障
	FAULT_SAMPLEREF,		 // 11 采样偏置值异常故障
	FAULT_STARTOVERVOLTAGE,	 // 12 母线电压启动过压故障
	FAULT_STARTUNDERVOLTAGE, // 13 母线电压启动欠压故障
} FaultCode;

typedef struct
{
	int32_t G_Vbus;
	int32_t G_Ibus;
	int32_t G_Power;
	int32_t Vbus;
	int32_t Ibus;
	int32_t Power;
	int32_t Pre_Power;
	int32_t Speed;
} struct_MotorRuning;

typedef struct
{
	int32_t PWMDutyStop;
	int32_t PWMDutyStart;
	int32_t PWMDutyMax;
	int32_t PWMADStop;
	int32_t PWMADStart;
	int32_t PWMADMax;
	float PWMPowerKe;
	float PWMADPowerKe;
} struct_UserSet;

//---------------------------------------------------------------------------/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern volatile Union_Fault u_Fault_Flag;
extern volatile SystStates SYSTEM_STATE;
extern volatile MotorStates MOTOR_STATE;
extern struct_Base_Value stru_base_value;
extern struct_SpeedRamp stru_Speed_Ctrl;
extern struct_ADC_Sample stru_Sample;
extern struct_FOC_Ctrl stru_FOC;
extern struct_Basic_Para Basic;
extern struct_Config Config;
extern struct_Falg Flag;
extern struct_Time Time;
extern struct_Count Count;
extern struct_Fault Fault;
extern struct_MotCtrl_Para Para;
extern struct_Capture Capture;
extern volatile FaultCode MOTOR_FAULT_CODE;
extern struct_MotorRuning Motor_Actual;
extern struct_UserSet UserPowerSet;

extern int32_t wObseFs;
extern int32_t wObseGs;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void MC_BaseValue_Init(void);
void MC_MotorInit_Process(void);
void MC_MotorFault_Check(void);
void MC_MotorSpeed_Calc(void);
void MC_Speed_Control(void);
void MC_Clear_FaultMessage(void);
void System_Control(void);
void SysTick_1ms(void);
void System_Modules_off(void);
// void UartView(int16_t view1, int16_t view2, int16_t view3, int16_t view4);
void User_Motor_On(void);
void User_Motor_Off(void);
void User_Motor_FRControl(uint8_t Input_Val);
void User_MotorSpeed_Set(int32_t Input_Val);
void User_Speed_Control(void);
void User_Speed_Capture(void);
void User_Speed_Out(void);
void User_Sleep_Manage(void);
void CCP_Capture_Init(void);
void CCP_PWM_Init(void);
void CCP_PWM_GPIO(uint8_t bMode, uint8_t bLevle);
void Motor_Actual_Calculate(void);

#endif /* __MOTOR_CONTROL_H__ */
