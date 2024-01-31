//==========================================================================//
/*****************************************************************************
*-----------------------------------------------------------------------------
* @file    mc_adaption.h
* @author  CMS Motor Control Team
* @version V3.0.0
* @date    2021年3月
* @brief   自适应观测器
*---------------------------------------------------------------------------//
*****************************************************************************/
//==========================================================================//

#ifndef  __MC_ADAPTION_H__
#define  __MC_ADAPTION_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include <stdint.h>
#include "base_define.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
#define		_IQmpy(x,y) 				(((x)*(y))>>15)
#define 	_IQabs(Value)			 	((Value)>=0?(Value):-(Value))
#define		_IQdiv(x,y)					(HWDIV_Div((x),(y)))


/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/

//---------------------------------------------------------------------------/
typedef struct
{
	int32_t 	G;
	int32_t 	B;
	int32_t 	C;	

	int32_t 	Valpha;	
	int32_t 	Vbeta;	
	int32_t 	Ialpha;
	int32_t 	Ibeta;	
	int32_t 	Ealpha;		
	int32_t 	Ebeta;		
	int32_t		Error;

	int32_t		PI_Kp;	
	int32_t		PI_Ki;	
	int32_t		PI_Ui;
	int32_t 	PI_Out;		
	int32_t		PI_Upper;
	int32_t		PI_Lower;
	
	int32_t		We;	
	int32_t		GWe;	
	int32_t 	CWe;		
	int32_t		LWe;
	int32_t		We_LPF;	
	int32_t		Angle;
	
}struct_adaption;

//---------------------------------------------------------------------------/
typedef struct
{
	int32_t 	XR;
	int32_t 	XL;
	int32_t 	Tc;	
	int32_t 	Fb;	
	int32_t 	Fs;
	int32_t 	Gs;		
	int32_t 	Fc_LPF;	
	
	int32_t 	Valpha;	
	int32_t 	Vbeta;	
	int32_t 	Ialpha;
	int32_t 	Ibeta;	
	int32_t 	Ealpha;		
	int32_t 	Ebeta;		
	int32_t 	Zalpha;		
	int32_t 	Zbeta;	
	int32_t 	Est_Ialpha;		
	int32_t 	Est_Ibeta;
	int32_t		IalphaError;
	int32_t		IbetaError;

	int32_t		PI_Error;
	int32_t		PI_Kp;	
	int32_t		PI_Ki;	
	int32_t		PI_Ui;
	int32_t 	PI_Out;		
	int32_t		PI_Upper;
	int32_t		PI_Lower;
	
	int32_t		E0;
	int32_t		Kslide;
	int32_t		Kslf;

	int32_t		We;		
	int32_t		We_LPF;	
	int32_t		GWe;
	int32_t		Angle;	
	int32_t		Speed;	
	
}struct_rpd_est;
 
typedef struct
{	
	int32_t 	Valpha;	
	int32_t 	Vbeta;	
	int32_t 	Ialpha;
	int32_t 	Ibeta;	
	int32_t 	Ealpha;		
	int32_t 	Ebeta;		
	int32_t 	Zalpha;		
	int32_t 	Zbeta;	
	int32_t 	Est_Ialpha;		
	int32_t 	Est_Ibeta;
	int32_t		IalphaError;
	int32_t		IbetaError;

	int32_t		PI_Error;
	int32_t		PI_Kp;
	int32_t		PI_Ki;
	int32_t		PI_Ui;
	int32_t 	PI_Out;
	int32_t		PI_Upper;
	int32_t		PI_Lower;
	
	int32_t 	Fs;
	int32_t 	Gs;
	
	int32_t		E0;
	int32_t		Kslide;
	int32_t		Kslf;

	int32_t		We;
	int32_t		GWe;
	int32_t		Angle;		
	int32_t		OffsetAngle;
}struct_smo_est;

//---------------------------------------------------------------------------/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern struct_adaption			stru_adap_est;
extern struct_rpd_est				stru_rpd_est;
extern struct_smo_est				stru_smo_est;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void FOC_Adap_Angle_Init(void);
void FOC_Rpd_Angle_Init(void);
void FOC_RotorPosition_Init(void);
void FOC_RotorPosition_Detection(void);
void FOC_BaseParameter_Init(void);
void FOC_Adaption_Angle_Calc(struct_adaption *pHandle);
void FOC_RotorPosition_Calc(struct_rpd_est *pHandle);
void FOC_SMO_Angle_Calc(struct_smo_est *pHandle);

#endif	/* __MC_ADAPTION_H__ */

