#ifndef  _LEDriver_H_
#define  _LEDriver_H_

#define   LED_Size      8                             //单行显示的字数。
#define   LED_SCAN      8

#define   LED_Speed_0   0
#define   LED_Speed_1   1
#define   LED_Speed_2   2
#define   LED_Speed_3   3
#define   LED_Speed_4   4
#define   LED_Speed_5   5
#define   LED_Speed_6   6
#define   LED_Speed_7   7
#define   LED_Speed_8   8
#define   LED_Speed_9   9

#define INT8U   unsigned char 
#define INT16U   unsigned short int 
#define uint   unsigned int
#define uchar unsigned char


/*********************新版PCB*********************************/

#define Set_A  GPIOA->BSRR  = 0x0001<<5;
#define Clr_A  GPIOA->BRR   = 0x0001<<5;

#define Set_B  GPIOA->BSRR  = 0x0001<<4;
#define Clr_B  GPIOA->BRR   = 0x0001<<4;

#define Set_C GPIOB->BSRR  = 0x0001<<10;
#define Clr_C GPIOB->BRR   = 0x0001<<10;

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

#define Set_SCK GPIOA->BSRR  = 0x0001<<12;
#define Clr_SCK GPIOA->BRR   = 0x0001<<12;

#define Set_RCK GPIOA->BSRR  = 0x0001<<11;
#define Clr_RCK GPIOA->BRR   = 0x0001<<11;

/*
#define Set_DS1302_CLK  GPIOB->BSRR  = 0x0001<<7
#define Clr_DS1302_CLK  GPIOB->BRR   = 0x0001<<7

#define Set_DS1302_RST  GPIOB->BSRR  = 0x0001<<5
#define Clr_DS1302_RST  GPIOB->BRR   = 0x0001<<5

#define Set_DS1302_IO  GPIOB->BSRR  = 0x0001<<6
#define Clr_DS1302_IO  GPIOB->BRR   = 0x0001<<6

#define GetDSIO   (GPIOB->IDR  & 0x0001<<6)


#define Set_GTSCK  GPIOB->BSRR  = 0x0001<<13;
#define Clr_GTSCK  GPIOB->BRR   = 0x0001<<13;

#define Set_GTMOSI  GPIOB->BSRR  = 0x0001<<15;
#define Clr_GTMOSI  GPIOB->BRR   = 0x0001<<15;

#define Set_GTCE GPIOB->BSRR  = 0x0001<<12;
#define Clr_GTCE GPIOB->BRR   = 0x0001<<12;

#define Get_GTMOSO  (GPIOB->IDR  & 0x0001<<14)
*/

/*硬件相关的宏定义 旧版PCB*/
/********************************************************************************/
/*
#define Set_A  GPIOB->BSRR  = 0x0001<<1;
#define Clr_A  GPIOB->BRR   = 0X0001<<1;

#define Set_B  GPIOB->BSRR  = 0x0001<<0;
#define Clr_B  GPIOB->BRR   = 0x0001<<0;

#define Set_C GPIOA->BSRR  = 0x0001<<7;
#define Clr_C GPIOA->BRR   = 0x0001<<7;

#define Set_CE GPIOA->BSRR  = 0x0001<<5;
#define Clr_CE GPIOA->BRR   = 0x0001<<5;

#define Set_CE00 GPIOB->BSRR  = 0x0001<<2;
#define Clr_CE00 GPIOB->BRR   = 0x0001<<2;

#define Set_CE01 GPIOB->BSRR  = 0x0001<<10;
#define Clr_CE01 GPIOB->BRR   = 0x0001<<10;

#define Set_DATL  GPIOB->BSRR  = 0x0001<<13;
#define Clr_DATL  GPIOB->BRR   = 0x0001<<13;

#define Set_DATH  GPIOB->BSRR  = 0x0001<<14;
#define Clr_DATH  GPIOB->BRR   = 0x0001<<14;

#define Set_SCK GPIOB->BSRR  = 0x0001<<11;
#define Clr_SCK GPIOB->BRR   = 0x0001<<11;

#define Set_RCK GPIOB->BSRR  = 0x0001<<12;
#define Clr_RCK GPIOB->BRR   = 0x0001<<12;
*/
//#define Set_TS GPIOA->BSRR  = 0x0001;
//#define Clr_TS GPIOA->BRR   = 0x0001;

//typedef struct
//{
//    INT8U second;
//    INT8U minute;
//    INT8U hour;
//    INT8U day;
//    INT8U week;
//    INT8U month;
//    INT8U year;
//}systemtime;   //定义时间的类型


extern void LEDriver_Init(void);
extern void Send_Test();
extern void Send_8Bit(INT8U led_data);
extern void HanScan(INT8U num);
extern void LED_OUT(void);
extern void DisPlay(void);
extern void Left_Dis(void);
extern void Left_Dat(void);
extern void Delay_ms(unsigned int time);
extern void zuo_you();   
extern void Dis_left(unsigned char *str, unsigned char num);
extern void Up_Dat(void);
extern void Dis_Up(unsigned char *str, unsigned char num);
extern void Dis_Left_Mix(INT8U *str, INT8U num, INT8U speed);
extern void Dis_Up_Mix(INT8U *str, INT8U num,INT8U speed);
extern void Dis_Time(INT8U flag);
extern void Dis_LED_Way(INT8U *str,INT8U num,INT8U speed,INT8U flag);
extern void LED_ONOFF(INT8U flag);
extern void Dis_Clr();

#endif 