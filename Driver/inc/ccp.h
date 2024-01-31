/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/*****************************************************************************/
/** \file ccp.h
**
** History:
** 
*****************************************************************************/
#ifndef __CCP_H_
#define __CCP_H_
 
#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "common.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
/*----------------------------------------------------------------------------
 ** CCP ģ�� & ͨ��
-----------------------------------------------------------------------------*/
#define		CCP0			(0x00UL)		/*CCPģ��0*/
#define		CCP1			(0x01UL)		/*CCPģ��1*/	
/*----------PWMģʽ/����ģʽ0ͨ��--------------------------------------------*/
#define		CCPxA			(0x00UL)
#define		CCPxB			(0x01UL)

/*----------����ģʽ1�ڲ�ͨ��------------------------------------------------*/
#define		CAP0			(0x00UL)
#define		CAP1			(0x01UL)
#define		CAP2			(0x02UL)
#define		CAP3			(0x03UL)
/*----------����ģʽ2--------------------------------------------------------*/
#define 	CAP1_SUB_CAP2	(0x04UL)		
#define 	CAP1_SUB_CAP3	(0x05UL)

/*----------����ģʽ1 �ڲ�ͨ������ѡ���ⲿͨ��-------------------------------*/
#define		ECAP00			(0x00UL)
#define		ECAP01			(0x01UL)
#define		ECAP02			(0x02UL)
#define		ECAP03			(0x03UL)

#define		ECAP10			(0x10UL)
#define		ECAP11			(0x11UL)
#define		ECAP12			(0x12UL)
#define		ECAP13			(0x13UL)

/*----------����ģʽ1 ��ѡACMPͨ��---------------------------------------------*/
#define		CAP3_ACMP0		(0x38UL)
#define		CAP3_ACMP1		(0x39UL)
/*----------����ģʽ1 ��ѡCCPnͨ��---------------------------------------------*/
#define		CAP0_CCP0A		(0x0FUL)
#define		CAP1_CCP0B		(0x1FUL)
#define		CAP2_CCP1A		(0x2FUL)
#define		CAP3_CCP1B		(0x3FUL)


/*----------------------------------------------------------------------------
 ** CCP ģʽ
-----------------------------------------------------------------------------*/
#define		CCP_PWM_MODE		(0x00UL)		/*CCPģ��PWM���ģʽ*/
#define 	CCP_CAP_MODE0		(0x01UL)		/*CCPģ�鲶��ģʽ0*/
#define 	CCP_CAP_MODE1		(0x02UL)		/*CCPģ�鲶��ģʽ1*/
#define 	CCP_CAP_MODE2		(0x03UL)		/*CCPģ�鲶��ģʽ2*/
/*----------------------------------------------------------------------------
 ** CCP ����������ģʽ
-----------------------------------------------------------------------------*/
#define		CCP_RELOAD_0XFFFF		(0x00)		/*�������ؼ���ֵΪ0xFFFF*/
#define		CCP_RELOAD_CCPLOAD		(0x01)		/*���������ؼ���ֵΪCCPxLOAD*/
/*----------------------------------------------------------------------------
 ** CCP ʱ��
-----------------------------------------------------------------------------*/
#define		CCP_CLK_DIV_1		(0x00UL)		/*Fccp = PCLK/1*/
#define		CCP_CLK_DIV_4		(0x01UL)		/*Fccp = PCLK/4*/
#define		CCP_CLK_DIV_16		(0x02UL)		/*Fccp = PCLK/16*/
#define		CCP_CLK_DIV_64		(0x03UL)		/*Fccp = PCLK/64*/

/*----------------------------------------------------------------------------
 ** CCP ����ģʽ
-----------------------------------------------------------------------------*/
/*----------����ģʽ0---------------------------------------------*/
#define		CCP_CAP_MODE0_RISING				(0x00)	/*�����ز���*/
#define		CCP_CAP_MODE0_FALLING				(0x01)	/*�½��ز���*/
#define		CCP_CAP_MODE0_RISING_TO_FALLING		(0x02)	/*����������ص��½���*/
#define		CCP_CAP_MODE0_FALLING_TO_RISING		(0x03)  /*������½��ص�������*/
/*----------����ģʽ1------------------------------------------------*/
#define 	CCP_CAP_MODE1_DISABLE				(0x00)	/*�����ֹ*/
#define		CCP_CAP_MODE1_RISING				(0x01)	/*�����ز���*/
#define		CCP_CAP_MODE1_FALLING				(0x02)	/*�½��ز���*/
#define		CCP_CAP_MODE1_BOTH_EDGE				(0x03)	/*˫��(�½���&&������)����*/

/*----------����ģʽ2------------------------------------------------*/
#define 	CCP_CAP_MODE2_DISABLE				(0x00)	/*�����ֹ*/
#define		CCP_CAP_MODE2_RISING				(0x01)	/*�����ز���*/
#define		CCP_CAP_MODE2_FALLING				(0x02)	/*�½��ز���*/
#define		CCP_CAP_MODE2_BOTH_EDGE				(0x03)	/*˫��(�½���&&������)����*/

/*----------------------------------------------------------------------------
 ** CCP д����
-----------------------------------------------------------------------------*/
#define		CCP_P1A_WRITE_KEY		(0xAAUL)
#define		CCP_P1AB_WRITE_KEY		(0x55UL)


/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/

/*****************************************************************************
 ** \brief	CCP_Start
 **			����CCPģ��
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_Start(uint32_t CCPn );
/*****************************************************************************
 ** \brief	CCP_Stop
 **			�ر�CCPģ��
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_Stop(uint32_t CCPn );
/*****************************************************************************
 ** \brief	CCP_ConfigCLK
 **			����CCPnģ��ʱ��
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				ClkDiv:(1) CCP_CLK_DIV_1
 **					   (2) CCP_CLK_DIV_4
 **					   (3) CCP_CLK_DIV_16
 **					   (4) CCP_CLK_DIV_64
 **				ReloadMode: (1) CCP_RELOAD_0XFFFF    �������ؼ���ֵΪ0xFFFF (����ģʽ)
 **							(2) CCP_RELOAD_CCPLOAD   ���������ؼ���ֵΪCCPxLOAD
 **				Preiod: 0x00~ 0xffff; ���������ֵ��Ϊ0��		
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ConfigCLK(uint32_t CCPn, uint32_t ClkDiv ,uint32_t ReloadMode, uint32_t Period);

/*****************************************************************************
 ** \brief	CCP_EnableRun
 **			CCPn����
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableRun(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_DisableRun
 **			CCPn�ر�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableRun(uint32_t CCPn);

/*****************************************************************************
 ** \brief	CCP_EnablePWMMode
 **			����PWM����ģʽ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnablePWMMode(uint32_t CCPn);

/*****************************************************************************
 ** \brief	CCP_EnablePWMHalfCycleMode
 **			����PWM����ģʽ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB	
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnablePWMHalfCycleMode(uint32_t CCPn, uint32_t CCPChannel);
/*****************************************************************************
 ** \brief	CCP_DisablePWMHalfCycleMode
 **			�ر�PWM����ģʽ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB	
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisablePWMHalfCycleMode(uint32_t CCPn, uint32_t CCPChannel);

/*****************************************************************************
 ** \brief	CCP_ConfigCompare
 **			����CCPnģ��ͨ��n �Ƚ�ֵ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB			
 **				Compare : 0x00~ 0xffff;
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ConfigCompare(uint32_t CCPn, uint32_t CCPChannel ,uint32_t Compare);
/*****************************************************************************
 ** \brief	CCP_EnableReverseOutput
 **			�����������
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableReverseOutput(uint32_t CCPn, uint32_t CCPChannel);
/*****************************************************************************
 ** \brief	CCP_DisableReverseOutput
 **			�رշ������
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 **				CCPChannel: CCPxA ��CCPxB
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableReverseOutput(uint32_t CCPn, uint32_t CCPChannel);
/*****************************************************************************
 ** \brief	CCP_EnableOverflowInt
 **			��������ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableOverflowInt(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_DisableOverflowInt
 **			�ر�����ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableOverflowInt(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_GetOverflowIntFlag
 **			��ȡ����ж�ʹ�ܲ�����жϱ�־λ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  1:����ж�ʹ�ܲ��Ҳ����ж�  0:���ж�
 ** \note	
*****************************************************************************/
uint32_t CCP_GetOverflowIntFlag(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_ClearOverflowIntFlag
 **			�������жϱ�־λ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ClearOverflowIntFlag(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_EnableCompareInt
 **			�����Ƚ��ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableCompareInt(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_DisableCompareInt
 **			�رձȽ��ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableCompareInt(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_GetCompareIntFlag
 **			��ȡ�Ƚ��ж�ʹ�ܲ������жϱ�־
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCompareIntFlag(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_ClearCompareIntFlag
 **			����Ƚ��жϱ�־
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ClearCompareIntFlag(uint32_t CCPn);




/*****************************************************************************
 ** \brief	CCP_EnableCAPMode0
 **			��������ģʽ0
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPMode0(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_ConfigCAPMode0
 **			���ò���ģʽ0 �Ĳ���ʽ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��	
 **				Mode : (1)CCP_CAP_MODE0_RISING
 **					   (2)CCP_CAP_MODE0_FALLING
 **					   (3)CCP_CAP_MODE0_RISING_TO_FALLING
 **					   (4)CCP_CAP_MODE0_FALLING_TO_RISING
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_ConfigCAPMode0(uint32_t CCPn, uint32_t Mode);
/*****************************************************************************
 ** \brief	CCP_SelCAPMode0Channel
 **			ѡ�񲶻�ģʽ0 �Ĳ���ͨ��
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��	
 **				Channel : CCPxA ��CCPxB 
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_SelCAPMode0Channel(uint32_t CCPn, uint32_t Channel);
/*****************************************************************************
 ** \brief	CCP_GetCAPMode0Result
 **			��ȡ����ģʽ0 �Ĳ���ͨ��ֵ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��	
**				Channel : CCPxA ��CCPxB 
 ** \return  16bit result
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCAPMode0Result(uint32_t CCPn, uint32_t Channel);
/*****************************************************************************
 ** \brief	CCP_EnableCAPMode0Int
 **			��������ģʽ�����ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableCAPMode0Int(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode0Int
 **			�رղ���ģʽ�����ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableCAPMode0Int(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_GetCAPMode0IntFlag
 **			��ȡ����ģʽ�����ж�ʹ�ܲ��Ҳ����ж�
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return 1�������ж�  0:���ж�
 ** \note	
*****************************************************************************/
uint32_t CCP_GetCAPMode0IntFlag(uint32_t CCPn);
/*****************************************************************************
 ** \brief	CCP_ClearCAPMode0IntFlag
 **			�������ģʽ�����жϱ�־λ
 ** \param [in] CCPn : CCP0ģ�顢CCP1ģ��
 ** \return none
 ** \note	
*****************************************************************************/
void CCP_ClearCAPMode0IntFlag(uint32_t CCPn);





/*****************************************************************************
 ** \brief	CCP_EnableCAPMode1
 **			��������ģʽ1 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPMode1(void);
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode1
 **			�رղ���ģʽ1 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_DisableCAPMode1(void);
/*****************************************************************************
 ** \brief	CCP_EnableCAPnReload
 **			��������ģʽ1��CAPn���񴥷�CCP0����
 ** \param [in] CAPn: CAP0��CAP1��CAP2��CAP3 
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPnReload(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_DisableCAPnReload
 **			�رղ���ģʽ1��CAPn���񴥷�CCP0����
 ** \param [in] CAPn: CAP0��CAP1��CAP2��CAP3 
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_DisableCAPnReload(uint32_t CAPn);

/*****************************************************************************
 ** \brief	CCP_ConfigCAPMode1
 **			���ò���ģʽ1 ͨ������ģʽ
 ** \param [in] CAPn: CAP0��CAP1��CAP2��CAP3 
 **				Mode:(1)CCP_CAP_MODE1_RISING
 **					 (2)CCP_CAP_MODE1_FALLING
 **					 (3)CCP_CAP_MODE1_BOTH_EDGE
 **					 (4)CCP_CAP_MODE1_DISABLE
 ** \return  none
*****************************************************************************/
void  CCP_ConfigCAPMode1(uint32_t CAPn, uint32_t Mode);
/*****************************************************************************
 ** \brief	CCP_SelCAPMode1Channel
 **			ѡ�񲶻�ģʽ1 �Ĳ���ͨ��
 ** \param [in] CAPn: CAP0��CAP1��CAP2��CAP3 
 **				Channel :(1) ECAP00 ~ ECAP03
 **						 (2) ECAP10 ~ ECAP13
 **						 (3) CAP0_CCP0A ��CAP1_CCP0B��CAP2_CCP1A��CAP3_CCP1B
 **						 (4) CAP3_ACMP0��CAP3_ACMP1 ��  CAP3ͨ������
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_SelCAPMode1Channel(uint32_t CAPn, uint32_t Channel);
/*****************************************************************************
 ** \brief	CCP_TriggerCaputer
 **			д�Ĵ�������������
 ** \param [in] CAPn ��CAP0 ��CAP1��CAP2 ��CAP3
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_TriggerCaputer(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_GetCAPMode1Result
 **			��ȡ����ģʽ1 �Ĳ���ͨ��ֵ
 ** \param [in] CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  16bit result
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCAPMode1Result(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_EnableCAPMode1Int
 **			��������ģʽ1�ж�
 ** \param [in]CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_EnableCAPMode1Int(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode1Int
 **			�رղ���ģʽ1�ж�
 ** \param [in]CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_DisableCAPMode1Int(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_GetCAPMode1IntFlag
 **			��ȡ����ģʽ1�ж�ʹ�ܲ��Ҳ����жϱ�־λ
 ** \param [in]CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  1:�ж�ʹ�ܲ��Ҳ����ж�   0�����ж�
 ** \note	 
*****************************************************************************/
uint32_t  CCP_GetCAPMode1IntFlag(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_ClearCAPMode1IntFlag
 **			�������ģʽ1�жϱ�־λ
 ** \param [in]CAPn ��CAP0 ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void  CCP_ClearCAPMode1IntFlag(uint32_t CAPn);


/*****************************************************************************
 ** \brief	CCP_EnableCAPMode2
 **			��������ģʽ2 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPMode2(void);
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode2
 **			�رղ���ģʽ2 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_DisableCAPMode2(void);

/*****************************************************************************
 ** \brief	CCP_ConfigCAPMode2
 **			���ò���ģʽ2 ͨ������ģʽ
 ** \param [in] CAPn: CAP1��CAP2��CAP3 
 **				Mode:(1)CCP_CAP_MODE2_RISING
 **					 (2)CCP_CAP_MODE2_FALLING
 **					 (3)CCP_CAP_MODE2_BOTH_EDGE
 **					 (4)CCP_CAP_MODE2_DISABLE
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_ConfigCAPMode2(uint32_t CAPn, uint32_t Mode);

/*****************************************************************************
 ** \brief	CCP_SelCAPMode2Channe2
 **			ѡ�񲶻�ģʽ2 �Ĳ���ͨ��
 ** \param [in] Channel :(1) ECAP00 ~ ECAP03
 **						 (2) ECAP10 ~ ECAP13
 **						 (3) CAP1_CCP0B
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_SelCAPMode2Channel(uint32_t Channel);
/*****************************************************************************
 ** \brief	CCP_TriggerCAMMode2
 **			д�Ĵ�����������ģʽ2������
 ** \param [in] CAPn :CAP1��CAP2 ��CAP3
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_TriggerCaputer2(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_GetCAPMode1Result
 **			��ȡ����ģʽ2 �Ĳ���ͨ��ֵ
 ** \param [in] CAPn ��(1)CAP1_SUB_CAP2, CAP1_SUB_CAP3
 **					   (2)CAP1��CAP2 ��CAP3	
 ** \return  16bit result
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCAPMode2Result(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_EnableCAPMode2Int
 **			��������ģʽ2�ж�
 ** \param [in]CAPn ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_EnableCAPMode2Int(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode2Int
 **			�رղ���ģʽ2�ж�
 ** \param [in]CAPn ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_DisableCAPMode2Int(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_GetCAPMode2IntFlag
 **			��ȡ����ģʽ2�ж�ʹ�ܲ��Ҳ����жϱ�־λ
 ** \param [in]CAPn ��CAP1��CAP2 ��CAP3	
 ** \return  1:�ж�ʹ�ܲ��Ҳ����ж�   0�����ж�
 ** \note	 
*****************************************************************************/
uint32_t  CCP_GetCAPMode2IntFlag(uint32_t CAPn);
/*****************************************************************************
 ** \brief	CCP_ClearCAPMode2IntFlag
 **			�������ģʽ2�жϱ�־λ
 ** \param [in]CAPn ��CAP1��CAP2 ��CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void  CCP_ClearCAPMode2IntFlag(uint32_t CAPn);




#ifdef __cplusplus
}
#endif

#endif /* __CCP_H_ */

