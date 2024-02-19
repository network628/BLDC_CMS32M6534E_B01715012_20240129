/*****************************************************************************/
/** \file mc_openloop.h
**
**
**
** History:
**
*****************************************************************************/
#ifndef __MC_OPENLOOP_H__
#define __MC_OPENLOOP_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
#define START_REVUP_TIME1 (3000) // 阶段1 爬升时间 单位：ms
#define START_REVUP_TIME2 (4000) // 阶段2 爬升时间 单位：ms
#define START_REVUP_TIME3 (4000) // 阶段3 爬升时间 单位：ms

#define START_REVUP_CURRENT1 (6000) // 阶段1 电流 单位：Q15
#define START_REVUP_CURRENT2 (6000) // 阶段2 电流 单位：Q15
#define START_REVUP_CURRENT3 (6000) // 阶段3 电流 单位：Q15

#define START_REVUP_WE_K1 (100) // 阶段1 速度斜率 每WE_K1周期角度增量加1
#define START_REVUP_WE_K2 (75)	// 阶段2 速度斜率 每WE_K2周期角度增量加1
#define START_REVUP_WE_K3 (70)	// 阶段3 速度斜率 每WE_K3周期角度增量加1

#define START_REVUP_WEMIN (1)	// 开环最小We
#define START_REVUP_WEMAX (100) // 开环最大We

/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef struct
{
	unsigned char Falg;
	signed int Count;

	unsigned short int RevUp_T1;
	unsigned short int RevUp_T2;
	unsigned short int RevUp_T3;
	signed int RevUp_I1;
	signed int RevUp_I2;
	signed int RevUp_I3;
	signed int We_K1;
	signed int We_K2;
	signed int We_K3;
	signed int We;
	signed int We_Min;
	signed int We_Max;
	signed int We_Coffe;

} struct_OpenLoop;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern struct_OpenLoop stru_OpenLoop;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void FOC_OpenLoop_Init(void);
void FOC_Align_Start(void);
void FOC_OpenLoop_Start(void);

#endif /* __MC_OPENLOOP_H__ */
