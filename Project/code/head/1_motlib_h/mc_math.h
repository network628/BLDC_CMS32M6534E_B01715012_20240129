/*****************************************************************************/
/** \file mc_wgl.h
**
** 
**
** History:
** 
*****************************************************************************/
#ifndef  __MC_WGL_H__
#define  __MC_WGL_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include  "base_define.h"
#include  <stdint.h>

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/


/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
unsigned int 	_sqrt(unsigned long value);
unsigned int 	sqrt_Vs(signed int *Vd);
signed int 		Vector_Vq_Limit(signed int *Volt_d);
unsigned int 	Sqrt_Q15(signed long value);
void 			    FOC_Field_Weakening(void);
void 			    MC_MotorPower_Calc(void);
void 			    MC_KeSpeed_Calc(void);
void  		    FOC_ThetaLimit(signed int *theta);
int32_t 	    FOC_MotorSpeed_Filter(int32_t xn);
 
/*****************************************************************************/

#endif	 /* __MC_WGL_H__ */


