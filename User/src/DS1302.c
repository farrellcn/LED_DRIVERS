#include "includes.h"

void DS1302_Init(void)
{
	struct tm time;
    GPIO_InitTypeDef GPIO_InitStructure;
	u32t timeStamp;
	u32t initTime=0x4F619370;
	u8t i=0;

    //开启相应的时钟
	/* LD1 & LD2 & LD3 & LD4 configration */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	SetTime(&initTime);
	//timeStamp=GetTimeStamp();
	//time=GetLocalTime();
	//time=GetLocalTime();
    //init_ds1302(); 
   // init_time();                         	
}

void IO_Set(INT8U flag)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if(flag == 0)
    {
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;	
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOB, &GPIO_InitStructure); 
    }
    else
    {
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;	
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOB, &GPIO_InitStructure); 
    }
}

void DS1302_WriteByte(u8t dat)
{
	u8t i;
	Clr_DS1302_CLK;
	IO_Set(IO_OUT);
	Delay_ms(1);
	for(i=0;i<8;i++)
	{
		if(dat&0x01)
		{
			Set_DS1302_IO;
		}
		else
		{
			Clr_DS1302_IO;
		}
		Delay_ms(1);
		Set_DS1302_CLK;
		Delay_ms(1);
		Clr_DS1302_CLK;
		dat>>=1;
	}
}

u8t DS1302_ReadByte(void)
{
	u8t i;
	u8t dat=0;
	Clr_DS1302_CLK;
	IO_Set(IO_IN);
	for(i=0;i<8;i++)
	{
		dat>>=1;
		if(DS1302_IO)
		{
			dat|=0x80;
		}
		Set_DS1302_CLK;
		Delay_ms(1);
		Clr_DS1302_CLK;
		Delay_ms(1);
	}
	return dat;
}

void DS1302_Write(u8t cmd,u8t dat)
{
	Clr_DS1302_RST;
	Clr_DS1302_CLK;
	Set_DS1302_RST;
	DS1302_WriteByte(cmd);
	DS1302_WriteByte(dat);
	Set_DS1302_CLK;
	Clr_DS1302_RST;
}

u8t DS1302_Read(u8t cmd)
{
	u8t dat;
	Clr_DS1302_RST;
	Clr_DS1302_CLK;
	Set_DS1302_RST;
	DS1302_WriteByte(cmd);
	dat=DS1302_ReadByte();
	Set_DS1302_CLK;
	Clr_DS1302_RST;
	return dat;	
}

void SetTime(u32t *newTime)
{
	//struct tm *pTime;
	struct tm time;
	//pTime=localtime(newTime);
	time=*(localtime(newTime));
	SetSec(time.tm_sec);
	SetMin(time.tm_min);
	SetHour(time.tm_hour);
	SetDay(time.tm_mday==0?7:time.tm_mday);
	SetMon(time.tm_mon+1);
	SetWeekDay(time.tm_wday);
	SetYear(time.tm_year-100);
	//time=GetLocalTime();
}

void SetSec(u8t sec)
{
	u8t cmd;
	u8t dat;
	cmd=REG_SEC;
	dat=((sec/10)<<4)|(sec%10);
	DS1302_Write(cmd,dat);
}

void SetMin(u8t min)
{
	u8t cmd;
	u8t dat;
	cmd=REG_MIN;
	dat=((min/10)<<4)|(min%10);
	DS1302_Write(cmd,dat);
}
void SetHour(u8t hour)
{
	u8t cmd;
	u8t dat;
	cmd=REG_HOUR;
	dat=((hour/10)<<4)|(hour%10);
	DS1302_Write(cmd,dat);
	dat=DS1302_Read(REG_HOUR+1);
}
void SetDay(u8t day)
{
	u8t cmd;
	u8t dat;
	cmd=REG_DAY;
	dat=((day/10)<<4)|(day%10);
	DS1302_Write(cmd,dat);
}

void SetMon(u8t mon)
{
	u8t cmd;
	u8t dat;
	cmd=REG_MON;
	dat=((mon/10)<<4)|(mon%10);
	DS1302_Write(cmd,dat);
}

void SetWeekDay(u8t weekDay)
{
	u8t cmd;
	u8t dat;
	cmd=REG_WEEKDAY;
	dat=((weekDay/10)<<4)|(weekDay%10);
	DS1302_Write(cmd,dat);
}

void SetYear(u8t year)
{
	u8t cmd;
	u8t dat;
	cmd=REG_YEAR;
	dat=((year/10)<<4)|(year%10);
	DS1302_Write(cmd,dat);
}

u32t GetTimeStamp(void)
{
	struct tm time;
	time_t timeStamp;
	time=GetLocalTime();
	timeStamp=mktime(&time);
	return timeStamp;
}

struct tm GetLocalTime(void)
{
	struct tm time;
	u8t sec;
	u8t min;
	u8t hour;
	u8t	day;
	u8t mon;
	u8t weekDay;
	u8t year;
	sec=DS1302_Read(REG_SEC+1);
	min=DS1302_Read(REG_MIN+1);
	hour=DS1302_Read(REG_HOUR+1);
	day=DS1302_Read(REG_DAY+1);
	mon=DS1302_Read(REG_MON+1);
	weekDay=DS1302_Read(REG_WEEKDAY+1);
	year=DS1302_Read(REG_YEAR+1);
	time.tm_sec=((sec&0x7f)>>4)*10+(sec&0x0f);
	time.tm_min=((min&0xff)>>4)*10+(min&0x0f);
	time.tm_hour=((hour&0xff)>>4)*10+(hour&0x0f);
	time.tm_mday=((day&0xff)>>4)*10+(day&0x0f);
	time.tm_mon=((mon&0xff)>>4)*10+(mon&0x0f)-1;
	time.tm_year=((year&0xff)>>4)*10+(year&0x0f)+100;	//the year since 1970
	time.tm_wday=(((weekDay&0xff)>>4)*10+(weekDay&0x0f))%7;
	return time;
}

boolean IsInTime(u32t *startTime,u32t *endTime)
{
	struct tm t_startTime;
	struct tm t_endTime;
	u32t nowTime=0;
	struct tm currentTime;
			
	currentTime=GetLocalTime();		
	

	t_startTime=*(localtime(startTime)); //convert to localtime
	
	
	t_startTime.tm_mday=currentTime.tm_mday;
	t_startTime.tm_mon=currentTime.tm_mon;
	t_startTime.tm_year=currentTime.tm_year;
	t_startTime.tm_wday=currentTime.tm_wday;
	t_startTime.tm_yday=currentTime.tm_yday;
	t_startTime.tm_isdst=currentTime.tm_isdst;

	*startTime=mktime(&t_startTime);	   //convert to timestamp

	t_endTime=*(localtime(endTime));	   //convert to localtime
	
	t_endTime.tm_mday=currentTime.tm_mday;
	t_endTime.tm_mon=currentTime.tm_mon;
	t_endTime.tm_year=currentTime.tm_year;
	t_endTime.tm_wday=currentTime.tm_wday;
	t_endTime.tm_yday=currentTime.tm_yday;
	t_endTime.tm_isdst=currentTime.tm_isdst;

	*endTime=mktime(&t_endTime);		  //convert to timestamp

	nowTime=mktime(&currentTime);
	
	if((nowTime>=*startTime)&&(nowTime<=*endTime))
	{
		return DEF_TRUE;
	}
	else
	{
		return DEF_FALSE;
	}
}