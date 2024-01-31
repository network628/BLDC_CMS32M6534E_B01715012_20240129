;/**************************************************************************//**
; * @file     startup_CMS32L051.s
; * @brief    CMSIS Cortex-M0+ Core Device Startup File for
; *           Device CMS32L051
; * @version  V1.00
; * @date     2019/04/26
; ******************************************************************************/
;/*
; * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     0
        DCD     0
        DCD     0
__vector_table_0x1c
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     0
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External Interrupts
; ToDo:  Add here the vectors for the device specific external interrupts handler
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
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

; ToDo:  Add here the export definition for the device specific external interrupts handler
; ToDo:  Add here the names for the device specific external interrupts handler
        PUBWEAK LVI_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LVI_IRQHandler
        B LVI_IRQHandler

        PUBWEAK INTP0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
INTP0_IRQHandler
        B INTP0_IRQHandler

        PUBWEAK INTP1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
INTP1_IRQHandler
        B INTP1_IRQHandler

        PUBWEAK INTP2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
INTP2_IRQHandler
        B INTP2_IRQHandler

        PUBWEAK INTP3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
INTP3_IRQHandler
        B INTP3_IRQHandler

        PUBWEAK TM01H_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TM01H_IRQHandler
        B TM01H_IRQHandler

        PUBWEAK CCP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CCP_IRQHandler
        B CCP_IRQHandler

        PUBWEAK EPWM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EPWM_IRQHandler
        B EPWM_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_IRQHandler
        B ADC_IRQHandler

        PUBWEAK ACMP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ACMP_IRQHandler
        B ACMP_IRQHandler

        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_IRQHandler
        B UART0_IRQHandler

        PUBWEAK I2C0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_IRQHandler
        B I2C0_IRQHandler

        PUBWEAK SSP0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP0_IRQHandler
        B SSP0_IRQHandler

        PUBWEAK TIMER0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER0_IRQHandler
        B TIMER0_IRQHandler

        PUBWEAK TIMER1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER1_IRQHandler
        B TIMER1_IRQHandler

        PUBWEAK LSITIMER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LSITIMER_IRQHandler
        B LSITIMER_IRQHandler

        PUBWEAK Reserved_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved_IRQHandler
        B Reserved_IRQHandler

        PUBWEAK TM00_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TM00_IRQHandler
        B TM00_IRQHandler

        PUBWEAK TM01_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TM01_IRQHandler
        B TM01_IRQHandler

        PUBWEAK TM02_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TM02_IRQHandler
        B TM02_IRQHandler

        PUBWEAK TM03_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TM03_IRQHandler
        B TM03_IRQHandler

        PUBWEAK FMC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FMC_IRQHandler
        B FMC_IRQHandler

        END
