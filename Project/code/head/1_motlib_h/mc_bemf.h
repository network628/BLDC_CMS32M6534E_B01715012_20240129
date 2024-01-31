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
	int32_t  wEuADValue;             //Bemf_U���ѹAD����ֵ
	int32_t  wEvADValue;             //Bemf_V���ѹAD����ֵ
	int32_t  wEwADValue;             //Bemf_W���ѹAD����ֵ
	int32_t  wEuCrossZero;           //Bemf_U���ѹ����ֵ
	int32_t  wEvCrossZero;           //Bemf_V���ѹ����ֵ
	int32_t  wEwCrossZero;           //Bemf_W���ѹ����ֵ

	int32_t  wEPWMDuty;              //EPWMռ�ձ����  
	int32_t  wEPWMDutyStart;         //EPWMռ�ձ��������ֵ
	int32_t  wEPWMDutyMax;           //EPWMռ�ձ�������ֵ
	
  int32_t  wGWe;                   //Weʵʱֵ
  int32_t  wWe;                    //We�˲�ֵ
  int32_t  wSpeed;                 //ת��	
  int32_t  wWeTime;                //����ʱ���������ת�ټ���
  
  uint32_t wMaskTimeInit;          //��ʼ����ʱ��
  uint32_t wMaskTimeMin;           //��С����ʱ��
	uint32_t wMaskTime;              //����ʱ��
  
 	uint16_t hEPWMDutyRamp;          //EPWMռ�ձ���������
 	uint16_t hFirstLockTime;         //��һ������ʱ��
  
	uint16_t hForceSwitchTimeMax;    //ǿ�ƻ������ʱ��  
	uint16_t hForceSwitchTime;       //ǿ�ƻ���ʱ��
	uint16_t hForceSwitchCount;      //ǿ�ƻ������
	
	uint16_t hSwitchPhaseTime;       //��ʱ����ʱ��
	uint16_t hSwitchPhaseCount;      //����㻻�����
  
	uint8_t  bIndexNow;              //��ǰ����
	uint8_t  bIndexPre;              //��һ�β���
	uint8_t  bSwitchPhaseFlag;       //�����־
	uint8_t  bCrossZeroFlag;         //�����־
	uint8_t  bForceSwitchSteps;      //ǿ�ƻ��ಽ��
	uint8_t  bStartSuccessFlag;      //�����ɹ���־	
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
