/*****************************************************************************/
/** \file mc_pmsm.h
**
** 
**
** History:
** 
*****************************************************************************/
#ifndef  __MC_PMSM_H__
#define  __MC_PMSM_H__


/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include <stdint.h>
#include "mc_foc.h"
#include "para_define.h"
#include "base_define.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/

	
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef struct
{
	int32_t 	Alpha;
	int32_t 	Beta;
	
}struct_Clark;

typedef struct
{
	int32_t 	Id;
	int32_t 	Iq;
	
}struct_Park;

typedef struct
{
	int32_t 	Alpha;
	int32_t 	Beta;
	
}struct_RevPark;

typedef struct
{
	int32_t 	Cos;
	int32_t 	Sin;
	
}struct_SinCos;

typedef struct 
{
	int32_t 	Ud;
	int32_t 	Uq;
	
}struct_Volt_dq;

typedef struct
{
	int32_t 	Angel;
	
}struct_Arctan;

typedef struct
{
	uint8_t  	Sector;
	int32_t 	Tu;
	int32_t 	Tv;
	int32_t 	Tw;
	int32_t 	T1;
	int32_t 	T2;
	int32_t 	Tu_up;
	int32_t 	Tu_dn;
	int32_t 	Tv_up;
	int32_t 	Tv_dn;
	int32_t 	Tw_up;
	int32_t 	Tw_dn;
	int32_t 	Ta_up;
	int32_t 	Ta_dn;
	int32_t 	Tb_up;
	int32_t 	Tb_dn;
	int32_t 	Tc_up;
	int32_t 	Tc_dn;	

	
}struct_SVPWM;



/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern struct_SinCos	  		stru_SinCos;
extern struct_Clark	  			stru_Curr_alphabeta;
extern struct_Park		  		stru_Curr_dq;
extern struct_Park		  		stru_Curr_dq_ref;
extern struct_RevPark	  		stru_Volt_alphabeta;
extern struct_Volt_dq	  		stru_Volt_dq;
extern struct_Arctan      	stru_Arctan;  
extern struct_SVPWM        	stru_SVM_Calc;


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
struct_SinCos  							FOC_SinCos_Calc(int32_t Angel);
struct_Clark 								Clarke(struct_Curr_abc Iphase);
struct_Park 								Park(struct_SinCos SinCosVal, struct_Clark Clarke);
struct_RevPark 							RevPark(struct_SinCos SinCosVal, struct_Volt_dq Volt);
struct_Arctan 							ArcTan_Cal(int32_t hSin, int32_t hCos);
void 												FOC_CALC_SVPWM(struct_SVPWM *SVM_Compt, struct_RevPark  Volt_alfa_beta);
void  											FOC_SinCosCal(struct_SinCos  *p_SinCosRegS ,  int32_t l_angel);
void 												FOC_DoubleShunt_Calc(struct_SVPWM *SVM_Compt, struct_RevPark  Volt_alfa_beta);
void 												FOC_EPWM_UPDATA(void);
int32_t 										FOC_ArcTan_Cal(int32_t hSin,  int32_t hCos);

#endif	/* __MC_PMSM_H__ */
