/****************************************Copyright (c)****************************************************
**                            shanghai aian electronics Co.,LTD.
**                                 上海艾安电子有限公司
**                                 http://www.sh2a.com
**                                     秋叶工作室
**------File Info-----------------------------------------------------------------------------------------
** File name:               SST25VF040B.c
** Latest modified Date:    2010-07-01
** Latest Version:          1.0
** Descriptions:            4Mbit FLASH的驱动文件   《使用模拟SPI协议》            
**--------------------------------------------------------------------------------------------------------
** Created by:              李正星
** Created date:            2010-07-01
** Version:                 1.0
** Descriptions:            
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
/*********************************************************************************************************
硬件寄存器相关操作头文件
*********************************************************************************************************/
#include "includes.h"

/*********************************************************************************************************
** 函数名称: Flash_Init
** 功能描述: SST25VF040 flash初始化
** 输  　入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: Flash_Port_Init(),Flash_SPI0_Init();
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2010年7月01日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Flash_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //开启相应的时钟
	/* LD1 & LD2 & LD3 & LD4 configration */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* 按键引脚配置 */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}
/*********************************************************************************************************
** 函数名称: write_to_flash_byte
** 功能描述: 向SST25VF040 flash写入一个字节
** 输  　入: cdata为待写入的数据
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2010年7月01日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
U8 Write_To_Flash_Byte(U8 byte)
{
	U8 i ,temp;
	Clr_FLASH_SCLK;
	for(temp=0;temp<200;temp++);            //不能太快
	for(i=0;i<8;i++)
	{

		if(0x80 == (byte & 0x80))
		{
			Set_FLASH_MOSI;
		}
		else
		{
			Clr_FLASH_MOSI;
		}
		byte = (byte<<1);
		Set_FLASH_SCLK;
		for(temp=0;temp<200;temp++);			   //使用模式SPI时间的延时要把握好，不能太短	   

		if(FLASH_MISO !=0)
			byte |=1;
		else
			byte&=~1;

		Clr_FLASH_SCLK;
		for(temp=0;temp<200;temp++);            //不能太快

	}
	return (byte);
}
/*********************************************************************************************************
** 函数名称: SSTF040B_RD
** 功能描述: 从SST25VF040 flash读出相应的数据
** 输  　入: add 为要读出的地址 0-0x07ff;NByte 要读出的数据个数，*str要存放的数据数组
** 输　  出: 无
** 全局变量: 无
** 调用模块: Write_To_Flash_Byte();
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2010年7月01日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SSTF040B_RD(ulong add, ulong NByte,U8 * str)
{
	ulong i = 0;
		Clr_FLASH_NSEL;                    
		Write_To_Flash_Byte(0x06);						// 发送写使能命令
		Set_FLASH_NSEL; 
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x03); 						// 发送读命令
	Write_To_Flash_Byte(((add & 0xFFFFFF) >> 16));	// 发送地址信息:该地址由3个字节组成
	Write_To_Flash_Byte(((add & 0xFFFF) >> 8));
	Write_To_Flash_Byte(add & 0xFF);
 
 	for (i = 0; i < NByte; i++)		
	{
	  	str[i]=Write_To_Flash_Byte(0xff);
	}
	Set_FLASH_NSEL;                    
}

/*********************************************************************************************************
** 函数名称: SSTF040B_SECWR
** 功能描述: 在Flash中以扇区为单位写入数据
** 输  　入: sec 要写入的扇区 count 为要写入的数据个数 *str 为要写入的数据组
** 输　  出: 无
** 全局变量: 无
** 调用模块: Write_To_Flash_Byte();
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2010年7月01日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Write_Flash(U8 sec, U16 count , U8 *str)
{
	U8 temp = 0,StatRgVal = 0;
	ulong i=0 , add=0;
    Set_FLASH_NSEL;
//	sec = sec+11;               //由于flash中加入了41K字库所以要有11个扇区的偏移量。
	add= 0x1000 * (ulong)sec;
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x05);							// 发送读状态寄存器命令
	temp =Write_To_Flash_Byte(0xff);							// 发送读状态寄存器命令
	Set_FLASH_NSEL;                    

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x50);							// 使状态寄存器可写
	Set_FLASH_NSEL;                    
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x01);							// 发送写状态寄存器指令
	Write_To_Flash_Byte(0x00);								// 清0BPx位，使Flash芯片全区可写 
	Set_FLASH_NSEL;                    

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x06);						// 发送写使能命令
	Set_FLASH_NSEL;                    

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x20); 						// 发送字节数据烧写命令
	Write_To_Flash_Byte(((add & 0xFFFFFF) >> 16));// 发送3个字节的地址信息 
	Write_To_Flash_Byte(((add & 0xFFFF) >> 8));
	Write_To_Flash_Byte(add & 0xFF);
	Set_FLASH_NSEL;        
	do
	{
		Clr_FLASH_NSEL;                    
		Write_To_Flash_Byte(0x05);							// 发送读状态寄存器命令
		StatRgVal =Write_To_Flash_Byte(0xff);
		Set_FLASH_NSEL;                    
	}            
	while (StatRgVal == 0x03);
				
	for(i = 0; i < count; i++)
	{
		Clr_FLASH_NSEL;                    
		Write_To_Flash_Byte(0x06);						// 发送写使能命令
		Set_FLASH_NSEL;                    

		Clr_FLASH_NSEL;                    
		Write_To_Flash_Byte(0x02); 						// 发送字节数据烧写命令
		Write_To_Flash_Byte((((add+i) & 0xFFFFFF) >> 16));// 发送3个字节的地址信息 
		Write_To_Flash_Byte((((add+i) & 0xFFFF) >> 8));
		Write_To_Flash_Byte((add+i) & 0xFF);
		Write_To_Flash_Byte(str[i]);					// 发送被烧写的数据
		Set_FLASH_NSEL;                    

		do
		{
			Clr_FLASH_NSEL;                    
			Write_To_Flash_Byte(0x05);					// 发送读状态寄存器命令
			StatRgVal =Write_To_Flash_Byte(0xff);							// 发送读状态寄存器命令
			Set_FLASH_NSEL;                    
  		}
		while (StatRgVal == 0x03);				// 一直等待，直到芯片空闲
	}

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x04);					// 发送读状态寄存器命令
	temp =Write_To_Flash_Byte(0xff);							// 发送读状态寄存器命令
	Set_FLASH_NSEL;                    
        
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x06);					// 发送读状态寄存器命令
	Set_FLASH_NSEL;                    

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x50);					// 发送读状态寄存器命令
	Set_FLASH_NSEL;  
                  
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x01);					// 发送读状态寄存器命令
	Write_To_Flash_Byte(temp);					// 发送读状态寄存器命令
	Set_FLASH_NSEL; 
}
/*********************************************************************************************************
** 函数名称: SSSTF040B_SECRD
** 功能描述: 在Flash中以扇区为单位读出数据 
** 输  　入: sec 要读出的扇区 count 为要读出的数据的个数 *str 为读出数据存储的数据组
** 输　  出: 无
** 全局变量: 无
** 调用模块: Write_To_Flash_Byte();
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2010年7月01日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Read_Flash(U8 sec, U16 count,U8 * str)
{
	ulong i = 0;
	ulong add;
//	sec = sec+11;               //由于flash中加入了41K字库所以要有11个扇区的偏移量。
	add = 0x1000 * (ulong)sec;
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x06);						
	Set_FLASH_NSEL; 

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x03); 						
	Write_To_Flash_Byte(((add & 0xFFFFFF) >> 16));	
	Write_To_Flash_Byte(((add & 0xFFFF) >> 8));
	Write_To_Flash_Byte(add & 0xFF);
 
 	for (i = 0; i < count; i++)		
	{
	  	str[i]=Write_To_Flash_Byte(0xff);
	}
	Set_FLASH_NSEL;                    
}
/*********************************************************************************************************
** 函数名称: SSTF040B_Erase
** 功能描述: 擦除Flash中扇区内的内容
** 输  　入: sec1 为起始扇区 sec2为终止扇区 （范围为0-128）
** 输　  出: 无
** 全局变量: 无
** 调用模块: Write_To_Flash_Byte();
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2010年7月01日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
//void SSTF040B_Erase(ulong sec1,ulong sec2)
//{
//	U8 temp1=0,temp2 = 0,StatRfVal = 0;
//	ulong SenHdAddr = 0;
//	ulong no_SecsToEr = 0;
//	ulong CurSecToEr = 0;
//
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x05);							// 发送读状态寄存器命令
//	Write_To_Flash_Byte(0xff);
//	temp1 = SPI0DAT;     							// 发送读状态寄存器命令
//	Set_FLASH_NSEL;                    
//
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x50);							// 使状态寄存器可写
//	Set_FLASH_NSEL;    
//	                
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x01);							// 发送写状态寄存器指令
//	Write_To_Flash_Byte(0x00);							// 清0BPx位，使Flash芯片全区可写 
//	Set_FLASH_NSEL;    
//	                
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x06);						// 发送写使能命令
//	Set_FLASH_NSEL;                    
//
//	if(sec1 > sec2)
//	{
//		temp1 = sec1;
//		sec1 = sec2;
//		sec2 = temp1; 
//	}	
//
//	if(sec1 == sec2)
//	{
//		SenHdAddr = 0x1000 * sec1;
//		Clr_FLASH_NSEL;                    
//		Write_To_Flash_Byte(0x20); 						// 发送字节数据烧写命令
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFFFF) >> 16));// 发送3个字节的地址信息 
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFF) >> 8));
//		Write_To_Flash_Byte(SenHdAddr & 0xFF);
//		Set_FLASH_NSEL;        
//		do
//		{
//			Clr_FLASH_NSEL;                    
//			Write_To_Flash_Byte(0x05);							// 发送读状态寄存器命令
//			StatRfVal=Write_To_Flash_Byte(0xff);
//			Set_FLASH_NSEL;                    
//		}            
//		while (StatRfVal == 0x03);
//	}
//
//	if(sec2 - sec1 == 128)
//	{
//		Clr_FLASH_NSEL;                    
//		Write_To_Flash_Byte(0x60);						// 发送写使能命令
//		Set_FLASH_NSEL;
//		do
//		{
//			Clr_FLASH_NSEL;                    
//			Write_To_Flash_Byte(0x05);							// 发送读状态寄存器命令
//			StatRfVal = Write_To_Flash_Byte(0xff);
//			Set_FLASH_NSEL;                    
//		}            
//		while (StatRfVal == 0x03);
//	}
//	no_SecsToEr = sec2 - sec1 + 1;
//	CurSecToEr = sec1;
//	
//	while(no_SecsToEr >= 8)
//	{
//		SenHdAddr = 0x1000 * sec1;
//		FLASH_NSEL_PIN = 0;
//		Write_To_Flash_Byte(0x52); 						// 发送字节数据烧写命令
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFFFF) >> 16));// 发送3个字节的地址信息 
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFF) >> 8));
//		Write_To_Flash_Byte(SenHdAddr & 0xFF);
//		Set_FLASH_NSEL;   
//		do
//		{
//			Clr_FLASH_NSEL;                    
//			Write_To_Flash_Byte(0x05);							// 发送读状态寄存器命令
//			StatRfVal =Write_To_Flash_Byte(0xff);
//			Set_FLASH_NSEL;                    
//		}            
//		while (StatRfVal == 0x03);
//		CurSecToEr += 8;
//		no_SecsToEr -=8;		     
//	}
//
//	while(no_SecsToEr >= 1)
//	{
//		SenHdAddr = 0x1000 * sec1;
//
//		FLASH_NSEL_PIN = 0;
//		Write_To_Flash_Byte(0x20); 						// 发送字节数据烧写命令
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFFFF) >> 16));// 发送3个字节的地址信息 
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFF) >> 8));
//		Write_To_Flash_Byte(SenHdAddr & 0xFF);
//		Set_FLASH_NSEL;   
//		do
//		{
//			Clr_FLASH_NSEL;                    
//			Write_To_Flash_Byte(0x05);							// 发送读状态寄存器命令
//			StatRfVal =Write_To_Flash_Byte(0xff);
//			Set_FLASH_NSEL;                    
//		}            
//		while (StatRfVal == 0x03);
//		CurSecToEr += 1;
//		no_SecsToEr -=1;		     
//	}
//
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x06);							// 发送读状态寄存器命令
//	Set_FLASH_NSEL;                    
//
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x50);							// 使状态寄存器可写
//	Set_FLASH_NSEL;    
//	                
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x01);							// 发送写状态寄存器指令
//	Write_To_Flash_Byte(temp1);							// 清0BPx位，使Flash芯片全区可写 
//	Set_FLASH_NSEL;    
//	                          
//}