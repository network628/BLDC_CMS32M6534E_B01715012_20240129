#include <stdio.h> 
#include "delay.h"

static  uint8_t  Value_us =0;

/***********************************************************************************************************************
* Function Name: delay1_us
* @brief  Configure one millisecond count
* @param  num: systemclock divided coefficient
* @return   
***********************************************************************************************************************/
void delay1_us(uint32_t num)
{            
	SysTick->LOAD = num * Value_us;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	SysTick->CTRL= SysTick_CTRL_CLKSOURCE_Msk;     
}

/***********************************************************************************************************************
* Function Name: m0_delay_ms
* @brief  Configure second count
* @param  number_of_ms
* @return   
***********************************************************************************************************************/
void m0_delay_ms(uint32_t volatile number_of_ms)
{
    while(number_of_ms != 0)
    {
        number_of_ms--;
        delay1_us(1000);
    }
}

/***********************************************************************************************************************
* Function Name: m0_delay_us
* @brief  Configure millisecond count
* @param  number_of_ms
* @return   
***********************************************************************************************************************/
void m0_delay_us(uint32_t volatile number_of_us)
{
	delay1_us(number_of_us);
}

/***********************************************************************************************************************
* Function Name: delay_init
* @brief  
* @param  SYSCLK
* @return   
***********************************************************************************************************************/
void delay_init(uint32_t sysclk)
{
	Value_us = sysclk/1000000;
}
