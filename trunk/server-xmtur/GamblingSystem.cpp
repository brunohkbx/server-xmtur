// GamblingSystem.cpp: implementation of the CGamblingItemBag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GamblingSystem.h"
#include "Readscript.h"
#include "user.h"
#include "GameMain.h"
#include "LogProc.h"
#include "LargeRand.h"
#include "DSProtocol.h"

extern ITEM_ATTRIBUTE      ItemAttribute [];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGamblingItemBag::CGamblingItemBag()
{

}

CGamblingItemBag::~CGamblingItemBag()
{

}

void CGamblingItemBag::GamblingItemBagInit(char *name)
{
	m_bLoad = FALSE;
	m_iBagObjectCount = 0;
	m_iRateKindCount = 0;

	int i;
	for(i = 0; i < 4; i++)	{	m_iAddOption[i]				= 0;	}
	for(i = 0; i < 5; i++)	{	m_iGamblingItemGetRate[i]	= 0;	}
	for(i = 0; i < 4; i++)	{										}
	for(i = 0; i < 6; i++)	{										}
	for(i = 0; i < 25; i++)	{	m_iDropRatePerItemCount[i]	= 0;	}
	
	LoadItem(name);
}

void CGamblingItemBag::LoadItem(char *script_file)
{
	m_bLoad = FALSE;
	m_iBagObjectCount = 0;

	if((SMDFile = fopen(script_file, "r")) == NULL)
	{
		LogAdd(lMsg.Get(453), script_file);
		return;
	}

	SMDToken Token;
	int nDropRatePerItemCnt=0;
	int nRateCnt=0;
	int iCount=0;

	while(true)
	{
		Token = GetToken();
		if( Token == END )	break;
		if( Token == NUMBER )
		{
			int Type = TokenNumber;
			if(Type == 0)
			{
				iCount = 0;

				while(true)
				{
					Token = GetToken();

					if(Token == NAME && strcmp("end",TokenString) == 0)	break;

					if( iCount > 25 )
					{
						MsgBox("Load Script Error %s",script_file);
						break;
					}

					m_stGamblingRate[iCount].iSkillRate = TokenNumber;

					Token = GetToken();	m_stGamblingRate[iCount].iLuckRate = TokenNumber;
					Token = GetToken();	m_stGamblingRate[iCount].iAddOptionRate = TokenNumber;
					Token = GetToken();	m_stGamblingRate[iCount].iExcellentRate = TokenNumber;
					Token = GetToken();	m_stGamblingRate[iCount].iSelectedRate = TokenNumber;
					iCount++;
				}
			}
			else if(Type == 1)
			{
				iCount = 0;

				while(true)
				{
					Token = GetToken();

					if(Token == NAME && strcmp("end",TokenString) == 0)	break;

					if( iCount > 4 )
					{
						MsgBox("Load Script Error %s",script_file);
						break;
					}

					m_iAddOption[iCount] = TokenNumber;
					iCount++;
				}
			}
			else if(Type == 2)
			{
				iCount = 0;

				while(true)
				{
					Token = GetToken();

					if(Token == NAME && strcmp("end",TokenString) == 0)	break;

					if( iCount > 5 )
					{
						MsgBox("Load Script Error %s",script_file);
						break;
					}

					m_iGamblingItemGetRate[iCount] = TokenNumber;
					iCount++;
				}
			}
			else if(Type == 3)
			{
				iCount = 0;

				while(true)
				{
					Token = GetToken();

					if(Token == NAME && strcmp("end",TokenString) == 0)	break;

					if( iCount > 4 )
					{
						MsgBox("Load Script Error %s",script_file);
						break;
					}

					m_iGamblingOptionCountRate[iCount] = TokenNumber;
					iCount++;
				}
			}
			else if(Type == 4)
			{
				iCount = 0;

				while(true)
				{
					Token = GetToken();

					if(Token == NAME && strcmp("end",TokenString) == 0)	break;

					if( iCount > 6 )
					{
						MsgBox("Load Script Error %s",script_file);
						break;
					}

					m_iGamblingExOptionRate[iCount] = TokenNumber;
					Token = GetToken();	m_iNormalExOptionRate[iCount] = TokenNumber;
					iCount++;
				}
			}
			else if(Type >= 5)
			{
				nDropRatePerItemCnt = 0;

				while(true)
				{
					Token = GetToken();

					if(Token == NAME && strcmp("end",TokenString) == 0)
					{
						m_iDropRatePerItemCount[nRateCnt] = nDropRatePerItemCnt;
						break;
					}
					
					BagObject[m_iBagObjectCount].m_type = TokenNumber;	
					Token = GetToken();	BagObject[m_iBagObjectCount].m_index = TokenNumber;
					Token = GetToken();	BagObject[m_iBagObjectCount].m_minLevel = TokenNumber;
					Token = GetToken();	BagObject[m_iBagObjectCount].m_maxLevel = TokenNumber;
					Token = GetToken();	BagObject[m_iBagObjectCount].m_isskill = TokenNumber;
					Token = GetToken();	BagObject[m_iBagObjectCount].m_isluck = TokenNumber;
					Token = GetToken();	BagObject[m_iBagObjectCount].m_isoption = TokenNumber;
					Token = GetToken();	BagObject[m_iBagObjectCount].m_isexitem = TokenNumber;

					if( BagObject[m_iBagObjectCount].m_minLevel > 
						BagObject[m_iBagObjectCount].m_maxLevel )
					{
						MsgBox("Load Script Error %s",script_file);
						return;
					}
					
					nDropRatePerItemCnt++;
					m_iBagObjectCount++;

					if(m_iBagObjectCount > MAX_ITEMBAG-1 )	break;
				}

				nRateCnt++;
				if(nRateCnt > 24)	break;

				m_iRateKindCount = nRateCnt;
			}
		}
	}
	fclose(SMDFile);
	LogAdd(lMsg.Get(454), script_file);
	m_bLoad = TRUE;
}

BYTE CGamblingItemBag::GetLevel(int n)
{
	if( n <0 || n > MAX_ITEMBAG-1 )	return 0;
	if( BagObject[n].m_minLevel == BagObject[n].m_maxLevel ) {
		return BagObject[n].m_minLevel;
	}
	int sub = (BagObject[n].m_maxLevel-BagObject[n].m_minLevel)+1;
	int level = BagObject[n].m_minLevel+(rand()%sub);
	return level;
}

BYTE CGamblingItemBag::GetGamblingSystemItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY, BYTE btItemType)
{
	if(!m_bLoad) return 0;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int iDropItemNum = 0;
	int iDropItemRate = 0;
	int iExOption = 0;
	OBJECTSTRUCT* lpObj = (OBJECTSTRUCT*)&gObj[aIndex];

	//LogAdd("GetGamblingSystemItem::GetBagCount -> %d", GetBagCount());

	if(GetBagCount() > 0)
	{
		if(m_iGamblingItemGetRate[btItemType] >= GetLargeRand() % 1000000)
		{
			iLevel = 0;
			fDur = 0;
			iOption1 = 1;
			iOption2 = 0;

			DWORD dwOptionTemp = GetLargeRand() % 1000000;

			//LogAdd("Option Rate MAx: %d", m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3]);

			if( dwOptionTemp <= m_iAddOption[0])
			{
				iOption3 = 1;
			}
			else if( dwOptionTemp <= m_iAddOption[0] + m_iAddOption[1])
			{
				iOption3 = 2;
			}
			else if( dwOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2])
			{
				iOption3 = 3;
			}
			else if( dwOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3])
			{
				iOption3 = 4;
			}

			DWORD dwExOptionTemp = GetLargeRand() % 1000000;

			//LogAdd("Option Exc Rate MAx: %d", m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1] + m_iGamblingOptionCountRate[2] + m_iGamblingOptionCountRate[3]);

			if( dwExOptionTemp <= m_iGamblingOptionCountRate[0])
			{
				iExOption = GamblingOptionRand(1);
			}
			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1])
			{
				iExOption = GamblingOptionRand(2);
			}
			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1] + m_iGamblingOptionCountRate[2])
			{
				iExOption = GamblingOptionRand(3);
			}
			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1] + m_iGamblingOptionCountRate[2] + m_iGamblingOptionCountRate[3])
			{
				iExOption = GamblingOptionRand(4);
			}

			switch ( btItemType ) //5 Types of Special Item
			{
				case 0:
					iType = ItemGetNumberMake(3, 11);
				break;
				case 1:
					iType = ItemGetNumberMake(5, 33);
				break;
				case 2:
					iType = ItemGetNumberMake(4, 24);
				break;
				case 3:
					iType = ItemGetNumberMake(2, 18);
				break;
				case 4:
					iType = ItemGetNumberMake(5, 34);
				break;
			}
		}
		else //else for common items like a bag
		{
			int iCount = 0;
			int iBagPositionStart = 0;
			int iBagPositionEnd = 0;
			int iSelectedGroup = 0;

			switch( btItemType )
			{
				case 0:
					iSelectedGroup = 0;
					break;
				case 1:
					iSelectedGroup = 5;
					break;
				case 2:
					iSelectedGroup = 10;
					break;
				case 3:
					iSelectedGroup = 15;
					break;
				case 4:
					iSelectedGroup = 20;
					break;
			}

			DWORD dwRandTemp = GetLargeRand() % 1000000;
			
			int iSelectedRate = 0;

			for( int i=iSelectedGroup; i<iSelectedGroup+5; i++)
			{
				iSelectedRate += m_stGamblingRate[i].iSelectedRate;
				if(dwRandTemp <= iSelectedRate)
				{
					iSelectedGroup = i;
					break;
				}
			}
			
			while(true) //Max 25
			{
				iBagPositionEnd += m_iDropRatePerItemCount[iCount];
				
				if(iSelectedGroup <= iCount)	break;
				
				iBagPositionStart += m_iDropRatePerItemCount[iCount];
				iCount++;
			}

			iDropItemNum = (GetLargeRand() % (iBagPositionEnd - iBagPositionStart)) + iBagPositionStart;
		
			fDur = 0;
			X = 0;
			Y = 0;

			iLevel = GetLevel(iDropItemNum);

			iType = ItemGetNumberMake(BagObject[iDropItemNum].m_type,BagObject[iDropItemNum].m_index);

			if(iType == -1)	return 0;

			if( BagObject[iDropItemNum].m_isskill != 0)
			{
				if(m_stGamblingRate[iSelectedGroup].iSkillRate >= GetLargeRand()%1000000)
				{
					iOption1 = 1;
				}
			}

			if(BagObject[iDropItemNum].m_isluck != 0)
			{
				if(m_stGamblingRate[iSelectedGroup].iLuckRate >= GetLargeRand()%1000000)
				{
					iOption2 = 1;
				}
			}

			if(BagObject[iDropItemNum].m_isoption != 0)
			{
				if(m_stGamblingRate[iSelectedGroup].iAddOptionRate >= GetLargeRand()%1000000)
				{
					int iOptionTemp = GetLargeRand() % 1000000;

					if( iOptionTemp <= m_iAddOption[0])
					{
						iOption3 = 1;
					}
					else if( iOptionTemp <= m_iAddOption[0] + m_iAddOption[1])
					{
						iOption3 = 2;
					}
					else if( iOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2])
					{
						iOption3 = 3;
					}
					else if( iOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3])
					{
						iOption3 = 4;
					}
				}
			}
			
			if(BagObject[iDropItemNum].m_isexitem != 0)
			{
				if(m_stGamblingRate[iSelectedGroup].iExcellentRate >= GetLargeRand()%1000000)
				{
					iExOption = ExcellentOptionRand();
					iOption2 = 0;
					iOption1 = 1;
					iLevel = 0;	
				}
			}
		}

		ItemSerialCreateSend(lpObj->m_Index,235,lpObj->X,lpObj->Y,iType,iLevel,fDur,iOption1,iOption2,iOption3,lpObj->m_Index,iExOption,0);
		LogAddTD("[ GAMBLING ][ TryGambling ] %s(%s) Try Item : [%s]%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",lpObj->AccountID,lpObj->Name,ItemAttribute[iType].Name,iType,iLevel,iOption1,iOption2,iOption3,iExOption);
	}
	return 1;
}

BYTE CGamblingItemBag::GamblingOptionRand(int iOptionCount)
{
	BYTE NOption = 0;
	int iFailCount = 0;
	int iCount = 0;

	while(true)
	{
		if(m_iGamblingExOptionRate[iCount] >= GetLargeRand() % 1000000) {
			NOption |= 1 << iCount;
		}
		else iFailCount++;

		if(iFailCount >= 6-iOptionCount)
		{
			while(true)
			{
				iCount++;
				if(iCount >= 6)	break;

				NOption |= 1<<iCount;
			}
			break;
		}
		iCount++;
	}

	return NOption;
}

BYTE CGamblingItemBag::ExcellentOptionRand()
{
	BYTE NOption = 0;
	int iCount = 0;
	int iSuccessCount = 0;

	while(true)
	{
		if(m_iNormalExOptionRate[iCount] >= GetLargeRand() % 1000000)
		{
			NOption |= 1<<iCount;
			iSuccessCount++;
		}
		iCount++;

		if(iSuccessCount == 0)
		{
			if(iCount >= 6)	iCount = 0;
		}
		else if(iSuccessCount == 1)
		{
			if(iCount >= 6)	break;
		}
		else if(iSuccessCount >= 2)	break;
	}
	return NOption;
}