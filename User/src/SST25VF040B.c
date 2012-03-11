/****************************************Copyright (c)****************************************************
**                            shanghai aian electronics Co.,LTD.
**                                 �Ϻ������������޹�˾
**                                 http://www.sh2a.com
**                                     ��Ҷ������
**------File Info-----------------------------------------------------------------------------------------
** File name:               SST25VF040B.c
** Latest modified Date:    2010-07-01
** Latest Version:          1.0
** Descriptions:            4Mbit FLASH�������ļ�   ��ʹ��ģ��SPIЭ�顷            
**--------------------------------------------------------------------------------------------------------
** Created by:              ������
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
Ӳ���Ĵ�����ز���ͷ�ļ�
*********************************************************************************************************/
#include "includes.h"

/*********************************************************************************************************
** ��������: Flash_Init
** ��������: SST25VF040 flash��ʼ��
** ��  ����: ��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: Flash_Port_Init(),Flash_SPI0_Init();
** ��    �أ���
** ��  ����: ������
** �ա�  ��: 2010��7��01��
**------------------------------------------------------------------------------------------------------
** �� �� ��:
** �ա�  ��:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Flash_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //������Ӧ��ʱ��
	/* LD1 & LD2 & LD3 & LD4 configration */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* ������������ */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}
/*********************************************************************************************************
** ��������: write_to_flash_byte
** ��������: ��SST25VF040 flashд��һ���ֽ�
** ��  ����: cdataΪ��д�������
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: ��
** ��    �أ���
** ��  ����: ������
** �ա�  ��: 2010��7��01��
**------------------------------------------------------------------------------------------------------
** �� �� ��:
** �ա�  ��:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
U8 Write_To_Flash_Byte(U8 byte)
{
	U8 i ,temp;
	Clr_FLASH_SCLK;
	for(temp=0;temp<200;temp++);            //����̫��
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
		for(temp=0;temp<200;temp++);			   //ʹ��ģʽSPIʱ�����ʱҪ���պã�����̫��	   

		if(FLASH_MISO !=0)
			byte |=1;
		else
			byte&=~1;

		Clr_FLASH_SCLK;
		for(temp=0;temp<200;temp++);            //����̫��

	}
	return (byte);
}
/*********************************************************************************************************
** ��������: SSTF040B_RD
** ��������: ��SST25VF040 flash������Ӧ������
** ��  ����: add ΪҪ�����ĵ�ַ 0-0x07ff;NByte Ҫ���������ݸ�����*strҪ��ŵ���������
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: Write_To_Flash_Byte();
** ��    �أ���
** ��  ����: ������
** �ա�  ��: 2010��7��01��
**------------------------------------------------------------------------------------------------------
** �� �� ��:
** �ա�  ��:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void SSTF040B_RD(ulong add, ulong NByte,U8 * str)
{
	ulong i = 0;
		Clr_FLASH_NSEL;                    
		Write_To_Flash_Byte(0x06);						// ����дʹ������
		Set_FLASH_NSEL; 
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x03); 						// ���Ͷ�����
	Write_To_Flash_Byte(((add & 0xFFFFFF) >> 16));	// ���͵�ַ��Ϣ:�õ�ַ��3���ֽ����
	Write_To_Flash_Byte(((add & 0xFFFF) >> 8));
	Write_To_Flash_Byte(add & 0xFF);
 
 	for (i = 0; i < NByte; i++)		
	{
	  	str[i]=Write_To_Flash_Byte(0xff);
	}
	Set_FLASH_NSEL;                    
}

/*********************************************************************************************************
** ��������: SSTF040B_SECWR
** ��������: ��Flash��������Ϊ��λд������
** ��  ����: sec Ҫд������� count ΪҪд������ݸ��� *str ΪҪд���������
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: Write_To_Flash_Byte();
** ��    �أ���
** ��  ����: ������
** �ա�  ��: 2010��7��01��
**------------------------------------------------------------------------------------------------------
** �� �� ��:
** �ա�  ��:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Write_Flash(U8 sec, U16 count , U8 *str)
{
	U8 temp = 0,StatRgVal = 0;
	ulong i=0 , add=0;
    Set_FLASH_NSEL;
//	sec = sec+11;               //����flash�м�����41K�ֿ�����Ҫ��11��������ƫ������
	add= 0x1000 * (ulong)sec;
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
	temp =Write_To_Flash_Byte(0xff);							// ���Ͷ�״̬�Ĵ�������
	Set_FLASH_NSEL;                    

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x50);							// ʹ״̬�Ĵ�����д
	Set_FLASH_NSEL;                    
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x01);							// ����д״̬�Ĵ���ָ��
	Write_To_Flash_Byte(0x00);								// ��0BPxλ��ʹFlashоƬȫ����д 
	Set_FLASH_NSEL;                    

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x06);						// ����дʹ������
	Set_FLASH_NSEL;                    

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x20); 						// �����ֽ�������д����
	Write_To_Flash_Byte(((add & 0xFFFFFF) >> 16));// ����3���ֽڵĵ�ַ��Ϣ 
	Write_To_Flash_Byte(((add & 0xFFFF) >> 8));
	Write_To_Flash_Byte(add & 0xFF);
	Set_FLASH_NSEL;        
	do
	{
		Clr_FLASH_NSEL;                    
		Write_To_Flash_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
		StatRgVal =Write_To_Flash_Byte(0xff);
		Set_FLASH_NSEL;                    
	}            
	while (StatRgVal == 0x03);
				
	for(i = 0; i < count; i++)
	{
		Clr_FLASH_NSEL;                    
		Write_To_Flash_Byte(0x06);						// ����дʹ������
		Set_FLASH_NSEL;                    

		Clr_FLASH_NSEL;                    
		Write_To_Flash_Byte(0x02); 						// �����ֽ�������д����
		Write_To_Flash_Byte((((add+i) & 0xFFFFFF) >> 16));// ����3���ֽڵĵ�ַ��Ϣ 
		Write_To_Flash_Byte((((add+i) & 0xFFFF) >> 8));
		Write_To_Flash_Byte((add+i) & 0xFF);
		Write_To_Flash_Byte(str[i]);					// ���ͱ���д������
		Set_FLASH_NSEL;                    

		do
		{
			Clr_FLASH_NSEL;                    
			Write_To_Flash_Byte(0x05);					// ���Ͷ�״̬�Ĵ�������
			StatRgVal =Write_To_Flash_Byte(0xff);							// ���Ͷ�״̬�Ĵ�������
			Set_FLASH_NSEL;                    
  		}
		while (StatRgVal == 0x03);				// һֱ�ȴ���ֱ��оƬ����
	}

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x04);					// ���Ͷ�״̬�Ĵ�������
	temp =Write_To_Flash_Byte(0xff);							// ���Ͷ�״̬�Ĵ�������
	Set_FLASH_NSEL;                    
        
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x06);					// ���Ͷ�״̬�Ĵ�������
	Set_FLASH_NSEL;                    

	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x50);					// ���Ͷ�״̬�Ĵ�������
	Set_FLASH_NSEL;  
                  
	Clr_FLASH_NSEL;                    
	Write_To_Flash_Byte(0x01);					// ���Ͷ�״̬�Ĵ�������
	Write_To_Flash_Byte(temp);					// ���Ͷ�״̬�Ĵ�������
	Set_FLASH_NSEL; 
}
/*********************************************************************************************************
** ��������: SSSTF040B_SECRD
** ��������: ��Flash��������Ϊ��λ�������� 
** ��  ����: sec Ҫ���������� count ΪҪ���������ݵĸ��� *str Ϊ�������ݴ洢��������
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: Write_To_Flash_Byte();
** ��    �أ���
** ��  ����: ������
** �ա�  ��: 2010��7��01��
**------------------------------------------------------------------------------------------------------
** �� �� ��:
** �ա�  ��:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Read_Flash(U8 sec, U16 count,U8 * str)
{
	ulong i = 0;
	ulong add;
//	sec = sec+11;               //����flash�м�����41K�ֿ�����Ҫ��11��������ƫ������
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
** ��������: SSTF040B_Erase
** ��������: ����Flash�������ڵ�����
** ��  ����: sec1 Ϊ��ʼ���� sec2Ϊ��ֹ���� ����ΧΪ0-128��
** �䡡  ��: ��
** ȫ�ֱ���: ��
** ����ģ��: Write_To_Flash_Byte();
** ��    �أ���
** ��  ����: ������
** �ա�  ��: 2010��7��01��
**------------------------------------------------------------------------------------------------------
** �� �� ��:
** �ա�  ��:
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
//	Write_To_Flash_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
//	Write_To_Flash_Byte(0xff);
//	temp1 = SPI0DAT;     							// ���Ͷ�״̬�Ĵ�������
//	Set_FLASH_NSEL;                    
//
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x50);							// ʹ״̬�Ĵ�����д
//	Set_FLASH_NSEL;    
//	                
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x01);							// ����д״̬�Ĵ���ָ��
//	Write_To_Flash_Byte(0x00);							// ��0BPxλ��ʹFlashоƬȫ����д 
//	Set_FLASH_NSEL;    
//	                
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x06);						// ����дʹ������
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
//		Write_To_Flash_Byte(0x20); 						// �����ֽ�������д����
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFFFF) >> 16));// ����3���ֽڵĵ�ַ��Ϣ 
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFF) >> 8));
//		Write_To_Flash_Byte(SenHdAddr & 0xFF);
//		Set_FLASH_NSEL;        
//		do
//		{
//			Clr_FLASH_NSEL;                    
//			Write_To_Flash_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
//			StatRfVal=Write_To_Flash_Byte(0xff);
//			Set_FLASH_NSEL;                    
//		}            
//		while (StatRfVal == 0x03);
//	}
//
//	if(sec2 - sec1 == 128)
//	{
//		Clr_FLASH_NSEL;                    
//		Write_To_Flash_Byte(0x60);						// ����дʹ������
//		Set_FLASH_NSEL;
//		do
//		{
//			Clr_FLASH_NSEL;                    
//			Write_To_Flash_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
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
//		Write_To_Flash_Byte(0x52); 						// �����ֽ�������д����
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFFFF) >> 16));// ����3���ֽڵĵ�ַ��Ϣ 
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFF) >> 8));
//		Write_To_Flash_Byte(SenHdAddr & 0xFF);
//		Set_FLASH_NSEL;   
//		do
//		{
//			Clr_FLASH_NSEL;                    
//			Write_To_Flash_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
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
//		Write_To_Flash_Byte(0x20); 						// �����ֽ�������д����
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFFFF) >> 16));// ����3���ֽڵĵ�ַ��Ϣ 
//		Write_To_Flash_Byte(((SenHdAddr & 0xFFFF) >> 8));
//		Write_To_Flash_Byte(SenHdAddr & 0xFF);
//		Set_FLASH_NSEL;   
//		do
//		{
//			Clr_FLASH_NSEL;                    
//			Write_To_Flash_Byte(0x05);							// ���Ͷ�״̬�Ĵ�������
//			StatRfVal =Write_To_Flash_Byte(0xff);
//			Set_FLASH_NSEL;                    
//		}            
//		while (StatRfVal == 0x03);
//		CurSecToEr += 1;
//		no_SecsToEr -=1;		     
//	}
//
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x06);							// ���Ͷ�״̬�Ĵ�������
//	Set_FLASH_NSEL;                    
//
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x50);							// ʹ״̬�Ĵ�����д
//	Set_FLASH_NSEL;    
//	                
//	Clr_FLASH_NSEL;                    
//	Write_To_Flash_Byte(0x01);							// ����д״̬�Ĵ���ָ��
//	Write_To_Flash_Byte(temp1);							// ��0BPxλ��ʹFlashоƬȫ����д 
//	Set_FLASH_NSEL;    
//	                          
//}