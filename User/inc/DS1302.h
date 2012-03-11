#ifndef __DS1302_H__
#define __DS1302_H__

#define INT8U     unsigned char
#define INT16U    unsigned short int
#define INT32U    unsigned long int

#define  IO_IN    0
#define  IO_OUT   1

#define ds1302_second 0x80
#define ds1302_minute 0x82
#define ds1302_hour   0x84
#define ds1302_day    0x86
#define ds1302_week   0x8a
#define ds1302_month  0x88
#define ds1302_year   0x8c  

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

void DS1302_Init(void);
void IO_Set(INT8U flag);
void DS1302_Write(INT8U dat);   //写入一个字节。  
INT8U DS1302_Read(void);     //读一个字节  
void ds1302_protect(INT8U flag);       
void dat_write(INT8U addr, INT8U dat);   
INT8U dat_read(INT8U addr);      
void settime(INT8U addr,INT8U value); 
void set(INT8U adr,INT8U flag);        
void day_set(INT8U flag);				 //flag为1为12时制,为0则为24小时制    
void gettime(systemtime *time,INT8U *datastring);                       
void init_time(void);                     
void init_ds1302(void);
systemtime gottime(void);
INT8U numTOchar(INT8U num); //十进制和字符之间的切换。
void set_time(INT8U *str);  //时间的设置。


#endif
