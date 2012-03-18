#include "LedDriver.h"
u8t LedBuffer[LED_COLS/FONT_WIDTH][FONT_LENGTH];
u8t LedNextCol[FONT_LENGTH];
u8t LedNextRow[LED_COLS/FONT_WIDTH][FONT_LENGTH];

void LedInit(void)
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
    
    GT32_Init();
    Flash_Init();
    DS1302_Init();
    LED_ONOFF(0);
}

void LED_ONOFF(u8t flag)
{
    //INT8U word[34]={0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20,0X00,0X20};
    if(flag != 0)
{
        Clr_CE01;
        Clr_CE00;
    }
    else
    {
        Dis_ClearScreen();
        Set_CE01;
        Set_CE00;
    }
}

void WriteByte(u8t bytes,u8t row)
{
	u8t i;
	for(i=0;i<8;i++)
	{
		if(bytes&0x80)
		{
			if(row<8)
			{
				Clr_DATL;
			}
			else
			{
				Clr_DATH;
			}
		}
		else
		{
			if(row<8)
			{
				Set_DATL;
			}
			else
			{
				Set_DATH;
			}
		}
		/*
		if(row>8)
		{
			if(bytes&0x80)
			{
				Clr_DATH;
			}
			else
			{
				Set_DATH;
			}
		}
		else
		{
			if(bytes&0x80)
			{
				Clr_DATL;
			}
			else
			{
				Set_DATL;
			}
		}
		*/
		Set_SCK;
		Delay_ms(1);
		bytes<<=1;
		Clr_SCK;
	}
}

void WriteDoubleRowByte(u8t upperByte,u8t lowerByte,u8t row)
{
	u8t i;
	for(i=0;i<8;i++)
	{
		if(upperByte&0x80)
		{
			Clr_DATL;
		}
		else
		{
			Set_DATL;
		}
		if(lowerByte&0x80)
		{
			Clr_DATH;
		}
		else
		{
			Set_DATH;
		}
		Set_SCK;
		Delay_ms(1);
		upperByte<<=1;
		lowerByte<<=1;
		Clr_SCK;
	}
}

void DisplayInRow(void)
{
	Clr_RCK;
    Set_RCK;
    Clr_RCK;
}

void SelectOneRow(u8t row)
{
	row%=8;
	switch (row)
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

void LedDisplay(u8t *textStr,u8t strLength,u8t frequence,u8t runStyle)
{
	switch(runStyle)
	{
		case STYLE_RIGHT_TO_LEFT:
			Dis_RightToLeft(textStr,strLength);
			break;
		case STYLE_BOTTOM_TO_TOP:
			Dis_BottomToTop(textStr,strLength);
			break;
		default:
			break;
	}
}

void Dis_Static(u8t *textStr,u8t strLength,u32t fontSize)
{
	u32t i=0;
	//u8t j=0;
	u8t k=0;
	//u8t row=0;
	u16t ucode=0;
	u8t words=0;
	u8t fontWidth;
	u8t fontHeight;
	u8t fontLength;
	u16t row=0;
	u16t col=0;
	u16t ledWordsNum;
	u16t colWordsNum;
	u16t rowWordsNum;
	//u8t tmp[32]={0x1F,0xF0,0x10,0x10,0x10,0x10,0x1f,0xf0,0x10,0x10,0x10,0x10,0x1f,0xf0,0x00,0x00,0xff,0xfe,0x01,0x00,0x11,0x00,0x11,0xf8,0x11,0x00,0x29,0x00,0x45,0x00,0x83,0xfe};
	/*
	for(i=0;i<4;i++)
	{
		for(k=0;k<32;k++)
		{
			LedBuffer[i][k]=0xff;
		}
			
	}
	*/
	fontWidth=GET_FONT_WIDTH(FONT_SIZE_15X16);
	fontHeight=GET_FONT_HEIGHT(FONT_SIZE_15X16);
	fontLength=fontWidth*fontHeight/sizeof(u8t);
	ledWordsNum=(LED_COLS/fontWidth)*(LED_ROWS/fontHeight);
	colWordsNum=LED_COLS/fontWidth;
	rowWordsNum=LED_ROWS/fontHeight;
	
	Dis_ClearScreen();
	for(k=0;k<strLength;k++)
	{
		
		if(textStr[k]>0x20&&textStr[k]<=0x7e)
		{
			ucode=(textStr[k]<<8)|textStr[k+1];
			 //Char2Lattice(ucode, LedBuffer[words], FONT_SIZE_8X16_ASCII);
			++k;		
		}
		else
		{	
			ucode=(textStr[k]<<8)|textStr[k+1];
			//Char2Lattice(u16t ucode, LedBuffer[words], FONT_SIZE_15X16)
			++k;
		}
		while(GT32_Test()!=DEF_TRUE)
		{
		}
		if((words%colWordsNum)==0&&(words!=0))
		{
			++row;
		}
		Char2Lattice(ucode, &LedBuffer[words%colWordsNum/2][words%colWordsNum%2+row*16], fontSize);
		//Char2Lattice(ucode, LedBuffer[words], fontSize);
		++words;
		//Dis_ClearScreen();
	}
	//memcpy(LedBuffer[1],tmp,sizeof(tmp));
	for(i=0;i<255555;i++)
	{
		Display();
	}
}

void Dis_Verbatim(u8t *textStr,u8t strLength)
{
	u8t i=0;
	//u8t j=0;
	u8t k=0;
	//u8t row=0;
	u16t ucode=0;
	u8t words=0;
	/*
	for(i=0;i<4;i++)
	{
		for(k=0;k<32;k++)
		{
			LedBuffer[i][k]=0xff;
		}
			
	}
	*/
	Dis_ClearScreen();
	for(k=0;k<strLength;k++)
	{
		
		if(textStr[k]>0x20&&textStr[k]<=0x7e)
		{
			ucode=(textStr[k]<<8)|textStr[k+1];
			 //Char2Lattice(ucode, LedBuffer[words], FONT_SIZE_8X16_ASCII);
			++k;		
		}
		else
		{	
			ucode=(textStr[k]<<8)|textStr[k+1];
			//Char2Lattice(u16t ucode, LedBuffer[words], FONT_SIZE_15X16)
			++k;
		}
		while(GT32_Test()!=DEF_TRUE)
		{
		}
		Char2Lattice(ucode, LedBuffer[words], FONT_SIZE_15X16);
		++words;
		//Dis_ClearScreen();
		for(i=0;i<255;i++)
		{
			Display();
		}
	}
	
}


void Dis_RightToLeft(u8t *textStr,u8t strLength)
{
	u8t i;
	u8t j;
	u8t k;
	u8t n;
	u8t col;
	u16t ucode;
	u8t words;
	u8t testLattice[32]={0x1F,0xF0,0x10,0x10,0x10,0x10,0x1f,0xf0,0x10,0x10,0x10,0x10,0x1f,0xf0,0x00,0x00,0xff,0xfe,0x01,0x00,0x11,0x00,0x11,0xf8,0x11,0x00,0x29,0x00,0x45,0x00,0x83,0xfe};
	Dis_ClearScreen();
	//memset(LedNextCol,0,sizeof(LedNextCol));
	//ucode=(textStr[0]<<8)|textStr[1];
	//Char2Lattice(ucode,LedBuffer[3],FONT_SIZE_15X16);
	//Char2Lattice((textStr[0]<<8)|textStr[1],LedBuffer[0],FONT_SIZE_15X16);
	//Char2Lattice((textStr[2]<<8)|textStr[3],LedBuffer[1],FONT_SIZE_15X16);
	//Char2Lattice((textStr[4]<<8)|textStr[5],LedBuffer[2],FONT_SIZE_15X16);
	//Char2Lattice((textStr[6]<<8)|textStr[7],LedBuffer[3],FONT_SIZE_15X16);
	for(k=0;k<strLength;k++)
	{
		
		if(textStr[k]>0x20&&textStr[k]<=0x7e)
		{
			ucode=(textStr[k]<<8)|textStr[k+1];
			 //Char2Lattice(ucode, LedBuffer[words], FONT_SIZE_8X16_ASCII);
			++k;		
		}
		else
		{	
			ucode=(textStr[k]<<8)|textStr[k+1];
			//Char2Lattice(u16t ucode, LedBuffer[words], FONT_SIZE_15X16)
			++k;
		}
		++words;
		//Char2Lattice(ucode,LedNextCol,FONT_SIZE_15X16);
		//Char2Lattice(ucode,LedBuffer[0],FONT_SIZE_15X16);
		while(GT32_Test()!=DEF_TRUE)
		{
			//Delay_ms(100);
		}
		Char2Lattice(ucode,LedBuffer[(LED_COLS/FONT_WIDTH)],FONT_SIZE_15X16);
		//Char2Lattice(ucode,LedNextCol,FONT_SIZE_15X16);
		//memcpy(LedNextCol,testLattice,sizeof(LedNextCol));
		
		for(col=0;col<FONT_WIDTH;col++)
		{	
			for(i=0;i<(LED_COLS/FONT_WIDTH);i++)
			{
				for(j=0;j<FONT_HEIGHT;j++)
				{
					LedBuffer[i][2*j]=(LedBuffer[i][2*j]<<1)|(LedBuffer[i][2*j+1]>>7);
					
					if(i!=((LED_COLS/FONT_WIDTH)-1))
					{
						LedBuffer[i][2*j+1]=(LedBuffer[i][2*j+1]<<1)|(LedBuffer[i+1][2*j]>>7);
					}
					else
					{
						//LedBuffer[i][2*j+1]=LedBuffer[i][2*j+1]<<1;
						LedBuffer[i][2*j+1]=(LedBuffer[i][2*j+1]<<1)|(LedNextCol[2*j]>>7);
						LedNextCol[2*j]=(LedNextCol[2*j]<<1)|(LedNextCol[2*j+1]>>7);
						LedNextCol[2*j+1]=LedNextCol[2*j+1]<<1;
					}
				}
			} 
			for(n=0;n<50;n++)
			{
				Display();
			}
		}
	}
	memset(LedBuffer[(LED_COLS/FONT_WIDTH)],0,sizeof(LedBuffer[(LED_COLS/FONT_WIDTH)]));
	for(k=0;k<(LED_COLS/FONT_WIDTH);k++)
	{
		//memcpy(LedBuffer[4],testLattice,sizeof(testLattice));
		//memset(LedBuffer[4],0x00,sizeof(testLattice));
		for(col=0;col<FONT_WIDTH;col++)
		{	
			for(i=0;i<(LED_COLS/FONT_WIDTH)+1;i++)
			{
				for(j=0;j<FONT_HEIGHT;j++)
				{
					LedBuffer[i][2*j]=(LedBuffer[i][2*j]<<1)|(LedBuffer[i][2*j+1]>>7);
					
					if(i!=(LED_COLS/FONT_WIDTH))
					{
						LedBuffer[i][2*j+1]=(LedBuffer[i][2*j+1]<<1)|(LedBuffer[i+1][2*j]>>7);
					}
					else
					{
						LedBuffer[i][2*j+1]=LedBuffer[i][2*j+1]<<1;
					}
				}
			} 
			for(n=0;n<50;n++)
			{
				Display();
			}
		}
	}
	
}

void Dis_BottomToTop(u8t *textStr,u8t strLength)
{
	u8t i=0;
	u8t j=0;
	u8t k=0;
	u8t n=0;
	u8t words=0;
	u16t ucode;
	Dis_ClearScreen();
	for(k=0;k<strLength;k++)
	{
		if(textStr[k]>0x20&&textStr[k]<=0x7e)
		{
			ucode=(textStr[k]<<8)|textStr[k+1];
			 //Char2Lattice(ucode, LedBuffer[words], FONT_SIZE_8X16_ASCII);
			++k;		
		}
		else
		{	
			ucode=(textStr[k]<<8)|textStr[k+1];
			//Char2Lattice(u16t ucode, LedBuffer[words], FONT_SIZE_15X16)
			++k;
		}
		++words;
		while(GT32_Test()!=DEF_TRUE)
		{
			//Delay_ms(100);
		}
		Char2Lattice(ucode,LedNextRow[(words-1)%(LED_COLS/FONT_WIDTH)],FONT_SIZE_15X16);
		if(((words%(LED_COLS/FONT_WIDTH))!=0)&&(k<strLength-1))
		{
			continue;
		}
		if((words%(LED_COLS/FONT_WIDTH))!=0)
		{
			for(i=(words%(LED_COLS/FONT_WIDTH));i<(LED_COLS/FONT_WIDTH);i++)
			{
				memset(LedNextRow[i],0,sizeof(LedNextRow[i]));
			}
		}
		words=0;
		for(i=0;i<LED_ROWS;i++)
		{
			for(j=0;j<(LED_COLS/FONT_WIDTH);j++)
			{
				memcpy(&LedBuffer[j][0],&LedBuffer[j][2],30);
				memcpy(&LedBuffer[j][30],&LedNextRow[j][0],2);
				memcpy(&LedNextRow[j][0],&LedNextRow[j][2],30);
			}
			for(n=0;n<50;n++)
			{	
				Display();
			}
		}
	}
	for(i=0;i<(LED_COLS/FONT_WIDTH);i++)
	{
		memset(LedNextRow[i],0,sizeof(LedNextRow[i]));
	}
	for(i=0;i<LED_ROWS;i++)
	{
		for(j=0;j<(LED_COLS/FONT_WIDTH);j++)
		{
			memcpy(&LedBuffer[j][0],&LedBuffer[j][2],30);
			memcpy(&LedBuffer[j][30],&LedNextRow[j][0],2);
			memcpy(&LedNextRow[j][0],&LedNextRow[j][2],30);
		}
		for(n=0;n<50;n++)
		{	
			Display();
		}
	}
}
void Dis_ClearScreen(void)
{
	//u16t i,j;
    memset(LedBuffer,0,sizeof(LedBuffer));
	Display();
}

void Display(void)
{
	u8t i;
	u8t j;
	for(i=0;i<8;i++)
	{
		SelectOneRow(i);
		for(j=0;j<8;j++)
		{
			//WriteByte(LedBuffer[j][i*2],i);
			//WriteByte(LedBuffer[j][i*2+1],i);
			WriteDoubleRowByte(LedBuffer[j][i*2],LedBuffer[j][(i+8)*2],i);
			WriteDoubleRowByte(LedBuffer[j][i*2+1],LedBuffer[j][(i+8)*2+1],i);

			
		}
	
		//for(j=0;j<4;j++)
		//{
		//	WriteByte(LedBuffer[j][(i+8)*2],i+8);
		//	WriteByte(LedBuffer[j][(i+8)*2+1],i+8);
		//}
		
		DisplayInRow();
		Clr_CE;
		Delay_ms(40);
		Set_CE;
	}
}

void Dis_Test(void)
{
	s8t i;
	s8t j;
	s8t n;
	s8t k;
	s8t dat;
	Dis_ClearScreen();
	for(i=3;i>=0;i--)
	{
		dat=0x01;
		for(k=0;k<8;k++)
		{
			memset(LedBuffer,0,sizeof(LedBuffer));
			for(j=0;j<16;j++)
			{
				LedBuffer[i][2*j+1]=dat;
			}
			for(n=0;n<50;n++)
			{	
				Display();
			}
			dat<<=1;
		}
		dat=0x01;
		for(k=0;k<8;k++)
		{
			memset(LedBuffer,0,sizeof(LedBuffer));
			for(j=0;j<16;j++)
			{
				LedBuffer[i][2*j]=dat;				
			}
			for(n=0;n<50;n++)
			{	
				Display();
			}
			dat<<=1;
		}
	}
	
}
void Delay_ms(u32t time)
{
    u32t i,j;
    for(i=0;i<time;i++)
    {
        for(j=0;j<50;j++);
    }
}


