/****************************************Copyright (c)****************************************************
**                            shanghai aian electronics Co.,LTD.
**                                 上海艾安电子有限公司
**                                 http://www.sh2a.com
**                                     秋叶工作室
**------File Info-----------------------------------------------------------------------------------------
** File name:               LEDriver.c
** Latest modified Date:    2011-7-18
** Latest Version:          1.0
** Descriptions:            LED显示屏驱动卡驱动部分。
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
#define zi_shu  33 //待显示的字数

#define zi_shu_dang  1 //当前只能显示的字数
unsigned char h;
unsigned int led_t;
unsigned int m;
unsigned int iii,jjj;
unsigned char ttt;
unsigned char zi;

unsigned char lll;

unsigned char flag_wei;
uchar speed=60;

unsigned char LED_Buffer[32][LED_Size*2+2];	                 //定义一下显示缓冲
unsigned char LED_Cache[32][LED_Size*2+2];                   //二级缓存
unsigned char  Flag_Word;  

/*******************************************************************************************************
用于测试LED屏所使用的字库索引号。
*******************************************************************************************************/
unsigned char  MY_DATA[]={
0x00,0x02,0x3C,0x00,0x3C,0x00,0x7E,0x00,
};

unsigned char TEACHER[]={


0x0C,0x60,0x1E,0x60,0xF8,0x60,0x18,0x60,0x19,0xE6,0xFF,0xE6,0x19,0xEC,0x3B,0x78,//
0x3C,0x60,0x7E,0xF0,0x7E,0xF0,0xD8,0xD8,0x19,0x98,0x19,0x8C,0x1B,0x06,0x1E,0x03,//秋//0

0x00,0x30,0x00,0x30,0x7C,0x30,0x6C,0x30,0x6C,0x30,0x6C,0x30,0x6F,0xFF,0x6C,0x30,//
0x6C,0x30,0x6C,0x30,0x7C,0x30,0x6C,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,//叶//1
};

unsigned char SH2A[]={

0x06,0x60,0x06,0x60,0xFF,0xFF,0x06,0x60,0x00,0x30,0x0C,0x30,0x0C,0x30,0x06,0x60,//
0x06,0x60,0x03,0xC0,0x01,0x80,0x03,0xC0,0x06,0x60,0x0C,0x30,0x38,0x1C,0xE0,0x07,//艾2

0x03,0x00,0x01,0x80,0x3F,0xFE,0x30,0x06,0x63,0x0C,0x03,0x00,0x03,0x00,0xFF,0xFF,//
0x06,0x30,0x0C,0x30,0x1C,0x60,0x07,0x60,0x01,0xC0,0x03,0x70,0x0E,0x18,0x78,0x0C,//安3

0x01,0x80,0x01,0x80,0x01,0x80,0x3F,0xFC,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x3F,0xFC,//
0x31,0x8C,0x31,0x8C,0x31,0x8C,0x3F,0xFC,0x31,0x8F,0x01,0x83,0x01,0x83,0x00,0xFF,//电4

0x00,0x00,0x7F,0xFC,0x00,0x18,0x00,0x30,0x00,0x60,0x01,0xC0,0x01,0x80,0xFF,0xFF,//
0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x07,0x80,0x03,0x00,//子5
};

uchar  dispaly[]={
/*--  文字:  李  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x01,0x00,0x7F,0xFC,0x03,0x80,0x05,0x40,0x09,0x30,0x31,0x0E,0xDF,0xE4,
0x00,0x80,0x01,0x00,0x7F,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00,

/*--  文字:  星  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x1F,0xF8,0x10,0x08,0x1F,0xF8,0x10,0x08,0x1F,0xF8,0x01,0x00,0x11,0x00,
0x1F,0xFC,0x21,0x00,0x21,0x00,0x4F,0xF8,0x01,0x00,0x01,0x00,0x7F,0xFE,0x00,0x00,

}; // 显

uchar  dispaly2[]=
{
0x00,0x00,0x1F,0xF8,0x10,0x08,0x1F,0xF8,0x10,0x08,0x1F,0xF8,0x01,0x00,0x11,0x00,
0x1F,0xFC,0x21,0x00,0x21,0x00,0x4F,0xF8,0x01,0x00,0x01,0x00,0x7F,0xFE,0x00,0x00,
};

uchar ao_yun[]=
{
/*--  文字:  油  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x40,0x20,0x40,0x20,0x40,0x00,0x40,0x97,0xFC,0x54,0x44,0x54,0x44,0x14,0x44,
0x24,0x44,0x27,0xFC,0xE4,0x44,0x24,0x44,0x24,0x44,0x24,0x44,0x27,0xFC,0x24,0x04,

/*--  文字:  加  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x00,0x08,0x00,0x08,0x00,0x7F,0x7C,0x09,0x44,0x09,0x44,0x09,0x44,0x09,0x44,
0x11,0x44,0x11,0x44,0x11,0x44,0x21,0x44,0x21,0x7C,0x45,0x44,0x82,0x40,0x00,0x00,

/*--  文字:  京  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x02,0x00,0x01,0x08,0x7F,0xFC,0x00,0x00,0x00,0x00,0x1F,0xF0,0x10,0x10,0x10,0x10,
0x1F,0xF0,0x01,0x00,0x09,0x30,0x19,0x18,0x31,0x0C,0x41,0x08,0x05,0x00,0x02,0x00,

/*--  文字:  北  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x44,0x04,0x4E,0x7C,0x58,0x04,0x60,0x04,0x40,
0x04,0x40,0x04,0x40,0x0C,0x40,0x34,0x42,0xE4,0x42,0x44,0x42,0x04,0x3E,0x00,0x00,

/*--  文字:  ，  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x00,0x00,

/*--  文字:  我  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x06,0x40,0x38,0x50,0x08,0x48,0x08,0x48,0x08,0x40,0xFF,0xFE,0x08,0x40,0x08,0x48,
0x0E,0x28,0x38,0x30,0xC8,0x20,0x08,0x50,0x09,0x92,0x08,0x0A,0x28,0x06,0x10,0x02,

/*--  文字:  有  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x01,0x00,0x7F,0xFE,0x02,0x00,0x04,0x00,0x0F,0xF0,0x18,0x10,0x28,0x10,
0x4F,0xF0,0x88,0x10,0x08,0x10,0x0F,0xF0,0x08,0x10,0x08,0x90,0x08,0x70,0x08,0x20,

/*--  文字:  运  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x40,0x00,0x23,0xFC,0x30,0x00,0x20,0x00,0x00,0x00,0x07,0xFE,0xE0,0x80,0x21,0x20,
0x21,0x10,0x22,0x08,0x24,0x7C,0x2F,0xCC,0x20,0x08,0x58,0x00,0x87,0xFE,0x00,0x00,

/*--  文字:  奥  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x02,0x00,0x3F,0xF8,0x21,0x08,0x29,0x28,0x25,0x48,0x3F,0xF8,0x23,0x88,0x25,0x68,
0x29,0x28,0x01,0x00,0x7F,0xFE,0x02,0x80,0x04,0x40,0x08,0x30,0x30,0x0E,0xC0,0x04,

};
uchar  hz0[]=			

{
/*--  文字:  欢  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/


0x00,0x80,0x00,0x80,0xFC,0x80,0x05,0xFE,0x85,0x04,0x4A,0x48,0x28,0x40,0x10,0x40,
0x18,0x40,0x18,0x60,0x24,0xA0,0x24,0x90,0x41,0x18,0x86,0x0E,0x38,0x04,0x00,0x00,
																			   
0x40,0x00,0x21,0x80,0x36,0x7C,0x24,0x44,0x04,0x44,0x04,0x44,0xE4,0x44,0x24,0x44,
0x25,0x44,0x26,0x54,0x24,0x48,0x20,0x40,0x20,0x40,0x50,0x00,0x8F,0xFE,0x00,0x00,

0x01,0x00,0x21,0x10,0x19,0x18,0x0D,0x10,0x09,0x20,0x01,0x04,0x7F,0xFE,0x04,0x40,
0x04,0x40,0x04,0x40,0x04,0x40,0x08,0x42,0x08,0x42,0x10,0x42,0x20,0x3E,0x40,0x00,

0x01,0x00,0x09,0x80,0x09,0x00,0x49,0xFE,0x4A,0x20,0x4A,0x10,0x4C,0x10,0x49,0x04,
0x49,0xFE,0x49,0x24,0x49,0x24,0x49,0x24,0x49,0x24,0x09,0xFC,0x09,0x04,0x00,0x00,

/*--  文字:  新  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x06,0x60,0x06,0x60,0xFF,0xFF,0x06,0x60,0x00,0x30,0x0C,0x30,0x0C,0x30,0x06,0x60,//
0x06,0x60,0x03,0xC0,0x01,0x80,0x03,0xC0,0x06,0x60,0x0C,0x30,0x38,0x1C,0xE0,0x07,//艾2

0x03,0x00,0x01,0x80,0x3F,0xFE,0x30,0x06,0x63,0x0C,0x03,0x00,0x03,0x00,0xFF,0xFF,//
0x06,0x30,0x0C,0x30,0x1C,0x60,0x07,0x60,0x01,0xC0,0x03,0x70,0x0E,0x18,0x78,0x0C,//安3

0x01,0x00,0x01,0x00,0x01,0x00,0x3F,0xF8,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x08,
0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x08,0x01,0x02,0x01,0x02,0x00,0xFE,0x00,0x00,

0x00,0x00,0x3F,0xF0,0x00,0x20,0x00,0x40,0x00,0x80,0x01,0x00,0x01,0x00,0x01,0x04,
0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00,

0x00,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,   //"！",
0x18,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x00,0x00,
      

      

/*--  文字:  专  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x01,0x00,0x3F,0xF8,0x02,0x00,0x02,0x00,0xFF,0xFE,0x04,0x00,0x08,0x20,
0x0F,0xF0,0x00,0x20,0x00,0x40,0x02,0x80,0x01,0x00,0x00,0x80,0x00,0x40,0x00,0x00,

/*--  文字:  业  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x44,0x44,0x46,0x24,0x4C,0x24,0x48,0x14,0x50,
0x1C,0x50,0x14,0x60,0x04,0x40,0x04,0x40,0x04,0x44,0xFF,0xFE,0x00,0x00,0x00,0x00,

/*--  文字:  单  --*/
 /*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x20,0x06,0x30,0x04,0x40,0x3F,0xF8,0x21,0x08,0x3F,0xF8,0x21,0x08,0x21,0x08,
0x3F,0xF8,0x21,0x08,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,

/*--  文字:  片  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x40,0x10,0x40,0x10,0x40,0x10,0x44,0x1F,0xFE,0x10,0x00,0x10,0x00,0x10,0x00,
0x1F,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x10,0x20,0x10,0x40,0x10,0x80,0x10,

/*--  文字:  机  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x10,0x00,0x10,0xF8,0x10,0x88,0xFE,0x88,0x10,0x88,0x10,0x88,0x38,0x88,0x34,0x88,
0x54,0x88,0x50,0x88,0x91,0x08,0x11,0x0A,0x12,0x0A,0x12,0x0A,0x14,0x06,0x10,0x00,

/*--  文字:  开  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x7F,0xFE,0x04,0x20,0x04,0x20,0x04,0x20,0x04,0x20,0xFF,0xFE,0x04,0x20,
0x04,0x20,0x04,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x20,0x20,0x40,0x20,0x00,0x00,

/*--  文字:  发  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x02,0x00,0x22,0x40,0x22,0x30,0x22,0x10,0x7F,0xFE,0x24,0x00,0x04,0x00,0x07,0xE0,
0x0C,0x20,0x0A,0x20,0x0A,0x20,0x11,0x40,0x20,0x80,0x41,0x60,0x86,0x1C,0x18,0x08,

/*--  文字:  全  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x01,0x00,0x02,0x80,0x04,0x40,0x08,0x30,0x10,0x1C,0x3F,0xF8,0x41,0x00,
0x01,0x00,0x01,0x10,0x1F,0xF8,0x01,0x00,0x01,0x00,0x01,0x04,0x7F,0xFE,0x00,0x00,

/*--  文字:  心  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x02,0x00,0x01,0x00,0x00,0x80,0x00,0xC0,0x08,0x80,0x08,0x00,0x08,0x10,0x48,0x08,
0x48,0x04,0x48,0x06,0x88,0x04,0x08,0x10,0x08,0x10,0x08,0x10,0x07,0xF0,0x00,0x00,

/*--  文字:  全  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x01,0x00,0x02,0x80,0x04,0x40,0x08,0x30,0x10,0x1C,0x3F,0xF8,0x41,0x00,
0x01,0x00,0x01,0x10,0x1F,0xF8,0x01,0x00,0x01,0x00,0x01,0x04,0x7F,0xFE,0x00,0x00,

/*--  文字:  意  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x3F,0xFC,0x08,0x20,0x04,0x40,0xFF,0xFE,0x00,0x00,0x1F,0xF0,0x12,0x10,
0x11,0x10,0x1F,0xF0,0x00,0x00,0x29,0x08,0x28,0x84,0x68,0x14,0x07,0xF0,0x00,0x00,

/*--  文字:  为  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x80,0x10,0x80,0x0C,0x80,0x04,0x84,0x7F,0xFE,0x01,0x04,0x01,0x04,0x01,0x84,
0x01,0x44,0x02,0x24,0x02,0x24,0x04,0x04,0x08,0x44,0x10,0x28,0x20,0x10,0x00,0x00,

/*--  文字:  您  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x00,0x09,0x00,0x11,0xFE,0x12,0x04,0x34,0x40,0x32,0x50,0x52,0x48,0x94,0x44,
0x11,0x44,0x10,0x80,0x00,0x00,0x29,0x04,0x28,0x92,0x68,0x12,0x07,0xF0,0x00,0x00,

/*--  文字:  服  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x3C,0xFC,0x24,0x84,0x24,0x84,0x24,0x94,0x3C,0x88,0x24,0x80,0x24,0xFC,0x3C,0xC4,
0x24,0xC4,0x24,0xA8,0x24,0xA8,0x24,0x90,0x44,0xA8,0x54,0xCE,0x88,0x84,0x00,0x00,

/*--  文字:  务  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x04,0x00,0x07,0xF0,0x0C,0x10,0x14,0x20,0x62,0x40,0x81,0x80,0x06,0x60,0x19,0x1E,
0x61,0x04,0x1F,0xF0,0x02,0x10,0x02,0x10,0x04,0x10,0x08,0x10,0x30,0x50,0xC0,0x20,


0x00,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,   //"！",
0x18,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  开  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x7F,0xFE,0x04,0x20,0x04,0x20,0x04,0x20,0x04,0x20,0xFF,0xFE,0x04,0x20,
0x04,0x20,0x04,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x20,0x20,0x40,0x20,0x00,0x00,

/*--  文字:  心  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x02,0x00,0x01,0x00,0x00,0x80,0x00,0xC0,0x08,0x80,0x08,0x00,0x08,0x10,0x48,0x08,
0x48,0x04,0x48,0x06,0x88,0x04,0x08,0x10,0x08,0x10,0x08,0x10,0x07,0xF0,0x00,0x00,

/*--  文字:  而  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x7F,0xFE,0x02,0x00,0x04,0x00,0x3F,0xFC,0x24,0x44,0x24,0x44,0x24,0x44,
0x24,0x44,0x24,0x44,0x24,0x44,0x24,0x44,0x24,0x44,0x24,0x54,0x24,0x08,0x00,0x00,

/*--  文字:  来  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x01,0x00,0x3F,0xFC,0x01,0x00,0x09,0x30,0x05,0x20,0x03,0x40,0x7F,0xFE,
0x01,0x80,0x03,0x40,0x05,0x20,0x09,0x18,0x31,0x0E,0xC1,0x04,0x01,0x00,0x01,0x00,

/*--  文字:  满  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x41,0x10,0x21,0x10,0x2F,0xFE,0x01,0x10,0x80,0x00,0x57,0xFE,0x51,0x20,0x17,0xFC,
0x25,0x24,0x25,0x24,0x25,0xB4,0xC6,0x4C,0x44,0x84,0x44,0x04,0x44,0x14,0x44,0x08,

/*--  文字:  意  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x01,0x00,0x3F,0xFC,0x08,0x20,0x04,0x40,0xFF,0xFE,0x00,0x00,0x1F,0xF0,0x12,0x10,
0x11,0x10,0x1F,0xF0,0x00,0x00,0x29,0x08,0x28,0x84,0x68,0x14,0x07,0xF0,0x00,0x00,

/*--  文字:  而  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0x7F,0xFE,0x02,0x00,0x04,0x00,0x3F,0xFC,0x24,0x44,0x24,0x44,0x24,0x44,
0x24,0x44,0x24,0x44,0x24,0x44,0x24,0x44,0x24,0x44,0x24,0x54,0x24,0x08,0x00,0x00,

/*--  文字:  归  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x08,0x00,0x08,0x00,0x0B,0xFC,0x48,0x04,0x48,0x04,0x48,0x04,0x48,0x04,0x49,0xFC,
0x48,0x04,0x48,0x04,0x48,0x04,0x08,0x04,0x10,0x04,0x13,0xFC,0x20,0x00,0x40,0x00,
};
/*********************************************************************************************************
** 函数名称: LEDriver_Init
** 功能描述: LED屏驱动初始化。
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
void LEDriver_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //开启相应的时钟

/******************旧版PCB************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
							RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);  
	//所有LED引脚配置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
*/   
//******************新版PCB************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
							RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);  
	//所有LED引脚配置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    Clr_CE01;       //打开74245使能端口。
    Clr_CE00;
    Clr_CE;
    Set_CE;
    
    GT23_Init();
    Flash_Init();
    DS1302_Init();
    LED_ONOFF(0);
}
/*********************************************************************************************************
** 函数名称: LE_ONOFF
** 功能描述: LED屏的开关，0关屏幕，1 打开屏幕。
** 输  　入: flag 0关屏幕，1 开屏幕。
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
void LED_ONOFF(INT8U flag)
{
    INT8U word[34]={0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20};
    if(flag != 0)
    {
        Clr_CE01;
        Clr_CE00;
    }
    else
    {
        Dis_Clr();
        Set_CE01;
        Set_CE00;
    }
}
#if LED_SCAN == 8
/*********************************************************************************************************
** 函数名称: Send_8Bit
** 功能描述: 用于发出8bit数据。
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
void Send_8Bit(INT8U led_data)
{
    INT8U i,j;
    for(i=0;i<8;i++)
    {
        if(flag_wei != 0)
        {
            if(led_data&0x80) 
            {
              Clr_DATH;
//            Clr_DATL;
            }
            else
            {
              Set_DATH;
//            Clr_DATL; 
            }
        }
        else
        {
            if(led_data&0x80) 
            {
//            Clr_DATH;
              Clr_DATL;
            }
            else
            {
//            Clr_DATH;
              Set_DATL; 
            }
        }
//         for(j=0;j<40;j++);               //延时的地方不同所显示的效果是不相同的。
         Set_SCK;
         led_data<<=1;
//         for(j=0;j<40;j++);
         Clr_SCK; 
    }
} 
#endif

#if LED_SCAN == 4 
/*********************************************************************************************************
** 函数名称: Send_8Bit
** 功能描述: 用于发出8bit数据。
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
void Send_8Bit(INT8U led_data)
{
    INT8U i,j;
    for(i=0;i<8;i++)
    {
        if(led_data&0x80)
        {
              Clr_DATL;
        }
        else
        {
              Set_DATL; 
        }
      //  for(j=0;j<30;j++);               //延时的地方不同所显示的效果是不相同的。
         Set_SCK;
         led_data<<=1;
       //  for(j=0;j<30;j++);
         Clr_SCK; 
    }
} 
#endif

/*********************************************************************************************************
** 函数名称: LED_OUT
** 功能描述: 将移位寄存器中数据显现出来。
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
void LED_OUT(void)
{
    unsigned int i;
    Clr_RCK;
    Set_RCK;
    Clr_RCK;
}
/*********************************************************************************************************
** 函数名称: HanScan
** 功能描述: 用于点阵的行扫描。
** 输  　入: num 行数，1/4 和 1/8 扫描方式num数值是有区别的。
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
#if LED_SCAN == 8
void HanScan(INT8U num)
{
    if(num<8)
    {
        flag_wei=0;
    }
    else
    {
        flag_wei=1;
        num=num-8;
    }
    switch (num)
    {
            case 0:
                Clr_A;
                Clr_B;
                Clr_C;
                break;
            case 1:
                Set_A;
                Clr_B;
                Clr_C;
                break;
            case 2:
                Clr_A;
                Set_B;
                Clr_C;
                break;
            case 3:
                Set_A;
                Set_B;
                Clr_C;
                break;
            case 4:
                Clr_A;
                Clr_B;
                Set_C;
                break;
            case 5:
                Set_A;
                Clr_B;
                Set_C;
                break;
            case 6:
                Clr_A;
                Set_B;
                Set_C;
                break;
            case 7:
                Set_A;
                Set_B;
                Set_C;
                break;
    }
}
#endif

#if LED_SCAN == 4
void HanScan(INT8U num)      //只要扫描四行就可以了。
{
    flag_wei=1;
    switch (num)
    {
            case 0:
                Clr_A;
                Clr_B;
                break;
            case 1:
                Set_A;
                Clr_B;
                break;
            case 2:
                Clr_A;
                Set_B;
                break;
            case 3:
                Set_A;
                Set_B;
                break;
    }
}
#endif

/*********************************************************************************************************
** 函数名称: Display
** 功能描述: 扫频显示，LED缓存中的数值。
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
#if LED_SCAN == 8
void DisPlay(void)
{
          INT8U i,j,k;
          for(i=0;i<16;i++)
          {
               HanScan(i);
               for(j=0;j<LED_Size;j++)
               {
                   Send_8Bit(LED_Buffer[i][j]);
               }
               LED_OUT();
               Clr_CE;           //注意时序的调整，否则会出现相应时钟错乱的情况。
               Delay_ms(40);
               Set_CE;
          }
}     
#endif

#if LED_SCAN == 4
void DisPlay(void)
{
          INT8U i,j,k;
          for(i=0;i<4;i++)
          {
               HanScan(i);
               for(j=0;j<2*LED_Size;j++)
               {

                  Send_8Bit(LED_Buffer[12+i][j]);
                  Send_8Bit(LED_Buffer[8+i][j]);
                  Send_8Bit(LED_Buffer[4+i][j]);
                  Send_8Bit(LED_Buffer[0+i][j]);
               }
                
               LED_OUT();
               Set_CE;            //注意时序的调整，否则会出现相应时钟错乱的情况。
               Delay_ms(50);
               Clr_CE;
          }
}     
#endif
/*********************************************************************************************************
** 函数名称: DE
** 功能描述: 用于点阵的行扫描。
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
void Delay_ms(unsigned int time)
{
    unsigned int i,j;
    for(i=0;i<time;i++)
    {
        for(j=0;j<50;j++);
    }
}

void Left_Dat(void)
{
    INT8U i,j;

    for(i=0;i<16;i++)
    {
        for(j=0;j<LED_Size*16+16;j++)
        {
          if(j<LED_Size*16+15)                                        //前几个移位
              LED_Cache[i][j]=(LED_Buffer[i][j]<<1)|((LED_Buffer[i][j+1]>>7)&0x01); 
          else
              LED_Cache[i][j]=(LED_Buffer[i][j]<<1) & 0xfe;
        }
    }
    memcpy(LED_Buffer, LED_Cache, sizeof(LED_Cache));      
}
void Up_Dat(void)
{
    INT8U i,j;

    for(i=0;i<32;i++)
    {
        for(j=0;j<LED_Size*2;j++)         //注意数字的操作不当会影响到串口的通信。要做好相应的工作。
        {
                LED_Cache[i][j]=LED_Buffer[i+1][j];
        }
    }
    memcpy(LED_Buffer, LED_Cache, sizeof(LED_Cache));      
}

void Dis_Up(unsigned char *str, unsigned char num)
{
    unsigned int i,j,k,l;
    unsigned char dis01,fenpin;
    dis01=0;
    k=0;
    l=0;
    j=0;
    if(num%LED_Size == 0)            //计算一下一个要分多少屏。
    {
        fenpin=num/LED_Size;
    }
    else
    {
        fenpin=num/LED_Size+1;
    }

    if(num<=LED_Size)
    {
        for(i=0;i<LED_Size;i++)
        {
           if(i<num)
           {
               for(j=0;j<32;j++)                    //相应的个数。
               {
                  LED_Buffer[j/2+16][j%2+2*i]=str[j+32*i];   //将数据放在缓存的地方。
               } 
           }
           else
           {
               for(j=0;j<32;j++)                    //相应的个数。
               {
                  LED_Buffer[j/2+16][j%2+2*i]=0x00;      //空余的地方就直接清零
               } 
           }
        }
    }
    else
    {
        for(i=0;i<LED_Size;i++)
        {
           for(j=0;j<32;j++)
           {
              LED_Buffer[j/2+16][j%2+2*i]=str[j+32*i]; 
           } 
        }
    }

    while(1)
    {
        k++;
        j++;
        if(k>20)
        {
            k=0;
            Up_Dat();
            l++;
            if(l>=31)   //注意全跑扫描的个数。   当为32时就会出现很多亮点。
            {
                l=0;
                dis01++;
                if(dis01<fenpin-1)
                {
                    for(i=0;i<LED_Size;i++)
                    {
                        for(j=0;j<32;j++)
                        {
                           LED_Buffer[j/2+16][j%2+2*i]=str[j+32*i+32*LED_Size*dis01]; 
                        } 
                    }
                 }
                 else if(dis01 == fenpin-1)
                 {
                    if(num%LED_Size !=0)
                    {
                        for(i=0;i<LED_Size;i++)
                        {
                            if(i<num%LED_Size)
                            {
                                for(j=0;j<32;j++)                    //相应的个数。
                                {
                                    LED_Buffer[j/2+16][j%2+2*i]=str[j+32*i+32*LED_Size*dis01];   //将数据放在缓存的地方。
                                } 
                             }
                             else
                             {
                                for(j=0;j<32;j++)                    //相应的个数。
                                {
                                    LED_Buffer[j/2+16][j%2+2*i]=0x00;      //空余的地方就直接清零
                                } 
                              }
                          }
                      }
                  }
                  else
                  {
                    dis01=0;
                    break;
                  }
                    
             }
        }
         if(j>20)
         {
            j=0;
            DisPlay();
         }
    }
    
} 

void Dis_left(unsigned char *str, unsigned char num)
{
    unsigned int i,j,k,l;
    unsigned char zishu;
    zishu=0;
    k=0;
    l=0;
    j=0;  //j作为调整LED的刷屏率。
    for(i=0;i<32;i++)
    {
        LED_Buffer[i/2][i%2+2*LED_Size]=str[i+32*0]; 
    } 
    while(1)
    {
        k++;
        j++;
        if(k>1000)
        {
            k=0;
            Left_Dat();
            l++;
            if(l>=16) //注意相关的参数。
            {
                l=0;
                zishu++;
                if(zishu >= num)
                {
                    for(i=0;i<32;i++)
                    {
                        LED_Buffer[i/2][i%2+2*LED_Size]=0x00; 
                    }
                    if(zishu >= num+4)
                    {
                        zishu=0;
                        break;
                    } 
                }
                else
                {
                    for(i=0;i<32;i++)     
                    {
                        LED_Buffer[i/2][i%2+2*LED_Size]=str[i+32*(zishu)]; 
                    } 
                }
            }

        }
         if(j>500)
         {
            j=0;
            DisPlay();
         }
           
    } 
}   

    
void Dis_Left_Mix(INT8U *str, INT8U num, INT8U speed)
{
    INT16U i,j,k,l,m,n;
    INT16U ucode;
    INT8U zishu, flag_ucode;      //记录显示的字数，记录字宽。
    INT8U word[32];              //作为字库点阵的临时缓存。
    INT16U speed01;
    switch(speed)
    {
        case LED_Speed_0: speed01 = 200; break;
        case LED_Speed_1: speed01 = 600; break;
        case LED_Speed_2: speed01 = 1000; break;
        case LED_Speed_3: speed01 = 1400; break;
        case LED_Speed_4: speed01 = 1800; break;
        case LED_Speed_5: speed01 = 2200; break;
        case LED_Speed_6: speed01 = 2600; break;
        case LED_Speed_7: speed01 = 3000; break;
        case LED_Speed_8: speed01 = 3400; break;
        case LED_Speed_9: speed01 = 3800; break;
    }
    flag_ucode =0;              //默认情况下8为字符，16为汉字
    zishu=0;
    k=0;
    l=0;
    n=0;
    j=0;  //j作为调整LED的刷屏率。
    ucode = ((INT16U)str[0*2]<<8 | str[0*2+1]);  //合成为unicode码。
    if(ucode >=0x20 && ucode<=0x7E)      //用于判断是字符还是汉字。
    {
        flag_ucode = 8;
        Read_Data(ZFindexASCII(ucode),16,word);
        for(i=0;i<16;i++)
        {
               if(i<2)
               {
                  LED_Buffer[i][2*LED_Size]=0x00; 
               }
               else
               {
                  LED_Buffer[i][2*LED_Size]=word[i-2]; 
               }
        }
    } 
    else
    {
        flag_ucode = 16;
        Read_Data(ZFindex16(ucode),32,word);
        for(i=0;i<32;i++)
        {
            LED_Buffer[i/2][i%2+2*LED_Size]=word[i]; 
        } 
    }
    while(1)
    {
        k++;
        if(k>speed01)
        {
            k=0;
          //  TIM_Cmd(TIM3, DISABLE);  //使能TIMx外设
            Left_Dat();
            l++;
            if(l>=flag_ucode) //注意相关的参数。
            {
                l=0;
                zishu++;
                if(zishu<num)
                {
                    ucode = ((INT16U)str[zishu*2]<<8 | str[zishu*2+1]);  //合成为unicode码。
                    if(ucode >=0x20 && ucode<=0x7E)      //用于判断是字符还是汉字。
                    {
                        flag_ucode = 8;
                        Read_Data(ZFindexASCII(ucode),16,word); // 主要ASII码和Unicode码的调用函数是用区别的，要注意区分。
                        for(i=0;i<16;i++)
                        {
                                if(i<2)
                                {
                                    LED_Buffer[i][2*LED_Size]=0x00; 
                                }
                                else
                                {
                                    LED_Buffer[i][2*LED_Size]=word[i-2]; 
                                }
                        }
                    } 
                    else
                    {
                        flag_ucode = 16;
                        Read_Data(ZFindex16(ucode),32,&word[0]);
                        for(i=0;i<32;i++)
                        {
                            LED_Buffer[i/2][i%2+2*LED_Size]=word[i]; 
                        } 
                    }
                }
                else
                {
                    zishu=0;                //显示完之后退出该函数。
                    break;
                }

            }
        }
  
    j++;
    if(j>200)           //显示相应的速度。
      {     
      j=0;
      DisPlay();
    } 
    }
}


void Dis_Up_Mix(INT8U *str, INT8U num,INT8U speed)
{
    INT16U i,j,k,l,m,n;
    INT16U ucode,speed01;
    INT8 los_zishu, los_space,flag_pause;//暂停标志；
    INT8U word[32];
    k=0;
    l=0;
    j=0;
    n=0;
    los_zishu = num;
    los_space = 2*LED_Size;

    switch(speed)
    {
        case LED_Speed_0: speed01 = 100; break;
        case LED_Speed_1: speed01 = 200; break;
        case LED_Speed_2: speed01 = 300; break;
        case LED_Speed_3: speed01 = 400; break;
        case LED_Speed_4: speed01 = 500; break;
        case LED_Speed_5: speed01 = 600; break;
        case LED_Speed_6: speed01 = 700; break;
        case LED_Speed_7: speed01 = 800; break;
        case LED_Speed_8: speed01 = 900; break;
        case LED_Speed_9: speed01 = 1000; break;
    }

    for(i=0;i<16;i++)
    {
       ucode = ((INT16U)str[i*2]<<8 | str[i*2+1]);  //合成为unicode码。
        if(ucode >=0x20 && ucode<=0x7E)      //用于判断是字符还是汉字。
        {
            if(los_space<=0)
            {
               break;
            }
            Read_Data(ZFindexASCII(ucode),16,word);
            for(j=0;j<16;j++)
            {
               if(j<2)
               {
                  LED_Buffer[j+16][LED_Size*2-los_space]=0x00; 
               }
               else
               {
                  LED_Buffer[j+16][LED_Size*2-los_space]=word[j-2]; 
                }
            }
            los_zishu--;
            los_space = los_space - 1;     //少八位。
            if(los_zishu<=0)
            {
               los_zishu=0;
                break;
            }
         } 
         else
         {
                if(los_space<=1)
                {
                    break;
                }
                Read_Data(ZFindex16(ucode),32,word);
                for(j=0;j<32;j++)
                {
                    LED_Buffer[j/2+16][j%2+(LED_Size*2-los_space)]=word[j]; 
                } 
                los_zishu--;
                los_space = los_space -2; //汉字是少16位
                if(los_zishu<=0)
                {
                    los_zishu=0;
                    break;
                }
             }
        }

        if(los_space>0)         //用于清除空白的地方。
        {
            for(i=0;i<los_space;i++)
            {
                for(j=0;j<16;j++)
                {
                    LED_Buffer[j+16][LED_Size*2-i-1]=0x00;
                }
            }
            los_space=0;            
        }               
    flag_pause=1;
    while(1)
    {
        k++;
        m++;
        
        if(k>speed01)
        {
            k=0;
            if(flag_pause != 1)
            {
            Up_Dat();
            l++;
            }
            else
            {
                n++;
                if(n>20)
                {
                    n=0;
                    flag_pause=0; 
                }
            }
            if(l>=16)   //注意全跑扫描的个数。   当为32时就会出现很多亮点。
            {
                l=0;
                flag_pause=1;
                if(los_zishu>0)
                {
                    los_space = LED_Size *2;
                    for(i=0;i<16;i++)
                    {
                        ucode = ((INT16U)str[(num-los_zishu)*2]<<8 | str[(num-los_zishu)*2+1]);  //合成为unicode码。
                        if(ucode >=0x20 && ucode<=0x7E)      //用于判断是字符还是汉字。
                        {
                            if(los_space<=0)
                            {
                                break;
                            }
                            Read_Data(ZFindexASCII(ucode),16,word);
                            for(j=0;j<16;j++)
                            {
                                if(j<2)
                                {
                                    LED_Buffer[j+16][LED_Size*2-los_space]=0x00; 
                                }
                                else
                                {
                                    LED_Buffer[j+16][LED_Size*2-los_space]=word[j-2]; 
                                }
                            }
                            los_zishu--;
                            los_space = los_space - 1;     //少八位。
                            if(los_zishu<=0)
                            {
                                los_zishu=0;
                                break;
                            }
                        } 
                        else
                        {
                            if(los_space<=1)
                            {
                                break;
                            }
                            Read_Data(ZFindex16(ucode),32,word);
                            for(j=0;j<32;j++)
                            {
                                LED_Buffer[j/2+16][j%2+(LED_Size*2-los_space)]=word[j]; 
                            } 
                            los_zishu--;
                            los_space = los_space -2; //汉字是少16位
                            if(los_zishu<=0)
                            {
                                los_zishu=0;
                                break;
                            }
                         }
                    }                                
                    if(los_space>0)         //用于清除空白的地方。
                    {
                        for(i=0;i<los_space;i++)
                        {
                            for(j=0;j<16;j++)
                            {
                                LED_Buffer[j+16][LED_Size*2-i-1]=0x00;   //用于清除多余的部分，注意-1所对应空间位置。
                            }
                        }         
                    }
                }
                else
                {
                    break;
                }                    
             }
        }        
         if(m>20)
         {
            m=0;
            DisPlay();
         }
    }    
} 

void Dis_Time(INT8U flag)
{
    INT8U ucode,i,j;
    INT8U num[12]={'0','1','2','3','4','5','6','7','8','9','-',':' };
    INT8U week[16]={0x00,0x00,0x4e,0x00,0x4e,0x8c,0x4e,0x09,0x56,0xdb,0x4e,0x94,0x51,0x6d,0x65,0xE5};
    INT8U  time[20];                      //用于存储从DS1302中提取的数据。
    INT8U  Date[46]={0x00,0x20,0x00,'2', 0x00,'0', 0x00,'0', 0x00,'0',0x5e,0x74,0x00,'0',0x00,'0',0x67,0x08,0x00,'0',0x00,'0',0x65,0xe5,0x00,0x20,      //2000年00月00日
                   0x00,0x20,0x66,0x1f,0x67,0x1f,0x4e,0x00,0x00,'0',0x00,'0',0x65,0xf6,0x00,'0',0x00,'0',0x52,0x06};  //时间的现实方式。

    systemtime realtime;
    INT8U word[32];
    for(i=0;i<16;i++)
    {
        for(j=0;j<16;j++)
        {
            LED_Buffer[i][j]=0x00;
        }
    }
    while(1)
    {
        gettime(&realtime,&time[0]);
        if(flag==0)     //时间的显示方式。
        {
            for(j=0;j<8;j++)
            {
               ucode = num[time[j+2]];  //合成为unicode码。
               Read_Data(ZFindex7x8(ucode),8,word);  
               for(i=0;i<8;i++)
               {
                  LED_Buffer[i][j]=word[i];      
               }
            } 
            for(j=0;j<8;j++)
            {
               ucode = num[time[j+10]];  //合成为unicode码。
               Read_Data(ZFindex7x8(ucode),8,word);  
               for(i=0;i<8;i++)
               {
                  LED_Buffer[i+8][j]=word[i];      
               }
            }
            DisPlay(); 
            break; 
        }
        else
        {
                 Date[3*2+1]=num[time[2]];           //年
                 Date[4*2+1]=num[time[3]];

                 Date[6*2+1]=num[time[5]];          //月
                 Date[7*2+1]=num[time[6]];

                 Date[9*2+1]=num[time[8]];            //日
                 Date[10*2+1]=num[time[9]];

                 Date[16*2]=week[time[18]*2];        //星期
                 Date[16*2+1]=week[time[18]*2+1];

                 Date[17*2+1]=num[time[10]];         //时
                 Date[18*2+1]=num[time[11]];

                 Date[20*2+1]=num[time[13]];         //分
                 Date[21*2+1]=num[time[14]];


                 Dis_Up_Mix(Date,23,LED_Speed_5);
                 break;
         }
       
       
    }
       
}

void Dis_LED_Way(INT8U *str,INT8U num,INT8U speed,INT8U flag)
{
    INT8U i;
    INT8U word[34]={0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20};
    switch(flag)
    {
        case 0x00: break;
        case 0x01: Dis_Left_Mix(&str[0],num,speed);
                   for(i=0;i<16;i++)                    //用于显示结尾。显示空白
                   {
                     Dis_Left_Mix(word,1,speed); 
                   }
                   break;
        case 0x02: Dis_Up_Mix(&str[0],num,speed);
                   Dis_Up_Mix(word,16,speed);
                   break;

        case 0x03: Dis_Time(0);      break;
        case 0x04: Dis_Time(1);
                   Dis_Up_Mix(word,16,4);
                   break;
    }
}

void Dis_Clr()
{
    INT16U i,j;
    for(i=0;i<16;i++)
    {
        for(j=0;j<LED_Size*2;j++)
        {
             LED_Buffer[i][j]=0x00;
        }
    }

     DisPlay();

}
void Send_Test()
{
        INT8U i,j,k;
        INT8U word[64]={0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20};
        INT8U word1[]={0x00,0x28,0x00,0x30,0x00,0X3C,0x4E,0x0A,0x6D,0x77,0x82,0x7E,0x5B,0x89,0x75,0x35,0x5B,0x50,0x00,0x28,0x00,0x30,0x00,0X3C,0x4E,0x0A,0x6D,0x77,0x82,0x7E,0x5B,0x89,0x75,0x35,0x5B,0x50,0x00,0x28,0x00,0x30,0x00,0X3C,0x4E,0x0A,0x6D,0x77,0x82,0x7E,0x5B,0x89,0x75,0x35,0x5B,0x50,0x00,0x28,0x00,0x30,0x00,0X3C,0x4E,0x0A,0x6D,0x77,0x82,0x7E,0x5B,0x89,0x75,0x35,0x5B,0x50};
        systemtime realtime;
		
		LED_ONOFF(1);
        for(i=0;i<32;i++)
        {
           LED_Buffer[i/2][i%2]=TEACHER[i]; 
      // LED_Buffer[i/2][i%2+6*2]=TEACHER[i]; 
        }

        for(i=0;i<32;i++)
        {
           LED_Buffer[i/2][i%2+2]=TEACHER[i+32]; 
        //   LED_Buffer[i/2][i%2+2*7]=TEACHER[i+32]; 
        } 
        while(1)
        {
         //   Dis_Time(0);
        //  Dis_left(hz0,33);
           // gettime(&realtime);
        //   Dis_Time(&word[0]);
     //      Dis_Left_Mix(word1,9,LED_Speed_5);
       //    Dis_Up_Mix(word1,30,LED_Speed_5);
          DisPlay();
        //  Delay_ms(400);
       //    Dis_Left_Mix(word,8);
       //    Dis_Left_Mix(word,8);
        }
}