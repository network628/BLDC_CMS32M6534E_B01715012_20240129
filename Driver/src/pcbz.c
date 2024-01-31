/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    pcbz.c
* @brief   This file implements device driver for PCLBUZ module.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "pcbz.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/**
  * @brief  Initializes the pclbuz which can offer clock for external peripheral and buzzer by two
  *			channels
  * @param  PCBZ_InitStruct: pointer to a PCBZ_InitTypeDef structure that contains
  *         the configuration information for the specified pclbuz peripheral.
  * @retval initial result
  */
void PCBZ_Init(PCBZ_InitTypeDef* PCBZ_InitStruct)
{
	assert_param(IS_PCBZ_CHANNEL(PCBZ_InitStruct->PCBZ_ChanSelect));
	assert_param(IS_PCBZ_CLKSOURCE(PCBZ_InitStruct->PCBZ_ClkSource));
	assert_param(IS_PCBZ_FREQSCALAR(PCBZ_InitStruct->PCBZ_Freqscaler));
	
    if((PCBZ_InitStruct->PCBZ_ChanSelect & PCBZ_CHANNEL_0) == PCBZ_CHANNEL_0)
    {
        PCBZ->CKS0  = 0x80U | PCBZ_InitStruct->PCBZ_ClkSource | PCBZ_InitStruct->PCBZ_Freqscaler;    	
    }
    if((PCBZ_InitStruct->PCBZ_ChanSelect & PCBZ_CHANNEL_1) == PCBZ_CHANNEL_1)
    {
        PCBZ->CKS1  = 0x80U | PCBZ_InitStruct->PCBZ_ClkSource | PCBZ_InitStruct->PCBZ_Freqscaler;    	
    }
}

/***********************************************************************************************************************
* Function Name: PCBZ_Start
* @brief  This function start the pclbuz.
* @param  ch - specify the channel of pcbz
* @return None
***********************************************************************************************************************/
void PCBZ_Start(PCBZ_Channel_t ch)
{
    if(ch == PCBZ_CHANNEL_0)
    {
        PCBZ->CKS0  |= 0x80U;    	
    }
    if(ch == PCBZ_CHANNEL_1)
    {
        PCBZ->CKS1  |= 0x80U;    	
    }
}

/***********************************************************************************************************************
* Function Name: PCBZ_Stop
* @brief  This function stop the pclbuz.
* @param  ch - specify the channel of pcbz
* @return None
***********************************************************************************************************************/
void PCBZ_Stop(PCBZ_Channel_t ch)
{
    if(ch == PCBZ_CHANNEL_0)
    {
        PCBZ->CKS0  &= ~0x80U;    	
    }
    if(ch == PCBZ_CHANNEL_1)
    {
        PCBZ->CKS1  &= ~0x80U;    	
    }
}
/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
