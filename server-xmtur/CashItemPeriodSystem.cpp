// CashItemPeriodSystem.cpp: implementation of the CCashItemPeriodSystem class.
// GS-N	1.00.18	JPN	0x00591EB0	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CashItemPeriodSystem.h"
#include "DBSockMng.h"
#include "LogProc.h"
#include "ItemAddOption.h"
#include "Winutil.h"
#include "BuffEffect.h"


CCashItemPeriodSystem g_CashItemPeriodSystem;
BOOL g_bRunningPeriodCheckThread = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCashItemPeriodSystem::CCashItemPeriodSystem()
{
	return;
}

CCashItemPeriodSystem::~CCashItemPeriodSystem()
{
	return;
}



void CCashItemPeriodSystem::Initialize()
{
	this->iItemPeriodInfoCount = 0;

	for ( int i=0;i<MAX_ITEM_PERIOD_INFO;i++)
	{
		this->ItemPeriodInfo[i].btUsedInfo = 0;
		this->ItemPeriodInfo[i].Unk1 = 0; //Test Only
		this->ItemPeriodInfo[i].Unk2 = 0; //Test Only
		this->ItemPeriodInfo[i].iUserIndex = -1;
		this->ItemPeriodInfo[i].iUserGuid = -1;
		this->ItemPeriodInfo[i].iItemCode = -1;
		this->ItemPeriodInfo[i].iItemPeriodLeftTime = 0;
		this->ItemPeriodInfo[i].btEffectType1 = 0;
		this->ItemPeriodInfo[i].btEffectType2 = 0;
		memset(this->ItemPeriodInfo[i].chCharacterName, 0, sizeof(this->ItemPeriodInfo[i].chCharacterName));
		memset(this->ItemPeriodInfo[i].chItemPeriodBuyDate, 0, sizeof(this->ItemPeriodInfo[i].chItemPeriodBuyDate));
		memset(this->ItemPeriodInfo[i].chItemPeriodEndDate, 0, sizeof(this->ItemPeriodInfo[i].chItemPeriodEndDate));
		this->ItemPeriodInfo[i].dwItemCheckTickCount = 0;
	}

	this->hThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CCashItemPeriodSystem::PeriodCheckThread, this->ItemPeriodInfo, 0, NULL);
}




DWORD CCashItemPeriodSystem::PeriodCheckThread(void * lpPeriodInfo)
{
	ITEMPERIOD_INFO* lpPeriodInfoList = (ITEMPERIOD_INFO *)lpPeriodInfo;
	int iIndex = 0;
	DWORD dwCurrentTick = 0;
	LPOBJ lpObj = NULL;
	int iUseTime = 0;

	while ( true )
	{
		if ( (GetTickCount() - dwCurrentTick) > 30000 )
		{
			dwCurrentTick = GetTickCount();

			for ( iIndex=0;iIndex<MAX_ITEM_PERIOD_INFO;iIndex++)
			{
				if ( lpPeriodInfoList[iIndex].btUsedInfo == 1 )
				{
					iUseTime = GetTickCount() - lpPeriodInfoList[iIndex].dwItemCheckTickCount;

					if ( iUseTime > 60000 )
					{
						lpPeriodInfoList[iIndex].dwItemCheckTickCount = GetTickCount();
						lpPeriodInfoList[iIndex].iItemPeriodLeftTime--;
						lpPeriodInfoList[iIndex].iItemPeriodUsedTime++;
						lpObj = &gObj[lpPeriodInfoList[iIndex].iUserIndex];

						if ( lpPeriodInfoList[iIndex].iItemPeriodLeftTime <= 0 )
						{
							g_CashItemPeriodSystem.GDReqPeriodItemUpdate(lpObj);
							g_CashItemPeriodSystem.ClearPeriodItemEffect(lpObj);
						}
					}
				}
			}
		}
		else
		{
			Sleep(10);
		}
	}

	return 0;
}

void CCashItemPeriodSystem::GetUserPeriodItemList(LPOBJ lpObj) //GS-CS Decompiled 100%
{
	//int iUserIndex = lpObj->m_Index; //Webzen removed on GS-CS
	//int iUserGuid = lpObj->DBNumber; //Webzen removed on GS-CS
	this->GDReqPeriodItemList(lpObj);
}



int CCashItemPeriodSystem::CalcItemUseLeftTime(LPSTR pchBuyDate)
{
	int iUsedTime = 0;
	CTime tCurrentTime = CTime::GetTickCount();
	CTime tBuyTime;
	CTimeSpan tTimeSpan;

	this->ConvertStringToTime(pchBuyDate, &tBuyTime);
	tCurrentTime - tBuyTime;
	tTimeSpan = tBuyTime - tCurrentTime;
	iUsedTime = tTimeSpan.GetTotalMinutes();

	return iUsedTime;
}


CTime CCashItemPeriodSystem::CalcItemUseEndTime(int iLeftTime)
{
	CTime tExpireTime;
	CTime tCurrentTime = CTime::GetTickCount();
	CTimeSpan tLeftTime(0, 0, iLeftTime, 0);

	tExpireTime = tCurrentTime + tLeftTime;

	return tExpireTime;
}


void CCashItemPeriodSystem::ConvertStringToTime(const char * pchDate, class CTime * lpTime)
{
	int iYear=0;
	int iMonth=0;
	int iDay=0;
	int iHour=0;
	int iMinute=0;
	int iDateLength=0;
	char chDate[20];
	char* Token;
	char seps[4]="-: ";

	if ( pchDate == NULL || lpTime == NULL )
		return;

	memcpy(chDate, pchDate, sizeof(chDate));
	iDateLength = strlen(chDate);

	if ( iDateLength <= 0 || iDateLength > 20 )
		return;

	Token = strtok( chDate, seps);
	iYear = atoi(Token);
	Token = strtok( NULL, seps);
	iMonth = atoi(Token);
	Token = strtok( NULL, seps);
	iDay = atoi(Token);
	Token = strtok( NULL, seps);
	iHour = atoi(Token);
	Token = strtok( NULL, seps);
	iMinute = atoi(Token);

	CTime tReturnTime(iYear, iMonth, iDay, iHour, iMinute, 0, -1);

	*lpTime = tReturnTime;
}

void CCashItemPeriodSystem::ConvertTimeToString(const CTime * lpTime, LPSTR pchDate)
{
	int iYear=0;
	int iMonth=0;
	int iDay=0;
	int iHour=0;
	int iMinute=0;
	char chDate[20];

	iYear = lpTime->GetYear();
	iMonth = lpTime->GetMonth();
	iDay = lpTime->GetDay();
	iHour = lpTime->GetHour();
	iMinute = lpTime->GetMinute();

	wsprintf(chDate, "%d-%d-%d %d:%d", iYear, iMonth, iDay, iHour, iMinute);
	memcpy(pchDate, chDate, sizeof(chDate));
}




BOOL CCashItemPeriodSystem::SetPeriodItemEffect(LPOBJ lpObj, int iItemCode, BYTE btEffectType1, BYTE btEffectType2, DWORD dwItemPeriod)
{
	/*
	if ( lpObj->m_iPeriodItemEffectIndex != -1 )
	{
		if ( this->ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex].iUserIndex == lpObj->m_Index )
		{
			return FALSE;
		}
	}*/

	for ( int i=0;i<MAX_ITEM_PERIOD_INFO;i++)
	{
		for ( int n=0;n<10;++n)
		{
			if ( this->ItemPeriodInfo[i].btUsedInfo == FALSE && lpObj->m_iPeriodItemEffectIndex[n] == -1 )
			{
				this->ItemPeriodInfo[i].btUsedInfo = TRUE;
				this->ItemPeriodInfo[i].iUserIndex = lpObj->m_Index;
				this->ItemPeriodInfo[i].iUserGuid = lpObj->DBNumber;
				this->ItemPeriodInfo[i].iItemCode = iItemCode;
				this->ItemPeriodInfo[i].btEffectType1 = btEffectType1;
				this->ItemPeriodInfo[i].btEffectType2 = btEffectType2;
				this->ItemPeriodInfo[i].iItemPeriodLeftTime = dwItemPeriod;
				this->ItemPeriodInfo[i].iItemPeriodUsedTime = 0;
				this->ItemPeriodInfo[i].dwItemCheckTickCount = GetTickCount();

				lpObj->m_iPeriodItemEffectIndex[n] = i;

				g_ItemAddOption.SetItemEffect(lpObj, iItemCode, dwItemPeriod);

				return TRUE;
			}
		}
	}

	return FALSE;
}





BOOL CCashItemPeriodSystem::ClearPeriodItemEffect(LPOBJ lpObj)
{
	int iItemInfoIndex =0 ;
	for(int i=0;i<10;++i)
	{
		iItemInfoIndex = lpObj->m_iPeriodItemEffectIndex[i];

		if ( iItemInfoIndex < 0 || iItemInfoIndex > MAX_ITEM_PERIOD_INFO )
			continue;

		if ( this->ItemPeriodInfo[iItemInfoIndex].btUsedInfo != TRUE )
		{
			if ( lpObj->m_Index != this->ItemPeriodInfo[iItemInfoIndex].iUserIndex )
			{
				continue;
			}
		}

		g_ItemAddOption.ClearItemEffect2(lpObj, this->ItemPeriodInfo[iItemInfoIndex].iItemCode, 2);

		this->ItemPeriodInfo[iItemInfoIndex].btUsedInfo = FALSE;
		this->ItemPeriodInfo[iItemInfoIndex].iUserIndex = 0;
		this->ItemPeriodInfo[iItemInfoIndex].iItemCode = 0;
		this->ItemPeriodInfo[iItemInfoIndex].iItemPeriodLeftTime = 0;
		this->ItemPeriodInfo[iItemInfoIndex].btEffectType1 = 0;
		this->ItemPeriodInfo[iItemInfoIndex].btEffectType2 = 0;
		this->ItemPeriodInfo[iItemInfoIndex].iUserGuid = 0;

		memset(this->ItemPeriodInfo[iItemInfoIndex].chCharacterName, 0, sizeof(this->ItemPeriodInfo[iItemInfoIndex].chCharacterName));
		memset(this->ItemPeriodInfo[iItemInfoIndex].chItemPeriodBuyDate, 0, sizeof(this->ItemPeriodInfo[iItemInfoIndex].chItemPeriodBuyDate));
		memset(this->ItemPeriodInfo[iItemInfoIndex].chItemPeriodEndDate, 0, sizeof(this->ItemPeriodInfo[iItemInfoIndex].chItemPeriodEndDate));

		lpObj->m_iPeriodItemEffectIndex[i] = -1;
	}
	return TRUE;
}


struct PMSG_REQ_PERIODITEM_INSERT
{
	PBMSG_HEAD2 head;	// C1:D0:03
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	int iItemCode;	// C
	int iItemPeriodDate;	// 10
	BYTE btItemEffectType1;	// 14
	BYTE btItemEffectType2;	// 15
	char chUserID[11];	// 16
	char chExpireDate[20];	//21
	BYTE btItemOption;
};



void CCashItemPeriodSystem::GDReqPeriodItemInsert(LPOBJ lpObj, int iItemCode, int iItemEffectDate)
{
	PMSG_REQ_PERIODITEM_INSERT pMsg;
	int iEffectType1 =0;
	int iEffectType2 =0;
	int iOptionType = 0;
	CTime tExpireDate;
	int iEffectValidTime =0; //GS-CS OK

	if ( g_ItemAddOption.SearchItemEffectType(iItemCode, &iEffectType1, &iEffectType2, &iEffectValidTime, &iOptionType) == false ) //GS-CS OK
		return;
	
	if ( iItemEffectDate == -1 ) //GS-CS OK
	{
		//?? 005C861A
		int iEffectValidTime =0; //TestOnly, Here comes the new data
	}

	//New Struct comes here first
	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid = lpObj->DBNumber;
	pMsg.iItemCode = iItemCode;
	pMsg.btItemEffectType1 = iEffectType1;
	pMsg.btItemEffectType2 = iEffectType2;
	pMsg.iItemPeriodDate = iItemEffectDate;
	pMsg.btItemOption = iOptionType;
	memcpy(pMsg.chUserID, lpObj->Name, sizeof(pMsg.chUserID));

	tExpireDate = this->CalcItemUseEndTime(iItemEffectDate);

	wsprintf(pMsg.chExpireDate, "%d-%d-%d %d:%d", tExpireDate.GetYear(), tExpireDate.GetMonth(), tExpireDate.GetDay(), tExpireDate.GetHour(), tExpireDate.GetMinute());

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x03, sizeof(PMSG_REQ_PERIODITEM_INSERT));

	cDBSMng.Send((char *)&pMsg, sizeof(PMSG_REQ_PERIODITEM_INSERT));

	LogAddTD("[CashShop][PeriodItemInsert Req] - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d, ExpireDate : %s",
		lpObj->AccountID, lpObj->Name, pMsg.iItemCode, pMsg.btItemEffectType1, pMsg.btItemEffectType2, pMsg.iItemPeriodDate, pMsg.chExpireDate);
}



struct PMSG_REQ_PERIODITEM_LIST
{
	PBMSG_HEAD2 head;	// C1:D0:05
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	char chCharacterName[11];	// C
};





void CCashItemPeriodSystem::GDReqPeriodItemList(LPOBJ lpObj)
{
	PMSG_REQ_PERIODITEM_LIST pMsg;

	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid = lpObj->DBNumber;
	memcpy(pMsg.chCharacterName, lpObj->Name, sizeof(pMsg.chCharacterName));

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x05, sizeof(PMSG_REQ_PERIODITEM_LIST));

	cDBSMng.Send((char *)&pMsg, sizeof(PMSG_REQ_PERIODITEM_LIST));
}


struct PMSG_REQ_PERIODITEM_INQUIRY
{
	PBMSG_HEAD2 head;	// C1:D0:01
	int iUserIndex;	// 4
	int iUserGuid;	// 8

};

void CCashItemPeriodSystem::GDReqPeriodItemInquiry(LPOBJ lpObj)
{
	PMSG_REQ_PERIODITEM_INQUIRY pMsg;

	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid = lpObj->DBNumber;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x01, sizeof(PMSG_REQ_PERIODITEM_INQUIRY));

	cDBSMng.Send((char *)&pMsg, sizeof(PMSG_REQ_PERIODITEM_INQUIRY));
}




struct PMSG_REQ_PERIODITEM_UPDATE
{
	PBMSG_HEAD2 head;	// C1:D0:07
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	int iItemCode;	// C
	int iUsedTime;	// 10
	int iLeftTime;	// 14
	char chCharacterName[11];	// 18
};




void CCashItemPeriodSystem::GDReqPeriodItemUpdate(LPOBJ lpObj)
{
	PMSG_REQ_PERIODITEM_UPDATE pMsg;
	ITEMPERIOD_INFO* lpItemInfo = NULL;

	for(int i=0;i<10;++i)
	{

		if ( lpObj->m_iPeriodItemEffectIndex[i] < 0 || lpObj->m_iPeriodItemEffectIndex[i] >=MAX_ITEM_PERIOD_INFO )
			return;

		lpItemInfo = &this->ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex[i]];

		pMsg.iUserGuid = lpObj->DBNumber;
		pMsg.iItemCode = lpItemInfo->iItemCode;
		pMsg.iUserIndex = lpObj->m_Index;
		pMsg.iUsedTime = lpItemInfo->iItemPeriodUsedTime;
		pMsg.iLeftTime = lpItemInfo->iItemPeriodLeftTime;
		memcpy(pMsg.chCharacterName, lpObj->Name, sizeof(pMsg.chCharacterName));

		PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x07, sizeof(PMSG_REQ_PERIODITEM_UPDATE));

		cDBSMng.Send((char *)&pMsg, sizeof(PMSG_REQ_PERIODITEM_UPDATE));
	}
}





void CCashItemPeriodSystem::DGAnsPeriodItemInsert(PMSG_ANS_PERIODITEM_INSERT * aRecv)
{
	int iIndex = aRecv->iUserIndex;
	LPOBJ lpObj = &gObj[iIndex];
	BYTE btResult = FALSE;

	switch ( aRecv->btResult )
	{
		case 0: btResult = 1; break;
		case 1: btResult = 2; break;
		case 2: btResult = 3; break;
		default: btResult = 1;
	}

	if ( btResult == 1 )
	{
		this->SetPeriodItemEffect(lpObj, aRecv->iItemCode, aRecv->btItemEffectType1, aRecv->btItemEffectType2, aRecv->iItemPeriodDate);

		LogAddTD("[CashShop][PeriodItemInsert Ans] Success - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
			lpObj->AccountID, lpObj->Name, aRecv->iItemCode, aRecv->btItemEffectType1, aRecv->btItemEffectType2, aRecv->iItemPeriodDate);
	}
	else
	{
		LogAddTD("[CashShop][PeriodItemInsert Ans] Fail - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d, Result : %d",
			lpObj->AccountID, lpObj->Name, aRecv->iItemCode, aRecv->btItemEffectType1, aRecv->btItemEffectType2, aRecv->iItemPeriodDate, btResult);	
	}

	this->GCSendPeriodItemInserResult(lpObj, btResult);
}


void CCashItemPeriodSystem::DGAnsPeriodItemList(PMSG_ANS_PERIODITEM_LIST* aRecv)
{
	if ( aRecv->iUserIndex <= 0 || aRecv->iUserIndex > OBJMAX )
		return;

	LPOBJ lpObj = &gObj[aRecv->iUserIndex];
	int iItemLeftUseTime = 0;
	int iResult = 0;
	iResult = aRecv->btResult;

	if ( lpObj->DBNumber != aRecv->iUserGuid )
		return;

	if ( iResult == -1 ) //iResult
	{
		LogAddTD("[CashShop][PeriodItemSelect Ans] Failed - Item over 1 item ID : %s, Name : %s",
			lpObj->AccountID, lpObj->Name);
	}

	else if ( iResult == -2 )
	{
		LogAddTD("[CashShop][PeriodItemSelect Ans] Failed - Item usedtime is over ID : %s, Name : %s",
			lpObj->AccountID, lpObj->Name);
	}

	for(int i=0;i<aRecv->btCount;++i)
	{
		iItemLeftUseTime = aRecv->Items[i].iItemPeriodLeftTime; // GS-CS Good (GS18 = aRecv->iLeftTime)

		if ( iItemLeftUseTime > 0 )
		{
			this->SetPeriodItemEffect(lpObj, aRecv->Items[i].iItemCode, aRecv->Items[i].btItemEffectType1, aRecv->Items[i].btItemEffectType2, iItemLeftUseTime);

			LogAddTD("[CashShop][PeriodItemList Ans] Success - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
				lpObj->AccountID, lpObj->Name, aRecv->Items[i].iItemCode, aRecv->Items[i].btItemEffectType1, aRecv->Items[i].btItemEffectType2, iItemLeftUseTime);
		}
		else
		{
			LogAddTD("[CashShop][PeriodItemList Ans] Fail - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
				lpObj->AccountID, lpObj->Name, aRecv->Items[i].iItemCode, aRecv->Items[i].btItemEffectType1, aRecv->Items[i].btItemEffectType2, iItemLeftUseTime);
		}
	}
}



void CCashItemPeriodSystem::DGAnsPeriodItemInquiry(PMSG_ANS_PERIODITEM_INQUIRY *aRecv)
{
	return;
}



void CCashItemPeriodSystem::DGAnsPeriodItemUpdate(PMSG_ANS_PERIODITEM_UPDATE *aRecv)
{
	int iUserIndex = aRecv->iUserIndex;
	int PeriodIndex = -1;

	if ( iUserIndex < 0 || iUserIndex > OBJMAX )
		return;

	LPOBJ lpObj = &gObj[iUserIndex];
	ITEMPERIOD_INFO* lpItemInfo = NULL;

	for(int i=0;i<10;++i)
	{
		if ( lpObj->m_iPeriodItemEffectIndex[i] < 0 || lpObj->m_iPeriodItemEffectIndex[i] >=MAX_ITEM_PERIOD_INFO )
			continue;

		lpItemInfo = &this->ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex[i]];

		if(lpItemInfo->iItemCode == aRecv->iItemCode)
		{
			PeriodIndex = i;
			break;
		}
	}

	switch ( aRecv->btResult )
	{
		case 3:
			if ( this->ClearPeriodItemEffect(lpObj) == TRUE )
			{		
				LogAddTD("[CashShop][PeriodItemUpdate Ans] Remove Item Effect - ID : %s, Name : %s, ItemCode : %d, Result : %d",
					lpObj->AccountID, lpObj->Name, aRecv->iItemCode, aRecv->btResult);
			}
	}

	LogAddTD("[CashShop][PeriodItemUpdate Ans] - ID : %s, Name : %s, ItemCode : %d, Result %d",
		lpObj->AccountID, lpObj->Name, aRecv->iItemCode, aRecv->btResult);
}



struct PMSG_REQ_PERIODITEM_DELETE
{
	PBMSG_HEAD2 head;	// C1:D0:09
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	char chCharacterName[11];	// C
};



void CCashItemPeriodSystem::GDReqPeriodItemDelete(LPOBJ lpObj, LPSTR pchCharacterName)
{
	PMSG_REQ_PERIODITEM_DELETE pMsg;
	ITEMPERIOD_INFO* lpItemInfo = NULL;
	
	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid = lpObj->DBNumber;
	memcpy(pMsg.chCharacterName, pchCharacterName, sizeof(pMsg.chCharacterName));

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x09, sizeof(PMSG_REQ_PERIODITEM_DELETE));

	cDBSMng.Send((char*)&pMsg, sizeof(PMSG_REQ_PERIODITEM_DELETE));

}

#pragma warning ( disable : 4060 )
void CCashItemPeriodSystem::DGAnsPeriodItemDelete(PMSG_ANS_PERIODITEM_DELETE *aRecv) //GS-CS Decompiled 100%
{
	LPOBJ lpObj = NULL;

	if ( aRecv->iUserIndex < 0 || aRecv->iUserIndex > OBJMAX )
		return;

	lpObj = &gObj[aRecv->iUserGuid];	// #error : Apply Deathway Fix Here

	if ( lpObj->DBNumber != aRecv->iUserGuid )
		return;

	switch ( aRecv->btResult ) //Good
	{
		case 0:
			LogAddTD("[CashShop][PeriodItemDelete Ans] - ID : %s, Name : %s", lpObj->AccountID, lpObj->Name);
	}
	
}
#pragma warning ( disable : 4060 )


void CCashItemPeriodSystem::GCSendPeriodItemInserResult(LPOBJ lpObj, BYTE btResult)
{
	PMSG_ANS_PERIODITEM_INSERT_RESULT pMsg;
	
	pMsg.btResult = btResult;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x03, sizeof(PMSG_ANS_PERIODITEM_INSERT_RESULT));

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg,	sizeof(PMSG_ANS_PERIODITEM_INSERT_RESULT));
}



