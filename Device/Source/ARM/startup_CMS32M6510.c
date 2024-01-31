/**************************************************************************//**
 * @file     startup_CMS32M6510.c
 * @brief    CMSIS Cortex-M ARMv7-M based Core Device Startup File for
 *           Device <Device>
 * @version  1.0.0
; * @date    2022/8/24
 ******************************************************************************/
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#define STACK_TOP  (0x20002000U)  /*!< Stack is placed on RAM TOP  */

#include <stdio.h>
#include <rt_misc.h>
#include <stdint.h>

//
// Build a stand-alone image
//

#pragma import(__use_no_semihosting)


//
// Dummy Interrupt Handlers
//
// The following functions are defined weakly to allow the user
// to override them at link time simply by declaring their own
// function of the same name.
//
// If no user function is provided, the weak function is used.
//

__weak void NMI_Handler(void)
{
  while(1);
}

__weak void HardFault_Handler(void)
{
  while(1);
}

__weak void SVC_Handler(void)
{
  while(1);
}

__weak void PendSV_Handler(void)
{
  while(1);
}

__weak void SysTick_Handler(void)
{
  while(1);
}

__weak void LVI_IRQHandler(void)
{
   while(1);
}

__weak void INTP0_IRQHandler(void)
{
   while(1);
}

__weak void INTP1_IRQHandler(void)
{
   while(1);
}

__weak void INTP2_IRQHandler(void)
{
   while(1);
}

__weak void INTP3_IRQHandler(void)
{
   while(1);
}

__weak void TM01H_IRQHandler(void)
{
   while(1);
}

__weak void CCP_IRQHandler(void)
{
   while(1);
}

__weak void EPWM_IRQHandler(void)
{
   while(1);
}

__weak void ADC_IRQHandler(void)
{
   while(1);
}

__weak void ACMP_IRQHandler(void)
{
   while(1);
}

__weak void UART0_IRQHandler(void)
{
   while(1);
}

__weak void I2C0_IRQHandler(void)
{
   while(1);
}

__weak void SSP0_IRQHandler(void)
{
   while(1);
}

__weak void TIMER0_IRQHandler(void)
{
   while(1);
}

__weak void TIMER1_IRQHandler(void)
{
   while(1);
}

__weak void LSITIMER_IRQHandler(void)
{
   while(1);
}

__weak void IRQ16_Handler(void)
{
   while(1);
}

__weak void TM00_IRQHandler(void)
{
   while(1);
}

__weak void TM01_IRQHandler(void)
{
   while(1);
}

__weak void TM02_IRQHandler(void)
{
   while(1);
}

__weak void TM03_IRQHandler(void)
{
   while(1);
}

__weak void IRQ21_Handler(void)
{
   while(1);
}

__weak void IRQ22_Handler(void)
{
   while(1);
}

__weak void IRQ23_Handler(void)
{
   while(1);
}

__weak void IRQ24_Handler(void)
{
   while(1);
}

__weak void IRQ25_Handler(void)
{
   while(1);
}

__weak void IRQ26_Handler(void)
{
   while(1);
}

__weak void IRQ27_Handler(void)
{
   while(1);
}

__weak void IRQ28_Handler(void)
{
   while(1);
}

__weak void IRQ29_Handler(void)
{
   while(1);
}

__weak void IRQ30_Handler(void)
{
   while(1);
}

__weak void FMC_IRQHandler(void)
{
   while(1);
}

//
// Reset Handler
//

extern void SystemInit(void); 
extern void __main(void);

//#pragma arm section code = "sec_resethandler"
void Reset_Handler(void)
{
  SystemInit();
  __main();
}
//#pragma arm section code


//
// Set up Vector Table
//

typedef void (*const vect_t)(void) __irq;

vect_t __Vectors[]
__attribute__ ((section("vectors"))) = {
  (vect_t)(STACK_TOP),      // Top of Stack
  (vect_t)Reset_Handler,    // Reset Handler
  (vect_t)NMI_Handler,      // NMI Handler
  (vect_t)HardFault_Handler,// Hard Fault Handler
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  0,                        // Reserved
  (vect_t)SVC_Handler,      // SVCall Handler
  0,                        // Reserved
  0,                        // Reserved
  (vect_t)PendSV_Handler,   // PendSV Handler
  (vect_t)SysTick_Handler,  // SysTick Handler

  (vect_t) LVI_IRQHandler      ,
  (vect_t) INTP0_IRQHandler    ,
  (vect_t) INTP1_IRQHandler    ,
  (vect_t) INTP2_IRQHandler    ,
  (vect_t) INTP3_IRQHandler    ,
  (vect_t) TM01H_IRQHandler    ,
  (vect_t) CCP_IRQHandler      ,
  (vect_t) EPWM_IRQHandler     ,
  (vect_t) ADC_IRQHandler      ,
  (vect_t) ACMP_IRQHandler     ,
  (vect_t) UART0_IRQHandler    ,
  (vect_t) I2C0_IRQHandler     ,
  (vect_t) SSP0_IRQHandler     ,
  (vect_t) TIMER0_IRQHandler   ,
  (vect_t) TIMER1_IRQHandler   ,
  (vect_t) LSITIMER_IRQHandler ,
  (vect_t) IRQ16_Handler ,
  (vect_t) TM00_IRQHandler     ,
  (vect_t) TM01_IRQHandler     ,
  (vect_t) TM02_IRQHandler     ,
  (vect_t) TM03_IRQHandler     ,
  (vect_t) IRQ21_Handler ,
  (vect_t) IRQ22_Handler ,
  (vect_t) IRQ23_Handler ,
  (vect_t) IRQ24_Handler ,
  (vect_t) IRQ25_Handler ,
  (vect_t) IRQ26_Handler ,
  (vect_t) IRQ27_Handler ,
  (vect_t) IRQ28_Handler ,
  (vect_t) IRQ29_Handler ,
  (vect_t) IRQ30_Handler ,
  (vect_t) FMC_IRQHandler      ,
};


//
// Set up initial stack and heap
//

__value_in_regs struct __initial_stackheap
__user_initial_stackheap(unsigned hb, unsigned sb, unsigned hl, unsigned sl)
{
  struct __initial_stackheap s;

  s.heap_base   = hb;
  s.stack_base  = sb;
  s.heap_limit  = s.stack_base;
  s.stack_limit = s.heap_base;
  return s;
}


//
// Default exit function
//

__weak void _sys_exit(int return_code)
{
  while(1);
}
