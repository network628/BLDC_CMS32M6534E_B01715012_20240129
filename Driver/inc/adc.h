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
/** \file adc.h
**
** History:
**
*****************************************************************************/
#ifndef __ADC_H_
#define __ADC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "common.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 **ADC 转换模式
-----------------------------------------------------------------------------*/
#define ADC_MODE_HIGH (0x00UL)						  /*高速转换模式*/
#define ADC_MODE_LOW (0x03UL << ADC_CON_ADMODE10_Pos) /*低电流模式*/

#define ADC_CONVERT_SINGLE (0x00UL)							 /*单次模式*/
#define ADC_CONVERT_CONTINUOUS (0x01UL << ADC_CON_ADCMS_Pos) /*连续模式*/

#define ADC_SWITCH_HARDWARE (0x00UL)						/*硬件自动切换模式*/
#define ADC_SWITCH_SOFTWARE (0x1UL << ADC_CON_ADCSWCHE_Pos) /*软件切换模式*/

/*----------------------------------------------------------------------------
 **ADC 时钟分频
-----------------------------------------------------------------------------*/
#define ADC_CLK_DIV_1 (0x00UL)	 /*ADC 时钟分频为 1*/
#define ADC_CLK_DIV_2 (0x01UL)	 /*ADC 时钟分频为 2*/
#define ADC_CLK_DIV_4 (0x02UL)	 /*ADC 时钟分频为 4*/
#define ADC_CLK_DIV_8 (0x03UL)	 /*ADC 时钟分频为 8*/
#define ADC_CLK_DIV_16 (0x04UL)	 /*ADC 时钟分频为 16*/
#define ADC_CLK_DIV_32 (0x05UL)	 /*ADC 时钟分频为 32*/
#define ADC_CLK_DIV_64 (0x06UL)	 /*ADC 时钟分频为 64*/
#define ADC_CLK_DIV_128 (0x07UL) /*ADC 时钟分频为 128*/

/*----------------------------------------------------------------------------
 **ADC 充电、放电时间
-----------------------------------------------------------------------------*/
#define ADC_CG_2 (0x02UL)  /* 2*ADCCLK*/
#define ADC_CG_3 (0x03UL)  /* 3*ADCCLK*/
#define ADC_CG_4 (0x04UL)  /* 4*ADCCLK*/
#define ADC_CG_5 (0x05UL)  /* 5*ADCCLK*/
#define ADC_CG_6 (0x06UL)  /* 6*ADCCLK*/
#define ADC_CG_7 (0x07UL)  /* 7*ADCCLK*/
#define ADC_CG_8 (0x08UL)  /* 8*ADCCLK*/
#define ADC_CG_9 (0x09UL)  /* 9*ADCCLK*/
#define ADC_CG_10 (0x0AUL) /* 10*ADCCLK*/
#define ADC_CG_11 (0x0BUL) /* 11*ADCCLK*/
#define ADC_CG_12 (0x0CUL) /* 12*ADCCLK*/
#define ADC_CG_13 (0x0DUL) /* 13*ADCCLK*/
#define ADC_CG_14 (0x0EUL) /* 14*ADCCLK*/
#define ADC_CG_15 (0x0FUL) /* 15*ADCCLK*/

/*----------------------------------------------------------------------------
 **ADC 参考源
-----------------------------------------------------------------------------*/
#define ADC_VREFP_VDD (0x00UL)					   /*选择VDD作为正端参考*/
#define ADC_VREFP_BG (0x02UL << ADC_CON_ADCVS_Pos) /*选择BG作为正端参考*/

/*---------------------------------------------------------------------------*/
#define ADC_VREFP_AVREFP (0x01UL << ADC_CON_ADCVS_Pos) /*选择外部AVREFP作为正端参考*/

/*----------------------------------------------------------------------------
 **ADC 内部触发源选择
-----------------------------------------------------------------------------*/
// #define		ADC_TG_INTNEL_ADCA				(0x0UL | ADC_HWTG_ADCINTTGEN_Msk)		/*内部ADCA触发*/
#define ADC_TG_INTNEL_ADC ((0x1UL << ADC_HWTG_ADCINTTGSS_Pos) | ADC_HWTG_ADCINTTGEN_Msk)   /*内部ADC触发*/
#define ADC_TG_INTNEL_ACMP0 ((0x2UL << ADC_HWTG_ADCINTTGSS_Pos) | ADC_HWTG_ADCINTTGEN_Msk) /*内部ACMP0事件触发*/
#define ADC_TG_INTNEL_ACMP1 ((0x3UL << ADC_HWTG_ADCINTTGSS_Pos) | ADC_HWTG_ADCINTTGEN_Msk) /*内部ACMP1事件触发*/
#define ADC_TG_INTNEL_TMR0 ((0x4UL << ADC_HWTG_ADCINTTGSS_Pos) | ADC_HWTG_ADCINTTGEN_Msk)  /*内部TMR0已使能中断触发*/
#define ADC_TG_INTNEL_TMR1 ((0x5UL << ADC_HWTG_ADCINTTGSS_Pos) | ADC_HWTG_ADCINTTGEN_Msk)  /*内部TMR1已使能中断触发*/
// #define		ADC_TG_INTNEL_TMR2				((0x6UL<<ADC_HWTG_ADCINTTGSS_Pos)| ADC_HWTG_ADCINTTGEN_Msk)	/*内部TMR2已使能中断触发*/
// #define		ADC_TG_INTNEL_TMR3				((0x7UL<<ADC_HWTG_ADCINTTGSS_Pos)| ADC_HWTG_ADCINTTGEN_Msk)	/*内部TMR3已使能中断触发*/

/*----------------------------------------------------------------------------
 **ADC EPWM触发源选择
-----------------------------------------------------------------------------*/
#define ADC_TG_EPWM_CMP0 (0x01UL << ADC_HWTG_ADCPTG0EN_Pos) /*EPWM计数比较器0触发*/
#define ADC_TG_EPWM_CMP1 (0x01UL << ADC_HWTG_ADCPTG1EN_Pos) /*EPWM计数比较器1触发*/

#define ADC_TG_EPWM0_RISING (0x1UL << ADC_HWTG_ADCEPWMTEN_Pos)										 /*EPWM0上升沿触发中断*/
#define ADC_TG_EPWM0_FALLING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x2 << ADC_HWTG_ADCEPWMTPS_Pos)) /*EPWM0下降沿触发中断*/
#define ADC_TG_EPWM0_PERIOD ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x1 << ADC_HWTG_ADCEPWMTPS_Pos))	 /*EPWM0周期点触发中断*/
#define ADC_TG_EPWM0_ZERO ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x3 << ADC_HWTG_ADCEPWMTPS_Pos))	 /*EPWM0零点触发中断*/
#define ADC_TG_EPWM1_RISING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x1 << ADC_HWTG_ADCEPWMTSS_Pos))
#define ADC_TG_EPWM1_FALLING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x1 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x2 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM1_PERIOD ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x1 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x1 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM1_ZERO ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x1 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x3 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM2_RISING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x2 << ADC_HWTG_ADCEPWMTSS_Pos))
#define ADC_TG_EPWM2_FALLING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x2 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x2 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM2_PERIOD ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x2 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x1 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM2_ZERO ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x2 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x3 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM3_RISING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x3 << ADC_HWTG_ADCEPWMTSS_Pos))
#define ADC_TG_EPWM3_FALLING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x3 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x2 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM3_PERIOD ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x3 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x1 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM3_ZERO ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x3 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x3 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM4_RISING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x4 << ADC_HWTG_ADCEPWMTSS_Pos))
#define ADC_TG_EPWM4_FALLING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x4 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x2 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM4_PERIOD ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x4 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x1 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM4_ZERO ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x4 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x3 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM5_RISING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x5 << ADC_HWTG_ADCEPWMTSS_Pos))
#define ADC_TG_EPWM5_FALLING ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x5 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x2 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM5_PERIOD ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x5 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x1 << ADC_HWTG_ADCEPWMTPS_Pos))
#define ADC_TG_EPWM5_ZERO ((0x1UL << ADC_HWTG_ADCEPWMTEN_Pos) | (0x5 << ADC_HWTG_ADCEPWMTSS_Pos) | (0x3 << ADC_HWTG_ADCEPWMTPS_Pos))

/*----------------------------------------------------------------------------
 **ADC 采样通道
-----------------------------------------------------------------------------*/
#define ADC_CH_0 (0x0UL)
#define ADC_CH_1 (0x1UL)
#define ADC_CH_2 (0x2UL)
#define ADC_CH_3 (0x3UL)
#define ADC_CH_4 (0x4UL)
#define ADC_CH_5 (0x5UL)
#define ADC_CH_6 (0x6UL)
#define ADC_CH_7 (0x7UL)
#define ADC_CH_8 (0x8UL)
#define ADC_CH_9 (0x9UL)
#define ADC_CH_10 (0xAUL)
#define ADC_CH_11 (0xBUL)
#define ADC_CH_12 (0xCUL)
#define ADC_CH_13 (0xDUL)
#define ADC_CH_14 (0xEUL)
#define ADC_CH_15 (0xFUL)
#define ADC_CH_16 (0x10UL)
#define ADC_CH_17 (0x11UL)
#define ADC_CH_18 (0x12UL)
#define ADC_CH_19 (0x13UL)
#define ADC_CH_20 (0x14UL)
#define ADC_CH_21 (0x15UL)
#define ADC_CH_22 (0x16UL)
#define ADC_CH_23 (0x17UL)

#define ADC_CH_0_MSK (0x1UL)
#define ADC_CH_1_MSK (0x2UL)
#define ADC_CH_2_MSK (0x4UL)
#define ADC_CH_3_MSK (0x8UL)
#define ADC_CH_4_MSK (0x10UL)
#define ADC_CH_5_MSK (0x20UL)
#define ADC_CH_6_MSK (0x40UL)
#define ADC_CH_7_MSK (0x80UL)
#define ADC_CH_8_MSK (0x100UL)
#define ADC_CH_9_MSK (0x200UL)
#define ADC_CH_10_MSK (0x400UL)
#define ADC_CH_11_MSK (0x800UL)
#define ADC_CH_12_MSK (0x1000UL)
#define ADC_CH_13_MSK (0x2000UL)
#define ADC_CH_14_MSK (0x4000UL)
#define ADC_CH_15_MSK (0x8000UL)
#define ADC_CH_16_MSK (0x10000UL)
#define ADC_CH_17_MSK (0x20000UL)
#define ADC_CH_18_MSK (0x40000UL)
#define ADC_CH_19_MSK (0x80000UL)

#define ADC_CH_20_MSK (0x100000UL)
#define ADC_CH_21_MSK (0x200000UL)
#define ADC_CH_22_MSK (0x400000UL)
#define ADC_CH_23_MSK (0x800000UL)

/*----------------------------------------------------------------------------
 **ADC  AN22选择
-----------------------------------------------------------------------------*/
#define ADC_AN22_SEL_TS (0x0UL) /*内部温度传感器*/
#define ADC_AN22_SEL_BG (0x1UL) /*内部BG 1.45V*/

/*----------------------------------------------------------------------------
 **ADC  AN23选择
-----------------------------------------------------------------------------*/
#define ADC_AN23_SEL_BG (0x0UL)		 /*内部BG*/
#define ADC_AN23_SEL_ADCLDO (0x1UL)	 /*ADCLDO输出*/
#define ADC_AN23_SEL_VDDHA (0x2UL)	 /*VDDHA通道输出*/
#define ADC_AN23_SEL_VDDHCVA (0x3UL) /*VDDHCVA通道输出*/
#define ADC_AN23_SEL_DAC (0x4UL)	 /*DAC输出*/

/*----------------------------------------------------------------------------
 **ADC1   比较
-----------------------------------------------------------------------------*/
#define ADC_CMP_LESS_THAN_RESULT (0x00UL)								/*ADC 结果<预设值*/
#define ADC_CMP_NOT_LESS_THAN_RESULT (0x01UL << ADC_CMP_ADCCMPCOND_Pos) /*ADC 结果>=预设值*/

#define ADC_CMP0 (0x00UL) /*ADC比较器0*/
#define ADC_CMP1 (0x01UL) /*ADC比较器1*/

/*----------------------------------------------------------------------------
 **ADC1   写保护
-----------------------------------------------------------------------------*/
#define ADC_LOCK_WRITE_KEY (0x55UL)

/*****************************************************************************
 ** \brief	ADC_IS_BUSY
 **			获取ADC转换结束/空闲标志位
 ** \param [in] none
 ** \return  1：忙   0：空闲/转换完成
 ** \note
 *****************************************************************************/
#define ADC_IS_BUSY() ((ADC->CON2 & ADC_CON2_ADCST_Msk) ? 1 : 0)

	/*****************************************************************************
	 ** \brief	ADC_Go
	 **			开启ADC1转换
	 ** \param [in] none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	__STATIC_INLINE void ADC_Go(void)
	{
		ADC->LOCK = ADC_LOCK_WRITE_KEY;
		ADC->CON2 |= ADC_CON2_ADCST_Msk;
		ADC->LOCK = 0x00;
	}

	/*****************************************************************************/
	/* Global type definitions ('typedef') */
	/*****************************************************************************/

	/*****************************************************************************/
	/* Global variable declarations ('extern', definition in C source) */
	/*****************************************************************************/

	/*****************************************************************************/
	/* Global function prototypes ('extern', definition in C source) */
	/*****************************************************************************/

	/*****************************************************************************
	 ** \brief	ADC_ConfigRunMode
	 **			设置ADC运行模式
	 ** \param [in] RunMode: (1)ADC_MODE_HIGH
	 **						 (2)ADC_MODE_LOW
	 **				ConvertMode:(1)ADC_CONVERT_SINGLE
	 **							(2)ADC_CONVERT_CONTINUOUS
	 **				ClkDiv :(1)ADC_CLK_DIV_1
	 **						(2)ADC_CLK_DIV_2
	 **						(3)ADC_CLK_DIV_4
	 **						(4)ADC_CLK_DIV_8
	 **						(5)ADC_CLK_DIV_16
	 **						(6)ADC_CLK_DIV_32
	 **						(7)ADC_CLK_DIV_64
	 **						(8)ADC_CLK_DIV_128
	 **				Holdtime: 5~255
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_ConfigRunMode(uint32_t RunMode, uint32_t ConvertMode, uint32_t ClkDiv, uint32_t Holdtime);

	/*****************************************************************************
	 ** \brief	ADC_ConfigChannelSwitchMode
	 **			设置ADC通道切换模式
	 ** \param [in] 	ChSwitch:(1)ADC_SWITCH_HARDWARE
	 **							 (2)ADC_SWITCH_SOFTWARE
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_ConfigChannelSwitchMode(uint32_t ChSwitch);

	/*****************************************************************************
	 ** \brief	ADC_EnableCharge
	 **			开启ADC充电功能
	 ** \param [in] ChargeTime:	ADC_CG_2...... ADC_CG_30
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableCharge(uint32_t ChargeTime);
	/*****************************************************************************
	 ** \brief	ADC_EnableDischarge
	 **			开启ADC放电功能
	 ** \param [in] ChargeTime:	ADC_CG_2...... ADC_CG_30
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableDischarge(uint32_t ChargeTime);

	/*****************************************************************************
	 ** \brief	ADC_DisableChargeAndDischarge
	 **			关闭ADC放电、充电功能
	 ** \param [in] none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableChargeAndDischarge(void);

	/*****************************************************************************
	 ** \brief	ADC_ConfigVREF
	 **			设置ADC的参考源
	 ** \param [in]  VrefPos:  (1)ADC_VREFP_VDD		选择VDD作为正端参考
	 **						   (2)ADC_VREFP_BG			选择BG作为正端参考
	 **						   (3)ADC_VREFP_AVREFP		选择外部AVREFP作为正端参考
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_ConfigVREF(uint32_t VrefPos);

	/*****************************************************************************
	 ** \brief	ADC_ConfigSampleTime
	 **			配置采样时间
	 ** \param [in]  sampletime: 5 ~ 255;
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_ConfigSampleTime(uint32_t sampletime);

	/*****************************************************************************
	 ** \brief	ADC_EnableSampleWait
	 **			使能ADC保持采样状态
	 ** \param [in]  none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableSampleWait(void);
	/*****************************************************************************
	 ** \brief	ADC_DisableSampleWait
	 **			关闭ADC保持采样状态
	 ** \param [in]  none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableSampleWait(void);

	/*****************************************************************************
	 ** \brief	ADC_SelSoftwareSwitchChannel
	 **			选择软件切换转换的通道
	 ** \param [in] Channel: ADC_CH_0 ~ ADC_CH_30
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_SelSoftwareSwitchChannel(uint32_t Channel);

	/*****************************************************************************
	 ** \brief	ADC_SelAN22Source
	 **			   选择AN22通道的转换源
	 ** \param [in] AN22Sel : (1)ADC_AN22_SEL_TS
	 **						  (2)ADC_AN22_SEL_BG
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_SelAN22Source(uint8_t AN22Sel);

	/*****************************************************************************
	 ** \brief	ADC_EnableScanChannel
	 **			开启ADC转换通道
	 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableScanChannel(uint32_t ChannelMask);

	/*****************************************************************************
	 ** \brief	ADC_AN23Select
	 **			AN23通道选择
	 ** \param [in] AN23Sel : (1)ADC_AN23_SEL_BG
	 **						  (2)ADC_AN23_SEL_ADCLDO
	 **						  (3)ADC_AN23_SEL_VDDHA
	 **						  (4)ADC_AN23_SEL_VDDHCVA
	 **						  (5)ADC_AN23_SEL_DAC
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_AN23Select(uint32_t AN23Sel);

	/*****************************************************************************
	 ** \brief	ADC_DisableScanChannel
	 **			关闭ADC转换通道
	 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableScanChannel(uint32_t ChannelMask);

	/*****************************************************************************
	 ** \brief	ADC_GetResult
	 **			获取ADC转换结果
	 ** \param [in] Channel: ADC1_CH_0 ~ ADC1_CH_30
	 ** \return  12 bits Value
	 ** \note
	 *****************************************************************************/
	uint32_t ADC_GetResult(uint32_t Channel);

	/*****************************************************************************
	 ** \brief	ADC_EnableHardwareTrigger
	 **			开启硬件触发ADC转换功能
	 ** \param [in] TriggerSource: (1)ADC_TG_EXT_FALLING
	 **							(2)ADC_TG_EXT_RISING
	 **							(3)ADC_TG_INTNEL_ADCA
	 **							(4)ADC_TG_INTNEL_ADC
	 **							(5)ADC_TG_INTNEL_ACMP0
	 **							(6)ADC_TG_INTNEL_ACMP1
	 **							(7)ADC_TG_INTNEL_TMR0
	 **							(8)ADC_TG_INTNEL_TMR1
	 **							(9)ADC_TG_EPWM_CMP0
	 **							(10)ADC_TG_EPWM_CMP1
	 **							(11)ADC_TG_EPWM0_RISING
	 **							(12)ADC_TG_EPWM0_FALLING
	 **							(13)ADC_TG_EPWM0_PERIOD
	 **							(14)ADC_TG_EPWM0_ZERRO
	 **							(15)ADC_TG_EPWM1_RISING
	 **							(16)ADC_TG_EPWM1_FALLING
	 **							(17)ADC_TG_EPWM1_PERIOD
	 **							(18)ADC_TG_EPWM1_ZERRO
	 **							(19)ADC_TG_EPWM2_RISING
	 **							(20)ADC_TG_EPWM2_FALLING
	 **							(21)ADC_TG_EPWM2_PERIOD
	 **							(22)ADC_TG_EPWM2_ZERRO
	 **							(23)ADC_TG_EPWM3_RISING
	 **							(24)ADC_TG_EPWM3_FALLING
	 **							(25)ADC_TG_EPWM3_PERIOD
	 **							(26)ADC_TG_EPWM3_ZERRO
	 **							(27)ADC_TG_EPWM4_RISING
	 **							(28)ADC_TG_EPWM4_FALLING
	 **							(29)ADC_TG_EPWM4_PERIOD
	 **							(30)ADC_TG_EPWM4_ZERRO
	 **							(31)ADC_TG_EPWM5_RISING
	 **							(32)ADC_TG_EPWM5_FALLING
	 **							(33)ADC_TG_EPWM5_PERIOD
	 **							(34)ADC_TG_EPWM5_ZERRO
	 **							(35)ADC_TG_INTNEL_TMR2
	 **							(36)ADC_TG_INTNEL_TMR3
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableHardwareTrigger(uint32_t TriggerSource);
	/*****************************************************************************
	 ** \brief	ADC1_DisableHardwareTrigger
	 **			关闭硬件触发ADC1转换功能
	 ** \param [in] TriggerSource: (1)ADC_TG_EXT_FALLING
	 **							(2)ADC_TG_EXT_RISING
	 **							(3)ADC_TG_INTNEL_ADCA
	 **							(4)ADC_TG_INTNEL_ADC
	 **							(5)ADC_TG_INTNEL_ACMP0
	 **							(6)ADC_TG_INTNEL_ACMP1
	 **							(7)ADC_TG_INTNEL_TMR0
	 **							(8)ADC_TG_INTNEL_TMR1
	 **							(9)ADC_TG_EPWM_CMP0
	 **							(10)ADC_TG_EPWM_CMP1
	 **							(11)ADC_TG_EPWM0_RISING
	 **							(12)ADC_TG_EPWM0_FALLING
	 **							(13)ADC_TG_EPWM0_PERIOD
	 **							(14)ADC_TG_EPWM0_ZERRO
	 **							(15)ADC_TG_EPWM1_RISING
	 **							(16)ADC_TG_EPWM1_FALLING
	 **							(17)ADC_TG_EPWM1_PERIOD
	 **							(18)ADC_TG_EPWM1_ZERRO
	 **							(19)ADC_TG_EPWM2_RISING
	 **							(20)ADC_TG_EPWM2_FALLING
	 **							(21)ADC_TG_EPWM2_PERIOD
	 **							(22)ADC_TG_EPWM2_ZERRO
	 **							(23)ADC_TG_EPWM3_RISING
	 **							(24)ADC_TG_EPWM3_FALLING
	 **							(25)ADC_TG_EPWM3_PERIOD
	 **							(26)ADC_TG_EPWM3_ZERRO
	 **							(27)ADC_TG_EPWM4_RISING
	 **							(28)ADC_TG_EPWM4_FALLING
	 **							(29)ADC_TG_EPWM4_PERIOD
	 **							(30)ADC_TG_EPWM4_ZERRO
	 **							(31)ADC_TG_EPWM5_RISING
	 **							(32)ADC_TG_EPWM5_FALLING
	 **							(33)ADC_TG_EPWM5_PERIOD
	 **							(34)ADC_TG_EPWM5_ZERRO
	 **							(35)ADC_TG_INTNEL_TMR2
	 **							(36)ADC_TG_INTNEL_TMR3
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableHardwareTrigger(uint32_t TriggerSource);
	/*****************************************************************************
	 ** \brief	ADC_SetAnalogSwitchDelayTime
	 **			设置ADC 模拟开关打开到开始采样的时间
	 ** \param [in] DelayTime: 0x00~ 0xff;
	 ** \return  none
	 ** \note  Time = (DelayTime+2)*ADCCLK
	 *****************************************************************************/
	void ADC_SetAnalogSwitchDelayTime(uint32_t DelayTime);

	/*****************************************************************************
	 ** \brief	ADC_SetTriggerDelayTime
	 **			设置ADC EPWM触发的延时时间
	 ** \param [in] DelayTime: 0x00~ 0x1ff;
	 ** \return  none
	 ** \note  Time = (DelayTime+2)*PCLK
	 *****************************************************************************/
	void ADC_SetTriggerDelayTime(uint32_t DelayTime);
	/*****************************************************************************
	 ** \brief	ADC_EnableEPWMTriggerChannel
	 **			使能 EPWM 输出触发的ADC转换通道
	 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableEPWMTriggerChannel(uint32_t ChannelMask);
	/*****************************************************************************
	 ** \brief	ADC_DisableEPWMTriggerChannel
	 **			关闭 EPWM 输出触发的ADC转换通道
	 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableEPWMTriggerChannel(uint32_t ChannelMask);

	/*****************************************************************************
	 ** \brief	ADC_EnableEPWMCmp0TriggerChannel
	 **			使能 EPWM 比较器0 触发的ADC转换通道
	 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableEPWMCmp0TriggerChannel(uint32_t ChannelMask);
	/*****************************************************************************
	 ** \brief	ADC_DisableEPWMCmp0TriggerChannel
	 **			关闭 EPWM 比较器0 触发的ADC转换通道
	 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableEPWMCmp0TriggerChannel(uint32_t ChannelMask);

	/*****************************************************************************
	 ** \brief	ADC_EnableEPWMCmp1TriggerChannel
	 **			使能 EPWM 比较器1 触发的ADC转换通道
	 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableEPWMCmp1TriggerChannel(uint32_t ChannelMask);
	/*****************************************************************************
	 ** \brief	ADC_DisableEPWMCmp1TriggerChannel
	 **			关闭 EPWM 比较器1 触发的ADC转换通道
	 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableEPWMCmp1TriggerChannel(uint32_t ChannelMask);

	/*****************************************************************************
	 ** \brief	ADC_EnableCompare
	 **			开启ADC比较功能
	 ** \param [in] Compare : ADC_CMP0 、ADC_CMP1
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableCompare(uint8_t ACMPx, uint32_t Compare);
	/*****************************************************************************
	 ** \brief	ADC_DisableCompare
	 **			关闭ADC比较功能
	 ** \param [in] Compare : ADC_CMP0 、ADC_CMP1
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableCompare(uint8_t ACMPx);
	/*****************************************************************************
	 ** \brief	ADC_ConfigCompare
	 **			配置ADC比较功能
	 ** \param [in]  Compare : ADC_CMP0 、ADC_CMP1
	 **				 CmpMode:(1)ADC_CMP_LESS_THAN_RESULT
	 **						 (2)ADC_CMP_NOT_LESS_THAN_RESULT
	 **				 CmpCount: 0x00~0xf  预设比较次数
	 **				 CmpChannel: ADC_CH_0 ~ ADC_CH_30
	 **				 CmpData : 12 bit 比较值
	 ** \return  none
	 ** \note	当内部计数器等于ADCCMPxMCNT+1的值时产生ADC比较事件。
	 *****************************************************************************/
	void ADC_ConfigCompare(uint8_t ACMPx, uint32_t CmpMode, uint32_t CmpCount,
						   uint32_t CmpChannel, uint32_t CmpData);

	/*****************************************************************************
	 ** \brief	ADC_GetCompareResult
	 **			获取ADC比较器结果
	 ** \param [in] Compare : ADC_CMP0 、ADC_CMP1
	 ** \return  1 /0
	 ** \note
	 *****************************************************************************/
	uint32_t ADC_GetCompareResult(uint8_t ACMPx);

	/*****************************************************************************
	 ** \brief	ADC_Start
	 **			开启ADC
	 ** \param [in] none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_Start(void);
	/*****************************************************************************
	 ** \brief	ADC_Stop
	 **			关闭ADC
	 ** \param [in] none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_Stop(void);

	/*****************************************************************************
	 ** \brief	ADC_EnableReset
	 **			复位ADC
	 ** \param [in] none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableReset(void);

	/*****************************************************************************
	 ** \brief	ADC_EnableCompareInt
	 **			开启ADC比较中断
	 ** \param [in] none
	 ** \return  none
	 ** \note	只支持比较器 0
	 *****************************************************************************/
	void ADC_EnableCompareInt(void);
	/*****************************************************************************
	 ** \brief	ADC_DisableCompareInt
	 **			关闭ADC比较中断
	 ** \param [in] none
	 ** \return  none
	 ** \note	只支持比较器 0
	 *****************************************************************************/
	void ADC_DisableCompareInt(void);
	/*****************************************************************************
	 ** \brief	ADC_GetCompareIntFlag
	 **			获取ADC比较中断使能并且产生中断标志位
	 ** \param [in] none
	 ** \return  1:中断使能并且产生中断 0：无中断
	 ** \note	只支持比较器 0
	 *****************************************************************************/
	uint32_t ADC_GetCompareIntFlag(void);
	/*****************************************************************************
	 ** \brief	ADC_ClearCompareIntFlag
	 **			清除ADC比较中断标志位
	 ** \param [in] none
	 ** \return  none
	 ** \note	只支持比较器 0
	 *****************************************************************************/
	void ADC_ClearCompareIntFlag(void);

	/*****************************************************************************
	 ** \brief	ADC_EnableChannelInt
	 **			开启ADC通道n中断
	 ** \param [in]ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableChannelInt(uint32_t ChannelMask);
	/*****************************************************************************
	 ** \brief	ADC_DisableChannelInt
	 **			关闭ADC通道n中断
	 ** \param [in] ChannelMask: ADC_CH_0_MSK ~ ADC_CH_30_MSK
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableChannelInt(uint32_t ChannelMask);
	/*****************************************************************************
	 ** \brief	ADC_GetChannelIntFlag
	 **			获取ADC通道n中断使能并且产生中断标志位
	 ** \param [in] Channel:ADC_CH_0 ~ ADC_CH_30
	 ** \return  1:中断使能并且产生中断 0：无中断
	 ** \note
	 *****************************************************************************/
	uint32_t ADC_GetChannelIntFlag(uint32_t Channel);
	/*****************************************************************************
	 ** \brief	ADC_ClearChannelIntFlag
	 **			清除ADC通道n中断标志位
	 ** \param [in] Channel:ADC_CH_0 ~ ADC_CH_30
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_ClearChannelIntFlag(uint32_t Channel);

	/*****************************************************************************
	 ** \brief	ADC_EnableEPWMCmp0TriggerDelayTime
	 **			使能  EPWM 比较器0 触发的ADC 延时
	 ** \param [in] none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableEPWMCmp0TriggerDelayTime(void);
	/*****************************************************************************
	 ** \brief	ADC_DisableEPWMCmp0TriggerDelayTime
	 **			关闭 EPWM 比较器0 触发的ADC 延时
	 ** \param [in] none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableEPWMCmp0TriggerDelayTime(void);
	/*****************************************************************************
	 ** \brief	ADC_EnableEPWMCmp1TriggerDelayTime
	 **			使能  EPWM 比较器1 触发的ADC 延时
	 ** \param [in] none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_EnableEPWMCmp1TriggerDelayTime(void);
	/*****************************************************************************
	 ** \brief	ADC_DisableEPWMCmp1TriggerDelayTime
	 **			关闭 EPWM 比较器1 触发的ADC 延时
	 ** \param [in] none
	 ** \return  none
	 ** \note
	 *****************************************************************************/
	void ADC_DisableEPWMCmp1TriggerDelayTime(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H_ */
