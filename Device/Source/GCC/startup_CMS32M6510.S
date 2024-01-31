/**************************************************************************//**
 * @file     startup_CMS32M6510.S
 * @brief    CMSIS Cortex-M0+ Core Device Startup File for
 *           Device CMS32M6510
 * @version  1.0.0
 * @date     2019/12/24
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
 */


	.syntax	unified
	.arch	armv6-m

	.section .stack
	.align	3
#ifdef __STACK_SIZE
	.equ	Stack_Size, __STACK_SIZE
#else
	.equ	Stack_Size, 0x00000400
#endif
	.globl	__StackTop
	.globl	__StackLimit
__StackLimit:
	.space	Stack_Size
	.size	__StackLimit, . - __StackLimit
__StackTop:
	.size	__StackTop, . - __StackTop

	.section .heap
	.align	3
#ifdef __HEAP_SIZE
	.equ	Heap_Size, __HEAP_SIZE
#else
	.equ	Heap_Size, 0x00000C00
#endif
	.globl	__HeapBase
	.globl	__HeapLimit
__HeapBase:
	.if	Heap_Size
	.space	Heap_Size
	.endif
	.size	__HeapBase, . - __HeapBase
__HeapLimit:
	.size	__HeapLimit, . - __HeapLimit

	.section .vectors
	.align	2
	.globl	__Vectors
__Vectors:
	.long	__StackTop            /* Top of Stack */
	.long	Reset_Handler         /* Reset Handler */
	.long	NMI_Handler           /* NMI Handler */
	.long	HardFault_Handler     /* Hard Fault Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	SVC_Handler           /* SVCall Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	PendSV_Handler        /* PendSV Handler */
	.long	SysTick_Handler       /* SysTick Handler */

	/* External interrupts */
/* ToDo:  Add here the vectors for the device specific external interrupts handler */
	.long	 LVI_IRQHandler                  		/*  IRQ00: Handler         */
	.long	 INTP0_IRQHandler                		/*  IRQ01: Handler         */
	.long	 INTP1_IRQHandler                		/*  IRQ02: Handler         */
	.long	 INTP2_IRQHandler                		/*  IRQ03: Handler         */
	.long	 INTP3_IRQHandler                		/*  IRQ04: Handler         */
	.long	 TM01H_IRQHandler                		/*  IRQ05: Handler         */
	.long	 CCP_IRQHandler                  		/*  IRQ06: Handler         */
	.long	 EPWM_IRQHandler                 		/*  IRQ07: Handler         */
	.long	 ADC_IRQHandler                  		/*  IRQ08: Handler         */
	.long	 ACMP_IRQHandler                 		/*  IRQ09: Handler         */
	.long	 UART0_IRQHandler                		/*  IRQ10: Handler         */
	.long	 I2C0_IRQHandler                 		/*  IRQ11: Handler         */
	.long	 SSP0_IRQHandler                 		/*  IRQ12: Handler         */
	.long	 TIMER0_IRQHandler               		/*  IRQ13: Handler         */
	.long	 TIMER1_IRQHandler               		/*  IRQ14: Handler         */
	.long	 LSITIMER_IRQHandler             		/*  IRQ15: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ16: Handler         */
	.long	 TM00_IRQHandler                 		/*  IRQ17: Handler         */
	.long	 TM01_IRQHandler                 		/*  IRQ18: Handler         */
	.long	 TM02_IRQHandler                 		/*  IRQ19: Handler         */
	.long	 TM03_IRQHandler                 		/*  IRQ20: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ21: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ22: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ23: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ24: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ25: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ26: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ27: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ28: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ29: Handler         */
	.long	 Reserved_IRQHandler             		/*  IRQ30: Handler         */
	.long	 FMC_IRQHandler                  		/*  IRQ31: Handler         */

	.size	__Vectors, . - __Vectors

	.text
	.thumb
	.thumb_func
	.align	2
	.globl	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:
/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r4, =__copy_table_start__
	ldr	r5, =__copy_table_end__

.L_loop0:
	cmp	r4, r5
	bge	.L_loop0_done
	ldr	r1, [r4]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #8]

.L_loop0_0:
	subs	r3, #4
	ittt	ge
	ldrge	r0, [r1, r3]
	strge	r0, [r2, r3]
	bge	.L_loop0_0

	adds	r4, #12
	b	.L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__etext
	ldr	r2, =__data_start__
	ldr	r3, =__data_end__

.L_loop1:
	cmp	r2, r3
	ittt	lt
	ldrlt	r0, [r1], #4
	strlt	r0, [r2], #4
	blt	.L_loop1
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
	ldr	r3, =__zero_table_start__
	ldr	r4, =__zero_table_end__

.L_loop2:
	cmp	r3, r4
	bge	.L_loop2_done
	ldr	r1, [r3]
	ldr	r2, [r3, #4]
	movs	r0, 0

.L_loop2_0:
	subs	r2, #4
	itt	ge
	strge	r0, [r1, r2]
	bge	.L_loop2_0

	adds	r3, #8
	b	.L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__bss_start__
	ldr	r2, =__bss_end__

	movs	r0, 0
.L_loop3:
	cmp	r1, r2
	itt	lt
	strlt	r0, [r1], #4
	blt	.L_loop3
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifndef __NO_SYSTEM_INIT
	bl	SystemInit
#endif

#ifndef __START
#define __START _start
#endif
	bl	__START

	.pool
	.size	Reset_Handler, . - Reset_Handler

	.align	1
	.thumb_func
	.weak	Default_Handler
	.type	Default_Handler, %function
Default_Handler:
	b	.
	.size	Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
	.macro	def_irq_handler	handler_name
	.weak	\handler_name
	.set	\handler_name, Default_Handler
	.endm

	def_irq_handler	NMI_Handler
	def_irq_handler	HardFault_Handler
	def_irq_handler	SVC_Handler
	def_irq_handler	PendSV_Handler
	def_irq_handler	SysTick_Handler

/* IRQ Handlers */

/* ToDo:  Add here the export definition for the device specific external interrupts handler */
/* ToDo:  Add here the names for the device specific external interrupts handler             */
	def_irq_handler	 LVI_IRQHandler      
	def_irq_handler	 INTP0_IRQHandler    
	def_irq_handler	 INTP1_IRQHandler    
	def_irq_handler	 INTP2_IRQHandler    
	def_irq_handler	 INTP3_IRQHandler    
	def_irq_handler	 TM01H_IRQHandler    
	def_irq_handler	 CCP_IRQHandler      
	def_irq_handler	 EPWM_IRQHandler     
	def_irq_handler	 ADC_IRQHandler      
	def_irq_handler	 ACMP_IRQHandler     
	def_irq_handler	 UART0_IRQHandler    
	def_irq_handler	 I2C0_IRQHandler     
	def_irq_handler	 SSP0_IRQHandler     
	def_irq_handler	 TIMER0_IRQHandler   
	def_irq_handler	 TIMER1_IRQHandler   
	def_irq_handler	 LSITIMER_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 TM00_IRQHandler     
	def_irq_handler	 TM01_IRQHandler     
	def_irq_handler	 TM02_IRQHandler     
	def_irq_handler	 TM03_IRQHandler     
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 Reserved_IRQHandler 
	def_irq_handler	 FMC_IRQHandler      

	.end
