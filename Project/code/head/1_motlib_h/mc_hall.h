/*****************************************************************************/
/** \file mc_hall.h
**
** 
**
** History: 
** 
*****************************************************************************/
#ifndef  __MC_HALL_H__
#define  __MC_HALL_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include  "base_define.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
#define DEG_360 	(65535)
#define DEG_330 	(60074)
#define DEG_300 	(54613)
#define DEG_270 	(49152)
#define DEG_240 	(43690)
#define DEG_210 	(38229)
#define DEG_180 	(32768)
#define DEG_150 	(27306)
#define DEG_120 	(21845)
#define DEG_90  	(16384)
#define DEG_60  	(10923)
#define DEG_30  	(5461)
#define DEG_0   	(0)

/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef struct 
{	
	unsigned char  	Value;
	unsigned char  	Value_Pre;
	unsigned char  	Num_Pre;
	unsigned char  	Num;	
	unsigned char  	FR_Flag;

	unsigned char  	Start_Flag;

	unsigned int   	Speed;
	unsigned int 	Tim_Count[9];
	unsigned int 	Arry[8][2];
	
	unsigned int 	Start_Inc_Min;
	unsigned int 	Start_Inc_Max;	
	unsigned int 	Krpm;
	unsigned int 	Secotr_Count;		
	
	signed int 		ADC_Data[9];	
	signed int 		Theta_Inc;
	signed int 		Efreq;
	signed int 		Angle_Correct;
	signed int 		Angle_Sector;
	signed int 		Delta_Theta;
	signed int 		Fre;	
	signed int 		AngelEstimate;	
	signed int 		Tim_We;	
	signed int 		Tim_Sum;	
	signed int 		Tim_Ave;			
	signed int 		PI_Error;	
	signed int 		PI_Kp;	
	signed int 		PI_Out;		
	signed int 		We_Out;	
	
}struct_Hall;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern struct_Hall				stru_Hall;
extern const 	signed int 	HALL_Angle_CW[8][2];
extern const 	signed int 	HALL_Angle_CCW[8][2];

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void 	FOC_Hall_Init(struct_Hall *p);
void 	FOC_Get_HallState(struct_Hall *HALL_Sense);
void 	FOC_HallSpeed_Calc(struct_Hall *p);
void 	FOC_Hall_Start(struct_Hall *p);
void 	FOC_CloseLoop_Switch( s32 s32_Theta_LS , s32 s32_Theta_HS );

#endif	/* __MC_HALL_H__ */

