#include "cgc.h"

uint8_t USE_HSE_SYSTYEM_CLOCK = SYSTYEM_CLOCK_CLOSE;
uint8_t USE_HSI_SYSTYEM_CLOCK = SYSTYEM_CLOCK_CLOSE;
uint8_t USE_LSE_SYSTYEM_CLOCK = SYSTYEM_CLOCK_CLOSE;
uint8_t USE_LSI_SYSTYEM_CLOCK = SYSTYEM_CLOCK_CLOSE;

/**
  * @brief  Enables or disables the PER0 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  * @param  CGC_PER0Periph: specifies the PER0 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg CGC_PER0Periph_TIM4: TIM40 clock
  *            @arg CGC_PER0Periph_LSITIMER: LSITIMER clock

  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CGC_PER0PeriphClockCmd(uint8_t CGC_PER0Periph, FunctionalState NewState)
{
    /* Check the parameters */
//    assert_param(IS_CGC_PER0_PERIPH(CGC_PER0Periph));
//    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        CGC->PER0 |= CGC_PER0Periph;
    }
    else
    {
        CGC->PER0 &= ~CGC_PER0Periph;
    }
}

/**
  * @brief  Enables or disables the PER11 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  * @param  CGC_PER11Periph: specifies the PER0 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg CGC_PER11Periph_TIMER01:    TIMER01 clock
  *            @arg CGC_PER11Periph_EPWM:     EPWM clock
  *            @arg CGC_PER11Periph_CCP:      CCP clock

  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CGC_PER11PeriphClockCmd(uint32_t CGC_PER11Periph, FunctionalState NewState)
{
    /* Check the parameters */
//    assert_param(IS_CGC_PER11_PERIPH(CGC_PER1Periph));
//    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        CGC->PER11 |= CGC_PER11Periph;
    }
    else
    {
        CGC->PER11 &= ~CGC_PER11Periph;
    }
}


/**
  * @brief  Enables or disables the PER12 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  * @param  CGC_PER12Periph: specifies the PER12 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg CGC_PER12Periph_DIV:    DIVEN clock
  *            @arg CGC_PER12Periph_SPI:     SPI clock
  *            @arg CGC_PER12Periph_UART:    UART clock
  *            @arg CGC_PER12Periph_IIC:     Specifical IIC clock
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CGC_PER12PeriphClockCmd(uint32_t CGC_PER12Periph, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_CGC_PER12_PERIPH(CGC_PER12Periph));
//    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        CGC->PER12 |= CGC_PER12Periph;
    }
    else
    {
        CGC->PER12 &= ~CGC_PER12Periph;
    }
}
/**
  * @brief  Enables or disables the PER13 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  * @param  CGC_PER13Periph: specifies the PER0 peripheral to gates its clock.
  *          This parameter can be any combination of the following values:
  *            @arg CGC_PER13Periph_ADCEN:     ADC clock
  *            @arg CGC_PER13Periph_ACMPEN:    ACMP clock
  *            @arg CGC_PER13Periph_PGA0EN:    PGA0
  *            @arg CGC_PER13Periph_PGA12:     PGA12
  *            @arg CGC_PER13Periph_DAC :      DAC
  *            @arg CGC_PER13Periph_ADCLDO:     ADCLDO
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CGC_PER13PeriphClockCmd(uint32_t CGC_PER13Periph, FunctionalState NewState)
{
    /* Check the parameters */
//    assert_param(IS_CGC_PER13_PERIPH(CGC_PER13Periph));
//    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        CGC->PER13 |= CGC_PER13Periph;
    }
    else
    {
        CGC->PER13 &= ~CGC_PER13Periph;
    }
}


/**
  * @brief  Enables Internal Low Speed oscillator (LSI/FIL) 
  * @note   LSI can not be disabled for IWDG.  
  * @retval None
  */
void CGC_LSI_Enable(void)
{
	CGC->SUBCKSEL = CGC_SUBCKSEL_SELLOSC_Msk;
}

/**
  * @brief  Disables Internal Low Speed oscillator (LSI/FIL) 
  * @note   LSI can not be disabled for IWDG.  
  * @retval None
  */
void CGC_LSI_Disable(void)
{
	CGC->SUBCKSEL &= ~CGC_SUBCKSEL_SELLOSC_Msk;
}

/**
  * @brief  Enables Internal Low Speed oscillator (LSI/FIL) used as CPU 
  *		    system clock and Clock source of peripheral hardware circuit.	
  * @note   LSI can not be disabled for IWDG.  
  * @retval None
  */
void CGC_LSI_CFG_AS_FCLK()
{
	CGC->SUBCKSEL = CGC_SUBCKSEL_SELLOSC_Msk;
	CGC->CKC = (1 << CGC_CKC_CSS_Pos) ;
	
    while ((CGC->CKC & CGC_CKC_CLS_Msk) == 0);
	USE_LSI_SYSTYEM_CLOCK = SYSTYEM_CLOCK_OPEN;
}


/**
  * @brief  Enables Internal High Speed oscillator (HSI) used as CPU 
  *		    system clock and Clock source of peripheral hardware circuit.	
  * @note  internal High Speed oscillator FHOCO is divided to FIH,which will used as CPU system
  * 		clock
  * @retval None
  */
void CGC_HSI_CFG_AS_FCLK()
{
	CGC->CKC &= ~CGC_CKC_CSS_Msk ;

	while((CGC->CKC & CGC_CKC_CLS_Msk) == 1);
	USE_HSI_SYSTYEM_CLOCK = SYSTYEM_CLOCK_OPEN;	
}

/**
  * @brief This function stops the high speed on chip oscilator (HOCO).
  * @param  None
  * @retval None
  */
void CGC_Hoco_Stop(void)
{
	CGC->CSC |= 1<<0; 	/* HIOSTOP = 1 */
}

/**
  * @brief This function starts the high speed on chip oscilator (HOCO). 
  * @param  None
  * @retval None
  */
void CGC_Hoco_Start(void)
{
	CGC->CSC &= ~(1<<0); 	/* HIOSTOP = 0 */
}

/**
  * @brief This function enable the LSI module clock. 
  * @param  None
  * @retval None
  */
void CGC_LSITIMERCLK_Enable(void)
{
	CGC->OSMC |= CGC_OSMC_WUTMMCK0_Msk; 	/* WUTMMCK0 = 1 */
}

/**
  * @brief This function feedback the system reset source. 
  * @param  None
  * @retval reset source
  */
uint32_t Reset_Status_Get(void)
{
	uint8_t RstStatus=0,temp=0;
	
	RstStatus = RST->RESF;
	
	if((RstStatus & RST_RESF_LVIRF_Msk) == RST_RESF_LVIRF_Msk)
	{
		return(RESF_Src_LVD);
	}
	else if((RstStatus & RST_RESF_IAWRF_Msk) == RST_RESF_IAWRF_Msk)
	{
		return(RESF_Src_IAW);
	}
	else if((RstStatus & RST_RESF_RPERF_Msk) == RST_RESF_RPERF_Msk)
	{
		return(RESF_Src_RAMP);
	}
	else if((RstStatus & RST_RESF_WDTRF_Msk) == RST_RESF_WDTRF_Msk)
	{
		return(RESF_Src_WDT);
	}
	else if((RstStatus & RST_RESF_SYSRF_Msk) == RST_RESF_SYSRF_Msk)
	{
		return(RESF_Src_SYSR);
	}
	return(temp);
}

