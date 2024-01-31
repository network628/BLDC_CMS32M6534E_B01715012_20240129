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

/****************************************************************************/
/** \file gpio.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "gpio.h"
/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
*****************************************************************************/

/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/


/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/

 /**
   * @brief  Set the PORTx bit
   * @param  PORTx: where x can be 0~7, 12~14
   * @param  PINx: where x can be 0~7    
 	*
   * @retval None
   */
 void PORT_SetBit(PORT_TypeDef PORTx,PIN_TypeDef PINx)
 {
 	uint8_t pos = 1<<PINx;
 	*((volatile uint8_t*)(&PORT->PSET0+PORTx)) = pos;
 }

 /**
   * @brief  Clear the PORTx bit
   * @param  PORTx: where x can be 0~7, 12~14
   * @param  PINx: where x can be 0~7    
 	*
   * @retval None
   */
 void PORT_ClrBit(PORT_TypeDef PORTx,PIN_TypeDef PINx)
 {
 	uint8_t pos = 1<<PINx;
 	*((volatile uint8_t*)(&PORT->PCLR0+PORTx)) |= pos;
 }

 /**
   * @brief  Toggle the PORTx bit
   * @param  PORTx: where x can be 0~7, 12~14
   * @param  PINx: where x can be 0~7    
   *
   * @retval None
   */
 void PORT_ToggleBit(PORT_TypeDef PORTx,PIN_TypeDef PINx)
 {
 	uint8_t pos = 1<<PINx;
 	*((volatile uint8_t*)(&PORT->P0+PORTx)) ^= pos;
 }

 /**
   * @brief  Get the PORTx bit
   * @param  PORTx: where x can be 0~7, 12~14
   * @param  PINx: where x can be 0~7    
 	*
   * @retval None
   */
 uint8_t PORT_GetBit(PORT_TypeDef PORTx,PIN_TypeDef PINx)
 {
 	uint8_t pos = 1<<PINx;
 	PORT->PMS = 0x01;        /*!< Digital output level of the pin is read */
 	return *((volatile uint8_t*)(&PORT->P0+PORTx))&pos;
 }

  /**
   * @brief  Get the PORTx bit's value
   * @param  PORTx: where x can be 0~7, 12~14
   * @param  PINx: where x can be 0~7    
 	*
   * @retval 1 or 0
   */
 uint8_t PORT_GetBitValue(PORT_TypeDef PORTx,PIN_TypeDef PINx)
 {
 	uint8_t PortValue = 0;
	 
	PortValue = PORT_GetBit(PORTx,PINx);
  if(PortValue == (1<<PINx))
		return(1);
	else
		return(0);
 }
 
/**
  * @brief  Initializes the GPIO
  * @param  PORTx: where x can be 0~7, 12~14
  * @param  PINx: where x can be 0~7
  * @param  MODEx: such as INPUT,PULLUP_INPUT,TTL_INPUT,ANALOG_INPUT,OUTPUT,OPENDRAIN_OUTPUT,PULLDOWN_INPUT      
	*
  * @retval None
  */
void GPIO_Init(PORT_TypeDef PORTx,PIN_TypeDef PINx,PIN_ModeDef MODEx)
{
  uint8_t mode = MODEx;
	uint8_t pos = 1<<PINx;
	
	switch(mode)
	{
		case INPUT:
			*((volatile uint8_t*)(&PORT->PMC0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PM0+PORTx)) |= pos;
			*((volatile uint8_t*)(&PORT->POM0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PU0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PD0+PORTx)) &= ~pos;
			break;
		case PULLUP_INPUT:
			*((volatile uint8_t*)(&PORT->PMC0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PM0+PORTx)) |= pos;
			*((volatile uint8_t*)(&PORT->POM0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PU0+PORTx)) |= pos;
			*((volatile uint8_t*)(&PORT->PD0+PORTx)) &= ~pos;
			break;
		case TTL_INPUT:
			*((volatile uint8_t*)(&PORT->PMC0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PM0+PORTx)) |= pos;
			*((volatile uint8_t*)(&PORT->POM0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PU0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PD0+PORTx)) &= ~pos;
			break;
		case ANALOG_INPUT:
			*((volatile uint8_t*)(&PORT->PMC0+PORTx)) |= pos;
			break;
		case OUTPUT:
			*((volatile uint8_t*)(&PORT->PMC0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PM0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->POM0+PORTx)) &= ~pos;
			break;
		case OPENDRAIN_OUTPUT:
			*((volatile uint8_t*)(&PORT->PMC0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PM0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->POM0+PORTx)) |= pos;
			break;
		case PULLDOWN_INPUT:
			*((volatile uint8_t*)(&PORT->PMC0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PM0+PORTx)) |= pos;
			*((volatile uint8_t*)(&PORT->POM0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PU0+PORTx)) &= ~pos;
			*((volatile uint8_t*)(&PORT->PD0+PORTx)) |= pos;
			break;	
	}	
}

/***********************************************************************************************************************
* Function Name: RESTPinGpio_Set
* @brief  This function set the P02 to gpio or reset pin.
 ** \param [in] RStat: 
 **								ENABLE: P02 to gpio
 **								DISABLE: P02 to reset pin
* @return None
***********************************************************************************************************************/
void RESTPinGpio_Set(uint8_t RStat)
{
	if(RStat == ENABLE)
	{
		CGC->RSTM = 1;				//P02口选为GPIO
	}
	else
	{
		CGC->RSTM = 0;				//P02口选为复位脚
	}
}
