#ifndef  _GT23L16U2W_H_
#define  _GT23L16U2W_H_

#include "includes.h"

/*******************ÐÂ°æPCB***************/
#define Set_GTSCK  GPIOB->BSRR  = 0x0001<<13;
#define Clr_GTSCK  GPIOB->BRR   = 0x0001<<13;

#define Set_GTMOSI  GPIOB->BSRR  = 0x0001<<15;
#define Clr_GTMOSI  GPIOB->BRR   = 0x0001<<15;

#define Set_GTCE GPIOB->BSRR  = 0x0001<<12;
#define Clr_GTCE GPIOB->BRR   = 0x0001<<12;

#define Get_GTMOSO  (GPIOB->IDR  & 0x0001<<14)

/*******************¾É°æPCB***************
#define Set_GTSCK  GPIOA->BSRR  = 0x0001<<2;
#define Clr_GTSCK  GPIOA->BRR   = 0x0001<<2;

#define Set_GTMOSI  GPIOA->BSRR  = 0x0001<<4;
#define Clr_GTMOSI  GPIOA->BRR   = 0x0001<<4;

#define Set_GTCE GPIOA->BSRR  = 0x0001<<1;
#define Clr_GTCE GPIOA->BRR   = 0x0001<<1;
*/

#define FONT_SIZE_11X12			0x11616
#define FONT_SIZE_15X16			0x21616
#define FONT_SIZE_7X8_ASCII		0x30808
#define FONT_SIZE_8X16_ASCII	0x40816
/*
#define FONT_SIZE_11X12			0x11616
#define FONT_SIZE_15X16			0x21616
#define FONT_SIZE_7X8_ASCII		0x30808
#define FONT_SIZE_8X16_ASCII	0x40816
*/
#define FONT_11X12_LENGTH		12
#define FONT_15X16_LENGTH		32
#define FONT_7X8_LENGTH			8
#define FONT_8X16_LENGTH		16

#define FONT_8_BASE				0x0000
#define FONT_11X12_BASE			0x0000
#define FONT_15X16_BASE			0xA76B8
#define FONT_7X8_ASCII_BASE		0x188ED8
#define FONT_8X16_ASCII_BASE	0x1883D8

#define FONT_WIDTH		16
#define FONT_HEIGHT		16
#define FONT_LENGTH		FONT_15X16_LENGTH

#define GET_FONT_WIDTH(x)	((((x>>12)&0x00000f)*10)+((x>>8)&0x00000f))	
#define GET_FONT_HEIGHT(y)	((((y>>4)&0x00000f)*10)+(y&0x00000f))	


void GT32_Init(void);
boolean GT32_Test(void);
u32t GetFont8Address(u16t ucode);
u32t GetFont11X12Address(u16t ucode);
u32t GetFont15X16Address(u16t ucode);
u32t GetFont7X8AsciiAddress(u16t ucode);
u32t GetFont8X16AsciiAddress(u16t ucode);
u8t GT32_WirteByte(u8t bytes);
void Char2Lattice(u16t ucode,u8t *latticeArray,u32t fontSize);

                                      
#endif
