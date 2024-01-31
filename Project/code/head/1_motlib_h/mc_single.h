/*****************************************************************************/
/** \file single.h
**
** 
**
** History: 
** 
*****************************************************************************/
#ifndef  __MC_SINGLE_H__
#define  __MC_SINGLE_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include  "mc_pmsm.h"
//#include  "base_type.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/


/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef struct
{
	uint8_t Sector;
	int32_t Ia;
	int32_t Ib;
	int32_t Ic;
	volatile uint8_t	LN_States;
	volatile uint8_t 	LN_States_Pre;
	volatile uint8_t 	LN_Flag;

}struct_Singlecurr;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern struct_Singlecurr						stru_SingleCurr;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void FOC_SingleShunt_Calc(struct_SVPWM *SVM, struct_RevPark  Volt_alfa_beta);
void FOC_NSingleShunt_Calc(struct_SVPWM *SVM, struct_RevPark  Volt_alfa_beta);
void FOC_SingleCurrent_Calc(void);
void FOC_SingleShunt_Init(void);


#endif	/* __MC_WIND_H__ */
