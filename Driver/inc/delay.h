#ifndef _DELAY_H__
#define _DELAY_H__

#include <stdint.h> 
#include "common.h"
//#include "compiler_abstraction.h"
/**
 * @brief Function for delaying execution for number of microseconds.
 *
 * @note SystemCoreClock = 48MHz
 *
 * @param number_of_ms
 */
/*lint --e{438, 522} "Variable not used" "Function lacks side-effects" */
#if defined ( __CC_ARM   )

//static __ASM void __INLINE delay_us(uint32_t volatile number_of_us)
//{
//loop
//        SUBS    R0, R0, #1
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP
//        NOP


//        BNE    loop
//        BX     LR
//}
#endif

void m0_delay_ms(uint32_t volatile number_of_ms);
void m0_delay_us(uint32_t volatile number_of_us);
void delay_init(uint32_t sysclk);

#endif
