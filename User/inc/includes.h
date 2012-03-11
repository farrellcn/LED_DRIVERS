#ifndef __includes_h__
#define __includes_h__



#define INT8U       unsigned char
#define INT16U      unsigned short int
#define INT32U      unsigned long int

#define INT8       signed char
#define INT16      signed short int
#define INT32      signed long int

typedef unsigned char  boolean;
typedef unsigned char  u8t;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  s8t;                    /* Signed    8 bit quantity                           */
typedef unsigned short u16t;                   /* Unsigned 16 bit quantity                           */
typedef signed   short s16t;                   /* Signed   16 bit quantity                           */
typedef unsigned int   u32t;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   s32t;                   /* Signed   32 bit quantity  						 */

#define DEF_TRUE	1
#define DEF_FALSE	0

#include "stm32f10x.h"
//#include "LEDriver.h"
#include "string.h"
#include "UART.h"
#include "LedDriver.h"
#include "GT23L16U2W.h"
#include "SST25VF040B.h"
#include "DS1302.h"

#endif 							
