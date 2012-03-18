#ifndef __UART_H__
#define __UART_H__

#define INT8U       unsigned char
#define INT16U      unsigned short int
#define INT32U      unsigned long int

#define RX_MAX      512

#define Send_RS485 GPIOA->BSRR  = 0x0001<<1;
#define Recv_RS485 GPIOA->BRR   = 0x0001<<1;




void UART_GPIO_Configuration(void);

void UART_INT_Configuration(void);    //配置串口中断 使用了UART1

void USART1_Init(unsigned int baud);

void UART_Init(void);

void USART1_SendByte(unsigned char temp);

void USART1_Printf(char *pch);

void LED_Display(void);

void LED_Send(INT8U *pch,INT8U num);//发送指定数量的数据包。

INT8U Time_Judge(INT8U *str) ;//用于判断是否在时间内，如果在则返回1，否则，返回0

void Send_Packet(INT8U control,INT8U status,INT8U *str,INT8U num);//发送数据包作为反馈。

INT8U System_Test(INT8U flag);


#endif
