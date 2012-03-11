#ifndef _BROADCAST_H_
#define	_BROADCAST_H_

#define PROTOCOL_NORMAL_AD 0xDB
#define PROTOCOL_URGENT_AD 0xDD
#define PROTOCOL_BREAK_AD  0xDE

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
} FrameStruct;

typedef struct
{
	u8t	runSytle;
	u8t runSpeed;
	u8t startTime[4];
	u8t endTime[4];
} AdStruct;

void Broadcast(void);
void InsertAd(u8t *adStr,u8t adType);
void PlayAd(u8t *adContent,u8t runSytle,u8t times,u8t lightness);
u8t ReadNextAd(u8t *adItemData);
boolean ReadNormal(u8t *normalAd);
boolean ReadBreak(u8t *breakAd);
boolean ReadUrgent(u8t *urgentAd);
void AnalysisData(u8t *pData);

#endif

