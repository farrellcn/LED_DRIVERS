#include "includes.h"

void DS1302_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //������Ӧ��ʱ��
	/* LD1 & LD2 & LD3 & LD4 configration */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

    init_ds1302(); 
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

void DS1302_Write(INT8U dat)   //д��һ���ֽڡ�
{
    INT8U i;
	//INT8U j;
    Clr_DS1302_CLK;
    IO_Set(IO_OUT);
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
        Set_DS1302_CLK;
    //    for(j=0;j<50;j++);
        Clr_DS1302_CLK;
    //    for(j=0;j<50;j++);
        dat>>=1;
    }
}

INT8U DS1302_Read(void)     //��һ���ֽ�
{
    INT8U i;
	//INT8U j;
	INT8U dat;
    Clr_DS1302_CLK;
    IO_Set(IO_IN);
    for(i=0;i<8;i++)
    {
        dat>>=1;
        if(DS1302_IO !=0)
			dat |=0x80;
		else
			dat &=~0x80;

        Set_DS1302_CLK;
      //  for(j=0;j<50;j++);
        Clr_DS1302_CLK;
      //  for(j=0;j<50;j++);
    }
  //  IO_Set(IO_IN);
    return dat;
}


 void dat_write(INT8U addr, INT8U dat)
 {
    Clr_DS1302_RST;
    Clr_DS1302_CLK;
    Set_DS1302_RST;
    DS1302_Write(addr);
    DS1302_Write(dat);
    Set_DS1302_CLK;
    Clr_DS1302_RST;
 }

 INT8U dat_read(INT8U addr)
 {
    INT8U dat;
    Clr_DS1302_RST;
    Clr_DS1302_CLK;
    Set_DS1302_RST;
    DS1302_Write(addr|0x01);
    dat = DS1302_Read();
    Set_DS1302_CLK;
    Clr_DS1302_RST;
    return dat;
 }
/**�Ƿ�д����**/
void ds1302_protect(INT8U flag)
{
  if(flag)
    dat_write(0x8e,0x10);			 //��ֹд
  else
    dat_write(0x8e,0x00);			 //����д
} 
/**ʱ������**/
void settime(INT8U addr,INT8U value)
{
  ds1302_protect(0);
  dat_write(addr,value);
  ds1302_protect(1);
}
/**ʱ����������**/
void set(INT8U adr,INT8U flag)
{
  INT8U dat;
  dat=dat_read(adr);
  if(flag)
    dat_write(adr,dat+1);
  else
    dat_write(adr,dat-1);
}
/**12/24Сʱʱ������**/
void day_set(INT8U flag)				 //flagΪ1Ϊ12ʱ��,Ϊ0��Ϊ24Сʱ��
{ 
  INT8U hour;
  hour=(dat_read(0x85)&0x7f);		 //����Сʱ�Ĵ�����ԭ�е�ʱ��ֵ
  ds1302_protect(0)	;
  if(flag)
    {
	  dat_write(0x84,0x80|hour);
	}
  else
    {
	  dat_write(0x84,0x00|hour);
	}
  ds1302_protect(1);
} 
/**ȡʱ��**/
systemtime gottime(void)
{
  systemtime time;
  INT8U readvalue;
  readvalue=dat_read(ds1302_second);
  time.second=((readvalue&0x70)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_minute);
  time.minute=((readvalue&0x70)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_hour);
  time.hour=((readvalue&0x30)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_day);
  time.day=((readvalue&0x30)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_week);
  time.week=(readvalue&0x07);
  readvalue=dat_read(ds1302_month);
  time.month=((readvalue&0x01)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_year);
  time.year=((readvalue&0xf0)>>4)*10+(readvalue&0x0f);

  return time;
}
void gettime(systemtime *time,INT8U *datastring) 
{
  INT8U readvalue;
  readvalue=dat_read(ds1302_second);
  time->second=((readvalue&0x70)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_minute);
  time->minute=((readvalue&0x70)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_hour);
  time->hour=((readvalue&0x30)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_day);
  time->day=((readvalue&0x30)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_week);
  time->week=(readvalue&0x07);
  readvalue=dat_read(ds1302_month);
  time->month=((readvalue&0x10)>>4)*10+(readvalue&0x0f);
  readvalue=dat_read(ds1302_year);
  time->year=((readvalue&0xf0)>>4)*10+(readvalue&0x0f);
  datastring[0]=2;
  datastring[1]=0;
  datastring[2]=time->year/10;
  datastring[3]=time->year%10;
  datastring[4] = 10;
  datastring[5]=time->month/10;
  datastring[6]=time->month%10;
  datastring[7] = 10;
  datastring[8]=time->day/10;
  datastring[9]=time->day%10;
  //datastring[8] = '-';
  datastring[10]=time->hour/10;
  datastring[11]=time->hour%10;
  datastring[12] = 11;
  datastring[13]=time->minute/10;
  datastring[14]=time->minute%10;
  datastring[15] = 11;
  datastring[16]=time->second/10;
  datastring[17]=time->second%10;
  datastring[18]=time->week;
}
   /**��ʼʱ������Ϊ2008-08-08 08ʱ08��08��**/
void init_time(void)
{
  day_set(0);
  settime(ds1302_second,0);
  settime(ds1302_minute,0x30);
  settime(ds1302_hour,0x11);
  settime(ds1302_day,0x14);
  settime(ds1302_month,0x07);
  settime(ds1302_year,11);
  settime(ds1302_week,0x04);
  }

INT8U numTOchar(INT8U num)
{
    INT8U shi,ge,hex;
    if(num<=99)
    {
        shi=num/10;
        ge=num%10;
        hex=(shi<<4 | ge);
        return hex;
    }
    else
    {
        return 0;
    }
}
void set_time(INT8U *str)
{
  day_set(0);
  settime(ds1302_year,str[0]);
  settime(ds1302_month,numTOchar(str[1]));
  settime(ds1302_day,numTOchar(str[2]));
  settime(ds1302_week,numTOchar(str[3]));
  settime(ds1302_hour,numTOchar(str[4]));
  settime(ds1302_minute,numTOchar(str[5]));
  settime(ds1302_second,numTOchar(str[6]));
}
   /**DS1302��ʼ��**/
void init_ds1302(void)
{
  INT8U second=dat_read(ds1302_second);
  if(second&0x80)
    settime(ds1302_second,0);
}