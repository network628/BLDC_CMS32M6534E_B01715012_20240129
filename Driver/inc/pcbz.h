#ifndef __PCBZ_H__
#define __PCBZ_H__
#include <stdint.h>
#include "common.h"

typedef enum
{
	PCBZ_CHANNEL_0 = 0x01,//
	PCBZ_CHANNEL_1 = 0x02,
} PCBZ_Channel_t;
#define IS_PCBZ_CHANNEL(CHAN) 	((CHAN & 0xff) != 0xfb)

/** @defgroup PCBZ_ClkSource_definition 
  * @{
  */ 
#define PCBZ_FMAIN  0x00
#define IS_PCBZ_CLKSOURCE(CLK) ((CLK) == PCBZ_FMAIN)  \
                              


/** @defgroup PCBZ_ClkSource_definition 
  * @{
  */ 
#define  	PCBZ_FMAIN_0		0x00U  // fMAIN/2^0  
#define		PCBZ_FMAIN_1		0x01U  // fMAIN/2^1 
#define		PCBZ_FMAIN_2		0x02U  // fMAIN/2^2 
#define		PCBZ_FMAIN_3		0x03U  // fMAIN/2^3 
#define		PCBZ_FMAIN_4		0x04U  // fMAIN/2^4 
#define		PCBZ_FMAIN_11		0x05U  // fMAIN/2^11
#define		PCBZ_FMAIN_12		0x06U  // fMAIN/2^12 
#define		PCBZ_FMAIN_13		0x07U  // fMAIN/2^13


#define IS_PCBZ_FREQSCALAR(SCALAR)           ((SCALAR) < 0x07)


typedef struct
{
	PCBZ_Channel_t PCBZ_ChanSelect;    /*!< Specifies the channel to output clock for external peripheral or buzzer .
                                         This parameter can be a value of @ref PCBZ_Channel_t */

	uint8_t PCBZ_ClkSource;   /*!< Specifies the PCBZ timer clock source.
							   This parameter can be a value of @ref PCBZ_ClkSource_definition */
	
	uint8_t PCBZ_Freqscaler; /*!< Specifies the output signal clock Freqscaler, different clock source owns different 
	                             division factor.This parameter can be a value of @ref PCBZ_Freqscaler_definition */
}PCBZ_InitTypeDef;

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void PCBZ_Init(PCBZ_InitTypeDef* PCBZ_InitStruct);
void PCBZ_Start(PCBZ_Channel_t ch);
void PCBZ_Stop(PCBZ_Channel_t ch);
#endif
