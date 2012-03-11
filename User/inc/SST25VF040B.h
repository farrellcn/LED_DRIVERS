#ifndef  _SST25VF040B_H_
#define  _SST25VF040B_H_

#define U8          unsigned char
#define U16         unsigned short int
#define U32         unsigned long int
#define S8          char
#define S16         short int
#define S32         long int

#define uint				unsigned int
#define uchar				unsigned char
#define ulong               unsigned long

#define Set_FLASH_SCLK  GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define Clr_FLASH_SCLK  GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define Set_FLASH_NSEL  GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define Clr_FLASH_NSEL  GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define Set_FLASH_MOSI  GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define Clr_FLASH_MOSI  GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define FLASH_MISO       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)

//#define Set_FLASH_SCLK  GPIO_SetBits(GPIOA, GPIO_Pin_2)
//#define Clr_FLASH_SCLK  GPIO_ResetBits(GPIOA, GPIO_Pin_2)
//#define Set_FLASH_NSEL  GPIO_SetBits(GPIOA, GPIO_Pin_0)
//#define Clr_FLASH_NSEL  GPIO_ResetBits(GPIOA, GPIO_Pin_0)
//#define Set_FLASH_MOSI  GPIO_SetBits(GPIOA, GPIO_Pin_4)
//#define Clr_FLASH_MOSI  GPIO_ResetBits(GPIOA, GPIO_Pin_4)
//#define FLASH_MISO       GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)


void Flash_Init(void);

//void SSTF040B_Erase(ulong sec1,ulong sec2);

void Write_Flash(U8 sec, U16 count , U8 *str);

void Read_Flash(U8 sec, U16 count,U8 * str);
                                      
#endif						   
