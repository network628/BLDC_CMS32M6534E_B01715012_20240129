//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    hardinit.c
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
 ** \brief	ϵͳʱ�ӳ�ʼ��
 **			
 ** \param [in] none
 ** \return  none
 ** \note	64M��Ƶ
*****************************************************************************/
void SysClock_Init(void)
{
  SystemCoreClockUpdate();
}

/*****************************************************************************
 ** \brief	GPIO��ʼ��
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
 ** \brief	EPWMģ���ʼ����˫���跽��
 **			
 ** \param [in] none
 ** \return  none
 ** \note	����ɲ������ӳ��֮�⣬������벻����Ķ�
*****************************************************************************/
void EPWM_R2_Init(void)
{
  uint32_t T_CPMTG0, T_CPMTG1;

  /* 
  (1)����EPWMʱ��
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_EPWM, ENABLE );

  /* 
  (2)����EPWM����ģʽ
  */
  EPWM_ConfigRunMode( EPWM_COUNT_UP_DOWN      |  /*xx����ģʽ*/
                      EPWM_OCU_SYMMETRIC      |  /*�ԳƼ���ģʽ*/
                      EPWM_WFG_COMPLEMENTARYK |  /*����ģʽ*/
                      EPWM_OC_INDEPENDENT );     /*�������ģʽ*/
  /*
  (3)����EPWMʱ������
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
  (4)����EPWM�������
  */	
  #if 0	
  EPWM_EnableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
                            EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  #else	
  EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
                             EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  #endif

  /*
  (5)����EPWM����
  */
  EPWM_EnableDeadZone( 0x3F, (uint32_t)(EPWM_DT*EPWM_Tus) );

  /*
  (6)����EPWM���ط�ʽ
  */
  EPWM_EnableAutoLoadMode( EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK ); /*�Զ�����*/

  /*
  (7)����EPWM�Ƚ���0
  */			
  T_CPMTG0 = (uint32_t)(EPWM_CPMTG*EPWM_Tus );
  if( T_CPMTG0 < 12  )	T_CPMTG0 = 12; 	/*����������������Ϊ0*/
  if( T_CPMTG0 > 128 )	T_CPMTG0 = 128; 
  T_CPMTG1 = EPWM_PERIOD - T_CPMTG0;
  EPWM_ConfigCompareTriger( EPWM_CMPTG_0, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM0, T_CPMTG0 );
  EPWM_ConfigCompareTriger( EPWM_CMPTG_1, EPWM_CMPTG_RISING,  EPWM_CMPTG_EPWM0, T_CPMTG1 );

  /*
  (8)�����ж�
  */
  //EPWM_EnableZeroInt( EPWM_CH_0_MSK ); /*��������ж�*/			
  //NVIC_EnableIRQ( EPWM_IRQn );

  /*
  (9)����EPWMɲ��
  */
  EPWM_EnableFaultBrake( EPWM_BRK_ACMP1EE ); /*ACMP1ɲ��*/

  EPWM_ConfigFaultBrakeLevel( EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK, 0 );
  EPWM_ConfigFaultBrakeLevel( EPWM_CH_1_MSK | EPWM_CH_3_MSK | EPWM_CH_5_MSK, 0 );
  EPWM_AllBrakeEnable();

  Brige_Output_Off();

  /*
  (10)����IO�����
  */	
  GPIO_PinAFOutConfig( P10CFG, IO_OUTCFG_P10_EPWM0 ); /*����P10ΪEPWM0ͨ��*/
  GPIO_PinAFOutConfig( P11CFG, IO_OUTCFG_P11_EPWM1 ); /*����P11ΪEPWM1ͨ��*/
  GPIO_PinAFOutConfig( P12CFG, IO_OUTCFG_P12_EPWM2 ); /*����P12ΪEPWM2ͨ��*/
  GPIO_PinAFOutConfig( P13CFG, IO_OUTCFG_P13_EPWM3 ); /*����P13ΪEPWM3ͨ��*/
  GPIO_PinAFOutConfig( P14CFG, IO_OUTCFG_P14_EPWM4 ); /*����P14ΪEPWM4ͨ��*/
  GPIO_PinAFOutConfig( P15CFG, IO_OUTCFG_P15_EPWM5 ); /*����P15ΪEPWM5ͨ��*/

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
  (11)��ӳ��,�ͻ�����ʵ��Ӧ������ 
  */	
  EPWM->LOCK  = EPWM_LOCK_P1B_WRITE_KEY;
  EPWM->POREMAP = 0xAA531420;
  EPWM->LOCK  = 0x0;

  /*
  (12)��zero�����ռ�ձ�
  */	
  EPWM->LOCK  = EPWM_LOCK_P1B_WRITE_KEY;
  EPWM->CON3 |= 0x05550000;
  EPWM->LOCK  = 0x0;

  /*
  (13)����EPWM
  */		
  EPWM_Start( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
              EPWM_CH_2_MSK | EPWM_CH_3_MSK |
              EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  
}

/*****************************************************************************
 ** \brief	EPWMģ���ʼ���������跽��
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_R1_Init(void)
{
  /* 
  (1)����EPWMʱ��
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_EPWM, ENABLE );

  /* 
  (2)����EPWM����ģʽ
  */
  EPWM_ConfigRunMode( EPWM_COUNT_UP_DOWN      |  /*xx����ģʽ*/
                      EPWM_OCU_ASYMMETRIC     |  /*�ǶԳƼ���ģʽ*/
                      EPWM_WFG_COMPLEMENTARYK |  /*����ģʽ*/
                      EPWM_OC_INDEPENDENT );     /*�������ģʽ*/

  /*
  (3)����EPWMʱ������
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
  (4)����EPWM�������
  */
  #if 0	
  EPWM_EnableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
                            EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  #else	
  EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK | EPWM_CH_2_MSK |
                             EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  #endif

  /*
  (5)����EPWM����
  */
  EPWM_EnableDeadZone( 0x3F, (uint32_t)(EPWM_DT*EPWM_Tus) );

  /*
  (6)����EPWM���ط�ʽ
  */
  EPWM_EnableAutoLoadMode( EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK ); /*�Զ�����*/
    
  /*
  (7)����EPWM�Ƚ���0
  */				
  EPWM_ConfigCompareTriger( EPWM_CMPTG_0, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM0, EPWM_HALFPERIOD>>1 );
  EPWM_ConfigCompareTriger( EPWM_CMPTG_1, EPWM_CMPTG_FALLING, EPWM_CMPTG_EPWM2, EPWM_HALFPERIOD    );

  /*
  (8)�����ж�
  */
  //EPWM_EnableZeroInt( EPWM_CH_0_MSK ); /*��������ж�*/		
  //NVIC_EnableIRQ( EPWM_IRQn );

  /*
  (9)����EPWMɲ��
  */
  EPWM_EnableFaultBrake( EPWM_BRK_ACMP1EE ); /*ACMP1ɲ��*/

  EPWM_ConfigFaultBrakeLevel( EPWM_CH_0_MSK | EPWM_CH_2_MSK | EPWM_CH_4_MSK, 0 );
  EPWM_ConfigFaultBrakeLevel( EPWM_CH_1_MSK | EPWM_CH_3_MSK | EPWM_CH_5_MSK, 0 );
  EPWM_AllBrakeEnable();

  Brige_Output_Off();

  /*
  (10)����IO�����
  */	
  GPIO_PinAFOutConfig( P10CFG, IO_OUTCFG_P10_EPWM0 ); /*����P10ΪEPWM0ͨ��*/
  GPIO_PinAFOutConfig( P11CFG, IO_OUTCFG_P11_EPWM1 ); /*����P11ΪEPWM1ͨ��*/
  GPIO_PinAFOutConfig( P12CFG, IO_OUTCFG_P12_EPWM2 ); /*����P12ΪEPWM2ͨ��*/
  GPIO_PinAFOutConfig( P13CFG, IO_OUTCFG_P13_EPWM3 ); /*����P13ΪEPWM3ͨ��*/
  GPIO_PinAFOutConfig( P14CFG, IO_OUTCFG_P14_EPWM4 ); /*����P14ΪEPWM4ͨ��*/
  GPIO_PinAFOutConfig( P15CFG, IO_OUTCFG_P15_EPWM5 ); /*����P15ΪEPWM5ͨ��*/

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
  (11)��ӳ��
  */	
  EPWM->LOCK  = EPWM_LOCK_P1B_WRITE_KEY;
  EPWM->POREMAP = 0xAA531420;
  EPWM->LOCK  = 0;

  /*
  (12)��zero�����ռ�ձ�
  */	
  EPWM->LOCK  = EPWM_LOCK_P1B_WRITE_KEY;
  EPWM->CON3 |= 0x05550000;
  EPWM->LOCK  = 0x0;

  /*
  (13)����EPWM
  */					 
  EPWM_Start( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
              EPWM_CH_2_MSK | EPWM_CH_3_MSK |
              EPWM_CH_4_MSK | EPWM_CH_5_MSK );
  
}

/*****************************************************************************
 ** \brief	ADCģ������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC_Init(void)
{
  /*
  (1)����ADCʱ��
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_ADCEN, ENABLE );

  /*
  (2)����ADCģʽ
  */
  ADC_ConfigRunMode( ADC_MODE_HIGH, ADC_CONVERT_CONTINUOUS, ADC_CLK_DIV_1, 30 );	

  ADC_ConfigChannelSwitchMode( ADC_SWITCH_HARDWARE ); /*Ӳ���Զ��л�*/

  /*
  (3)�ر�ADC�رճ��\�ŵ繦��
  */	
  ADC_DisableChargeAndDischarge();

  /*
  (4)����ADC�ο�Դ
  */
   /* �����ⲿ5VΪADC�ο�Դ */ 
  ADC_ConfigVREF( ADC_VREFP_VDD ); /*���òο�Դ*/

  /*
  (5)����ADCͨ��ʹ��
  */
  GPIO_Init( PORT1, PIN6, ANALOG_INPUT ); // HV-AD ĸ�ߵ�ѹ����ͨ��
 
  GPIO_Init( PORT0, PIN2, ANALOG_INPUT ); // IBUS  ĸ�ߵ�������ͨ��
  GPIO_Init( PORT2, PIN7, ANALOG_INPUT ); // NTC   �¶Ȳ���ͨ��
  GPIO_Init( PORT0, PIN5, ANALOG_INPUT ); // PWMAD   ���ٲ���ͨ��

  GPIO_Init( PORT2, PIN2, ANALOG_INPUT ); // MCU_BEF_U U�෴�綯�Ʋ���ͨ��
  GPIO_Init( PORT2, PIN0, ANALOG_INPUT ); // MCU_BEF_V V�෴�綯�Ʋ���ͨ��
  GPIO_Init( PORT2, PIN1, ANALOG_INPUT ); // MCU_BEF_W W�෴�綯�Ʋ���ͨ��

  /*
  (6)����ADC
  */		
  ADC_Start();
  
}

/*****************************************************************************
 ** \brief	PGA0ģ������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  PGA0_Init(void)
{
  /*
  (1)����PGA0 ʱ��
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_PGA0EN, ENABLE );

  /*
  (2)����PGA0�����IO��
  */		
  GPIO_Init( PORT0, PIN0, ANALOG_INPUT ); //PGA0 +
  GPIO_Init( PORT0, PIN1, ANALOG_INPUT ); //PGA0 -

  /*
  (3)����PGA0����
  */
  PGA_ConfigGain( PGA0x, PGA_GAIN_7d5 );

  /*
  (4)����PGA�ο���ѹ
  */		
  PGA_VrefCtrl( PGA0x, PGA0BG );

  /*
  (5)����PGA0ģʽ ȫ���ģʽ
  */		
  PGA_ModeSet( PGA0x, PgaDiffer );

  /*
  (6)����PGA0���
  */		
  #if 1	
  //PGA0�����PAD��������ѡ�� 
  PGA0_ConfigResistorPAD( PGA_R_10K );

  //PGA0�����PADͨ��ʹ��
  PGA_EnableOutput( PGA0x );

  //PGA0��������˲�ͨ��
  PGA0_Filter( DISABLE );

  GPIO_Init( PORT0, PIN2, ANALOG_INPUT ); //PGA0_OUT
  #endif

  /*
  (7)����PGA0
  */	
  PGA_Start( PGA0x );
	
}

/*****************************************************************************
 ** \brief	PGA1ģ������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA1_Init(void)
{
  /*
  (1)����PGA1 ʱ��
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_PGA1EN, ENABLE );	

  /*
  (2)����PGA1�����IO��
  */	
  GPIO_Init( PORT2, PIN4, ANALOG_INPUT ); //PGA1 +
  GPIO_Init( PORT2, PIN5, ANALOG_INPUT ); //PGA1 -

  /*
  (3)����PGA1����
  */	
  PGA_ConfigGain( PGA1x, PGA_GAIN_7d5 );

  /*
  (4)����PGA1�ο���ѹ
  */	
  PGA_VrefCtrl( PGA1x, PGA0BG );

  /*
  (5)����PGA1ģʽ ȫ���ģʽ
  */	
  PGA_ModeSet( PGA1x, PgaDiffer );	

  /*
  (6)����PGA1���
  */	
  #if (0)		
  PGA_EnableOutput( PGA1x );

  GPIO_Init( PORT0, PIN4, ANALOG_INPUT ); //PGA12_OUT
  #endif

  /*
  (7)����PGA1
  */	
  PGA_Start( PGA1x );
	
}

/*****************************************************************************
 ** \brief	PGA2ģ������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA2_Init(void)
{
  /*
  (1)����PGA2 ʱ��
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_PGA2EN, ENABLE );

  /*
  (2)����PGA2�����IO��
  */
  GPIO_Init( PORT2, PIN6, ANALOG_INPUT ); //PGA2 +
  GPIO_Init( PORT2, PIN7, ANALOG_INPUT ); //PGA2 -

  /*
  (3)����PGA2����
  */	
  PGA_ConfigGain( PGA2x, PGA_GAIN_7d5 );

  /*
  (4)����PGA2�ο���ѹ
  */	
  PGA_VrefCtrl( PGA1x, PGA0BG );

  /*
  (5)����PGA2ģʽ ȫ���ģʽ
  */	
  PGA_ModeSet( PGA2x, PgaDiffer );	

  /*
  (6)����PGA2���
  */	
  #if 0
  PGA_EnableOutput( PGA2x );	

  GPIO_Init( PORT0, PIN4, ANALOG_INPUT ); //PGA12_OUT

  #endif

  /*
  (7)����PGA2
  */	
  PGA_Start( PGA2x );
	
}

/*****************************************************************************
 ** \brief	DAC����
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void DAC_Init(void)
{
  uint8_t bLDOENFlag, bLDOVoltageValue;
  uint16_t hDACSetValue;

  // ��ȡADCLDOʹ��λ
  bLDOENFlag = (ADCLDO->CON0 >> 8) & 0x01;
  // ��ȡADCLDO�����ѹ	
  bLDOVoltageValue = ADCLDO->CON0 & 0xff;

  if(!bLDOENFlag) //δʹ�ܣ�5V
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
  (1)����DACģ��ʱ��
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_DAC, ENABLE );

  /*
  (2)����DAC����ֵ
  */
  DAC_ConfigInput( hDACSetValue );

  /*
  (3)ʹ��DAC���
  */
  DAC_Start();

}

/*****************************************************************************
 ** \brief	ACMP0ģ������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	����EPWMɲ������
*****************************************************************************/
void ACMP0_Init(void)
{
  /*
  (1)����ACMP0ģ��ʱ��
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_ACMPEN, ENABLE );

  /*
  (2)����ACMP0��������
  */
  ACMP_ConfigPositive( ACMP0, ACMP_POSSEL_0PGA2O );

  /*
  (3)����ACMP0��������
  */
  ACMP_ConfigNegative( ACMP0, ACMP_NEGSEL_BEF_N );

  /*
  (4)���ø��˲ο���ѹ
  */ 	
  DAC_Init();

  /*
  (5)����ACMP0����˲�ʱ��
  */ 
  ACMP_Filter_Config( ACMP0, ENABLE, ACMP_NGCLK_65_TSYS );

  /*
  (6)����ACMP0�������
  */ 
  ACMP_Polarity_Config( ACMP0, ACMP_POL_Pos );

  /*
  (7)����ACMP0���͵�ѹ
  */ 
  ACMP_EnableHYS( ACMP0, ACMP_HYS_POS, ACMP_HYS_S_20 );

  /*
  (8)����ACMP0�¼���������
  */ 
  ACMP_ConfigEventAndIntMode( ACMP0, ACMP_EVENT_INT_RISING );

  /*
  (9)����ACMP0�¼����
  */ 
  ACMP_EnableEventOut( ACMP0 );

  /*
  (11)ʹ��ACMP0���
  */   
  ACMPOut_Enable( ACMP0 );

  /*
  (10)����ACMP0
  */ 
  ACMP_Start( ACMP0 );

}

/*****************************************************************************
 ** \brief	ACMP1ģ������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	����EPWMɲ������
*****************************************************************************/
void ACMP1_Init(void)
{
  /*
  (1)����ACMP1ģ��ʱ��
  */
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_ACMPEN, ENABLE );

  /*
  (2)����ACMP1��������
  */
  ACMP_ConfigPositive( ACMP1, ACMP_POSSEL_1PGA0O );

  /*
  (3)����ACMP1��������
  */
  ACMP_ConfigNegative( ACMP1, ACMP_NEGSEL_DAC_O );

  /*
  (4)���ø��˲ο���ѹ
  */ 	
  DAC_Init();

  /*
  (5)����ACMP1����˲�ʱ��
  */ 
  ACMP_Filter_Config( ACMP1, ENABLE, ACMP_NGCLK_65_TSYS );

  /*
  (6)����ACMP1�������
  */ 
  ACMP_Polarity_Config( ACMP1, ACMP_POL_Pos );	

  /*
  (7)����ACMP1���͵�ѹ
  */ 
  ACMP_EnableHYS( ACMP1, ACMP_HYS_POS, ACMP_HYS_S_20 );

  /*
  (8)����ACMP1�¼���������
  */ 
  ACMP_ConfigEventAndIntMode( ACMP1, ACMP_EVENT_INT_RISING );

  /*
  (9)����ACMP1�¼����
  */ 
  ACMP_EnableEventOut( ACMP1 );

  /*
  (11)ʹ��ACMP1���
  */   
  ACMPOut_Enable( ACMP1 );

  /*
  (10)����ACMP1
  */ 
  ACMP_Start( ACMP1 );
  
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :CCP����ģʽ0����
* Description    :��Ҫ����Ӧ��IO���ó�CCP�ڣ���������Ҫ�Ķ�
* Function Call  :�����ʼ������
* Input Paragram :��
* Return Value   :��
*-----------------------------------------------------------------------------
******************************************************************************/
void CCP_Capture_Init(void)
{
  #if 0
  /*
  (1)����CCP��ʱ��
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_CCP, ENABLE ); /*����CCPģ��ʱ��*/

  #if (Speed_Control_Mode == PWM_Control)
    CCP_ConfigCLK( CCP_CAPTURE_MODULE, CCP_CLK_DIV_4, CCP_RELOAD_CCPLOAD, 0xffff ); /*Fahb=64M*/	
  #else
    CCP_ConfigCLK( CCP_CAPTURE_MODULE, CCP_CLK_DIV_64,  CCP_RELOAD_CCPLOAD, 0xffff ); /*Fahb=64M*/		
  #endif			

  /*
  (2)����CCP����ģʽ
  */
  CCP_EnableCAPMode0( CCP_CAPTURE_MODULE );	
  CCP_ConfigCAPMode0( CCP_CAPTURE_MODULE, CCP_CAP_MODE0_RISING_TO_FALLING );	/*����CCP�Ĳ���ģʽ0 �Ĳ���ʽΪ�����ص��½���(�ߵ�ƽʱ��)*/
                          /*����/�½��ز���CCPģ��ʹ��(CCPRUN =1)�󣬼���ֵ��4800(����ֵ)���¼�������������������ֹͣ�����������浱ǰ�ļ���ֵ*/
                          /*�������½��ز���CCPģ��ʹ��(CCPRUN =1)�󣬵���������ʱ����ֵ��4800(����ֵ)���¼�������������(�½���)������ֹͣ�����������浱ǰ�ļ���ֵ*/
                          /*�½��������ز���CCPģ��ʹ��(CCPRUN =1)�󣬵������½���ʱ����ֵ��4800(����ֵ)���¼�������������(������)������ֹͣ�����������浱ǰ�ļ���ֵ*/	
  /*
  (3)����CCP����ͨ��
  */
  CCP_SelCAPMode0Channel( CCP_CAPTURE_MODULE, CCP_CAPTURE_CH ); /*���ò���ͨ��*/

  GPIO_PinAFInConfig( CCP1AINCFG, 0x01 ); /*����P04Ϊ����ͨ��*/
  GPIO_Init( PORT0, PIN4,INPUT );	

  /*
  (4)����CCPģ��
  */	
  CCP_EnableRun( CCP_CAPTURE_MODULE );			
  CCP_Start( CCP_CAPTURE_MODULE ); /*CCPģ��ʹ��*/

  #else

  /*
  (1)����CCP��ʱ��
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_CCP, ENABLE ); /*����CCPģ��ʱ��*/

  #if (Speed_Control_Mode == PWM_Control)
    CCP_ConfigCLK( CCP_CAPTURE_MODULE, CCP_CLK_DIV_4, CCP_RELOAD_CCPLOAD, 0xffff ); /*Fahb=64M*/	
  #else
    CCP_ConfigCLK( CCP_CAPTURE_MODULE, CCP_CLK_DIV_64,  CCP_RELOAD_CCPLOAD, 0xffff ); /*Fahb=64M*/		
  #endif		

  /*
  (2)����CCP����ģʽ
  */
  CCP_EnableCAPMode2();
  CCP_ConfigCAPMode2( CAP1, CCP_CAP_MODE2_RISING  ); /* �����ش��� */
  CCP_ConfigCAPMode2( CAP2, CCP_CAP_MODE2_FALLING ); /* �½��ش��� */
  CCP_ConfigCAPMode2( CAP3, CCP_CAP_MODE2_RISING  ); /* �����ش��� */

  /*
  (3)����CCP����ͨ��
  */
  CCP_SelCAPMode2Channel( CAP1_CCP0B ); /*���ò���ͨ��*/

  GPIO_PinAFInConfig( CCP0BINCFG, IO_INCFG_P04_CCP0B_I ); /*����P04ΪCCP1A����ͨ��*/
  GPIO_Init( PORT0, PIN4, INPUT );

  /*
  (4)����CCPģ��
  */	
  CCP_EnableRun( CCP1 );			
  CCP_Start( CCP1 ); /*CCPģ��ʹ��*/
  #endif
}

/*****************************************************************************
*-----------------------------------------------------------------------------
* Function Name  :CCP PWMģ������
* Description    :��Ҫ����Ӧ��IO���ó�CCP�ڣ���������Ҫ�Ķ�
* Function Call  :�����ʼ������
* Input Paragram :��
* Return Value   :��
*-----------------------------------------------------------------------------
******************************************************************************/
void CCP_PWM_Init(void)
{
  /*
  (1)����CCP��ʱ��
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_CCP, ENABLE ); /*����CCPģ��ʱ��*/
  CCP_ConfigCLK( CCP_PWM_MODULE, CCP_CLK_DIV_64, CCP_RELOAD_CCPLOAD, 10 ); /*Fahb=64M*/	

  /*
  (2)����CCP����ģʽ
  */
  CCP_EnablePWMMode( CCP_PWM_MODULE );

  /*
  (3)����CCP �Ƚ�ֵ
  */	
  CCP_ConfigCompare( CCP_PWM_MODULE, CCP_PWM_CH, 5 ); /*50%*/

  /*
  (4)����CCP �������
  */		
  CCP_DisableReverseOutput( CCP_PWM_MODULE, CCP_PWM_CH );

  /*
  (5)����CCP IO�ڸ���
  */		
  GPIO_PinAFOutConfig( P26CFG, IO_OUTCFG_P26_CCP0A_O ); /*����P26ΪCCP0A FG���ͨ��*/
  GPIO_Init( PORT2, PIN6, OUTPUT );

  /*
  (5)����FG�����Сת��
  */		
  Capture.FG_SpeedMin = (uint16_t)(60 * 1000000 / (MOTOR_PAIRS * 65000)) + 1;
	
}

/*****************************************************************************
 ** \brief	CCP_PWM_GPIO
 **			
 ** \param [in] bMode  0:IO������ΪCCPģʽ  1:IO������ΪGPIO���ģʽ�������Ϊ��
 **           	bMode  0:GPIO���ģʽ�����Ϊ��  1:GPIO���ģʽ�����Ϊ��
 ** \return  none
 ** \note	FG�źŴ���
*****************************************************************************/
void CCP_PWM_GPIO(uint8_t bMode, uint8_t bLevle)
{
  if(0 == bMode )
  {
    GPIO_PinAFOutConfig( P26CFG, IO_OUTCFG_P26_CCP0A_O ); /*����P26ΪCCP0A FG���ͨ��*/
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
 ** \brief	Ӳ������������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	����Ķ�
*****************************************************************************/
void HWDIV_Init(void)
{
  /*
  (1)����HWDIVģ��ʱ��
  */	
  CGC_PER12PeriphClockCmd( CGC_PER12Periph_DIV, ENABLE );

  /*
  (2)ʹ�ܳ�������
  */		
  DIVSQRT_EnableDIVMode();

  /*
  (3)ʹ���з��ų���������
  */		
  DIVSQRT_EnableSingedMode();

}

/*****************************************************************************
 ** \brief	UARTģ������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	��������Ҫ���ڵ��ԣ��û�������Ϊͨ��
*****************************************************************************/
void UART0_Init(void)
{
  uint32_t  BuadRate;

  // BuadRate = 300000;
  BuadRate = 9600;
    
  /*
  (1)����UARTxʱ��
  */
  CGC_PER12PeriphClockCmd( CGC_PER12Periph_UART, ENABLE );

  /*
  (2)����UARTxģʽ
  */	
  UART_ConfigRunMode( UART0, BuadRate, UART_WLS_8, UART_PARITY_NONE, UART_STOP_BIT_1 );
  UART0->END = 0x0;

  /*
  (3)����UARTx���
  */			
  GPIO_PinAFOutConfig( P04CFG, IO_OUTCFG_P04_TXD );	
  GPIO_Init( PORT0, PIN4, OUTPUT );
		
}

/*****************************************************************************
 ** \brief	SysTick_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	����Ķ�
*****************************************************************************/
void SysTick_Init(void)
{
  // ϵͳ��ʱ��--1msʱ������
  SysTick->LOAD  = (uint32_t)(MCU_CLK/1000 - 1UL);
  SysTick->VAL   = 0UL;
  SysTick->CTRL  = 0x00000005;
	
}

/*****************************************************************************
 ** \brief	TMR0ģ������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	����hallƽ�����ٶȼ��㣬������Ķ�
*****************************************************************************/
void TMR0_Iint(void)
{
  /*
  (1)����TMR0��ʱ��
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_TIMER01, ENABLE ); /*��Timer0��ʱ��*/
  TMR_ConfigClk( TMR0, TMR_CLK_DIV_16 ); /*4MHz*/

  /*
  (2)����TMR0����ģʽ
  */	
  TMR_ConfigRunMode( TMR0, TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE );
  TMR_DisableOneShotMode( TMR0);

  /*
  (3)����TMR0��������
  */		
  TMR_SetPeriod( TMR0, 40000); /* 4us*12500=50ms*/

  /*
  (4)����TMR0�ж�
  */	
  TMR_EnableOverflowInt( TMR0 );

  /*
  (5)����TMR0
  */	
  TMR_Start( TMR0 );
	
}

/*****************************************************************************
 ** \brief	TMR1ģ������
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR1_Init(void)
{
  /*
  (1)����TMR1��ʱ��
  */
  CGC_PER11PeriphClockCmd( CGC_PER11Periph_TIMER01, ENABLE ); /*��Timer0��ʱ��*/
  TMR_ConfigClk( TMR1, TMR_CLK_DIV_1 ); /*64MHz*/

  /*
  (2)����TMR1����ģʽ
  */	
  TMR_ConfigRunMode( TMR1,TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE );
  TMR_DisableOneShotMode( TMR1 );

  /*
  (3)����TMR1��������
  */		
  TMR_SetPeriod( TMR1, 0xffffffff ); // 100us = 6400    4000

  /*
  (4)����TMR �ж�
  */	
  TMR_EnableOverflowInt( TMR1 );

  /*
  (5)����TMR1
  */	
  TMR_Start( TMR1 );
	
}

/*****************************************************************************
 ** \brief	ADCLDO_Init
 **			
 ** \param [in] none
 ** \return  none
 ** \note	����Ķ�
*****************************************************************************/
void ADCLDO_Init(void)
{
  /*
  (1)����ADCLDO��ʱ��
  */	
  CGC_PER13PeriphClockCmd( CGC_PER13Periph_ADCLDO, ENABLE );

  /*
  (2)����ADCLDO�����ѹ
  */		
  ADCLDO_OutVlotageSel( ADCLDO_OutV_4d2 );

  /*
  (3)ʹ��ADCLDOģ��
  */		
  ADCLDO_Disable();
	
}

/*****************************************************************************
 ** \brief	ϵͳ�ĳ�ʼ�����ã�����ģ���ʼ��������������ʼ����
 **			
 ** \param [in] none
 ** \return  none
 ** \note		������ģ���ʼ��˳������Ķ���������������ģ�鹦��
*****************************************************************************/
void System_Init(void)
{
  // ϵͳʼ������
  SysClock_Init();

  // ����P02��λ����
  RESTPinGpio_Set( ENABLE );

  // ����ʱ�򿪣������ϵ��ѷ����ˢ���ղ��˳�������ģʽ��Ϊ300ms
  DelayTime_ms( POWERON_DELAY_TIME );	

  // �������IO��������
  PORT_Init();
    
  // ADCLDO����
  ADCLDO_Init();

  // ADCģ������
  ADC_Init();

  // ������ģʽ��PWMģ������
  #if ( Config_Shunt_Mode == Single_Shunt )	
    EPWM_R1_Init();
  #endif

  // ˫����ģʽ��PWMģ������
  #if ( Config_Shunt_Mode == Double_Shunt )
    EPWM_R2_Init();
  #endif

  // PGAģ������
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

  // �Ƚ���ACMP0ģ������
  #if ( ACMP_CH == ACMP_CH0 )
    ACMP0_Init();
  #endif

  // �Ƚ���ACMP1ģ������
  #if ( ACMP_CH == ACMP_CH1 )
    ACMP1_Init();	
  #endif

  // ���ٲ�׽ģ������
  #if ( Config_CCP_Capture == CCP_Capture_Enable )
    CCP_Capture_Init();		
  #endif	

  // �ٶȷ���--FG���ģ������
  #if ( Config_CCP_PWM == CCP_PWM_Enable )
    CCP_PWM_Init();
    // ����SWO���Խӿڹ��� (������¼������Ҫ��)
  //		DBG->DBGSTOPCR |= (1<<24);
  #endif

  // Ӳ������������
  HWDIV_Init();

  // ����ͨ������--����ʱʹ��
  #if ( UART_CONFIG_MODE == UART_ON )
    UART0_Init();
  #endif

  // ϵͳ��ʱ��--1msʱ��Ƭ
  SysTick_Init();

  // ��ʱ��TMR0ģ������
  //TMR0_Iint();

  // ��ʱ��TMR1ģ������

  //TMR1_Init();

  // ���Ʋ�����ʼ��
  MC_Para_Init();

  // ��ʱ�ȴ�ADC����
  DelayTime_ms( 130 );

  //����ADC��������ģʽ
  ADC_TGSAMP_CONFIG();
	
}

/******************************** END OF FILE *******************************/
