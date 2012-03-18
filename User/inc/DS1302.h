#ifndef __DS1302_H__
#define __DS1302_H__

#include "includes.h"

#define INT8U     unsigned char
#define INT16U    unsigned short int
#define INT32U    unsigned long int

#define  IO_IN    0
#define  IO_OUT   1

#define REG_SEC		0x80
#define REG_MIN		0x82
#define REG_HOUR	0x84
#define REG_DAY		0x86
#define REG_MON		0x88
#define REG_WEEKDAY	0x8a
#define REG_YEAR	0x8c

#define Set_DS1302_CLK  GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define Clr_DS1302_CLK  GPIO_ResetBits(GPIOB, GPIO_Pin_7)

#define Set_DS1302_RST  GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define Clr_DS1302_RST  GPIO_ResetBits(GPIOB, GPIO_Pin_5)

#define Set_DS1302_IO  GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define Clr_DS1302_IO  GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define DS1302_IO      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)

typedef struct
{
    INT8U second;
    INT8U minute;
    INT8U hour;
    INT8U day;
    INT8U week;
    INT8U month;
    INT8U year;
}systemtime;   //定义时间的类型

typedef struct
{
	u8t tm_sce; 	//(0-59)
	u8t tm_min;		//(0-59)
	u8t tm_hour;	//(0-23)
	u8t tm_mday;	//(1-31)
	u8t tm_mon;		//(1-12)
	u8t tm_year;	//(current years minus 1990)
}TimeStruct;

void DS1302_Init(void);
void IO_Set(INT8U flag);
//void DS1302_Init(void);
void DS1302_WriteByte(u8t dat);
void DS1302_Write(u8t cmd,u8t dat);
u8t DS1302_ReadByte(void);
u8t DS1302_Read(u8t cmd);
void SetTime(u32t *newTime);
void SetSec(u8t sec);
void SetMin(u8t min);
void SetHour(u8t hour);
void SetDay(u8t day);
void SetMon(u8t mon);
void SetWeekDay(u8t weekDay);
void SetYear(u8t year);
u32t GetTimeStamp(void);
struct tm GetLocalTime(void);
boolean IsInTime(u32t *startTime,u32t *endTime);


#endif
