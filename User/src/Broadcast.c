#include "Broadcast.h"

void Broadcast(void)
{
	u16t nextOrder;
	u16t adNum;
	boolean newPlayList;
	nextOrder=0;
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
			adNum=GetAdNum();
		}
		nextOrder%=adNum;
		if(ReadNextAd(u8t * adData,&nextOrder)!=-1)
		{
			PlayAd(u8t * adStr, u8t appearMode, u8t times, u8t lightness);
		}
	}
}

u8t ReadNextAd(u8t * adItemData,u8t *pOrder)
{
	if(ReadUrgent(u8t * urgentAd))
	{
		return URGENT_ORDER;
	}
	else
	{
		if(ReadBreak(u8t * breakAd))
		{
			return BREAK_ORDER;
		}
		else
		{
			if(ReadNormal(u8t * normalAd))
			{
				++(*pOrder);
				return itemOrder;
			}
			else
			{
				++(*pOrder);
				return -1;
			}
		}
	}
}

void PlayAd(u8t *adContent, u8t runSytle, u8t times, u8t lightness)
{
	//分析广告数据
	LedDisplay(u8t * textStr, u8t frequence,u8t runStyle);
}

void InsertAd(u8t *adStr,u8t adType)
{
	if(adType==PROTOCOL_NORMAL_AD)	//普通广告
	{
		
	}
	else 
	{
		if(adType==PROTOCOL_URGENT_AD)	//紧急广告
		{
		}
		else	//插播广告
		{
		}
	}
}

void AnalysisData(u8t *pData)
{
	FrameStruct *pFrameStruct=(FrameStruct *)pData;
	switch(pFrameStruct->protocolID)
	{
		case PROTOCOL_NORMAL_AD:
			break;
		case PROTOCOL_URGENT_AD:
			break;
		case PROTOCOL_BREAK_AD:
			break;
		default:
			break;
	}
}