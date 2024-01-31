/*****************************************************************************/
/** \file adaptives.h
**
** 
**
** History: 
** 
*****************************************************************************/
#ifndef  __MC_WIND_H__
#define  __MC_WIND_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include  "base_define.h"


/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
 
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern volatile uint8_t FOC_Wind_Flag;
extern volatile uint8_t FOC_Wind_Status;
extern int32_t  FOC_Wind_We;
extern int32_t  FOC_Wind_We_Init;
extern uint32_t FOC_Wind_Check_Time;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void FOC_Wind_Init(void);
void FOC_Wind_Check(void);
void FOC_Wind_Start(void);
void FOC_Wind_Brake(void);
void FOC_Wind_ReverseBrake(void);
void FOC_TailWind_Start(void);

#endif	/* __MC_WIND_H__ */
