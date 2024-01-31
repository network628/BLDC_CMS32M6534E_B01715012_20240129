/*****************************************************************************/
/** \file mc_foc.h
**
** 
**
** History:
** 
*****************************************************************************/
#ifndef  __MC_FOC_H__
#define  __MC_FOC_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include  "base_define.h"


/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/


#define  FOC_STOP_UQ		(1000)		//电机停止UQ电压值



/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef struct
{
	int32_t Ia;
	int32_t Ib;
	int32_t Ic;
}struct_Curr_abc;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern struct_Curr_abc		  	stru_Curr_abc;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void FOC_Read_Current(void);
void FOC_ControlPara_Init(void);
void FOC_Control_Process(void);
void FOC_EPWM_UPDATA(void);
void FOC_Stop_Check(void);
void FOC_Start_Check(void);
void FOC_Overcurrent_Check(void);

#endif    /* __MC_FOC_H__ */   

