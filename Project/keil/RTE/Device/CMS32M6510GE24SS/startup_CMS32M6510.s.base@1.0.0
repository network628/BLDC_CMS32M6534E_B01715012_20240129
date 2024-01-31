;/*****************************************************************************
; * @file:    startup_CMS32M6510.s
; * @purpose: CMSIS Cortex-M0 Core Device Startup File for the 
; *           CMSARM CMS32M6510 Series 
; * @version: V1.0
; * @date     31-March-2015
; *
; * @note
; * Copyright (C) 2010-2011 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     LVI_IRQHandler            ; Low Voltage Detection Interrupt         ;0
                DCD     INTP0_IRQHandler          ; INTP0 Interrupt                         ;1
                DCD     INTP1_IRQHandler          ; INTP1 Interrupt                         ;2
                DCD     INTP2_IRQHandler          ; INTP2 Interrupt                         ;3
                DCD     INTP3_IRQHandler          ; INTP3 Interrupt                         ;4
                DCD     TM01H_IRQHandler          ; TM40 channel 1(higher 8 bit) Interrupt  ;5
                DCD     CCP_IRQHandler            ; Capture/PWM Interrupt                   ;6
                DCD     EPWM_IRQHandler           ; EPWM Interrupt	                        ;7
                DCD     ADC_IRQHandler            ; ADC Interrupt                           ;8
                DCD     ACMP_IRQHandler           ; ACMP Interrupt                          ;9
                DCD     UART0_IRQHandler          ; UART0 Interrupt                         ;10
                DCD     I2C0_IRQHandler           ; IIC Interrupt                           ;11
                DCD     SSP0_IRQHandler           ; SSP0 Interrupt                          ;12
                DCD     TIMER0_IRQHandler         ; Timer0 Interrupt                        ;13
                DCD     TIMER1_IRQHandler         ; Timer1 Interrupt                        ;14
                DCD     LSITIMER_IRQHandler       ; LSI Timer Interrupt                     ;15
                DCD     Reserved_IRQHandler       ; Reserved                                ;16
                DCD     TM00_IRQHandler           ; TM00 Interrupt                          ;17
                DCD     TM01_IRQHandler           ; TM01 Interrupt                          ;18
                DCD     TM02_IRQHandler           ; TM02 Interrupt                          ;19
                DCD     TM03_IRQHandler           ; TM03 Interrupt                          ;20
                DCD     Reserved_IRQHandler       ; Reserved                                ;21
                DCD     Reserved_IRQHandler       ; Reserved                                ;22
                DCD     Reserved_IRQHandler       ; Reserved                                ;23
                DCD     Reserved_IRQHandler       ; Reserved                                ;24
                DCD     Reserved_IRQHandler       ; Reserved                                ;25
                DCD     Reserved_IRQHandler       ; Reserved                                ;26
                DCD     Reserved_IRQHandler       ; Reserved                                ;27
                DCD     Reserved_IRQHandler       ; Reserved                                ;28
                DCD     Reserved_IRQHandler       ; Reserved                                ;29
                DCD     Reserved_IRQHandler       ; Reserved                                ;30
                DCD     FMC_IRQHandler            ; Flash Interrupt                         ;31


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
				
				LDR     R0, =SystemInit
				BLX     R0
				LDR     R0, =__main
				BX      R0
				ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP
Reserved_IRQHandler PROC
                EXPORT  Reserved_IRQHandler       [WEAK]
                B       .
                ENDP


Default_Handler PROC
                EXPORT  LVI_IRQHandler            [WEAK]		
                EXPORT  INTP0_IRQHandler          [WEAK]		
                EXPORT  INTP1_IRQHandler          [WEAK]
                EXPORT  INTP2_IRQHandler          [WEAK]
                EXPORT  INTP3_IRQHandler          [WEAK]
                EXPORT  TM01H_IRQHandler          [WEAK]
                EXPORT  CCP_IRQHandler            [WEAK]
                EXPORT  EPWM_IRQHandler           [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  ACMP_IRQHandler           [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]					
                EXPORT  I2C0_IRQHandler           [WEAK]
                EXPORT  SSP0_IRQHandler           [WEAK]
                EXPORT  TIMER0_IRQHandler         [WEAK]
                EXPORT  TIMER1_IRQHandler         [WEAK]
                EXPORT  LSITIMER_IRQHandler       [WEAK]
                EXPORT  TM00_IRQHandler           [WEAK]
                EXPORT  TM01_IRQHandler           [WEAK]
                EXPORT  TM02_IRQHandler           [WEAK]
                EXPORT  TM03_IRQHandler           [WEAK]
                EXPORT  FMC_IRQHandler            [WEAK]
LVI_IRQHandler     
INTP0_IRQHandler   
INTP1_IRQHandler   
INTP2_IRQHandler   
INTP3_IRQHandler   
TM01H_IRQHandler   
CCP_IRQHandler     
EPWM_IRQHandler    
ADC_IRQHandler     
ACMP_IRQHandler    
UART0_IRQHandler   
I2C0_IRQHandler    
SSP0_IRQHandler    
TIMER0_IRQHandler  
TIMER1_IRQHandler  
LSITIMER_IRQHandler
TM00_IRQHandler    
TM01_IRQHandler    
TM02_IRQHandler    
TM03_IRQHandler    
FMC_IRQHandler     
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
