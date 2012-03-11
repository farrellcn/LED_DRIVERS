/****************************************Copyright (c)****************************************************
**                            shanghai aian electronics Co.,LTD.
**                                 上海艾安电子有限公司
**                                 http://www.sh2a.com
**                                     秋叶工作室
**------File Info-----------------------------------------------------------------------------------------
** File name:               UART.c
** Latest modified Date:    2011-7-18
** Latest Version:          1.0
** Descriptions:            LED驱动卡的串口通信协议，解析和编码部分。
**                                        
**--------------------------------------------------------------------------------------------------------
** Created by:              李正星
** Created date:            2011-07-18
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
常用寄存器和标志符定义
*********************************************************************************************************/
char R_Buffer[1 + RX_MAX];

short  RsPos = 0;
short  RcPos = 0;

char flag_RX = 0;  //接收状态。
char flag_DAT = 0;  //数据标志位。1表示有数据，0表示无。
char flag_WORK = 0;  //显示屏工作状态。0表示工作，1表示停止。
char flag_OTH  = 0;  //显示其他状态，1 为紧急情况，2 为插播。

INT8U total =0, tiao =0;   //播放的条数和总共广告条数用全局变量。
/*********************************************************************************************************
** 函数名称: UART_GPIO_Configuration
** 功能描述: 串口引脚定义。这里使用串口1。
** 输  　入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void UART_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* 配置串口1引脚  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;        //定义485通信的控制脚。
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*********************************************************************************************************
** 函数名称: UART_INT_Configuration
** 功能描述: 配置串口中断。
** 输  　入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void UART_INT_Configuration(void)    //配置串口中断 使用了UART1
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************************************************
** 函数名称:  USART1_Init
** 功能描述: 串口波特率设置。
** 输  　入: baud 要设置的波特率。
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void USART1_Init(unsigned int baud)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}
/*********************************************************************************************************
** 函数名称:  USART_Init
** 功能描述: 初始化整个串口
** 输  　入: 无
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

void UART_Init(void)
{
    UART_GPIO_Configuration();
	USART1_Init(9600);		 // 配置串口1，波特率9600
    Recv_RS485;//485处于接受状态。

}

/*********************************************************************************************************
** 函数名称:  USART1_SendByte
** 功能描述:  发送一个字节。
** 输  　入: temp 要发送的字节。
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void USART1_SendByte(unsigned char temp)
{
    USART_SendData(USART1, temp);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
/*********************************************************************************************************
** 函数名称:  System_Test
** 功能描述: 测试驱动卡是否正常工作，正常工作返回1，否则返回0。
** 输  　入: flag 要测试的芯片代号。00是存储器，01是字库，03是时钟芯片。
** 输　  出: 0 表示失败，1 表示成功
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
/*INT8U System_Test(INT8U flag)
{
    INT8U temp01[]={0x11,0x22,0x33,0x44};
    INT8U temp02[32];
    systemtime realtime;
    switch(flag)
    {
        case 0x00:                //测试FLASH
                Write_Flash(125,4,&temp01[0]);   //更新存储器中的数据量。
                Read_Flash(125,4,&temp02[0]);   //更新存储器中的数据量。
                if(temp01[0] == temp02[0] && temp01[1] == temp02[1] &&temp01[2] == temp02[2] &&temp01[3] == temp02[3])
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
                break;
        case 0x01:                  //测试字库芯片，使用“正”
                Read_Data(ZFindex16(0x6B63),32,temp02);
                if(temp02[2] == 0x7f && temp02[3] == 0xfc &&temp02[4] == 0x01 &&temp02[5] == 0x00)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
                break;
        case 0x02:                  //测试时钟芯片。
                realtime = gottime();
                if(realtime.hour<=24 && realtime.minute<=60 && realtime.second<=60)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
                break;           
    }
}*/
/*********************************************************************************************************
** 函数名称:  USART1_Printf
** 功能描述: 发送一个字符串。。
** 输  　入: *pch 要发送的字符串。
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void USART1_Printf(char *pch)
{
    Send_RS485;  
    while(*pch != '\0'){
        USART1_SendByte(*(unsigned char *)pch);
        pch++;
    }
    Recv_RS485; 
}
/*********************************************************************************************************
** 函数名称:  LED_Send
** 功能描述:  发送指定数量的数据。
** 输  　入:  *pch 要发送的数组， num 发送的数量。
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void LED_Send(INT8U *pch,INT8U num)
{   
    Send_RS485;   //发送数据
    while(num--)
    {
        USART1_SendByte(*(unsigned char *)pch); 
        pch++;  
    }
    Recv_RS485;    //接受数据
} 
/*********************************************************************************************************
** 函数名称:  Send_Packet
** 功能描述:  发送通信反馈信息。
** 输  　入: control 反馈操作码前四位用B表示；status 接受状态，0接受失败，1接受成功，*str 要发送的数据数组
             num 协议包中数据的个数。
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void Send_Packet(INT8U control,INT8U status,INT8U *str,INT8U num)
{
    INT8U temp[255],i;
    temp[0] = 0x32;
    temp[1] = 0x41;
    temp[2] = 0x00;
    temp[3] = 0x00;
    temp[4] = 0x01;
    temp[5] = control;
    temp[6] = 0x00;
    temp[7] = num;
    temp[8] = status;
    temp[9] = flag_WORK;
    temp[10] = total;
    temp[11] = tiao;
    for(i=0;i<num-6;i++)
    {
        temp[12+i] = str[i];
    }
    temp[12+i] = 0x00;
    temp[13+i] = 0xaa;
    LED_Send(&temp[0],14+i);
}
/*********************************************************************************************************
** 函数名称:  Time_Judge
** 功能描述:  判断当前时间是否允许播放当前广告，允许返回1，不允许返回0。
** 输  　入: *str 起始时间和终止时间的数组。
** 输　  出: 1 允许，0 不允许。
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
INT8U Time_Judge(INT8U *str)
{                                      //用于判断是否在时间内，如果在则返回1，否则，返回0

     systemtime realtime;
     INT8U  Time[2],flag;
     flag = 0;

     if(str[0]<=str[3])              //先判断起始时间和结束时间是否有效，如果无效直接退出。
     {
        if(str[0]<str[3])
        {
          flag=1;   
        }
        else
        {
            if(str[1]<=str[4])
            {
                if(str[1]<str[4])
                {
                    flag=1;
                }
                else
                {
                    if(str[2]<=str[5])
                    {
                        if(str[2]<str[5])
                        {
                            flag=1;
                        }
                        else
                        {
                            flag=0;
                        }
                    }
                    else
                    {
                        flag=0;
                    }
                }
            }
            else
            {
                flag=0;
            }
        }
     }
     else
     {
        flag=0;
     }

     if(flag==1)           //判断是否在有效的时间段里面。
     {
      //   gettime(&realtime,&Time[0]);
         realtime = gottime();
         if(realtime.hour>=str[0]&&realtime.hour<=str[3])
         {
            if(realtime.hour>str[0] && realtime.hour<str[3])
            {
                return 1;
            }
            else if(realtime.hour == str[0] && realtime.hour != str[3])
            {
               if(realtime.minute>=str[1])
               {
                   if(realtime.minute>str[1])
                   {
                        return 1;
                   }
                   else 
                   {
                        if(realtime.second>=str[2])
                        {
                            return 1;
                        }     
                        else
                        {
                            return 0;
                        }
                   }     
               }
               else 
               {
                   return 0;
               }            
             }
            else if(realtime.hour != str[0] && realtime.hour == str[3])
            {
               if(realtime.minute<=str[4])
               {
                   if(realtime.minute<str[4])
                   {
                        return 1;
                   }
                   else 
                   {
                        if(realtime.second<=str[5])
                        {
                            return 1;
                        }     
                        else
                        {
                            return 0;
                        }
                   }     
               }
               else 
               {
                   return 0;
               }            
            }
            else if(realtime.hour == str[0] && realtime.hour == str[3])
            {
                if(realtime.minute >=str[1] && realtime.minute <=str [4])
                {
                    if(realtime.minute> str[1] && realtime.minute<str[4])
                    {
                        return 1;    
                    }
                    else if(realtime.minute == str[1] && realtime.minute != str[4])
                    {
                        if(realtime.second >= str[2])
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }                                
                    }
                    else if(realtime.minute != str[1] && realtime.minute == str[4])
                    {
                        if(realtime.second <= str[5])
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }           
            }
            
         }
         else 
         {
            return 0;
         }
     }
     else
     {
        return 0;
     }
}
/*********************************************************************************************************
** 函数名称:  LED_Display
** 功能描述: 驱动卡的全部操作都在这里。作为一个任务
** 输  　入: 无。
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
/*
void LED_Display()      //主要系统。
{
    INT8U buf[255];
    INT8U i,j,flag;  //用于判断广告中的次数。
    INT16U zishu;
    INT8U word[34]={0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20};
    INT8U word1[64]={0x00,0x28,0x00,0x30,0x00,0X3C,0x4E,0x0A,0x6D,0x77,0x82,0x7E,0x5B,0x89,0x75,0x35,0x5B,0x50};
    flag=0;
    flag_WORK = 0;
    LED_ONOFF(0);
    for(i=0;i<255;i++);
    for(i=0;i<255;i++);
    for(i=0;i<255;i++);
    Read_Flash(0, 2 , &buf[0]);         //第一扇区用来
    if(buf[1] >0 && buf[1] <= 200 )     //广告条数限制在200条以内
    {
        total = buf[1];
    }
    else
    {
        total = 0;
    }
    flag_WORK = 1;
    LED_ONOFF(1);

    while(1)
    {   
        Read_Flash(122, 10 , &buf[0]);
        if(Time_Judge(&buf[4]))      //判断是否在时间内。
        {
         //  flag++;
           zishu= (INT16U)buf[0]<<8 | buf[1] +2;
           if(zishu !=0 )
           {
               Read_Flash(122, zishu, &buf[0]); //读出该字库。
               Dis_LED_Way(&buf[10],(zishu/2)-5,buf[3],buf[2]);
               buf[0]=0x00;
               buf[1]=0x00;
               Write_Flash(122, 2, &buf[0]);
           }
         }

        Read_Flash(121, 10 , &buf[0]);
        if(Time_Judge(&buf[4]))      //判断是否在时间内。
        {
         //  flag++;
           zishu= (INT16U)buf[0]<<8 | buf[1] +2;
           if(zishu !=0 )
           {
               Read_Flash(121, zishu, &buf[0]); //读出该字库。
               Dis_LED_Way(&buf[10],(zishu/2)-5,buf[3],buf[2]);
               buf[0]=0x00;
               buf[1]=0x00;
               Write_Flash(121, 2, &buf[0]);
           }
         }

        if(flag_WORK != 0)
        {
            if(1)
            {
                if(total!=0)
                {
                    tiao++;
                    if(tiao > total)
                    {
                        tiao = 1;
                        flag=0;
                    }
                    Read_Flash(tiao, 10 , &buf[0]);     //读出该条字数
                    if(Time_Judge(&buf[4]))      //判断是否在时间内。
                    {
                        flag++;
                        zishu= (INT16U)buf[0]<<8 | buf[1] +2;
                        Read_Flash(tiao, zishu, &buf[0]); //读出该字库。
                        Dis_LED_Way(&buf[10],(zishu/2)-5,buf[3],buf[2]);
                    }
                    else if(flag == 0)       //当有广告的时候，但是没有播放时，恢复默认状态。显示时间。
                    {
                       Dis_LED_Way(word1,9,4,4);
                    }
                }
                else
                {
                    Dis_LED_Way(word1,9,4,3);
                }
            }
        }  
    }


}
*/
/*********************************************************************************************************
** 函数名称: USART1_IRQHandler
** 功能描述: 串口中断程序。用于接受数据、解析协议、执行相应的指令、反馈。
** 输  　入: 无。
** 输　  出: 无
** 全局变量: 无
** 调用模块: 无
** 返    回：无
** 作  　者: 李正星
** 日　  期: 2011年7月18日
**------------------------------------------------------------------------------------------------------
** 修 改 人:
** 日　  期:
**------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void USART1_IRQHandler(void)
{
    INT8U temp,i;
    INT16U zishu;
    INT8U buf[4];
    systemtime realtime;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){	  // 如果是串口接收中断
       if(RsPos < RX_MAX)
       {
            R_Buffer[RsPos++] = USART_ReceiveData(USART1);
            if(flag_RX == 0 )                                //识别是否是有效数据 , 同时
            {
                if((RsPos == 2) && R_Buffer[1] == 'A')        //用于判断是否开始接收数据，信息头为2A 0x32,0x41。
                {
                    if(R_Buffer[0] == '2')
                    {
                        flag_RX =1;
                    }
                    else
                    {
                        RsPos =0;
                        return ;
                    }
                } 
                else if(R_Buffer[0]!='2')                       //不是有效位时则清零。
                {
                    RsPos=0;
                    return;
                }
            }
            else if(flag_RX == 1)                               //如果是有效位则开始接收余下的数据。
            {
               if(RsPos > 8)                                    //用于判断结束位。
               {
                    temp=(short int)R_Buffer[6]<<8 | R_Buffer[7];
                    if(RsPos > ( temp + 9))
                    {
                        if(R_Buffer[temp+9] == 0xaa)             //一帧结束标志
                        {                        
                            RsPos=0;
                            flag_RX=0;                          //接受完数据后开始存储。
                            if(R_Buffer[5]==0xdb)       //普通广告
                            {
                                total++;
                                if(total>=120)
                                {
                                    total = 120;
                                }
                                zishu= (INT16U)R_Buffer[6]<<8 | R_Buffer[7]+2;        //排除显示方式。
                                Write_Flash(total,zishu,&R_Buffer[6]);
                                buf[0]=0;
                                buf[1]=total;
                                Write_Flash(0,2,&buf[0]);   //更新存储器中的数据量。
                                Send_Packet(0xbb,1,&R_Buffer[0],6+0);
                            }
                            else if(R_Buffer[5]==0xdd)    //插播广告
                            {
                                zishu= (INT16U)R_Buffer[6]<<8 | R_Buffer[7]+2;        //排除显示方式。
                                Write_Flash(121,zishu,&R_Buffer[6]);
                                Send_Packet(0xbd,1,&R_Buffer[0],6+0);
                            }
                            else if(R_Buffer[5] == 0xde)//紧急广告
                            {
                                zishu= (INT16U)R_Buffer[6]<<8 | R_Buffer[7]+2;        //排除显示方式。
                                Write_Flash(122,zishu,&R_Buffer[6]);
                                Send_Packet(0xbe,1,&R_Buffer[0],6+0);    //反馈信息
                            }
                            else if(R_Buffer[5] == 0xd1)//删除驱动板中全部信息。
                            {
                                total=0;
                                tiao =0;
                                buf[0]=0;
                                buf[1]=total;
                                Write_Flash(0,2,&buf[0]);   //更新存储器中的数据量。
                                Send_Packet(0xb1,1,&R_Buffer[0],6+0);
                            }
                            else if(R_Buffer[5] == 0xd2)   //关闭显示屏
                            {
                                flag_WORK = 0;
                                LED_ONOFF(0);
                                Send_Packet(0xb2,1,&R_Buffer[0],6+0);
                            }
                            else if(R_Buffer[5] == 0xd3)      //打开显示屏。
                            {
                                flag_WORK = 1;
                                LED_ONOFF(1);
                                Send_Packet(0xb3,1,&R_Buffer[0],6+0);
                            } 
                            else if(R_Buffer[5] == 0xd4)     // 测试系统的工作状态。
                            {
                                for(i=0;i<3;i++)
                                {
                                    //R_Buffer[i]=System_Test(i);   沈加修改
                                    R_Buffer[i]=0;
                                }
                                Send_Packet(0xb4,1,&R_Buffer[0],6+3);
                            }
                            else if(R_Buffer[5] == 0xd5)  // 发送本地时间。
                            {
                                realtime = gottime();
                                R_Buffer[0] = realtime.year;
                                R_Buffer[1] = realtime.month;
                                R_Buffer[2] = realtime.day;
                                R_Buffer[3] = realtime.week;
                                R_Buffer[4] = realtime.hour;
                                R_Buffer[5] = realtime.minute;
                                R_Buffer[6] = realtime.second;
                                Send_Packet(0xb5,1,&R_Buffer[0],6+7);
                            }
                            else if(R_Buffer[5] == 0xd6) 
                            {
                                set_time(&R_Buffer[16]);
                                Send_Packet(0xb6,1,&R_Buffer[0],6+0);
                            }                                                 
                            return;
                        }
                        else                                    //数据结尾不正确时则清零，
                        {
                            RsPos =0;
                            flag_RX=0;
                            Send_Packet(0x00,0,&R_Buffer[0],6+0);;        //接受失败。
                            return;
                        }
                    }
               } 
            }
       }
    }	
}