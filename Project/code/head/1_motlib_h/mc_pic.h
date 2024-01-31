/*****************************************************************************/
/** \file mc_pic.h
**
** 
**
** History:
** 
*****************************************************************************/

#ifndef __MC_PIC_H__
#define __MC_PIC_H__

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
typedef struct 
{ 	
  int32_t Kp;
  int32_t Ki;
  int32_t Lower_Output;     //Lower Limit for Output limitation
  int32_t Upper_Output;     //Lower Limit for Output limitation
  int32_t Lower_Integral;   //Lower Limit for Integral term limitation
  int32_t Upper_Integral;   //Lower Limit for Integral term limitation
  int32_t Integral;
  int32_t Out;
  int32_t KiSum;
  int32_t Error;
} PID_Components;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern  PID_Components   PID_ID, PID_IQ, PID_WL, PID_FW;
extern  int32_t PI_SpeedLoop_Kd;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void 	FOC_PID_Parameter_Init(void);
int32_t 	PID_CALC(PID_Components *PID_Struct,int32_t hReference, int32_t hPresentFeedback);
int32_t 	PID_CALC2(PID_Components *PID_Struct,int32_t hReference, int32_t hPresentFeedback);
int32_t 	PI_Regulator(PID_Components *PID_Struct,int32_t hReference, int32_t hPresentFeedback);
int32_t 	HL_PI_AntiDump(PID_Components *PID_Struct,int32_t hReference, int32_t hPresentFeedback);
int32_t 	PID_SpeedLoop(PID_Components *PID_Struct,int32_t hReference, int32_t hPresentFeedback);

#endif	/* __MC_PIC_H__ */


