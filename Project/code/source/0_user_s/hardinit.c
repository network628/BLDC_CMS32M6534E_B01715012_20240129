//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    hardinit.c
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

//---------------------------------------------------------------------------/
//	Global variable definitions(declared in header file with 'extern')
//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
//	Local function prototypes('static')
//---------------------------------------------------------------------------/


/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/

/*****************************************************************************
 ** \brief	系统时钟初始化
 **			
 ** \param [in] none
 ** \return  none
 ** \note	64M主频
*****************************************************************************/
void SysClock_Init(void)
{
  SystemCoreClockUpdate();
}

/*****************************************************************************
 ** \brief	GPIO初始化
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PORT_Init(void)
{
  PORT_ClrBit(PORT0,PIN4);
  GPIO_Init( PORT0, PIN4, OUTPUT );
  GPIO_Init( PORT0, PIN3, INPUT );
//	GPIO_Init( PORT0,P IN4, PULLDOWN_INPUT );
	
}

/*****************************************************************************
 ** \brief	EPWM模块初始化，双电阻方案
 **			
 ** \param [in] none
 ** \return  none
 ** \note	除了刹车和重映射之外，其余代码不允许改动
*****************************************************************************/
void EPWM_R2_Init(void)
{
  uint32_t T_CPMTG0, T_CPMTG1;

  /* 
  (1)开启EPWM时钟
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_EPWM, ENABLE );

  /* 
  (2)设置EPWM运行模式
  */
  EPWM_ConfigRunMode( EPWM_COUNT_UP_DOWN      |  /*xx计数模式*/
                      EPWM_OCU_SYMMETRIC      |  /*对称计数模式*/
                      EPWM_WFG_COMPLEMENTARYK |  /*互补模式*/
                      EPWM_OC_INDEPENDENT );     /*独立输出模式*/
  /*
  (3)设置EPWM时钟周期
  */

  EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1 );
  EPWM_ConfigChannelClk( EPWM2, EPWM_CLK_DIV_1 );	
  EPWM_ConfigChannelClk( EPWM4, EPWM_CLK_DIV_1 );	

  EPWM_ConfigChannelPeriod( EPWM0, EPWM_PERIOD );
  EPWM_ConfigChannelPeriod( EPWM2, EPWM_PERIOD );
  EPWM_ConfigChannelPeriod( EPWM4, EPWM_PERIOD );

  EPWM_ConfigChannelSymDuty( EPWM0, EPWM_HALFPERIOD );	
  EPWM_ConfigChannelSymDuty( EPWM2, EPWM_HALFPERIOD );		
  EPWM_ConfigChannelSymDuty( EPWM4, EPWM_HALFPERIOD );

  /*
  (4)设置EPWM反向输出
  */	
  #if 0	
  EPWM_EnableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
                            EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  #else	
  EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
                             EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  #endif

  /*
  (5)设置EPWM死区
  */
  EPWM_EnableDeadZone( 0x3F, (uint32_t)(EPWM_DT*EPWM_Tus) );

  /*
  (6)设置EPWM加载方式
  */
  EPWM_EnableAutoLoadMode( EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK ); /*自动加载*/

  /*
  (7)设置EPWM比较器0
  */			
  T_CPMTG0 = (uint32_t)(EPWM_CPMTG*EPWM_Tus );
  if( T_CPMTG0 < 12  )	T_CPMTG0 = 12; 	/*触发计数器不允许为0*/
  if( T_CPMTG0 > 128 )	T_CPMTG0 = 128; 
  T_CPMTG1 = EPWM_PERIOD - T_CPMTG0;
  EPWM_ConfigCompareTriger( EPWM_CMPTG_0, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM0, T_CPMTG0 );
  EPWM_ConfigCompareTriger( EPWM_CMPTG_1, EPWM_CMPTG_RISING,  EPWM_CMPTG_EPWM0, T_CPMTG1 );

  /*
  (8)设置中断
  */
  //EPWM_EnableZeroInt( EPWM_CH_0_MSK ); /*开启零点中断*/			
  //NVIC_EnableIRQ( EPWM_IRQn );

  /*
  (9)设置EPWM刹车
  */
  EPWM_EnableFaultBrake( EPWM_BRK_ACMP1EE ); /*ACMP1刹车*/

  EPWM_ConfigFaultBrakeLevel( EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK, 0 );
  EPWM_ConfigFaultBrakeLevel( EPWM_CH_1_MSK | EPWM_CH_3_MSK | EPWM_CH_5_MSK, 0 );
  EPWM_AllBrakeEnable();

  Brige_Output_Off();

  /*
  (10)设置IO口输出
  */	
  GPIO_PinAFOutConfig( P10CFG, IO_OUTCFG_P10_EPWM0 ); /*设置P10为EPWM0通道*/
  GPIO_PinAFOutConfig( P11CFG, IO_OUTCFG_P11_EPWM1 ); /*设置P11为EPWM1通道*/
  GPIO_PinAFOutConfig( P12CFG, IO_OUTCFG_P12_EPWM2 ); /*设置P12为EPWM2通道*/
  GPIO_PinAFOutConfig( P13CFG, IO_OUTCFG_P13_EPWM3 ); /*设置P13为EPWM3通道*/
  GPIO_PinAFOutConfig( P14CFG, IO_OUTCFG_P14_EPWM4 ); /*设置P14为EPWM4通道*/
  GPIO_PinAFOutConfig( P15CFG, IO_OUTCFG_P15_EPWM5 ); /*设置P15为EPWM5通道*/

  GPIO_Init( PORT1, PIN0, OUTPUT );
  GPIO_Init( PORT1, PIN1, OUTPUT );
  GPIO_Init( PORT1, PIN2, OUTPUT );
  GPIO_Init( PORT1, PIN3, OUTPUT );
  GPIO_Init( PORT1, PIN4, OUTPUT );
  GPIO_Init( PORT1, PIN5, OUTPUT );

  EPWM_EnableOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
                     EPWM_CH_2_MSK | EPWM_CH_3_MSK |
                     EPWM_CH_4_MSK | EPWM_CH_5_MSK );


  /*
  (11)重映射,客户根据实际应用配置 
  */	
  EPWM->LOCK  = EPWM_LOCK_P1B_WRITE_KEY;
  EPWM->POREMAP = 0xAA531420;
  EPWM->LOCK  = 0x0;

  /*
  (12)在zero点加载占空比
  */	
  EPWM->LOCK  = EPWM_LOCK_P1B_WRITE_KEY;
  EPWM->CON3 |= 0x05550000;
  EPWM->LOCK  = 0x0;

  /*
  (13)开启EPWM
  */		
  EPWM_Start( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
              EPWM_CH_2_MSK | EPWM_CH_3_MSK |
              EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  
}

/*****************************************************************************
 ** \brief	EPWM模块初始化，单电阻方案
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_R1_Init(void)
{
  /* 
  (1)开启EPWM时钟
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_EPWM, ENABLE );

  /* 
  (2)设置EPWM运行模式
  */
  EPWM_ConfigRunMode( EPWM_COUNT_UP_DOWN      |  /*xx计数模式*/
                      EPWM_OCU_ASYMMETRIC     |  /*非对称计数模式*/
                      EPWM_WFG_COMPLEMENTARYK |  /*互补模式*/
                      EPWM_OC_INDEPENDENT );     /*独立输出模式*/

  /*
  (3)设置EPWM时钟周期
  */
  EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1 );
  EPWM_ConfigChannelClk( EPWM2, EPWM_CLK_DIV_1 );	
  EPWM_ConfigChannelClk( EPWM4, EPWM_CLK_DIV_1 );

  EPWM_ConfigChannelPeriod( EPWM0, EPWM_PERIOD );
  EPWM_ConfigChannelPeriod( EPWM2, EPWM_PERIOD );
  EPWM_ConfigChannelPeriod( EPWM4, EPWM_PERIOD) ;

  EPWM_ConfigChannelAsymDuty( EPWM0, EPWM_HALFPERIOD, EPWM_HALFPERIOD );
  EPWM_ConfigChannelAsymDuty( EPWM2, EPWM_HALFPERIOD, EPWM_HALFPERIOD );
  EPWM_ConfigChannelAsymDuty( EPWM4, EPWM_HALFPERIOD, EPWM_HALFPERIOD );

  /*
  (4)设置EPWM反向输出
  */
  #if 0	
  EPWM_EnableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
                            EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  #else	
  EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
                             EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  #endif

  /*
  (5)设置EPWM死区
  */
  EPWM_EnableDeadZone( 0x3F, (uint32_t)(EPWM_DT*EPWM_Tus) );

  /*
  (6)设置EPWM加载方式
  */
  EPWM_EnableAutoLoadMode( EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK ); /*自动加载*/
    
  /*
  (7)设置EPWM比较器0
  */				
  EPWM_ConfigCompareTriger( EPWM_CMPTG_0, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM0, EPWM_HALFPERIOD>>1 );
  EPWM_ConfigCompareTriger( EPWM_CMPTG_1, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM2, EPWM_HALFPERIOD    );

  /*
  (8)设置中断
  */
  //EPWM_EnableZeroInt( EPWM_CH_0_MSK ); /*开启零点中断*/		
  //NVIC_EnableIRQ( EPWM_IRQn );

  /*
  (9)设置EPWM刹车
  */
  EPWM_EnableFaultBrake( EPWM_BRK_ACMP1EE ); /*ACMP1刹车*/

  EPWM_ConfigFaultBrakeLevel( EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK, 0 );
  EPWM_ConfigFaultBrakeLevel( EPWM_CH_1_MSK | EPWM_CH_3_MSK | EPWM_CH_5_MSK, 0 );
  EPWM_AllBrakeEnable();

  Brige_Output_Off();

  /*
  (10)设置IO口输出
  */	
  GPIO_PinAFOutConfig( P10CFG, IO_OUTCFG_P10_EPWM0 ); /*设置P10为EPWM0通道*/
  GPIO_PinAFOutConfig( P11CFG, IO_OUTCFG_P11_EPWM1 ); /*设置P11为EPWM1通道*/
  GPIO_PinAFOutConfig( P12CFG, IO_OUTCFG_P12_EPWM2 ); /*设置P12为EPWM2通道*/
  GPIO_PinAFOutConfig( P13CFG, IO_OUTCFG_P13_EPWM3 ); /*设置P13为EPWM3通道*/
  GPIO_PinAFOutConfig( P14CFG, IO_OUTCFG_P14_EPWM4 ); /*设置P14为EPWM4通道*/
  GPIO_PinAFOutConfig( P15CFG, IO_OUTCFG_P15_EPWM5 ); /*设置P15为EPWM5通道*/

  GPIO_Init( PORT1, PIN0, OUTPUT );
  GPIO_Init( PORT1, PIN1, OUTPUT );
  GPIO_Init( PORT1, PIN2, OUTPUT );
  GPIO_Init( PORT1, PIN3, OUTPUT );
  GPIO_Init( PORT1, PIN4, OUTPUT );
  GPIO_Init( PORT1, PIN5, OUTPUT );

  EPWM_EnableOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
                     EPWM_CH_2_MSK | EPWM_CH_3_MSK |
                     EPWM_CH_4_MSK | EPWM_CH_5_MSK );
            
  /*
  (11)重映射
  */	
  EPWM->LOCK  = EPWM_LOCK_P1B_WRITE_KEY;
  EPWM->POREMAP = 0xAA531420;
  EPWM->LOCK  = 0;

  /*
  (12)在zero点加载占空比
  */	
  EPWM->LOCK  = EPWM_LOCK_P1B_WRITE_KEY;
  EPWM->CON3 |= 0x05550000;
  EPWM->LOCK  = 0x0;

  /*
  (13)开启EPWM
  */					 
  EPWM_Start( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
              EPWM_CH_2_MSK | EPWM_CH_3_MSK |
              EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  
}

/*****************************************************************************
 ** \brief	ADC模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_Init(void)
{
  /*
  (1)开启ADC时钟
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_ADCEN, ENABLE );

  /*
  (2)设置ADC模式
  */
  ADC_ConfigRunMode( ADC_MODE_HIGH, ADC_CONVERT_CONTINUOUS, ADC_CLK_DIV_1, 30 );	

  ADC_ConfigChannelSwitchMode( ADC_SWITCH_HARDWARE ); /*硬件自动切换*/

  /*
  (3)关闭ADC关闭充电\放电功能
  */	
  ADC_DisableChargeAndDischarge();

  /*
  (4)设置ADC参考源
  */
   /* 设置外部5V为ADC参考源 */ 
  ADC_ConfigVREF( ADC_VREFP_VDD ); /*设置参考源*/

  /*
  (5)设置ADC通道使能
  */
  GPIO_Init( PORT1, PIN6, ANALOG_INPUT ); // HV-AD 母线电压采样通道
 
  GPIO_Init( PORT0, PIN2, ANALOG_INPUT ); // IBUS  母线电流采样通道
  GPIO_Init( PORT2, PIN7, ANALOG_INPUT ); // NTC   温度采样通道
  GPIO_Init( PORT0, PIN5, ANALOG_INPUT ); // PWMAD   调速采样通道

  GPIO_Init( PORT2, PIN2, ANALOG_INPUT ); // MCU_BEF_U U相反电动势采样通道
  GPIO_Init( PORT2, PIN0, ANALOG_INPUT ); // MCU_BEF_V V相反电动势采样通道
  GPIO_Init( PORT2, PIN1, ANALOG_INPUT ); // MCU_BEF_W W相反电动势采样通道

  /*
  (6)开启ADC
  */		
  ADC_Start();
  
}

/*****************************************************************************
 ** \brief	PGA0模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  PGA0_Init(void)
{
  /*
  (1)开启PGA0 时钟
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_PGA0EN, ENABLE );

  /*
  (2)设置PGA0输入端IO口
  */		
  GPIO_Init( PORT0, PIN0, ANALOG_INPUT ); //PGA0 +
  GPIO_Init( PORT0, PIN1, ANALOG_INPUT ); //PGA0 -

  /*
  (3)设置PGA0增益
  */
  PGA_ConfigGain( PGA0x, PGA_GAIN_7d5 );

  /*
  (4)设置PGA参考电压
  */		
  PGA_VrefCtrl( PGA0x, PGA0BG );

  /*
  (5)设置PGA0模式 全差分模式
  */		
  PGA_ModeSet( PGA0x, PgaDiffer );

  /*
  (6)设置PGA0输出
  */		
  #if 1	
  //PGA0输出到PAD串联电阻选择 
  PGA0_ConfigResistorPAD( PGA_R_10K );

  //PGA0输出到PAD通道使能
  PGA_EnableOutput( PGA0x );

  //PGA0输出经过滤波通道
  PGA0_Filter( DISABLE );

  GPIO_Init( PORT0, PIN2, ANALOG_INPUT ); //PGA0_OUT
  #endif

  /*
  (7)开启PGA0
  */	
  PGA_Start( PGA0x );
	
}

/*****************************************************************************
 ** \brief	PGA1模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA1_Init(void)
{
  /*
  (1)开启PGA1 时钟
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_PGA1EN, ENABLE );	

  /*
  (2)设置PGA1输入端IO口
  */	
  GPIO_Init( PORT2, PIN4, ANALOG_INPUT ); //PGA1 +
  GPIO_Init( PORT2, PIN5, ANALOG_INPUT ); //PGA1 -

  /*
  (3)设置PGA1增益
  */	
  PGA_ConfigGain( PGA1x, PGA_GAIN_7d5 );

  /*
  (4)设置PGA1参考电压
  */	
  PGA_VrefCtrl( PGA1x, PGA0BG );

  /*
  (5)设置PGA1模式 全差分模式
  */	
  PGA_ModeSet( PGA1x, PgaDiffer );	

  /*
  (6)设置PGA1输出
  */	
  #if (0)		
  PGA_EnableOutput( PGA1x );

  GPIO_Init( PORT0, PIN4, ANALOG_INPUT ); //PGA12_OUT
  #endif

  /*
  (7)开启PGA1
  */	
  PGA_Start( PGA1x );
	
}

/*****************************************************************************
 ** \brief	PGA2模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA2_Init(void)
{
  /*
  (1)开启PGA2 时钟
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_PGA2EN, ENABLE );

  /*
  (2)设置PGA2输入端IO口
  */
  GPIO_Init( PORT2, PIN6, ANALOG_INPUT ); //PGA2 +
  GPIO_Init( PORT2, PIN7, ANALOG_INPUT ); //PGA2 -

  /*
  (3)设置PGA2增益
  */	
  PGA_ConfigGain( PGA2x, PGA_GAIN_7d5 );

  /*
  (4)设置PGA2参考电压
  */	
  PGA_VrefCtrl( PGA1x, PGA0BG );

  /*
  (5)设置PGA2模式 全差分模式
  */	
  PGA_ModeSet( PGA2x, PgaDiffer );	

  /*
  (6)设置PGA2输出
  */	
  #if 0
  PGA_EnableOutput( PGA2x );	

  GPIO_Init( PORT0, PIN4, ANALOG_INPUT ); //PGA12_OUT

  #endif

  /*
  (7)开启PGA2
  */	
  PGA_Start( PGA2x );
	
}

/*****************************************************************************
 ** \brief	DAC配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DAC_Init(void)
{
  uint8_t bLDOENFlag, bLDOVoltageValue;
  uint16_t hDACSetValue;

  // 获取ADCLDO使能位
  bLDOENFlag = (ADCLDO->CON0 >> 8) & 0x01;
  // 获取ADCLDO输出电压	
  bLDOVoltageValue = ADCLDO->CON0 & 0xff;

  if(!bLDOENFlag) //未使能，5V
  {
    hDACSetValue = (FAULT_HARD_OVER_CURRENT * HW_IBUS_RES * HW_AMP_GAIN_IBUS + HW_AMP_REF_IBUS) * 255 / 5;
  }
  else
  {
    if(bLDOVoltageValue == 0x55) //4.2V
    {
      hDACSetValue = (FAULT_HARD_OVER_CURRENT * HW_IBUS_RES * HW_AMP_GAIN_IBUS + HW_AMP_REF_IBUS) * 2550 / 42;
    }	
    else //3.6V
    {
      hDACSetValue = (FAULT_HARD_OVER_CURRENT * HW_IBUS_RES * HW_AMP_GAIN_IBUS + HW_AMP_REF_IBUS) * 2550 / 36;
    }
  }

  if(hDACSetValue > 255)
    hDACSetValue = 255;

  /*
  (1)开启DAC模块时钟
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_DAC, ENABLE );

  /*
  (2)设置DAC输入值
  */
  DAC_ConfigInput( hDACSetValue );

  /*
  (3)使能DAC输出
  */
  DAC_Start();

}

/*****************************************************************************
 ** \brief	ACMP0模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	用于EPWM刹车保护
*****************************************************************************/
void ACMP0_Init(void)
{
  /*
  (1)开启ACMP0模块时钟
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_ACMPEN, ENABLE );

  /*
  (2)设置ACMP0正端输入
  */
  ACMP_ConfigPositive( ACMP0, ACMP_POSSEL_0PGA2O );

  /*
  (3)设置ACMP0负端输入
  */
  ACMP_ConfigNegative( ACMP0, ACMP_NEGSEL_BEF_N );

  /*
  (4)设置负端参考电压
  */ 	
  DAC_Init();

  /*
  (5)设置ACMP0输出滤波时间
  */ 
  ACMP_Filter_Config( ACMP0, ENABLE, ACMP_NGCLK_65_TSYS );

  /*
  (6)设置ACMP0输出极性
  */ 
  ACMP_Polarity_Config( ACMP0, ACMP_POL_Pos );

  /*
  (7)设置ACMP0迟滞电压
  */ 
  ACMP_EnableHYS( ACMP0, ACMP_HYS_POS, ACMP_HYS_S_20 );

  /*
  (8)设置ACMP0事件产生条件
  */ 
  ACMP_ConfigEventAndIntMode( ACMP0, ACMP_EVENT_INT_RISING );

  /*
  (9)开启ACMP0事件输出
  */ 
  ACMP_EnableEventOut( ACMP0 );

  /*
  (11)使能ACMP0输出
  */   
  ACMPOut_Enable( ACMP0 );

  /*
  (10)开启ACMP0
  */ 
  ACMP_Start( ACMP0 );

}

/*****************************************************************************
 ** \brief	ACMP1模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	用于EPWM刹车保护
*****************************************************************************/
void ACMP1_Init(void)
{
  /*
  (1)开启ACMP1模块时钟
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_ACMPEN, ENABLE );

  /*
  (2)设置ACMP1正端输入
  */
  ACMP_ConfigPositive( ACMP1, ACMP_POSSEL_1PGA0O );

  /*
  (3)设置ACMP1负端输入
  */
  ACMP_ConfigNegative( ACMP1, ACMP_NEGSEL_DAC_O );

  /*
  (4)设置负端参考电压
  */ 	
  DAC_Init();

  /*
  (5)设置ACMP1输出滤波时间
  */ 
  ACMP_Filter_Config( ACMP1, ENABLE, ACMP_NGCLK_65_TSYS );

  /*
  (6)设置ACMP1输出极性
  */ 
  ACMP_Polarity_Config( ACMP1, ACMP_POL_Pos );	

  /*
  (7)设置ACMP1迟滞电压
  */ 
  ACMP_EnableHYS( ACMP1, ACMP_HYS_POS, ACMP_HYS_S_20 );

  /*
  (8)设置ACMP1事件产生条件
  */ 
  ACMP_ConfigEventAndIntMode( ACMP1, ACMP_EVENT_INT_RISING );

  /*
  (9)开启ACMP1事件输出
  */ 
  ACMP_EnableEventOut( ACMP1 );

  /*
  (11)使能ACMP1输出
  */   
  ACMPOut_Enable( ACMP1 );

  /*
  (10)开启ACMP1
  */ 
  ACMP_Start( ACMP1 );
  
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :CCP捕获模式0配置
* Description    :需要将相应的IO配置成CCP口，其它不需要改动
* Function Call  :程序初始化调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void CCP_Capture_Init(void)
{
  #if 0
  /*
  (1)设置CCP的时钟
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_CCP, ENABLE ); /*开启CCP模块时钟*/

  #if (Speed_Control_Mode == PWM_Control)
    CCP_ConfigCLK( CCP_CAPTURE_MODULE, CCP_CLK_DIV_4, CCP_RELOAD_CCPLOAD, 0xffff ); /*Fahb=64M*/	
  #else
    CCP_ConfigCLK( CCP_CAPTURE_MODULE, CCP_CLK_DIV_64,  CCP_RELOAD_CCPLOAD, 0xffff ); /*Fahb=64M*/		
  #endif			

  /*
  (2)设置CCP运行模式
  */
  CCP_EnableCAPMode0( CCP_CAPTURE_MODULE );	
  CCP_ConfigCAPMode0( CCP_CAPTURE_MODULE, CCP_CAP_MODE0_RISING_TO_FALLING );	/*设置CCP的捕获模式0 的捕获方式为上升沿到下降沿(高电平时间)*/
                          /*上升/下降沿捕获：CCP模块使能(CCPRUN =1)后，计数值从4800(加载值)向下计数，捕获条件产生，停止计数，并锁存当前的计数值*/
                          /*上升到下降沿捕获：CCP模块使能(CCPRUN =1)后，当出现上升时计数值从4800(加载值)向下计数，捕获条件(下降沿)产生，停止计数，并锁存当前的计数值*/
                          /*下降到上升沿捕获：CCP模块使能(CCPRUN =1)后，当出现下降沿时计数值从4800(加载值)向下计数，捕获条件(上升沿)产生，停止计数，并锁存当前的计数值*/	
  /*
  (3)设置CCP捕获通道
  */
  CCP_SelCAPMode0Channel( CCP_CAPTURE_MODULE, CCP_CAPTURE_CH ); /*设置捕获通道*/

  GPIO_PinAFInConfig( CCP1AINCFG, 0x01 ); /*设置P04为捕获通道*/
  GPIO_Init( PORT0, PIN4,INPUT );	

  /*
  (4)开启CCP模块
  */	
  CCP_EnableRun( CCP_CAPTURE_MODULE );			
  CCP_Start( CCP_CAPTURE_MODULE ); /*CCP模块使能*/

  #else

  /*
  (1)设置CCP的时钟
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_CCP, ENABLE ); /*开启CCP模块时钟*/

  #if (Speed_Control_Mode == PWM_Control)
    CCP_ConfigCLK( CCP_CAPTURE_MODULE, CCP_CLK_DIV_4, CCP_RELOAD_CCPLOAD, 0xffff ); /*Fahb=64M*/	
  #else
    CCP_ConfigCLK( CCP_CAPTURE_MODULE, CCP_CLK_DIV_64,  CCP_RELOAD_CCPLOAD, 0xffff ); /*Fahb=64M*/		
  #endif		

  /*
  (2)设置CCP运行模式
  */
  CCP_EnableCAPMode2();
  CCP_ConfigCAPMode2( CAP1, CCP_CAP_MODE2_RISING  ); /* 上升沿触发 */
  CCP_ConfigCAPMode2( CAP2, CCP_CAP_MODE2_FALLING ); /* 下降沿触发 */
  CCP_ConfigCAPMode2( CAP3, CCP_CAP_MODE2_RISING  ); /* 上升沿触发 */

  /*
  (3)设置CCP捕获通道
  */
  CCP_SelCAPMode2Channel( CAP1_CCP0B ); /*设置捕获通道*/

  GPIO_PinAFInConfig( CCP0BINCFG, IO_INCFG_P04_CCP0B_I ); /*设置P04为CCP1A捕获通道*/
  GPIO_Init( PORT0, PIN4, INPUT );

  /*
  (4)开启CCP模块
  */	
  CCP_EnableRun( CCP1 );			
  CCP_Start( CCP1 ); /*CCP模块使能*/
  #endif
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :CCP PWM模块配置
* Description    :需要将相应的IO配置成CCP口，其它不需要改动
* Function Call  :程序初始化调用
* Input Paragram :无
* Return Value   :无
*-----------------------------------------------------------------------------
******************************************************************************/
void CCP_PWM_Init(void)
{
  /*
  (1)设置CCP的时钟
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_CCP, ENABLE ); /*开启CCP模块时钟*/
  CCP_ConfigCLK( CCP_PWM_MODULE, CCP_CLK_DIV_64, CCP_RELOAD_CCPLOAD, 10 ); /*Fahb=64M*/	

  /*
  (2)设置CCP运行模式
  */
  CCP_EnablePWMMode( CCP_PWM_MODULE );

  /*
  (3)设置CCP 比较值
  */	
  CCP_ConfigCompare( CCP_PWM_MODULE, CCP_PWM_CH, 5 ); /*50%*/

  /*
  (4)设置CCP 反相输出
  */		
  CCP_DisableReverseOutput( CCP_PWM_MODULE, CCP_PWM_CH );

  /*
  (5)设置CCP IO口复用
  */		
  GPIO_PinAFOutConfig( P26CFG, IO_OUTCFG_P26_CCP0A_O ); /*设置P26为CCP0A FG输出通道*/
  GPIO_Init( PORT2, PIN6, OUTPUT );

  /*
  (5)设置FG输出最小转速
  */		
  Capture.FG_SpeedMin = (uint16_t)(60 * 1000000 / (MOTOR_PAIRS * 65000)) + 1;
	
}

/*****************************************************************************
 ** \brief	CCP_PWM_GPIO
 **			
 ** \param [in] bMode  0:IO口配置为CCP模式  1:IO口配置为GPIO输出模式，并输出为低
 **           	bMode  0:GPIO输出模式，输出为低  1:GPIO输出模式，输出为高
 ** \return  none
 ** \note	FG信号处理
*****************************************************************************/
void CCP_PWM_GPIO(uint8_t bMode, uint8_t bLevle)
{
  if(0 == bMode )
  {
    GPIO_PinAFOutConfig( P26CFG, IO_OUTCFG_P26_CCP0A_O ); /*设置P26为CCP0A FG输出通道*/
    GPIO_Init( PORT2, PIN6, OUTPUT );	
  }
  else if( 1 == bMode )
  {
    if( 1 == bLevle )
      PORT_SetBit( PORT2, PIN6 );
    else
      PORT_ClrBit( PORT2, PIN6 );
    
    GPIO_PinAFOutConfig( P26CFG, IO_OUTCFG_P26_GPIO );
    GPIO_Init( PORT2, PIN6, OUTPUT );	
  }
}

/*****************************************************************************
 ** \brief	硬件触发器配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	无需改动
*****************************************************************************/
void HWDIV_Init(void)
{
  /*
  (1)开启HWDIV模块时钟
  */	
  CGC_PER12PeriphClockCmd( CGC_PER12Periph_DIV, ENABLE );

  /*
  (2)使能除法功能
  */		
  DIVSQRT_EnableDIVMode();

  /*
  (3)使能有符号除法器功能
  */		
  DIVSQRT_EnableSingedMode();

}

/*****************************************************************************
 ** \brief	UART模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	该配置主要用于调试，用户可配置为通信
*****************************************************************************/
void UART0_Init(void)
{
  uint32_t  BuadRate;

  // BuadRate = 300000;
  BuadRate = 9600;
    
  /*
  (1)开启UARTx时钟
  */
  CGC_PER12PeriphClockCmd( CGC_PER12Periph_UART, ENABLE );

  /*
  (2)设置UARTx模式
  */	
  UART_ConfigRunMode( UART0, BuadRate, UART_WLS_8, UART_PARITY_NONE, UART_STOP_BIT_1 );
  UART0->END = 0x0;

  /*
  (3)开启UARTx输出
  */			
  GPIO_PinAFOutConfig( P04CFG, IO_OUTCFG_P04_TXD );	
  GPIO_Init( PORT0, PIN4, OUTPUT );
		
}

/*****************************************************************************
 ** \brief	SysTick_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	无需改动
*****************************************************************************/
void SysTick_Init(void)
{
  // 系统定时器--1ms时间配置
  SysTick->LOAD  = (uint32_t)(MCU_CLK/1000 - 1UL);
  SysTick->VAL   = 0UL;
  SysTick->CTRL  = 0x00000005;
	
}

/*****************************************************************************
 ** \brief	TMR0模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	用于hall平均角速度计算，不允许改动
*****************************************************************************/
void TMR0_Iint(void)
{
  /*
  (1)开启TMR0的时钟
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_TIMER01, ENABLE ); /*打开Timer0的时钟*/
  TMR_ConfigClk( TMR0, TMR_CLK_DIV_16 ); /*4MHz*/

  /*
  (2)设置TMR0运行模式
  */	
  TMR_ConfigRunMode( TMR0, TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE );
  TMR_DisableOneShotMode( TMR0);

  /*
  (3)设置TMR0运行周期
  */		
  TMR_SetPeriod( TMR0, 40000); /* 4us*12500=50ms*/

  /*
  (4)设置TMR0中断
  */	
  TMR_EnableOverflowInt( TMR0 );

  /*
  (5)开启TMR0
  */	
  TMR_Start( TMR0 );
	
}

/*****************************************************************************
 ** \brief	TMR1模块配置
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR1_Init(void)
{
  /*
  (1)开启TMR1的时钟
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_TIMER01, ENABLE ); /*打开Timer0的时钟*/
  TMR_ConfigClk( TMR1, TMR_CLK_DIV_1 ); /*64MHz*/

  /*
  (2)设置TMR1运行模式
  */	
  TMR_ConfigRunMode( TMR1,TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE );
  TMR_DisableOneShotMode( TMR1 );

  /*
  (3)设置TMR1运行周期
  */		
  TMR_SetPeriod( TMR1, 0xffffffff ); // 100us = 6400    4000

  /*
  (4)设置TMR 中断
  */	
  TMR_EnableOverflowInt( TMR1 );

  /*
  (5)开启TMR1
  */	
  TMR_Start( TMR1 );
	
}

/*****************************************************************************
 ** \brief	ADCLDO_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	无需改动
*****************************************************************************/
void ADCLDO_Init(void)
{
  /*
  (1)开启ADCLDO的时钟
  */	
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_ADCLDO, ENABLE );

  /*
  (2)设置ADCLDO输出电压
  */		
  ADCLDO_OutVlotageSel( ADCLDO_OutV_4d2 );

  /*
  (3)使能ADCLDO模块
  */		
  ADCLDO_Disable();
	
}

/*****************************************************************************
 ** \brief	系统的初始化配置，保护模块初始化，变量参数初始化等
 **			
 ** \param [in] none
 ** \return  none
 ** \note		基本的模块初始化顺序不允许改动，允许增加其他模块功能
*****************************************************************************/
void System_Init(void)
{
  // 系统始终配置
  SysClock_Init();

  // 禁用P02复位功能
  RESTPinGpio_Set( ENABLE );

  // 调试时打开，避免上电后把仿真口刷新烧不了程序，正常模式改为300ms
  DelayTime_ms( POWERON_DELAY_TIME );	

  // 输入输出IO口线配置
  PORT_Init();
    
  // ADCLDO配置
  ADCLDO_Init();

  // ADC模块配置
  ADC_Init();

  // 单电阻模式下PWM模块配置
  #if ( Config_Shunt_Mode == Single_Shunt )	
    EPWM_R1_Init();
  #endif

  // 双电阻模式下PWM模块配置
  #if ( Config_Shunt_Mode == Double_Shunt )
    EPWM_R2_Init();
  #endif

  // PGA模块配置
  #if ( PGA_CH == PGA_CH0 )
    PGA0_Init();	
  #elif ( PGA_CH == PGA_CH12 )	
    PGA1_Init();
    PGA2_Init();	
  #elif ( PGA_CH == PGA_CH012 )	
    PGA0_Init();
    PGA1_Init();
    PGA2_Init();	
  #endif

  // 比较器ACMP0模块配置
  #if ( ACMP_CH == ACMP_CH0 )
    ACMP0_Init();
  #endif

  // 比较器ACMP1模块配置
  #if ( ACMP_CH == ACMP_CH1 )
    ACMP1_Init();	
  #endif

  // 调速捕捉模块配置
  #if ( Config_CCP_Capture == CCP_Capture_Enable )
    CCP_Capture_Init();		
  #endif	

  // 速度反馈--FG输出模块配置
  #if ( Config_CCP_PWM == CCP_PWM_Enable )
    CCP_PWM_Init();
    // 禁用SWO调试接口功能 (复用烧录口线需要打开)
  //		DBG->DBGSTOPCR |= (1<<24);
  #endif

  // 硬件触发器配置
  HWDIV_Init();

  // 串口通信配置--调试时使用
  #if ( UART_CONFIG_MODE == UART_ON )
    UART0_Init();
  #endif

  // 系统定时器--1ms时间片
  SysTick_Init();

  // 定时器TMR0模块配置
  //TMR0_Iint();

  // 定时器TMR1模块配置

  //TMR1_Init();

  // 控制参数初始化
  MC_Para_Init();

  // 延时等待ADC采样
  DelayTime_ms( 130 );

  //配置ADC采样触发模式
  ADC_TGSAMP_CONFIG();
	
}

/******************************** END OF FILE *******************************/
