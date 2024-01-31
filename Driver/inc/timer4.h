#ifndef __TIMER4_H__
#define __TIMER4_H__
#include <stdint.h>
#include <stdbool.h>
#include "common.h"

#define TIMER_CHAN_MAX_NUM   4
#define	TIM_SUCC             0
#define	TIM_ERR              1
typedef enum 
{
	Disable = 0, 
	Enable = !Disable
}TIM_FunState;

#define TTM_Channel_0     0x01    //TO00
#define TTM_Channel_1     0x02    //TO01
#define TTM_Channel_2     0x04    //TO02
#define TTM_Channel_3     0x08    //TO03

#define IS_TIM_CHANNEL(CHAN)	( ((CHAN) != (uint8_t)0x00))

typedef enum
{
	TTM_Chan_None = 0x00, //when choose it will not use multi-tim combination
	TTM_Chan_0 = 0x01,
	TTM_Chan_2 = 0x04, 
}TTM_Master_Chan_t; //multi-channels function, even channel must be setted as master channel,it can choose one or more even channel as master channels
#define IS_TTM_Master_Chan(Master_Chan) (((Master_Chan) == TTM_Channel_0 || (Master_Chan) == TTM_Channel_2 ||  \
											(Master_Chan) == (TTM_Channel_0|TTM_Channel_2) )? 1:0) 


typedef enum
{
	TIM_NEGEDGE = 0x00,  // falling edge of TI
	TIM_POSEDGE = 0x01,  // rising edge of TI 
	TIM_BOTHEDGE = 0x02, //  both edge of TI 
}TIM_Edge_t;

typedef enum
{
	TIM_CLK0_Div1     = 0x0000,  //Operation clock slection FCLK/ 2^0
	TIM_CLK0_Div2     = 0x0001,	//Operation clock slection FCLK/ 2^1		
	TIM_CLK0_Div4     = 0x0002,  //Operation clock slection FCLK/2^2
	TIM_CLK0_Div8     = 0x0003,  //Operation clock slection FCLK/2^3
	TIM_CLK0_Div16    = 0x0004,  //Operation clock slection FCLK/2^4
	TIM_CLK0_Div32    = 0x0005,  //Operation clock slection FCLK/2^5
	TIM_CLK0_Div64    = 0x0006,  //Operation clock slection FCLK/2^6
	TIM_CLK0_Div128   = 0x0007,  //Operation clock slection FCLK/2^7
	TIM_CLK0_Div256   = 0x0008,  //Operation clock slection FCLK/2^8
	TIM_CLK0_Div512   = 0x0009,  //Operation clock slection FCLK/2^9
	TIM_CLK0_Div1024  = 0x000A,  //Operation clock slection FCLK/2^10
	TIM_CLK0_Div2048  = 0x000B,  //Operation clock slection FCLK/2^11
	TIM_CLK0_Div4096  = 0x000C,  //Operation clock slection FCLK/2^12
	TIM_CLK0_Div8192  = 0x000D,  //Operation clock slection FCLK/2^13
	TIM_CLK0_Div16384 = 0x000E,	//Operation clock slection FCLK/2^14
	TIM_CLK0_Div32768 = 0x000F,  //Operation clock slection FCLK/2^15
}TIM_Clk0_t;

typedef enum
{
	TIM_CLK1_Div1     = 0x0000,  //Operation clock slection FCLK/ 2^0
	TIM_CLK1_Div2     = 0x0010,	//Operation clock slection FCLK/ 2^1		
	TIM_CLK1_Div4     = 0x0020,  //Operation clock slection FCLK/2^2
	TIM_CLK1_Div8     = 0x0030,  //Operation clock slection FCLK/2^3
	TIM_CLK1_Div16    = 0x0040,  //Operation clock slection FCLK/2^4
	TIM_CLK1_Div32    = 0x0050,  //Operation clock slection FCLK/2^5
	TIM_CLK1_Div64    = 0x0060,  //Operation clock slection FCLK/2^6
	TIM_CLK1_Div128   = 0x0070,  //Operation clock slection FCLK/2^7
	TIM_CLK1_Div256   = 0x0080,  //Operation clock slection FCLK/2^8
	TIM_CLK1_Div512   = 0x0090,  //Operation clock slection FCLK/2^9
	TIM_CLK1_Div1024  = 0x00A0,  //Operation clock slection FCLK/2^10
	TIM_CLK1_Div2048  = 0x00B0,  //Operation clock slection FCLK/2^11
	TIM_CLK1_Div4096  = 0x00C0,  //Operation clock slection FCLK/2^12
	TIM_CLK1_Div8192  = 0x00D0,  //Operation clock slection FCLK/2^13
	TIM_CLK1_Div16384 = 0x00E0,	//Operation clock slection FCLK/2^14
	TIM_CLK1_Div32768 = 0x00F0,  //Operation clock slection FCLK/2^15
}TIM_Clk1_t;


typedef enum
{
	TIM_CLK2_Div2  = 0x0000,
	TIM_CLK2_Div4  = 0x0100,
	TIM_CLK2_Div16 = 0x0200,
	TIM_CLK2_Div64 = 0x0300,
}TIM_Clk2_t;

typedef enum
{
	TIM_CLK3_Div256    = 0x0000,
	TIM_CLK3_Div1024   = 0x1000,
	TIM_CLK3_Div4096   = 0x2000,
	TIM_CLK3_Div16384  = 0x3000,
}TIM_Clk3_t;

typedef enum
{
	TIM_Selection_Slave       = 0x0000,  //config for channel2 4 or 6
	TIM_Selection_Bits16      = 0x0000,  //config for channel 1 or 3
	TIM_Selection_Bits8       = 0x0800,  //config for channel 1 or 3
	TIM_Selection_Master      = 0x0800,   //config for channel 2 4 or 6
}TIM_Selection_t;

typedef enum
{
	TIM_Trigger_Software = 0x0000,     // only software trigger start is valid
	TIM_Trigger_IputEdge = 0x0100,     // TImn valid input edge is used as a start or capture trigger 
	TIM_Trigger_BothEdge = 0x0200,     //TImn input edges are used as a start or capture trigger 
	TIM_Trigger_UseMaster_Int = 0x0400,  //when slave channel use master channel's interrupt
}TIM_Trigger_t;
typedef enum
{
	TIM_Pulse_Falling   = 0x0000,      // measure falling edge interval of TI
	TIM_Pulse_Rising 	= 0x0040,      // measure rising edge interval of TI 
	TIM_Pulse_Both 	 	= 0x0080,      // measure both edge interval of TI 
	TIM_Pulse_Both_Width_Low = 0x0080, // measure low level width of TI 
	TIM_Pulse_Both_Width_High= 0x00C0, // measure high level width of TI 
}TIM_Pulse_t;

typedef enum
{
	TIM_Mode_Interval   = 0x0000,
	TIM_Mode_Square     = 0x0000,
	TIM_Mode_DivFreq    = 0x0000,
	TIM_Mode_PWM_Master = 0x0000,
	TIM_Mode_PluseInterval = 0x0004,
	TIM_Mode_EventCount    = 0x0006,
	TIM_Mode_DelayCount    = 0x0008,
	TIM_Mode_PWM_Slave     = 0x0008,
	TIM_Mode_SinglePulse   = 0x0008,
	TIM_Mode_Measure       = 0x000C,
}TIM_Mode_t;

typedef enum
{
	TIM4 = 0,
	
}TIMSelect_t;
#define IS_TIM(TIMx)		( (TIMx == TIM4) )

#define TIM_StartInt_Disable                 ((uint8_t)0x00) //Interrupt disable
#define TIM_StartInt_Enable         		 ((uint8_t)0x01) //Interrupt enable

typedef enum
{
	TM4_CH1_Input_TI01 = 0x00,  // input signal of timer input pin (TI01)
	TM4_CH1_Input_fIL 	= 0x04,  // low-speed on-chip oscillator clock (fIL)  
}TM4_Input_t;

typedef struct
{
	TIMSelect_t TIM;       /*!< Specifies the TIM Type. This parameter can be TIM4 or TM4 */
	
	uint8_t TIM_Channel;       /*!< Specifies the TIM channel. This parameter can be a value of @ref TTM_Channel_t */
	
    TIM_Pulse_t TIM_Pulse_Edge;	   /*!< Specifies the TIM pluse edge
									 This parameter can be a value of @ref TIM_Pulse_t */

	uint16_t TIM_Period[TIMER_CHAN_MAX_NUM];  /*!< Specifies the period value to be loaded into the active
	                                   Auto-Reload Register at the next update event.This parameter must be a number between 0x0000 and 0xFFFF.	*/ 
									   	
	uint16_t TIM_ClkDivision;     /*!< Specifies the clock division of different TIM channel
	                                  This parameter can be a value of @ref TIM_Clkx_t */

	uint16_t TIM_Selection_Master; /*!< Selection of  master of channle n or selection of 16 bits timer or 8bits timer
								  This parameter can be a value of @ref TTM_Master_Chan_t */
								  	
	uint16_t TIM_Trigger; 	     /*!< setting of start trigger or capture trigger of channle n
									This parameter can be a value of @ref TIM_Trigger_t */
									  
	uint16_t TIM_Mode; 	         /*!< Specifies the TIM work mode of different TIM channel
									      This parameter can be a value of @ref TIM_Mode_t */

	uint8_t TIM_StartInt;	      /*!< whether interrupt is generated when counting is started .*/

	TM4_Input_t TM4_Input;	  /*!< Specifies the input signal for channel of timer  
									  This parameter can be a value of @ref TIM_Input_t */
											
}TIM_InitTypeDef;       

typedef struct 
{
  volatile const uint16_t     *TCR[8];	
	volatile uint16_t           *TMR[8];	
	volatile const uint16_t     *TSR[8];	
	volatile const uint16_t	    *TE;
	volatile uint16_t			*TS;
	volatile uint16_t			*TT;
	volatile uint16_t			*TPS;
	volatile uint16_t			*TO;
	volatile uint16_t			*TOE;
	volatile uint16_t			*TOL;
	volatile uint16_t			*TOM;
	
	volatile uint16_t			*TDR[8];

}TIM_REG_t;

#define assert_param(expr) ((void)0)
#define _0000_TM_CLOCK_SELECT_CKM0             (0x0000U) // operation clock CK0 set by PRS register 
#define _8000_TM_CLOCK_SELECT_CKM1             (0x8000U) // operation clock CK1 set by PRS register 
#define _4000_TM_CLOCK_SELECT_CKM2             (0x4000U) // operation clock CK2 set by PRS register 
#define _C000_TM_CLOCK_SELECT_CKM3             (0xC000U) // operation clock CK3 set by PRS register 
/* Counter overflow status of channel n (OVF) */
#define _0000_TM4_OVERFLOW_NOT_OCCURS     (0x0000U) // overflow does not occur 
#define _0001_TM4_OVERFLOW_OCCURS         (0x0001U) // overflow occurs 


int TIM_Init(TIM_InitTypeDef *TIM_InitStruct);
void TIM_Cmd(TIMSelect_t TIM,uint8_t Channel,TIM_FunState state);
void TIM_SetCounter(TIMSelect_t TIM,uint8_t Channel,uint16_t counter);
bool TIM_GetStatus(TIMSelect_t TIM,uint8_t Channel);
void TIM_Channel_Stop(TIM_InitTypeDef *TIM_InitStruct);
void TIM_Channel_Start(TIM_InitTypeDef *TIM_InitStruct);
void TIM_Start(TIMSelect_t tim, uint8_t ch);
void TIM_Stop(TIMSelect_t tim, uint8_t ch);
uint32_t TIM_GetPulsePeriod(TIMSelect_t tim,uint8_t channel);
uint32_t TIM_GetPulseWidth(TIMSelect_t tim, uint8_t channel);

#endif
