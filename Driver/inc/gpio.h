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
/** \file gpio.h
**
** History:
** 
*****************************************************************************/
#ifndef __GPIO_H__
#define __GPIO_H__

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
 #define assert_param(expr) ((void)0)
/*----------------------------------------------------------------------------
 **GPIO PORT & PIN
-----------------------------------------------------------------------------*/
typedef enum {
	PORT0 = 0,
	PORT1 = 1,
	PORT2 = 2,	
}PORT_TypeDef;

typedef enum {
	PIN0 = 0,
	PIN1,
	PIN2,
	PIN3,
	PIN4,
	PIN5,
	PIN6,
	PIN7,	
}PIN_TypeDef;

typedef enum {
	INPUT = 0,
	PULLUP_INPUT,
	TTL_INPUT,
	ANALOG_INPUT,
	OUTPUT,
	OPENDRAIN_OUTPUT,
	PULLDOWN_INPUT,
}PIN_ModeDef;

typedef enum
{ 
	GPIO_Control_DIG   = 0, /*!< GPIO Control Digital */
	GPIO_Control_ANA	 = 1, /*!< GPIO Control Analogy  */
}GPIOControl_TypeDef;
#define IS_GPIO_Control(Ctrl) (((Ctrl) == GPIO_Control_ANA)  || ((Ctrl) == GPIO_Control_DIG) )? 1:0

typedef enum
{ 
	GPIO_Mode_OUT  = 0, /*!< GPIO Output Mode */
	GPIO_Mode_IN	 = 1, /*!< GPIO Input Mode */
}GPIOMode_TypeDef;
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_IN)  || ((MODE) == GPIO_Mode_OUT) )? 1:0

typedef enum
{ 
	GPIO_OType_PP = 0,//普通输出
	GPIO_OType_OD = 1 //开漏输出
}GPIOOType_TypeDef;
#define IS_GPIO_OTYPE(OTYPE) (((OTYPE) == GPIO_OType_PP) || ((OTYPE) == GPIO_OType_OD))? 1:0
typedef enum
{ 
	GPIO_PuPd_NOPULL = 0x00,
	GPIO_PuPd_UP     = 0x01,
	GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;
#define IS_GPIO_PUPD(PUPD) (((PUPD) == GPIO_PuPd_NOPULL) || ((PUPD) == GPIO_PuPd_UP) || \
                            ((PUPD) == GPIO_PuPd_DOWN))? 1:0
typedef enum
{ 
	GPIO_Level_LOW	= 0x00,
	GPIO_Level_HIGH = 0x01,
}GPIOInit_Level;
/*----------------------------------------------------------------------------
 **System  IO复用输出/输入配置
-----------------------------------------------------------------------------*/
/*----------GPIO_P00输出复用-----------------------------------------------*/
#define 	IO_OUTCFG_P00_GPIO			(0x00UL)
#define		IO_OUTCFG_P00_PCBUZ0		(0x01UL)
#define		IO_OUTCFG_P00_TO00			(0x02UL)
#define		IO_OUTCFG_P00_TO01			(0x03UL)
/*----------GPIO_P00输入复用-----------------------------------------------*/
#define 	IO_INCFG_P00_BKIN			(0x00UL)
#define 	IO_INCFG_P00_ECAP13			(0x00UL)
#define		IO_INCFG_P00_INTP0			(0x00UL)
#define		IO_INCFG_P00_INTP1			(0x00UL)
#define		IO_INCFG_P00_INTP2			(0x00UL)
#define		IO_INCFG_P00_INTP3			(0x00UL)
#define		IO_INCFG_P00_TAU0			(0x00UL)
#define		IO_INCFG_P00_TAU1			(0x00UL)
#define		IO_INCFG_P00_TAU2			(0x00UL)
#define		IO_INCFG_P00_TAU3			(0x00UL)

/*----------GPIO_P01输出复用-----------------------------------------------*/
#define 	IO_OUTCFG_P01_GPIO			(0x00UL)
#define		IO_OUTCFG_P01_EPWM0			(0x01UL)
#define		IO_OUTCFG_P01_TO02			(0x02UL)
#define		IO_OUTCFG_P01_TO03			(0x03UL)
/*----------GPIO_P01输入复用-----------------------------------------------*/
#define		IO_INCFG_P01_INTP0			(0x01UL)
#define		IO_INCFG_P01_INTP1			(0x01UL)
#define		IO_INCFG_P01_INTP2			(0x01UL)
#define		IO_INCFG_P01_INTP3			(0x01UL)
#define		IO_INCFG_P01_TAU0			(0x01UL)
#define		IO_INCFG_P01_TAU1			(0x01UL)
#define		IO_INCFG_P01_TAU2			(0x01UL)
#define		IO_INCFG_P01_TAU3			(0x01UL)

/*----------GPIO_P02输出复用-------------------------------------------------*/
//当配置此引脚时，需要配置RSTM
#define 	IO_OUTCFG_P02_GPIO			(0x00UL)
#define 	IO_OUTCFG_P02_C1_O			(0x01UL)
#define		IO_OUTCFG_P02_SSIO			(0x02UL)
#define		IO_OUTCFG_P02_CCP0A_O		(0x03UL)
#define		IO_OUTCFG_P02_CCP1A_O		(0x04UL)
#define		IO_OUTCFG_P02_EPWM0			(0x05UL)
#define		IO_OUTCFG_P02_TO00			(0x06UL)
#define		IO_OUTCFG_P02_TO01			(0x07UL)
/*----------GPIO_P02输入复用-----------------------------------------------*/
#define 	IO_INCFG_P02_CCP0A_I		(0x00UL)
#define		IO_INCFG_P02_CCP0B_I		(0x00UL)
#define		IO_INCFG_P02_CCP1A_I		(0x00UL)
#define		IO_INCFG_P02_CCP1B_I		(0x00UL)
#define		IO_INCFG_P02_INTP0			(0x02UL)
#define		IO_INCFG_P02_INTP1			(0x02UL)
#define		IO_INCFG_P02_INTP2			(0x02UL)
#define		IO_INCFG_P02_INTP3			(0x02UL)
#define		IO_INCFG_P02_TAU0				(0x02UL)
#define		IO_INCFG_P02_TAU1				(0x02UL)
#define		IO_INCFG_P02_TAU2				(0x02UL)
#define		IO_INCFG_P02_TAU3				(0x02UL)

/*----------GPIO_P03输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P03_GPIO			(0x00UL)
#define		IO_OUTCFG_P03_SCK			(0x02UL)
#define		IO_OUTCFG_P03_LPF_ASYN		(0x04UL)
#define		IO_OUTCFG_P03_SDA			(0x05UL)
#define		IO_OUTCFG_P03_TO02			(0x06UL)
#define		IO_OUTCFG_P03_TO03			(0x07UL)
/*----------GPIO_P03输入复用-----------------------------------------------*/
#define 	IO_INCFG_P03_RXD				(0x00UL)
#define		IO_INCFG_P03_INTP0			(0x03UL)
#define		IO_INCFG_P03_INTP1			(0x03UL)
#define		IO_INCFG_P03_INTP2			(0x03UL)
#define		IO_INCFG_P03_INTP3			(0x03UL)
#define		IO_INCFG_P03_TAU0				(0x03UL)
#define		IO_INCFG_P03_TAU1				(0x03UL)
#define		IO_INCFG_P03_TAU2				(0x03UL)
#define		IO_INCFG_P03_TAU3				(0x03UL)

/*----------GPIO_P04输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P04_GPIO			(0x00UL)
#define 	IO_OUTCFG_P04_TXD			(0x01UL)
#define		IO_OUTCFG_P04_MISO			(0x02UL)
#define		IO_OUTCFG_P04_CCP0B_O		(0x03UL)
#define		IO_OUTCFG_P04_CCP1B_O		(0x04UL)
#define		IO_OUTCFG_P04_SCL			(0x05UL)
#define		IO_OUTCFG_P04_C0_O			(0x06UL)
#define 	IO_OUTCFG_P04_C1_O			(0x07UL)
/*----------GPIO_P04输入复用-----------------------------------------------*/
#define 	IO_INCFG_P04_CCP0A_I		(0x01UL)
#define		IO_INCFG_P04_CCP0B_I		(0x01UL)
#define		IO_INCFG_P04_CCP1A_I		(0x01UL)
#define		IO_INCFG_P04_CCP1B_I		(0x01UL)
#define		IO_INCFG_P04_INTP0			(0x04UL)
#define		IO_INCFG_P04_INTP1			(0x04UL)
#define		IO_INCFG_P04_INTP2			(0x04UL)
#define		IO_INCFG_P04_INTP3			(0x04UL)
#define		IO_INCFG_P04_TAU0				(0x04UL)
#define		IO_INCFG_P04_TAU1				(0x04UL)
#define		IO_INCFG_P04_TAU2				(0x04UL)
#define		IO_INCFG_P04_TAU3				(0x04UL)

/*----------GPIO_P05输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P05_GPIO			(0x00UL)
#define		IO_OUTCFG_P05_C0_O			(0x01UL)
#define		IO_OUTCFG_P05_MOSI			(0x02UL)
/*----------GPIO_P05输入复用-----------------------------------------------*/
#define		IO_INCFG_P05_INTP0			(0x05UL)
#define		IO_INCFG_P05_INTP1			(0x05UL)
#define		IO_INCFG_P05_INTP2			(0x05UL)
#define		IO_INCFG_P05_INTP3			(0x05UL)
#define		IO_INCFG_P05_TAU0				(0x05UL)
#define		IO_INCFG_P05_TAU1				(0x05UL)
#define		IO_INCFG_P05_TAU2				(0x05UL)
#define		IO_INCFG_P05_TAU3				(0x05UL)


/*----------GPIO_P06输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P06_GPIO			(0x00UL)
#define		IO_OUTCFG_P06_LPF_ASYN		(0x02UL)
#define		IO_OUTCFG_P06_CCP0A_O		(0x03UL)
#define		IO_OUTCFG_P06_CCP1A_O		(0x04UL)
#define		IO_OUTCFG_P06_EPWM0			(0x05UL)
#define		IO_OUTCFG_P06_PCBUZ0		(0x06UL)
#define		IO_OUTCFG_P06_PCBUZ1		(0x07UL)
/*----------GPIO_P06输入复用-----------------------------------------------*/
#define 	IO_INCFG_P06_CCP0A_I		(0x02UL)
#define		IO_INCFG_P06_CCP0B_I		(0x02UL)
#define		IO_INCFG_P06_CCP1A_I		(0x02UL)
#define		IO_INCFG_P06_CCP1B_I		(0x02UL)
#define 	IO_INCFG_P06_RXD				(0x01UL)
#define		IO_INCFG_P06_INTP0			(0x06UL)
#define		IO_INCFG_P06_INTP1			(0x06UL)
#define		IO_INCFG_P06_INTP2			(0x06UL)
#define		IO_INCFG_P06_INTP3			(0x06UL)
#define		IO_INCFG_P06_TAU0				(0x06UL)
#define		IO_INCFG_P06_TAU1				(0x06UL)
#define		IO_INCFG_P06_TAU2				(0x06UL)
#define		IO_INCFG_P06_TAU3				(0x06UL)


/*----------GPIO_P07输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P07_GPIO			(0x00UL)
#define		IO_OUTCFG_P07_TXD			(0x01UL)
#define		IO_OUTCFG_P07_LPF_ASYN		(0x02UL)
#define 	IO_OUTCFG_P07_CCP0B_O		(0x03UL)
#define		IO_OUTCFG_P07_CCP1B_O		(0x04UL)
#define		IO_OUTCFG_P07_EPWM0			(0x05UL)
#define		IO_OUTCFG_P07_TO00			(0x06UL)
#define		IO_OUTCFG_P07_TO01			(0x07UL)
/*----------GPIO_P07输入复用-----------------------------------------------*/
#define 	IO_INCFG_P07_CCP0A_I		(0x03UL)
#define		IO_INCFG_P07_CCP0B_I		(0x03UL)
#define		IO_INCFG_P07_CCP1A_I		(0x03UL)
#define		IO_INCFG_P07_CCP1B_I		(0x03UL)
#define		IO_INCFG_P07_INTP0			(0x07UL)
#define		IO_INCFG_P07_INTP1			(0x07UL)
#define		IO_INCFG_P07_INTP2			(0x07UL)
#define		IO_INCFG_P07_INTP3			(0x07UL)
#define		IO_INCFG_P07_TAU0				(0x07UL)
#define		IO_INCFG_P07_TAU1				(0x07UL)
#define		IO_INCFG_P07_TAU2				(0x07UL)
#define		IO_INCFG_P07_TAU3				(0x07UL)


/*----------GPIO_P10输出复用------------------------------------------------*/
#define 	IO_OUTCFG_P10_GPIO			(0x00UL)
#define 	IO_OUTCFG_P10_EPWM0			(0x01UL)
/*----------GPIO_P10输入复用-----------------------------------------------*/
#define		IO_INCFG_P10_INTP0			(0x10UL)
#define		IO_INCFG_P10_INTP1			(0x10UL)
#define		IO_INCFG_P10_INTP2			(0x10UL)
#define		IO_INCFG_P10_INTP3			(0x10UL)
#define		IO_INCFG_P10_TAU0				(0x10UL)
#define		IO_INCFG_P10_TAU1				(0x10UL)
#define		IO_INCFG_P10_TAU2				(0x10UL)
#define		IO_INCFG_P10_TAU3				(0x10UL)


/*----------GPIO_P11输出复用------------------------------------------------*/
#define 	IO_OUTCFG_P11_GPIO			(0x00UL)
#define		IO_OUTCFG_P11_EPWM1			(0x01UL)
/*----------GPIO_P11输入复用-----------------------------------------------*/
#define		IO_INCFG_P11_INTP0			(0x11UL)
#define		IO_INCFG_P11_INTP1			(0x11UL)
#define		IO_INCFG_P11_INTP2			(0x11UL)
#define		IO_INCFG_P11_INTP3			(0x11UL)
#define		IO_INCFG_P11_TAU0				(0x11UL)
#define		IO_INCFG_P11_TAU1				(0x11UL)
#define		IO_INCFG_P11_TAU2				(0x11UL)
#define		IO_INCFG_P11_TAU3				(0x11UL)


/*----------GPIO_P12输出复用-----------------------------------------------*/
#define 	IO_OUTCFG_P12_GPIO				(0x00UL)
#define		IO_OUTCFG_P12_EPWM2				(0x01UL)
/*----------GPIO_P12输入复用-----------------------------------------------*/
#define		IO_INCFG_P12_INTP0			(0x12UL)
#define		IO_INCFG_P12_INTP1			(0x12UL)
#define		IO_INCFG_P12_INTP2			(0x12UL)
#define		IO_INCFG_P12_INTP3			(0x12UL)
#define		IO_INCFG_P12_TAU0				(0x12UL)
#define		IO_INCFG_P12_TAU1				(0x12UL)
#define		IO_INCFG_P12_TAU2				(0x12UL)
#define		IO_INCFG_P12_TAU3				(0x12UL)


/*----------GPIO_P13输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P13_GPIO			(0x00UL)
#define		IO_OUTCFG_P13_EPWM3			(0x01UL)
/*----------GPIO_P13输入复用-----------------------------------------------*/
#define		IO_INCFG_P13_INTP0			(0x13UL)
#define		IO_INCFG_P13_INTP1			(0x13UL)
#define		IO_INCFG_P13_INTP2			(0x13UL)
#define		IO_INCFG_P13_INTP3			(0x13UL)
#define		IO_INCFG_P13_TAU0				(0x13UL)
#define		IO_INCFG_P13_TAU1				(0x13UL)
#define		IO_INCFG_P13_TAU2				(0x13UL)
#define		IO_INCFG_P13_TAU3				(0x13UL)


/*----------GPIO_P14输出复用--------------------------------------------------*/
#define 	IO_OUTCFG_P14_GPIO			(0x00UL)
#define		IO_OUTCFG_P14_EPWM4			(0x01UL)
/*----------GPIO_P14输入复用-----------------------------------------------*/
#define		IO_INCFG_P14_INTP0			(0x14UL)
#define		IO_INCFG_P14_INTP1			(0x14UL)
#define		IO_INCFG_P14_INTP2			(0x14UL)
#define		IO_INCFG_P14_INTP3			(0x14UL)
#define		IO_INCFG_P14_TAU0				(0x14UL)
#define		IO_INCFG_P14_TAU1				(0x14UL)
#define		IO_INCFG_P14_TAU2				(0x14UL)
#define		IO_INCFG_P14_TAU3				(0x14UL)


/*----------GPIO_P15输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P15_GPIO			(0x00UL)
#define		IO_OUTCFG_P15_EPWM5			(0x01UL)
/*----------GPIO_P15输入复用-----------------------------------------------*/
#define		IO_INCFG_P15_INTP0			(0x15UL)
#define		IO_INCFG_P15_INTP1			(0x15UL)
#define		IO_INCFG_P15_INTP2			(0x15UL)
#define		IO_INCFG_P15_INTP3			(0x15UL)
#define		IO_INCFG_P15_TAU0				(0x15UL)
#define		IO_INCFG_P15_TAU1				(0x15UL)
#define		IO_INCFG_P15_TAU2				(0x15UL)
#define		IO_INCFG_P15_TAU3				(0x15UL)


/*----------GPIO_P16输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P16_GPIO			(0x00UL)
#define		IO_OUTCFG_P16_EPWM0			(0x01UL)
/*----------GPIO_P16输入复用-----------------------------------------------*/
#define		IO_INCFG_P16_INTP0			(0x16UL)
#define		IO_INCFG_P16_INTP1			(0x16UL)
#define		IO_INCFG_P16_INTP2			(0x16UL)
#define		IO_INCFG_P16_INTP3			(0x16UL)
#define		IO_INCFG_P16_TAU0				(0x16UL)
#define		IO_INCFG_P16_TAU1				(0x16UL)
#define		IO_INCFG_P16_TAU2				(0x16UL)
#define		IO_INCFG_P16_TAU3				(0x16UL)


/*----------GPIO_P20输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P20_GPIO			(0x00UL)
#define		IO_OUTCFG_P20_PCBUZ0		(0x01UL)
#define		IO_OUTCFG_P20_TO00			(0x02UL)
#define		IO_OUTCFG_P20_TO01			(0x03UL)
/*----------GPIO_P20输入复用-----------------------------------------------*/
#define 	IO_INCFG_P20_ECAP00			(0x00UL)
#define		IO_INCFG_P20_INTP0			(0x20UL)
#define		IO_INCFG_P20_INTP1			(0x20UL)
#define		IO_INCFG_P20_INTP2			(0x20UL)
#define		IO_INCFG_P20_INTP3			(0x20UL)
#define		IO_INCFG_P20_TAU0				(0x20UL)
#define		IO_INCFG_P20_TAU1				(0x20UL)
#define		IO_INCFG_P20_TAU2				(0x20UL)
#define		IO_INCFG_P20_TAU3				(0x20UL)


/*----------GPIO_P21输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P21_GPIO			(0x00UL)
#define		IO_OUTCFG_P21_PCBUZ1		(0x01UL)
#define		IO_OUTCFG_P21_TO02			(0x02UL)
#define		IO_OUTCFG_P21_TO03			(0x03UL)
/*----------GPIO_P21输入复用-----------------------------------------------*/
#define 	IO_INCFG_P21_ECAP01			(0x00UL)
#define		IO_INCFG_P21_INTP0			(0x21UL)
#define		IO_INCFG_P21_INTP1			(0x21UL)
#define		IO_INCFG_P21_INTP2			(0x21UL)
#define		IO_INCFG_P21_INTP3			(0x21UL)
#define		IO_INCFG_P21_TAU0				(0x21UL)
#define		IO_INCFG_P21_TAU1				(0x21UL)
#define		IO_INCFG_P21_TAU2				(0x21UL)
#define		IO_INCFG_P21_TAU3				(0x21UL)


/*----------GPIO_P22输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P22_GPIO			(0x00UL)
#define		IO_OUTCFG_P22_PCBUZ0		(0x01UL)
#define		IO_OUTCFG_P22_PCBUZ1		(0x02UL)
#define		IO_OUTCFG_P22_EPWM0			(0x03UL)
/*----------GPIO_P22输入复用-----------------------------------------------*/
#define 	IO_INCFG_P22_ECAP02			(0x00UL)
#define		IO_INCFG_P22_INTP0			(0x22UL)
#define		IO_INCFG_P22_INTP1			(0x22UL)
#define		IO_INCFG_P22_INTP2			(0x22UL)
#define		IO_INCFG_P22_INTP3			(0x22UL)
#define		IO_INCFG_P22_TAU0				(0x22UL)
#define		IO_INCFG_P22_TAU1				(0x22UL)
#define		IO_INCFG_P22_TAU2				(0x22UL)
#define		IO_INCFG_P22_TAU3				(0x22UL)


/*----------GPIO_P23输出复用-----------------------------------------------*/
#define 	IO_OUTCFG_P23_GPIO			(0x00UL)
#define		IO_OUTCFG_P23_CCP0A_O		(0x01UL)
#define		IO_OUTCFG_P23_CCP1A_O		(0x02UL)
#define		IO_OUTCFG_P23_C1_O			(0x03UL)
/*----------GPIO_P23输入复用-----------------------------------------------*/
#define		IO_INCFG_P23_INTP0			(0x23UL)
#define		IO_INCFG_P23_INTP1			(0x23UL)
#define		IO_INCFG_P23_INTP2			(0x23UL)
#define		IO_INCFG_P23_INTP3			(0x23UL)
#define		IO_INCFG_P23_TAU0				(0x23UL)
#define		IO_INCFG_P23_TAU1				(0x23UL)
#define		IO_INCFG_P23_TAU2				(0x23UL)
#define		IO_INCFG_P23_TAU3				(0x23UL)


/*----------GPIO_P24输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P24_GPIO			(0x00UL)
#define		IO_OUTCFG_P24_CCP0B_O		(0x01UL)
#define		IO_OUTCFG_P24_CCP1B_O		(0x02UL)
#define		IO_OUTCFG_P24_C1_O			(0x03UL)
/*----------GPIO_P24输入复用-----------------------------------------------*/
#define 	IO_INCFG_P24_BKIN			(0x01UL)
#define 	IO_INCFG_P24_ECAP10			(0x00UL)
#define		IO_INCFG_P24_INTP0			(0x24UL)
#define		IO_INCFG_P24_INTP1			(0x24UL)
#define		IO_INCFG_P24_INTP2			(0x24UL)
#define		IO_INCFG_P24_INTP3			(0x24UL)
#define		IO_INCFG_P24_TAU0				(0x24UL)
#define		IO_INCFG_P24_TAU1				(0x24UL)
#define		IO_INCFG_P24_TAU2				(0x24UL)
#define		IO_INCFG_P24_TAU3				(0x24UL)


/*----------GPIO_P25输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P25_GPIO			(0x00UL)
#define		IO_OUTCFG_P25_PCBUZ0		(0x01UL)
#define		IO_OUTCFG_P25_PCBUZ1		(0x02UL)
#define		IO_OUTCFG_P25_TO00			(0x03UL)
/*----------GPIO_P25输入复用-----------------------------------------------*/
#define 	IO_INCFG_P25_ECAP11			(0x00UL)
#define		IO_INCFG_P25_INTP0			(0x25UL)
#define		IO_INCFG_P25_INTP1			(0x25UL)
#define		IO_INCFG_P25_INTP2			(0x25UL)
#define		IO_INCFG_P25_INTP3			(0x25UL)
#define		IO_INCFG_P25_TAU0				(0x25UL)
#define		IO_INCFG_P25_TAU1				(0x25UL)
#define		IO_INCFG_P25_TAU2				(0x25UL)
#define		IO_INCFG_P25_TAU3				(0x25UL)


/*----------GPIO_P26输出复用-------------------------------------------------*/
#define 	IO_OUTCFG_P26_GPIO			(0x00UL)
#define 	IO_OUTCFG_P26_C1_O			(0x01UL)
#define		IO_OUTCFG_P26_CCP0A_O		(0x03UL)
#define		IO_OUTCFG_P26_CCP1A_O		(0x04UL)
#define		IO_OUTCFG_P26_EPWM0			(0x05UL)
#define		IO_OUTCFG_P26_TO00			(0x06UL)
#define		IO_OUTCFG_P26_TO01			(0x07UL)
/*----------GPIO_P26输入复用-----------------------------------------------*/
#define 	IO_INCFG_P26_BKIN			(0x02UL)
#define 	IO_INCFG_P26_ECAP12			(0x00UL)
#define 	IO_INCFG_P26_CCP0A_I		(0x04UL)
#define		IO_INCFG_P26_CCP0B_I		(0x04UL)
#define		IO_INCFG_P26_CCP1A_I		(0x04UL)
#define		IO_INCFG_P26_CCP1B_I		(0x04UL)
#define		IO_INCFG_P26_INTP0			(0x26UL)
#define		IO_INCFG_P26_INTP1			(0x26UL)
#define		IO_INCFG_P26_INTP2			(0x26UL)
#define		IO_INCFG_P26_INTP3			(0x26UL)
#define		IO_INCFG_P26_TAU0				(0x26UL)
#define		IO_INCFG_P26_TAU1				(0x26UL)
#define		IO_INCFG_P26_TAU2				(0x26UL)
#define		IO_INCFG_P26_TAU3				(0x26UL)


/*----------GPIO_P27输出复用--------------------------------------------------*/
#define 	IO_OUTCFG_P27_GPIO			(0x00UL)
#define 	IO_OUTCFG_P27_TO02			(0x01UL)
#define		IO_OUTCFG_P27_TO03			(0x02UL)
/*----------GPIO_P27输入复用-----------------------------------------------*/
#define		IO_INCFG_P27_INTP0			(0x27UL)
#define		IO_INCFG_P27_INTP1			(0x27UL)
#define		IO_INCFG_P27_INTP2			(0x27UL)
#define		IO_INCFG_P27_INTP3			(0x27UL)
#define		IO_INCFG_P27_TAU0				(0x27UL)
#define		IO_INCFG_P27_TAU1				(0x27UL)
#define		IO_INCFG_P27_TAU2				(0x27UL)
#define		IO_INCFG_P27_TAU3				(0x27UL)

/*----------外部中断输入引脚复用-------------------------------------------------------*/
#define		INTxCFGP00							(0x00UL)
#define		INTxCFGP01							(0x01UL)
#define		INTxCFGP02							(0x02UL)
#define		INTxCFGP03							(0x03UL)
#define		INTxCFGP04							(0x04UL)
#define		INTxCFGP05							(0x05UL)
#define		INTxCFGP06							(0x06UL)
#define		INTxCFGP07							(0x07UL)
#define		INTxCFGP10							(0x10UL)
#define		INTxCFGP11							(0x11UL)
#define		INTxCFGP12							(0x12UL)
#define		INTxCFGP13							(0x13UL)
#define		INTxCFGP14							(0x14UL)
#define		INTxCFGP15							(0x15UL)
#define		INTxCFGP16							(0x16UL)
#define		INTxCFGP20							(0x20UL)
#define		INTxCFGP21							(0x21UL)
#define		INTxCFGP22							(0x22UL)
#define		INTxCFGP23							(0x23UL)
#define		INTxCFGP24							(0x24UL)
#define		INTxCFGP25							(0x25UL)
#define		INTxCFGP26							(0x26UL)
#define		INTxCFGP27							(0x27UL)

#define		EXTINT_Rising							(0x00UL)		//外部中断上升沿触发
#define		EXTINT_Falling						(0x01UL)		//外部中断下降降沿触发
#define		EXTINT_DoublEdge					(0x02UL)		//外部中断双沿触发

#define		EXTINT_INT0						  (0x00UL)
#define		EXTINT_INT1						  (0x01UL)
#define		EXTINT_INT2						  (0x02UL)
#define		EXTINT_INT3						  (0x03UL)
/*****************************************************************************
 ** \brief	GPIO_PinAFOutConfig
 **			设置(设置IO口作为输出的复用功能)
 ** \param [in] PinCFG:	P00CFG ~ P27CFG寄存器
 **				MuxMode:
 ** \return none
 ** \note	 
*****************************************************************************/	
#define GPIO_PinAFOutConfig(PinCFG, MuxMode)	do{\
											PORT->PinCFG = MuxMode;\
											}while(0)

/*****************************************************************************
 ** \brief	GPIO_PinAFInConfig
 **			设置(设置IO口作为输入的复用功能)
 ** \param [in] PinCFG:	INT0CFG ~ CCP1BINCFG寄存器
 **				MuxMode:
 ** \return none
 ** \note	 
*****************************************************************************/	
#define GPIO_PinAFInConfig(PinCFG, MuxMode)	do{\
											PORT->PinCFG = MuxMode;\
											}while(0)											
											

void GPIO_Init(PORT_TypeDef PORTx,PIN_TypeDef PINx,PIN_ModeDef MODEx);
void PORT_SetBit(PORT_TypeDef PORTx,PIN_TypeDef PINx);
void PORT_ClrBit(PORT_TypeDef PORTx,PIN_TypeDef PINx);
void PORT_ToggleBit(PORT_TypeDef PORTx,PIN_TypeDef PINx);
uint8_t PORT_GetBit(PORT_TypeDef PORTx,PIN_TypeDef PINx);
void RESTPinGpio_Set(uint8_t RStat);
											
#ifdef __cplusplus
}
#endif
#endif /* __GPIO_H__ */

