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

/*****************************************************************************/
/** \file
 *  File Name  : base_define.h
 *  Author     : CMS  Appliction Team
 *  Version    : V2.0
 *  Date       : 01/03/2020
 ** Description: Basic Define
 **
 ******************************************************************************/

#ifndef __BASE_DEFINE_H__
#define __BASE_DEFINE_H__
/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "para_define.h"
#include "user_control.h"
#include <stdint.h>

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/

//---------------------------------------------------------------------------------//
typedef signed char s8;
typedef signed short int s16;
typedef signed int s32;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;

//---------------------------------------------------------------------------------//
#define m_True (1)
#define m_False (0)

//---------------------------------------------------------------------------------//

#define _Q10(A) (int32_t)((A) * 1024)  // Q10 format
#define _Q12(A) (int32_t)((A) * 4096)  // Q12 format
#define _Q15(A) (int32_t)((A) * 32767) // Q15 format
#define _2PI() (3.1415926 * 2)
#define _SQRT_3 (1.732051)
#define _SQRT_2 (1.414214)
#define _Q16_VAL (65535)
#define _Q15_VAL (32768)
#define _Q12_VAL (4096)

//---------------------------------------------------------------------------------//

#define CH_IP_A (0)
#define CH_IP_B (1)
#define CH_IBUS (2)
#define CH_VBUS (3)
#define CH_VCTR (4)
#define CH_TEMP (5)
#define CH_HALL (6)

//---------------------------------------------------------------------------------//

/* filter para */

#define LPF_SAMP_IBUS _Q15(0.1)
#define LPF_SAMP_VBUS _Q15(0.1)
#define LPF_SAMP_VCTRL _Q15(0.05)
#define LPF_SAMP_TEMP _Q15(0.1)
#define LPF_SPEED_VAL _Q15(0.1)
#define LPF_POWER_VAL _Q15(0.1)

//---------------------------------------------------------------------------------//

#define BASE_FREQ (MOTOR_SPEED_BASE * MOTOR_PAIRS / 60) /* 基准频率(HZ) */

#define TPWM_SAMP (1.0 / EPWM_FREQ) /* 采样周期(S) */

#define BASE_KLPF (_2PI() * BASE_FREQ * TPWM_SAMP) /* 2PI*Fbase*Tpwm */

#define _LPF(A) _Q15((BASE_KLPF * A) / (1 + BASE_KLPF * A))

//---------------------------------------------------------------------------------//

/* Q15 calibration */
#define SW_GAIN_IPHASE (float)((HW_ADC_REF * 8) / (HW_ADC_REF - HW_AMP_REF_IP))
#define SW_GAIN_VBUS (float)((HW_ADC_REF * 8) / ((float)HW_VBUS_VOLT * (float)HW_VBUS_SDR * 2))
#define SW_GAIN_IBUS (float)((HW_ADC_REF * 8) / ((HW_IBUS_RES * MOTOR_IBUS_BASE * HW_AMP_GAIN_IBUS * 1.2) + HW_AMP_REF_IBUS))
#define SW_GAIN_SPEED (float)((_Q15_VAL) / (MOTOR_SPEED_BASE * 2.0))
#define SW_GAIN_VCTRL (float)(_Q15_VAL / _Q12_VAL)

#define SW_GAIN_POWER (float)(_Q15_VAL / (MOTOR_POWER_BASE * 1.5))
#define SW_GAIN_VBEMF (float)((HW_ADC_REF * 8) / ((float)HW_VBUS_VOLT * (float)HW_VBEMF_SDR * 2))

//---------------------------------------------------------------------------------//

#define FRACMPY_Q15(x, y) (((x) * (y)) >> 15)

#define FRACMPY_Q12(x, y) (((x) * (y)) >> 12)

#define FRACMPY_Q10(x, y) (((x) * (y)) >> 10)

#define DFILTER(Xn1, Xn0, Filnum) (((Filnum * Xn1) >> 15) + (((32767 - Filnum) * Xn0) >> 15))

#define ABSFUN(Value) ((Value) >= 0 ? (Value) : (-(Value)))

#define MINFUN(varone, vartwo) ((varone) < (vartwo) ? (varone) : (vartwo))

#define MAXFUN(varone, vartwo) ((varone) > (vartwo) ? (varone) : (vartwo))

#define UPDNLMTFUN(Var, Max, Min)                 \
	{                                             \
		(Var) = ((Var) >= (Max)) ? (Max) : (Var); \
		(Var) = ((Var) <= (Min)) ? (Min) : (Var); \
	}

//---------------------------------------------------------------------------------//

#define I_Value(Curr_Val) _Q15((((float)Curr_Val * HW_PHASE_RES * HW_AMP_GAIN_IP) / (HW_ADC_REF - HW_AMP_REF_IP))) // Unit in A

#define Calc_Coeff_Vbus _Q12(((float)HW_VBUS_SDR * SW_GAIN_VBUS) / HW_ADC_REF)					// 82.85
#define Calc_Coeff_Ibus _Q12((HW_IBUS_RES * HW_AMP_GAIN_IBUS * SW_GAIN_IBUS) / HW_ADC_REF)		// 45875.2
#define Calc_Motor_Power(A) (int32_t)(((int32_t)(A) * Calc_Coeff_Vbus * Calc_Coeff_Ibus) >> 15) // 116

#define Calc_Vbus_Value(A) _Q12(((float)A * (float)HW_VBUS_SDR * SW_GAIN_VBUS) / HW_ADC_REF)
//---------------------------------------------------------------------------------//

#define Brige_Output_Down()                   \
	{                                         \
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; \
		EPWM->MASK = 0x00003F2A;              \
		EPWM->LOCK = 0x0;                     \
	}

/* 三相桥臂输出使能与关闭, 注意驱动是否需要输出使能 */
#if (Config_Debug_Mode == Debug_Disable)

#if (Config_Shunt_Mode == Double_Shunt)

#define Brige_Output_Off()                    \
	{                                         \
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; \
		EPWM->CMPDAT[0] = EPWM_HALFPERIOD;    \
		EPWM->CMPDAT[4] = EPWM_HALFPERIOD;    \
		EPWM->CMPDAT[2] = EPWM_HALFPERIOD;    \
		EPWM->CON3 |= 0x00001500;             \
		EPWM->MASK = 0x00003F00;              \
		EPWM->LOCK = 0x0;                     \
	}
#endif

#if (Config_Shunt_Mode == Single_Shunt)

#define Brige_Output_Off()                                           \
	{                                                                \
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;                        \
		EPWM->CMPDAT[0] = EPWM_HALFPERIOD | (EPWM_HALFPERIOD << 16); \
		EPWM->CMPDAT[4] = EPWM_HALFPERIOD | (EPWM_HALFPERIOD << 16); \
		EPWM->CMPDAT[2] = EPWM_HALFPERIOD | (EPWM_HALFPERIOD << 16); \
		EPWM->CON3 |= 0x00001500;                                    \
		EPWM->MASK = 0x00003F00;                                     \
		EPWM->LOCK = 0x0;                                            \
	}
#endif

#define Brige_Output_On()                     \
	{                                         \
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; \
		EPWM->MASK = 0x00000000;              \
		EPWM->LOCK = 0x0;                     \
	}

#endif

/* 调试模式下，开启三相输出，用于硬件波形测试 */
#if (Config_Debug_Mode == Debug_Enable)

#define Brige_Output_On()                     \
	{                                         \
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; \
		EPWM->MASK = 0x00000000;              \
		EPWM->LOCK = 0x0;                     \
	}

#if (Config_Shunt_Mode == Double_Shunt)

#define Brige_Output_Off()                    \
	{                                         \
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY; \
		EPWM->CMPDAT[0] = EPWM_HALFPERIOD;    \
		EPWM->CMPDAT[4] = EPWM_HALFPERIOD;    \
		EPWM->CMPDAT[2] = EPWM_HALFPERIOD;    \
		EPWM->CON3 |= 0x00001500;             \
		EPWM->MASK = 0x00003F00;              \
		EPWM->LOCK = 0x0;                     \
	}

#endif

#if (Config_Shunt_Mode == Single_Shunt)

#define Brige_Output_Off()                                           \
	{                                                                \
		EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;                        \
		EPWM->CMPDAT[0] = EPWM_HALFPERIOD | (EPWM_HALFPERIOD << 16); \
		EPWM->CMPDAT[4] = EPWM_HALFPERIOD | (EPWM_HALFPERIOD << 16); \
		EPWM->CMPDAT[2] = EPWM_HALFPERIOD | (EPWM_HALFPERIOD << 16); \
		EPWM->CON3 |= 0x00001500;                                    \
		EPWM->MASK = 0x00003F00;                                     \
		EPWM->LOCK = 0x0;                                            \
	}
#endif

#endif

//---------------------------------------------------------------------------------//

//============================================================================/
/****************************<< 以下为不常用参数放于此页面 >>*****************/
//============================================================================/

//===========================================================================//
// 电机参数
#define MOTOR_Ke_VPP (3.86)	  // 反电势电压峰峰值 单位：V
#define MOTOR_Ke_Freq (23.15) // 反电势频率 单位：Hz
#define MOTOR_Ke (((float)MOTOR_Ke_VPP / 2 / _SQRT_3) / (_2PI() * MOTOR_Ke_Freq))
#define MOTOR_Ke_K (1.0) // 校正系数
#define MOTOR_WE_BASE (MOTOR_VOLT_BASE * 2 * _SQRT_3 * _2PI() * MOTOR_Ke_Freq / MOTOR_Ke_VPP)

//===========================================================================//
// Brake刹车功能
#define Brake_Disable (0) // 刹车功能关闭
#define Brake_Enable (1)  // 刹车功能开启
#define Brake_Control_Mode (Brake_Disable)

//---------------------------------------------------------------------------//
// Brake刹车转速
#define Motor_Brake_Speed (200) // Brake 刹车转速 单位：RPM
#define Motor_Brake_Time (4000) // Brake 刹车停止时间 单位：ms

//===========================================================================//
// 故障保护重启机制
#define Motor_Restart_Disable (0) // 故障保护需要调速信号重启恢复
#define Motor_Restart_Enable (1)  // 故障保护后自动重启
#define Motor_Restart_Mode (Motor_Restart_Disable)

//===========================================================================//
// VSP速度控制
#define VSP_REF_L (u32)((0.6) * 32768 / 5) // 停止电压点 单位：V
#define VSP_REF_M (u32)((0.8) * 32768 / 5) // 启动电压点 单位：V
#define VSP_REF_H (u32)((4.5) * 32768 / 5) // 最大电压点 单位：V
#define VSP_SPEED_START (350)			   // 启动速度（需高于电机停止速度）
#define VSP_SPEED_MAX (4000)			   // 最大速度

//---------------------------------------------------------------------------//
// PWM速度控制
#define PWM_REF_L (120)		 // 停止占空比（千分位）
#define PWM_SPEED_HOLD (160) // 启动占空比（千分位）
#define PWM_REF_M (200)		 // 保持占空比（千分位）
#define PWM_REF_H (900)		 // 最大占空比（千分位）
#define PWM_SPEED_START (5)	 // 启动速度（需高于电机停止速度）
#define PWM_SPEED_MAX (40)	 // 最大速度

//---------------------------------------------------------------------------//
// CLK速度控制
#define CLK_REF_L (30)	// 停止频率 单位：Hz
#define CLK_REF_M (45)	// 启动频率 单位：Hz
#define CLK_REF_H (500) // 最大频率 单位：Hz

//---------------------------------------------------------------------------//
// 初始位置侦测方式
#define IPD_Disable (0) // IPD关闭
#define IPD_Enable (1)	// IPD使能
#define IPD_Locking (2) // 预定位
#define Config_IPD_Mode (IPD_Disable)

//---------------------------------------------------------------------------//
// 通信
#define Comm_None (0)  // 无通信模式
#define Comm_Scope (1) // 数据监测
#define Comm_UI (2)	   // 用户界面
#define Config_Comm_Mode (Comm_Scope)

//===========================================================================//
// IPD参数
#define IPD_TIME (55)		   // IPD时间 单位：ms
#define IPD_CURRENT _Q15(0.45) // IPD电流

//===========================================================================//
// 开环启动参数
#define START_VF (0)
#define START_IF (1)
#define START_MODE (START_VF) // 默认START_IF, Debug模式START_VF

//---------------------------------------------------------------------------//
#define START_RPM_INIT (0)	  // 启动初始转速 单位：RPM, 0
#define START_RPM_FINAL (200) // 启动限制转速 单位：RPM, 1%~10% MOTOR_SPEED_BASE

//---------------------------------------------------------------------------//
#define START_ALIGN_TADD (100)		   // 预定位加速时间 单位：ms, 0~1000ms
#define START_ALIGN_THOLD (500)		   // 预定位保持时间 单位：ms, 0~5000ms
#define START_RUN_TIME (2000)		   // 启动运行时间 单位：ms, 100~3000ms
#define START_ALIGN_CURRENT _Q15(0.10) // 预定位电流 (%), 0.05~0.5

//---------------------------------------------------------------------------//
#define START_ALIGN_TIME (START_ALIGN_TADD + START_ALIGN_THOLD)
#define START_ALIGN_KADD (START_ALIGN_CURRENT / START_ALIGN_TADD)

//---------------------------------------------------------------------------//
#define FOC_RUN_SPEED_MIN (1000)  // 电机运行最低转速（最小挡位转速）
#define FOC_RUN_SPEED_MAX (10000) // 电机运行最高转速（最大挡位转速）

/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/

#endif /* __BASE_DEFINE_H__ */
