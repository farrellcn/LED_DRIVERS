#ifndef _BROADCAST_H_
#define	_BROADCAST_H_

#include "includes.h"

#define PROTOCOL_NORMAL_AD 0xDB
#define PROTOCOL_URGENT_AD 0xDD
#define PROTOCOL_BREAK_AD  0xDE
#define PROTOCOL_DEL_ALL   0xDF


#define SEC_PARAM_OFFSET	0x00
#define SEC_REVERVE_OFFSET	0x01
#define SEC_URGENT_OFFSET  	0x05
#define SEC_BREAK_OFFSET  	0x06
#define SEC_NORMAL_OFFSET	0x07

#define MAX_AD_LENGTH	300  

typedef struct
{	
	u8t header[2];
	u8t reserve[2];
	u8t LedID;
	u8t protocolID;
	u8t dataLength[2];
	u8t	runSytle;
	u8t runSpeed;
	u8t startTime[4];
	u8t endTime[4];
	u8t adContent[2];
} FrameStruct;

typedef struct
{
	u8t textLen[2];
	u8t	runSytle;
	u8t runSpeed;
	u8t startTime[4];
	u8t endTime[4];
	u8t adContent[4];
} AdStruct;

typedef struct
{
	u8t	urgentAd;
	u8t breakAd;
	u8t normalAd;
	u8t reserve;
} BroadParamStruct;


void Broadcast(void);
void Broadcast_Init(void);
void InsertAd(u8t *pData,u8t adType);
void PlayAd(u8t *adItemData);		//(u8t *adContent,u8t runSytle,u8t times,u8t lightness);
u8t ReadNextAd(u8t * adItemData,u16t *pOrder);
boolean ReadNormal(u8t * normalAd,u8t order);
boolean ReadBreak(u8t *breakAd);
boolean ReadUrgent(u8t *urgentAd);
void AnalysisData(u8t *pData);

#endif

