#include "Broadcast.h"

void Broadcast(void)
{
	u8t adItemData[300];
	u16t nextOrder=0;
	//u16t adNum;
	boolean newPlayList;
	//u8t str[4];
	//BroadParamStruct *pBroadParam;
	while(DEF_TRUE)
	{
		/*
		for(;(ReadNextAd(u8t * adData,&nextOrder)==-1)&&!newPlayList;nextOrder%adNum)
		{
		}
		*/
		if(newPlayList)
		{
			nextOrder=0;
			//Read_Flash(SEC_PARAM_OFFSET,sizeof(str), str);
			//pBroadParam=(BroadParamStruct *)str;
			//pBroadParam->normalAd=0;
			//Write_Flash(SEC_PARAM_OFFSET,sizeof(str),str);
		}
		//nextOrder%=adNum;
		if(ReadNextAd(adItemData,&nextOrder))
		{
			PlayAd(adItemData);
		}
	}
}

u8t ReadNextAd(u8t * adItemData,u16t *pOrder)
{
	u8t str[4];
	BroadParamStruct *pBroadParam;
	Read_Flash(SEC_PARAM_OFFSET,sizeof(str), str);
	pBroadParam=(BroadParamStruct *)str;
	if(pBroadParam->urgentAd)
	{
		ReadUrgent(adItemData);
		pBroadParam->urgentAd=DEF_FALSE;
	}
	else
	{
		if(pBroadParam->breakAd)
		{
			ReadBreak(adItemData);
			pBroadParam->breakAd=DEF_FALSE;
		}
		else
		{
			if(*pOrder<pBroadParam->normalAd)
			{
				ReadNormal(adItemData,*pOrder);
				++(*pOrder);
			}
			else
			{
				*pOrder=0;
				return DEF_FALSE;
			}
		}
	}
	Write_Flash(SEC_PARAM_OFFSET,sizeof(str),str);
	return DEF_TRUE;
}

boolean ReadUrgent(u8t *urgentAd)
{
	Read_Flash(SEC_URGENT_OFFSET,MAX_AD_LENGTH,urgentAd);
	return DEF_TRUE;
}


boolean ReadBreak(u8t *breakAd)
{
	Read_Flash(SEC_BREAK_OFFSET,MAX_AD_LENGTH,breakAd);
	return DEF_TRUE;	
}

boolean ReadNormal(u8t * normalAd,u8t order)
{
	Read_Flash(SEC_NORMAL_OFFSET+order,MAX_AD_LENGTH,normalAd);
	return DEF_TRUE;	
}

void PlayAd(u8t *adItemData)		//(u8t *adContent, u8t runSytle, u8t times, u8t lightness)
{
	//分析广告数据
	AdStruct *pAdStruct=(AdStruct *)adItemData;
	u32t stTime=0;
	u32t enTime=0;
			
	stTime=(pAdStruct->startTime[0]<<24)|(pAdStruct->startTime[1]<<16)|(pAdStruct->startTime[2]<<8)|(pAdStruct->startTime[3]);
	enTime=(pAdStruct->endTime[0]<<24)|(pAdStruct->endTime[1]<<16)|(pAdStruct->endTime[2]<<8)|(pAdStruct->endTime[3]);
	
	if(IsInTime(&stTime,&enTime))
	{
		LedDisplay(pAdStruct->adContent,pAdStruct->textLen[0]<<8|pAdStruct->textLen[1],0,pAdStruct->runSytle);
	}

	/*
	t_startTime->tm_mday=currentTime.tm_mday;
	t_startTime->tm_mon=currentTime.tm_mon;
	t_startTime->tm_year=currentTime.tm_year;
	t_startTime->tm_wday=currentTime.tm_wday;
	t_startTime->tm_yday=currentTime.tm_yday;
	t_startTime->tm_isdst=currentTime.tm_isdst;

	t_endTime->tm_mday=currentTime.tm_mday;
	t_endTime->tm_mon=currentTime.tm_mon;
	t_endTime->tm_year=currentTime.tm_year;
	t_endTime->tm_wday=currentTime.tm_wday;
	t_endTime->tm_yday=currentTime.tm_yday;
	t_endTime->tm_isdst=currentTime.tm_isdst;

	nowTime=mktime(&currentTime);
	sTime=mktime(t_startTime);
	eTime=mktime(t_endTime);
	
	if((nowTime>=sTime)&&(nowTime<=eTime))
	{
		LedDisplay(pAdStruct->adContent,pAdStruct->textLen[0]<<8|pAdStruct->textLen[1],0,pAdStruct->runSytle);
	}
	*/
}

void InsertAd(u8t *pData,u8t adType)
{
	u8t str[4];
	u16t textLen;
	BroadParamStruct *pBroadParam;
	FrameStruct *pFrameStruct=(FrameStruct *)pData;
	textLen=(pFrameStruct->dataLength[0]<<8|pFrameStruct->dataLength[1])-18;	
	Read_Flash(SEC_PARAM_OFFSET,sizeof(str), str);
	pBroadParam=(BroadParamStruct *)str;
	pFrameStruct->dataLength[0]=(textLen>>8)&0x00ff;
	pFrameStruct->dataLength[1]=textLen&0x00ff;	
	if(adType==PROTOCOL_URGENT_AD)	//紧急广告
	{
		pBroadParam->urgentAd|=DEF_TRUE;
		Write_Flash(SEC_URGENT_OFFSET,textLen+12,&(pFrameStruct->dataLength[0]));
	}
	else 
	{
		if(adType==PROTOCOL_BREAK_AD)	//插播广告
		{
			pBroadParam->breakAd|=DEF_TRUE;
			Write_Flash(SEC_BREAK_OFFSET,textLen+12,&(pFrameStruct->dataLength[0]));
			
			Write_Flash(SEC_NORMAL_OFFSET+pBroadParam->normalAd,textLen+12,&(pFrameStruct->dataLength[0]));
			pBroadParam->normalAd++;
			Write_Flash(SEC_PARAM_OFFSET,sizeof(str),str);
		}
		else	//普通广告
		{
			Write_Flash(SEC_NORMAL_OFFSET+pBroadParam->normalAd,textLen+12,&(pFrameStruct->dataLength[0]));
			pBroadParam->normalAd++;
			Write_Flash(SEC_PARAM_OFFSET,sizeof(str),str);
		}
	}
}

void AnalysisData(u8t *pData)
{
	FrameStruct *pFrameStruct=(FrameStruct *)pData;
	switch(pFrameStruct->protocolID)
	{
		case PROTOCOL_NORMAL_AD:
			InsertAd(pData,PROTOCOL_NORMAL_AD);
			break;
		case PROTOCOL_URGENT_AD:
			InsertAd(pData,PROTOCOL_URGENT_AD);			
			break;
		case PROTOCOL_BREAK_AD:
			InsertAd(pData,PROTOCOL_BREAK_AD);			
			break;
		case PROTOCOL_DEL_ALL:
			break;
		default:
			break;
	}
}

void DelAllAd(void)
{
	BroadParamStruct broadParam;
	broadParam.urgentAd=DEF_FALSE;
	broadParam.breakAd=DEF_FALSE;
	broadParam.normalAd=0;
	broadParam.reserve=0xd9;
	Write_Flash(SEC_PARAM_OFFSET,sizeof(BroadParamStruct),(u8t *)(&broadParam));
}

void Broadcast_Init(void)
{
	u8t str[4];
	u8t i;
	BroadParamStruct *pBroadParam;	
	Read_Flash(SEC_PARAM_OFFSET,sizeof(str), str);
	pBroadParam=(BroadParamStruct *)str;
	if(pBroadParam->reserve!=0xd9)
	{
		pBroadParam->urgentAd=DEF_FALSE;
		pBroadParam->breakAd=DEF_FALSE;
		pBroadParam->normalAd=0;
		pBroadParam->reserve=0xd9;
	}	
	Write_Flash(SEC_PARAM_OFFSET,sizeof(str),str);
	//for(i=0;i<4;i++)
	//{
	//	str[i]=0;
	//}	
	//Read_Flash(SEC_PARAM_OFFSET,sizeof(str), str);
	//i=5;
}