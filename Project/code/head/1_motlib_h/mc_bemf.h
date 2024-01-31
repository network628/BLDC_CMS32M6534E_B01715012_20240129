/*****************************************************************************/
/** \file mc_bemf.h
**
** 
**
** History:
** 
*****************************************************************************/
#ifndef  __MC_BEMF_H__
#define  __MC_BEMF_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include <stdint.h>

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/


/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef struct
{
	int32_t  wEuADValue;             //Bemf_U相电压AD采样值
	int32_t  wEvADValue;             //Bemf_V相电压AD采样值
	int32_t  wEwADValue;             //Bemf_W相电压AD采样值
	int32_t  wEuCrossZero;           //Bemf_U相电压过零值
	int32_t  wEvCrossZero;           //Bemf_V相电压过零值
	int32_t  wEwCrossZero;           //Bemf_W相电压过零值

	int32_t  wEPWMDuty;              //EPWM占空比输出  
	int32_t  wEPWMDutyStart;         //EPWM占空比输出启动值
	int32_t  wEPWMDutyMax;           //EPWM占空比输出最大值
	
  int32_t  wGWe;                   //We实时值
  int32_t  wWe;                    //We滤波值
  int32_t  wSpeed;                 //转速	
  int32_t  wWeTime;                //换相时间计数用于转速计算
  
  uint32_t wMaskTimeInit;          //初始屏蔽时间
  uint32_t wMaskTimeMin;           //最小屏蔽时间
	uint32_t wMaskTime;              //屏蔽时间
  
 	uint16_t hEPWMDutyRamp;          //EPWM占空比爬坡周期
 	uint16_t hFirstLockTime;         //第一拍锁定时间
  
	uint16_t hForceSwitchTimeMax;    //强制换相最大时间  
	uint16_t hForceSwitchTime;       //强制换相时间
	uint16_t hForceSwitchCount;      //强制换相计数
	
	uint16_t hSwitchPhaseTime;       //延时换相时间
	uint16_t hSwitchPhaseCount;      //过零点换相次数
  
	uint8_t  bIndexNow;              //当前步序
	uint8_t  bIndexPre;              //上一次步序
	uint8_t  bSwitchPhaseFlag;       //换相标志
	uint8_t  bCrossZeroFlag;         //过零标志
	uint8_t  bForceSwitchSteps;      //强制换相步序
	uint8_t  bStartSuccessFlag;      //启动成功标志	
	uint8_t  bTIM1State;             //	
	uint8_t  bBLDCState;             //	
  
}struct_Bemf_Start;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern struct_Bemf_Start stru_BLDC_BEMF;
extern const uint8_t  bBLDCIndexNextArr[7];
extern const uint32_t  EPWM_MaskArr_CW[7];
extern const uint32_t  EPWM_MaskArr_CCW[7];

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
void  BLDC_BEMF_Start_Control(void);
void  BLDC_BEMF_Start_Config(void);


/*****************************************************************************/

#endif	 /* __MC_BEMF_H__ */
