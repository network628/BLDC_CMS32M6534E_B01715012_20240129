#ifndef _MC_DEADCOMP_H_
#define _MC_DEADCOMP_H_

/************************************************************************/
/*                                                                      */
/*  FILE        :mc_deadcomp.h                                          */
/*  DESCRIPTION :�����������ͷ�ļ�                                     */
/*  VERSION:    :1.0                                                    */
/*                                                                      */
/*  CPU TYPE    :CMS32M5xxx                                             */
/*  AUTHOR      :�����  �ƻ�ƽ                                         */
/*  COPYRIGHT   :�����΢���ӵ��ӿƼ����޹�˾                           */
/*  DATE        :2020-8-6                                               */
/************************************************************************/

#define            m_s08                    signed    char       
#define            m_s16                    signed    short int 
#define	           m_s32                    signed    int        
#define	           m_u08                    unsigned  char     
#define	           m_u16                    unsigned  short int 
#define	           m_u32                    unsigned  int 
//-----------------------------------------------------------
//-----------------------------------------------------------
typedef struct 
{   
	m_s32       s32_VolBus;                  //ĸ�ߵ�ѹ
	m_s32       s32_Comp_Coeff;              //����ϵ��
	m_s32       s32_Comp_Alpha;              // Output: Alafa
	m_s32       s32_Comp_Beta; 	            // Output: Beta
	m_s32		s32_Comp_Angle;
	m_s32		s32_Vol_Phase_A;
	m_s32		s32_Vol_Phase_B;
	m_s32		s32_Vol_Phase_C;

}_DeadComp;	
//-----------------------------------------------------------
//#define m_DEADCOMP_INIT_DEFAULTS { 0, 0, 0 , 0, 0 ,0, 0 , (void (*)(m_s32))FOC_Calc_DeadComp }
////-----------------------------------------------------------
//void FOC_Calc_DeadComp( _DeadComp *p_PMSM_DeadComp_Handle );

extern 	_DeadComp	stru_DeadComp;
void 	FOC_Calc_DeadComp( void );
void 	FOC_DeadComp_Init( void );


#endif       
//*****************************************************************************
//End File
//*****************************************************************************
