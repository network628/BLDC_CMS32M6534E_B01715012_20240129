/****************************************************************************/
/**
* @file    mc_ipd.h
* @author  CMS Motor Control Team
* @version V2.0.0
* @date    01-03-2018
* @brief   
**
*****************************************************************************/
#ifndef __MC_IPD_H__
#define __MC_IPD_H__

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include  "base_define.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
#define		PULSE_SINE					

#ifndef		PULSE_SINE					// ���Ҳ����巽ʽ��˫���跽��
	#define	PULSE_SQUARE				// �������巽ʽ��˫����/�����跽��
#else
	#define PULSE_SINE
#endif

 

#define		PUlSE_INJECT_ROUND			(3)			// ����ע���ִ�
#define		PUlSE_VECTOR_LIMIT			(26214)		// �������ռ�ձ����ƣ�26214 = 32767*80%
#define		PUlSE_RETURN_ANGLE			(DEG_60)	// ����Ƕȳ�ʼУ��

// ��������
#define		PUlSE_SINE_START			(6)			// ���忪ʼ����ʱ��
#define		PUlSE_SINE_END				(2)			// ��������ʱ��
#define		PUlSE_SINE_OFF				(50)		// ����ض�ʱ��

// ��������
#define		PUlSE_SQUARE_CHARGE			(10)		// �Ծٳ��ʱ��
#define		PUlSE_SQUARE_CHARGE_OFF		(10+PUlSE_SQUARE_CHARGE)		// �������ض�����

#define		PUlSE_SQUARE_END			(20+PUlSE_SQUARE_CHARGE_OFF)	// ����ض�ʱ��	


#define		PUlSE_SQUARE_SAMP_START		(PUlSE_SQUARE_CHARGE_OFF)		// ��ʼ����ʱ��

	


#define	 	ERRORRETURN					(100000)




//IPD DRIVE PAHSE SELECT
#define 	TWO_PAHSE                	1
#define 	THREE_PAHSE              	2                          
#define 	IPD_DRIVE               	TWO_PAHSE 

#define		AH							(0x0001ul)  //0000 0001		
#define		BH							(0x0004ul)	//0000 0100			
#define		CH							(0x0010ul)  //0001 0000

#define		AL							(0x0002ul) 	//0000 0010			
#define		BL							(0x0008ul)  //0000 1000		
#define		CL							(0x0020ul)	//0010 0000

/*POSITION CHECK USE*/
#define		PDRIVE_A_BC					(0X3F00 | AH | BL | CL)
#define		PDRIVE_B_CA					(0X3F00 | BH | CL | AL)
#define		PDRIVE_C_AB					(0X3F00 | CH | AL | BL)
#define		PDRIVE_AB_C					(0X3F00 | AH | BH | CL)
#define		PDRIVE_BC_A					(0X3F00 | BH | CH | AL)
#define		PDRIVE_CA_B					(0X3F00 | CH | AH | BL)

#define		PDRIVE_A_B					(0X3F00 |AH | BL)
#define		PDRIVE_A_C					(0X3F00 |AH | CL)
#define		PDRIVE_B_C					(0X3F00 |BH | CL)
#define		PDRIVE_B_A					(0X3F00 |BH | AL)
#define		PDRIVE_C_A					(0X3F00 |CH | AL)
#define		PDRIVE_C_B					(0X3F00 |CH | BL)

/*OTHER USE*/
#define  	DRIVER_OFF					(0X3F00)
#define  	DRIVER_AL      			(0X3F00 | AL)
#define  	DRIVER_BL      			(0X3F00 | BL)
#define  	DRIVER_CL      			(0X3F00 | CL)
#define  	DRIVER_ABCL      		(0X3F00 | AL | BL | CL)
#define  	DRIVER_ABCH					(0X3F00 | AH | BH | CH)

/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/
typedef struct
{
	int32_t A;
	int32_t B;
	int32_t C;
}Abccur_Component_t;

typedef struct
{
	uint8_t			Correct_Flag;			//������ȷʶ���־
	uint8_t			InjectRound;			//����ע���ִα�ʶ����0�����һ������ע��
	uint8_t			InjectAngleCounter;		//�������
	uint16_t		InjectTimeCounter;		//һ��������Ҫ����ʱ��
	uint16_t		InjectAngleIndex;		//���idֵ���
	uint16_t		InjectAngleIndex2;		//���idֵ���
	int32_t			InjectAngle;			//����ע��Ƕ�ֵ
	int32_t			InjectMaxAcquiredValue;
	uint16_t		InjectTime;
	int32_t			InjectCurrent;
	int32_t			InjectCurrent_Base;
}Pulse_Component_t;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/
extern Pulse_Component_t	stru_Pulse;

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/
extern int32_t  FOC_IPD_PulseSine(Pulse_Component_t	*p);
extern void     FOC_IPD_Init(Pulse_Component_t *p);
extern uint32_t MC_IPD_PulseSquare(Pulse_Component_t *p);

#endif	/* __MC_IPD_H__ */

