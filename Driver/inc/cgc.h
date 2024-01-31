/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CGC_H_
#define __CGC_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#define	SYSTYEM_CLOCK_CLOSE		0
#define	SYSTYEM_CLOCK_OPEN		1

#define IS_CGC_PER0_PERIPH(PERIPH) ((((PERIPH) & 0xFF) != 0x00) && ((PERIPH) != 0x00))

#define IS_CGC_PER11_PERIPH(PERIPH) ((((PERIPH) & 0xAC) != 0x00) && ((PERIPH) != 0x00))
#define IS_CGC_PER12_PERIPH(PERIPH) ((((PERIPH) & 0xAC) != 0x00) && ((PERIPH) != 0x00))
#define IS_CGC_PER13_PERIPH(PERIPH) ((((PERIPH) & 0xAC) != 0x00) && ((PERIPH) != 0x00))


#define CGC_PER0Periph_TIM4             ((uint8_t)0x01) /*!< CGC PER0: TIM4EN (Bit 0)       */
#define CGC_PER0Periph_LSITIMER         ((uint8_t)0x80) /*!< CGC PER0: LSITIMEREN  (Bit 7)       */


#define CGC_PER11Periph_TIMER01         ((uint8_t)0x01) /*!< CGC PER1: TIMER01EN   (Bit 0)     */
#define CGC_PER11Periph_EPWM            ((uint8_t)0x08) /*!< CGC PER1: EPWMEN    (Bit 3)     */
#define CGC_PER11Periph_CCP           	((uint8_t)0x10) /*!< CGC PER1: CCPEN (Bit 4)     */


#define CGC_PER12Periph_DIV             ((uint8_t)0x01) /*!< CGC PER2: DIVEN   (Bit 0)     */
#define CGC_PER12Periph_SPI             ((uint8_t)0x02) /*!< CGC PER2: SPIEN    (Bit 1)     */
#define CGC_PER12Periph_UART            ((uint8_t)0x04) /*!< CGC PER2: UARTEN (Bit 2)     */
#define CGC_PER12Periph_IIC             ((uint8_t)0x10) /*!< CGC PER2: IICEN    (Bit 4)     */


#define CGC_PER13Periph_ADCEN           ((uint8_t)0x01) /*!< CGC PER3: ADCEN (Bit 0)       */
#define CGC_PER13Periph_ACMPEN          ((uint8_t)0x02) /*!< CGC PER3: ACMPEN (Bit 1)       */
#define CGC_PER13Periph_PGA0EN          ((uint8_t)0x08) /*!< CGC PER3: PGA0EN (Bit 3)       */
#define CGC_PER13Periph_PGA1EN           ((uint8_t)0x10) /*!< CGC PER3: PGA1EN (Bit 4)       */
#define CGC_PER13Periph_PGA2EN           ((uint8_t)0x10) /*!< CGC PER3: PGA2EN (Bit 4)       */
#define CGC_PER13Periph_DAC           	((uint8_t)0x20) /*!< CGC PER3: DACEN (Bit 5)     */
#define CGC_PER13Periph_ADCLDO          ((uint8_t)0x40) /*!< CGC PER3: ADCLDOEN    (Bit 6)     */



void CGC_PER0PeriphClockCmd(uint8_t CGC_PER0Periph, FunctionalState NewState);
void CGC_PER11PeriphClockCmd(uint32_t CGC_PER11Periph, FunctionalState NewState);
void CGC_PER12PeriphClockCmd(uint32_t CGC_PER12Periph, FunctionalState NewState);
void CGC_PER13PeriphClockCmd(uint32_t CGC_PER13Periph, FunctionalState NewState);

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/*
    System clock control register (CKC) 
*/
/* Selection of CPU/peripheral hardware clock (fCLK) (CSS) */
#define _00_CGC_MAINCLK_SELECTED                   (0x00U) /* main system clock (fMAIN) */
#define _40_CGC_MAINCLK_FIL                        (0x40U) /* subsystem clock (fIL) */
/* Status of CPU/peripheral hardware clock (fCLK) (CLS) */
#define _00_CGC_MAINCLK_STATUS                     (0x00U) /* main system clock (fMAIN) */
#define _80_CGC_MAINCLK_STATUS                     (0x80U) /* subsystem clock (fIL) */

/*
    Clock operation status control register (CSC) 
*/
/* High-speed on-chip oscillator clock operation control (HIOSTOP) */
#define _00_CGC_HIO_OPER                           (0x00U) /* high-speed on-chip oscillator operating */
#define _01_CGC_HIO_STOP                           (0x01U) /* high-speed on-chip oscillator stopped */

/*
    Oscillation stabilization time counter status register (OSTC) 
*/
/* oscillation stabilization time status (MOST8,MOST9,MOST10,MOST11,MOST13,MOST15,MOST17,MOST18) */
#define _00_CGC_OSCSTAB_STA0                       (0x00U) /* 2^8/fX max. */
#define _80_CGC_OSCSTAB_STA8                       (0x80U) /* 2^8/fX */
#define _C0_CGC_OSCSTAB_STA9                       (0xC0U) /* 2^9/fX */
#define _E0_CGC_OSCSTAB_STA10                      (0xE0U) /* 2^10/fX */
#define _F0_CGC_OSCSTAB_STA11                      (0xF0U) /* 2^11/fX */
#define _F8_CGC_OSCSTAB_STA13                      (0xF8U) /* 2^13/fX */
#define _FC_CGC_OSCSTAB_STA15                      (0xFCU) /* 2^15/fX */
#define _FE_CGC_OSCSTAB_STA17                      (0xFEU) /* 2^17/fX */
#define _FF_CGC_OSCSTAB_STA18                      (0xFFU) /* 2^18/fX */

/*
    Oscillation stabilization time select register (OSTS) 
*/
/* oscillation stabilization time selection (OSTS2,OSTS1,OSTS0) */
#define _00_CGC_OSCSTAB_SEL8                       (0x00U) /* 2^8/fX */
#define _01_CGC_OSCSTAB_SEL9                       (0x01U) /* 2^9/fX */
#define _02_CGC_OSCSTAB_SEL10                      (0x02U) /* 2^10/fX */
#define _03_CGC_OSCSTAB_SEL11                      (0x03U) /* 2^11/fX */
#define _04_CGC_OSCSTAB_SEL13                      (0x04U) /* 2^13/fX */
#define _05_CGC_OSCSTAB_SEL15                      (0x05U) /* 2^15/fX */
#define _06_CGC_OSCSTAB_SEL17                      (0x06U) /* 2^17/fX */
#define _07_CGC_OSCSTAB_SEL18                      (0x07U) /* 2^18/fX */

/*
    Subsystem clock supply mode control register (OSMC) 
*/
/* Setting in STOP mode or HALT mode while subsystem clock is selected as CPU clock (RTCLPC) */
#define _00_CGC_CLK_ENABLE                         (0x00U) /* enables supply of subsystem clock to peripheral functions */
#define _80_CGC_CLK_STOP                           (0x80U) /* stops supply of subsystem clock to peripheral functions  */
/* Selection of operation clock for real-time clock, 12-bit interval timer, and timer RJ (WUTMMCK0) */
#define _00_CGC_TMRJ_IT_CLK_SUBSYSTEM_CLK          (0x00U) /* subsystem clock */
#define _10_CGC_TMRJ_IT_CLK_FIL                    (0x10U) /* low-speed on-chip oscillator clock  */

/*
    PLL Control Register (PLLCR)
*/
/* PLL Operation Control (PLLON[0:0]) */
#define _00_CGC_PLL_OPER            (0x00U) /* PLL operation */
#define _01_CGC_PLL_STOP            (0x01U) /* PLL Stop */
/* Frequency Multiplication Factor Select (PLLMUL[1:1]) */
#define _00_CGC_PLL_MUL_12_0        (0x00U) /* x12 */
#define _02_CGC_PLL_MUL_16_0        (0x02U) /* x16 */
/* PLL Input Frequency Division Ratio Select (PLIDIV[3:2]) */
#define _00_CGC_PLL_DIV_1           (0x00U) /* x1 */
#define _04_CGC_PLL_DIV_2           (0x04U) /* x1/2 */
#define _08_CGC_PLL_DIV_4           (0x08U) /* x1/4 */
/* PLL Source Select (PLLSRSEL) */
#define _00_CGC_PLLSR_fIH           (0x00U) /* fIH is selected for PLL source */
#define _80_CGC_PLLSR_fMX           (0x80U) /* fMX is selected for PLL source */
/*
    USB-dedicated PLL Control Register (UPLLCR)
*/
/* USB-dedicated PLL Operation Control (UPLLON[0:0]) */
#define _00_CGC_UPLL_OPER            (0x00U) /* UPLL operation */
#define _01_CGC_UPLL_STOP            (0x01U) /* UPLL Stop */
/* Frequency Multiplication Factor Select (UPLLMUL[1:1]) */
#define _00_CGC_UPLL_MUL_12_0        (0x00U) /* x12 */
#define _02_CGC_UPLL_MUL_16_0        (0x02U) /* x16 */
/* PLL Input Frequency Division Ratio Select (UPLIDIV[3:2]) */
#define _00_CGC_UPLL_DIV_1           (0x00U) /* x1 */
#define _04_CGC_UPLL_DIV_2           (0x04U) /* x1/2 */
#define _08_CGC_UPLL_DIV_4           (0x08U) /* x1/4 */
/* USB-Dedicated PLL Source Select (UPLLSRSEL) */
#define _00_CGC_UPLLSR_fIH           (0x00U) /* fIH is selected for UCLK PLL source */
#define _80_CGC_UPLLSR_fMX           (0x80U) /* fMX is selected for UCLK PLL source */

#define RESF_Src_LVD           	(0x01U) /* system reset is caused by LVD */
#define RESF_Src_IAW           	(0x02U) /* system reset is caused by Illegal storage */
#define RESF_Src_RAMP           (0x04U) /* system reset is caused by RAM Parity */
#define RESF_Src_WDT           	(0x10U) /* system reset is caused by WDT */
#define RESF_Src_SYSR           (0x80U) /* system reset is caused by reset register */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CGC_SUBWAITTIME               (360U)   /* change the waiting time according to the system */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

typedef enum
{
	OSC_PORT,	   //端口模式
	OSC_OSCILLATOR,//振荡模式
	OSC_EXCLK	   //外部时钟输入模式
} OSC_Pin_Mode_t;
#define IS_CGC_LSE_MODE(MODE)  (((MODE) == OSC_PORT)  || ((MODE) == OSC_OSCILLATOR) || ((MODE) == OSC_EXCLK))

typedef enum
{
	MAINCLK_FIH = 0, //高速内部振荡时钟
	MAINCLK_FIL = 1, //低速内部振荡时钟

} CLK_Select_t;

typedef enum
{ 
	OSC_LOW_POWER = 0,	//低功耗
	OSC_NORMAL_POWER,//普通功耗
	OSC_ULTRA_LOW_POWER //超低功耗
} OSC_Power_Mode_t;
#define IS_CGC_LSE_PWR_MODE(PWR)  (((PWR) == OSC_LOW_POWER)  || ((PWR) == OSC_NORMAL_POWER) || ((PWR) == OSC_ULTRA_LOW_POWER))

typedef enum
{
	OSC_UNDER_10M,
	OSC_OVER_10M,
} OSC_Speed_Mode_t;
#define IS_CGC_HSE_OSC_SPEED(SPEED)  (((SPEED) == OSC_UNDER_10M)  || ((SPEED) == OSC_OVER_10M))


typedef enum
{
    PLL_DIV_1,
    PLL_DIV_2,
    PLL_DIV_4
} PLL_Div_t;

typedef enum
{
    PLL_MUL_12,
    PLL_MUL_16,
} PLL_Mul_t;

typedef enum
{
    PLL_SR_fIH,
    PLL_SR_fMX,
} PLL_Src_t;


/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void CGC_MainOsc_Setting(OSC_Pin_Mode_t main, OSC_Speed_Mode_t amph);
void CGC_SubOsc_Setting(OSC_Pin_Mode_t sub, OSC_Power_Mode_t amphs);
void CGC_PLL_Setting(PLL_Src_t src,PLL_Div_t div, PLL_Mul_t mul);
void CGC_UPLL_Setting(PLL_Src_t src, PLL_Div_t div, PLL_Mul_t mul);

void CGC_SubOsc_Start(void);
void CGC_Hoco_Stop(void);
void CGC_Hoco_Start(void);

void CGC_LSI_Enable(void);
void CGC_LSI_Disable(void);
extern uint8_t USE_HSE_SYSTYEM_CLOCK;
void CGC_LSI_CFG_AS_FCLK(void);
void CGC_HSI_CFG_AS_FCLK(void);
void CGC_LSITIMERCLK_Enable(void);
uint32_t Reset_Status_Get(void);
#endif

