#ifndef __COMMON_H__
#define __COMMON_H__

#include "CMS32M6510.h"
#ifndef NULL
	#define NULL 0
#endif
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
//#define SPI_INTERFACE

#endif
