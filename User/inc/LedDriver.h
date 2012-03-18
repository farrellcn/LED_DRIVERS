#ifndef _LED_DIRVER_H_
#define _LED_DIRVER_H_

#include "includes.h"

#define Set_A  GPIOA->BSRR  = 0x0001<<5;
#define Clr_A  GPIOA->BRR   = 0x0001<<5;

#define Set_B  GPIOA->BSRR  = 0x0001<<4;
#define Clr_B  GPIOA->BRR   = 0x0001<<4;

#define Set_C GPIOB->BSRR  = 0x0001<<10;
#define Clr_C GPIOB->BRR   = 0x0001<<10;

#define Set_D GPIOB->BSRR  = 0x0001<<11;
#define Clr_D GPIOB->BRR   = 0x0001<<11;

#define Set_CE GPIOB->BSRR  = 0x0001<<0;
#define Clr_CE GPIOB->BRR   = 0x0001<<0;

#define Set_CE00 GPIOB->BSRR  = 0x0001<<1;
#define Clr_CE00 GPIOB->BRR   = 0x0001<<1;

#define Set_CE01 GPIOB->BSRR  = 0x0001<<2;
#define Clr_CE01 GPIOB->BRR   = 0x0001<<2;

#define Set_DATL  GPIOA->BSRR  = 0x0001<<7;
#define Clr_DATL  GPIOA->BRR   = 0x0001<<7;

#define Set_DATH  GPIOA->BSRR  = 0x0001<<6;
#define Clr_DATH  GPIOA->BRR   = 0x0001<<6;

#define Set_SCK GPIOA->BSRR  = 0x0001<<12;	//move clk
#define Clr_SCK GPIOA->BRR   = 0x0001<<12;

#define Set_RCK GPIOA->BSRR  = 0x0001<<11;	//save clk
#define Clr_RCK GPIOA->BRR   = 0x0001<<11; 

#define LED_ROWS	16		//led屏行数
#define LED_COLS 	128		//led屏列数

#define FONT_SIZE	16

#define MAX_FONT_WIDTH 16

#define SCANNING_QUARTER   	(1/4)		//四分之一扫描
#define SCANNING_EIGHTH	   	(1/8)		//八分之一扫描
#define SCANNING_SIXTEENTH 	(1/16)		//十六分之一扫描

#define STYLE_RIGHT_TO_LEFT 	1
#define STYLE_BOTTOM_TO_TOP		3
#define STYLE_STATIC		 	5
#define STYLE_VERBATIM			7


void LedInit(void);
void LED_ONOFF(u8t flag);
void WriteByte(u8t bytes,u8t row);
void WriteDoubleRowByte(u8t upperByte,u8t lowerByte,u8t row);
void DisplayInRow(void);
void SelectOneRow(u8t row);
void LedDisplay(u8t *textStr,u8t wordNum,u8t frequence,u8t runStyle);
void Dis_Static(u8t *textStr,u8t strLength,u32t fontSize);
//void Dis_Small(u8t *textStr,u8t strLength,u32t fontSize);
void Dis_Verbatim(u8t *textStr,u8t strLength);
void Dis_RightToLeft(u8t *textStr,u8t strLength);
void Dis_BottomToTop(u8t *textStr,u8t strLength);
void Dis_ClearScreen(void);
void Display(void);
void Dis_Test(void);
void Delay_ms(u32t time);
#endif
