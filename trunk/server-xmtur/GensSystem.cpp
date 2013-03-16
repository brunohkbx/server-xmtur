#include "Stdafx.h"
#include "User.h"
#include "GensSystem.h"
#include "LogProc.h"
#include "Readscript.h"
#include "DSProtocol.h"
#include "BuffManager.h"
#include "GameMain.h"

unsigned char Kill_WinPoints[7] = { 1,2,3,5,6,6,7 };
unsigned char Kill_LosePoints[7] = { 3,3,3,3,3,2,1 };

CGensSystem GensSystem;

void GensSystem_UpdateTimerRank(void * lpParam);

CGensSystem::CGensSystem(){
	_beginthread(GensSystem_UpdateTimerRank,0,NULL);
}

void CGensSystem::GetGensInformation(int aIndex){
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	DGGetGensInfo(aIndex);
}

void CGensSystem::SaveGensInfo(int aIndex){
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->GensFamily == 0) return;

	DGUpdateGensInfo(aIndex);

	LogAddTD("[GensSystem][%s][%s] Gens Information Saved",lpObj->AccountID,lpObj->Name);
}

void CGensSystem::SendGensInfo(int aIndex){
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->GensFamily == 0) return;

	this->GetGensSymbol(aIndex); //Get Gens Symbol and Next Contribution

	PMSG_GENS_INFO pMsg;

	pMsg.C = 0xC1;
	pMsg.Headcode = 0xF8;
	pMsg.Subcode = 0x07;
	pMsg.Size = sizeof(pMsg);
	pMsg.GensType = lpObj->GensFamily; //1: Duprians - 2: Vernet
	pMsg.Ranking = lpObj->GensRank;
	pMsg.Symbol = lpObj->GensSymbol; 
	pMsg.Contribution = lpObj->GensContribution;
	pMsg.NextContribution = lpObj->GensNextContribution;
	
	DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
}

void CGensSystem::GetGensSymbol(int aIndex){
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
		
	if(lpObj->GensContribution >= 0 && lpObj->GensContribution <= 499){
		lpObj->GensNextContribution = 500;
		lpObj->GensSymbol = 0;
	} else if(lpObj->GensContribution >= 500 && lpObj->GensContribution <= 1499){
		lpObj->GensNextContribution = 500;
		lpObj->GensSymbol = 13;
	} else if(lpObj->GensContribution >= 1500 && lpObj->GensContribution <= 2999){
		lpObj->GensNextContribution = 1500;
		lpObj->GensSymbol = 12;
	} else if(lpObj->GensContribution >= 3000 && lpObj->GensContribution <= 5999){
		lpObj->GensNextContribution = 5000;
		lpObj->GensSymbol = 11;
	} else if(lpObj->GensContribution >= 6000 && lpObj->GensContribution <= 9999){
		lpObj->GensNextContribution = 10000;
		lpObj->GensSymbol = 10;
	}

	if(lpObj->GensContribution > 10000){
		if(lpObj->GensRank == 1){
			lpObj->GensSymbol = 1;
		}
		if(lpObj->GensRank >= 2 && lpObj->GensRank <= 5){
			lpObj->GensSymbol = 2;
		}
		if(lpObj->GensRank >= 6 && lpObj->GensRank <= 10){
			lpObj->GensSymbol = 3;
		}
		if(lpObj->GensRank >= 11 && lpObj->GensRank <= 30){
			lpObj->GensSymbol = 4;
		}
		if(lpObj->GensRank >= 31 && lpObj->GensRank <= 50){
			lpObj->GensSymbol = 5;
		}
		if(lpObj->GensRank >= 51 && lpObj->GensRank <= 100){
			lpObj->GensSymbol = 6;
		}
		if(lpObj->GensRank >= 101 && lpObj->GensRank <= 200){
			lpObj->GensSymbol = 7;
		}
		if(lpObj->GensRank >= 201 && lpObj->GensRank <= 300){
			lpObj->GensSymbol = 8;
		}
		if(lpObj->GensRank >= 301){
			lpObj->GensSymbol = 9;
		}
	}
}

void CGensSystem::EnterGensFamily(int aIndex, int GensFamily){

	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_GENS_JOIN pMsg;
	pMsg.C = 0xC1;
	pMsg.Size = 0x06;
	pMsg.Headcode = 0xF8;
	pMsg.Subcode = 0x02;

	//Type Information
	//0x00 - Join Successfully
	//0x01 - Already Joined a Gens
	//0x02 - Leaved Recently
	//0x03 - User is Below Level 50
	//0x04 - Guild Belong to other Gens
	//0x05 - GuildMaster is Not in Gens
	//0x06 - User is in Party
	//0x07 - Guild in Alliance

	if(lpObj->GensFamily == 1 || lpObj->GensFamily == 2){ //If has Already Joined a Gens
		pMsg.Type = 0x01;
		pMsg.Sub = 0x00;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
		LogAddTD("[GensSystem][%s][%s] Player is Already on Gens Family",lpObj->AccountID,lpObj->Name);
		return;
	}

	/*if(lpObj->GensFamily == 0){ //If has Leave Gens Recently
		pMsg.Type = 0x02;
		pMsg.Sub = 0x00;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
		LogAddTD("[GensSystem][%s][%s] Player has Leave Gens Recently",lpObj->AccountID,lpObj->Name);
		return;
	}*/

	if(lpObj->Level < 50){ //Check Level
		pMsg.Type = 0x03;
		pMsg.Sub = 0x00;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
		LogAddTD("[GensSystem][%s][%s] Need to be Level 50",lpObj->AccountID,lpObj->Name);
		return;
	}

	if(lpObj->PartyNumber != -1){ //Check Party
		pMsg.Type = 0x06;
		pMsg.Sub = 0x00;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
		LogAddTD("[GensSystem][%s][%s] Player is in Party",lpObj->AccountID,lpObj->Name);
		return;
	}

	DGJoinGensRequest(aIndex,GensFamily);	
}

void CGensSystem::LeaveGensFamily(int aIndex){
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_GENS_LEAVE pMsg;
	pMsg.C = 0xC1;
	pMsg.Size = 0x05;
	pMsg.Headcode = 0xF8;
	pMsg.Subcode = 0x04;

	//Type Information
	//0x00 - Successfull Leave
	//0x01 - Not part of Gens
	//0x02 - Guild Master Can't Leave Gens
	//0x03 - Part of Other Gens

	//Get PlayerInformation Index
	int pIndex = (aIndex-OBJ_STARTUSERINDEX);

	if(lpObj->GensFamily == 0){

		pMsg.Type = 0x01;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
		LogAddTD("[GensSystem][%s][%s] Can't Leave Gens because is not Part of Family",lpObj->AccountID,lpObj->Name);
		return;
	}

	if(lpObj->GuildStatus == 128){
		pMsg.Type = 0x02;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
		LogAddTD("[GensSystem][%s][%s] Can't Leave Gens because is a Guild Master",lpObj->AccountID,lpObj->Name);
		return;
	}

	if((lpObj->GensFamily == 1 && lpObj->TargetShopNumber == 544)
		|| (lpObj->GensFamily == 2 && lpObj->TargetShopNumber == 543)){
		pMsg.Type = 0x03;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
		LogAddTD("[GensSystem][%s][%s] Can't Leave Gens because is part of Other Family",lpObj->AccountID,lpObj->Name);
		return;
	}

	DGDeleteGensInfo(aIndex);
	
	lpObj->GensFamily = 0;
	lpObj->GensRank = 0;
	lpObj->GensContribution = 0;

	pMsg.Type = 0x00;
	DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
	this->SendGensInfo(aIndex);
	LogAddTD("[GensSystem][%s][%s] Leaved Gens Family Successfully!",lpObj->AccountID,lpObj->Name);
	return;
}


void CGensSystem::SendGensViewport(int aIndex){

	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	PWMSG_GENS_VIEWPORT lpMsg = { 0xC2, 0x00, 0x00, 0xF8, 0x05, 0x01 };

	lpMsg.SizeH = HIBYTE(sizeof(lpMsg));
	lpMsg.SizeL = LOBYTE(sizeof(lpMsg));

	for(int i = 0; i < 75; i++){

		if(lpObj->VpPlayer[i].number > OBJ_STARTUSERINDEX){

			int TargetIndex = lpObj->VpPlayer[i].number;
			LPOBJ lpTargetObj = &gObj[TargetIndex];

			if(lpTargetObj->GensFamily != 0){
				lpMsg.Family = lpTargetObj->GensFamily;
				lpMsg.Symbol = lpTargetObj->GensSymbol;
				lpMsg.IndexH = HIBYTE(TargetIndex);
				lpMsg.IndexL = LOBYTE(TargetIndex);
				DataSend(lpObj->m_Index,(LPBYTE)&lpMsg,sizeof(lpMsg));
			}

			if(lpObj->GensFamily != 0){
				lpMsg.Family = lpObj->GensFamily;
				lpMsg.Symbol = lpObj->GensSymbol;
				lpMsg.IndexH = HIBYTE(aIndex);
				lpMsg.IndexL = LOBYTE(aIndex);
				DataSend(TargetIndex,(LPBYTE)&lpMsg,sizeof(lpMsg));
			}
		}
	}

}

void CGensSystem::GetGensRankingReward(PMSG_GENS_REWARD *aRecv, int aIndex)
{
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_GENS_REWARD pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xF8, 0x0A, sizeof(pMsg));


	if(lpObj->GensFamily == 0){
		pMsg.Gens = 6;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
		return;
	}

	SYSTEMTIME Time;
	GetLocalTime(&Time);

	if((lpObj->GensFamily == 1 && lpObj->TargetShopNumber == 544) || (lpObj->GensFamily == 2 && lpObj->TargetShopNumber == 543)){
		pMsg.Gens = 5;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
		return;
	}

	if(Time.wDay != 1){
		pMsg.Gens = 1;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
		return;
	}

	if(lpObj->GensRank > 300){
		pMsg.Gens = 2;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
		return;
	}

	if(lpObj->GensPrize == 1){
		pMsg.Gens = 4;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
		return;
	}

	int Count = 0;
	int Item = 0;

	switch(lpObj->GensSymbol){

		case 8:
			Count = 1;
			Item = ITEMGET(14,144);
			break;
		case 7:
			Count = 1;
			Item = ITEMGET(14,144);
			break;
		case 6:
			Count = 1;
			Item = ITEMGET(14,143);
			break;
		case 5:
			Count = 2;
			Item = ITEMGET(14,143);
			break;
		case 4:
			Count = 2;
			Item = ITEMGET(14,142);
			break;
		case 3:
			Count = 3;
			Item = ITEMGET(14,142);
			break;
		case 2:
			Count = 3;
			Item = ITEMGET(14,141);
			break;
		case 1:
			Count = 5;
			Item = ITEMGET(14,141);
			break;
	}

	if(Item == 0 || Count == 0){
		return;
	}

	for(int X=0; X < Count;++X){
		if(!CheckInventoryEmptySpace(lpObj,1,5)){
			pMsg.Gens = 3;
			DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
			return;
		}
	}

	lpObj->GensPrize = 1;
	pMsg.Gens = 0;
	DataSend(aIndex,(LPBYTE)&pMsg, pMsg.h.size);

	for(int X=0;X < Count;++X){
		ItemSerialCreateSend(aIndex,235,0,0,Item,0,0,0,0,0,aIndex,0,0);
	}

	LogAddTD("[GensSystem][%s][%s] Got Gens Ranking Prize [%d][%d][%d]",lpObj->AccountID,lpObj->Name,lpObj->GensRank,Item,Count);
}

void CGensSystem::GensProtocolCore(int aIndex, unsigned char* aRecv){
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	switch(aRecv[3]){

		case 0x01: //Join Gens (01:Duprian - 02: Vernet)
			this->EnterGensFamily(aIndex,aRecv[4]);;
			return;
		break;

		case 0x09: //Gens Ranking
			GetGensRankingReward((PMSG_GENS_REWARD*)aRecv,aIndex);;
			return;
		break;

		case 0x03: //Leave Gens
			this->LeaveGensFamily(aIndex);
			return;
		break;

		case 0x0B:
			this->SendGensInfo(aIndex);;
			return;
		break;
	}
}


void CGensSystem::AddContribution(int aIndex, int Points)
{
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->GensContribution += Points;

	if(lpObj->GensContribution < 0) lpObj->GensContribution = 0;
	
	PMSG_GENS_CONTRIBUTION pMsg = {0xC1, sizeof(pMsg), 0x0B, 0x12};
	pMsg.Contrib = lpObj->GensContribution;
	strcpy(pMsg.Name,lpObj->Name);

	DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));

	this->GetGensSymbol(aIndex);

	LogAddTD("[GensSystem][%s][%s] Modified Contribution: %d",lpObj->AccountID,lpObj->Name,Points);
	
	GensSystem.SendGensInfo(aIndex);
}

int CGensSystem::GetLevelDifference(short Level1, short Level2)
{
	int Dif = Level1 - Level2;

	if(Dif < -51) return 0;
	if(Dif < -30) return 1;
	if(Dif < -10) return 2;
	if(Dif > 10) return 4;
	if(Dif > 30) return 5;
	if(Dif > 50) return 6;

	return 3;
}

BOOL CGensSystem::GetContributionPoints(int aIndex, int aTargetIndex)
{
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return FALSE;
	if(!gObjIsConnected(aIndex)) return FALSE;

	if(aTargetIndex < OBJ_STARTUSERINDEX || aTargetIndex > (OBJMAX-1)) return FALSE;
	if(!gObjIsConnected(aTargetIndex)) return FALSE;

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->GensFamily == 0 || lpTargetObj->GensFamily == 0) return FALSE;

	int DifIndex = this->GetLevelDifference(lpObj->Level,lpTargetObj->Level);

	LogAddTD("[GensSystem][%s][%s] Killed [%s][%s]",(lpObj->GensFamily == 1) ? "Duprian" : "Vanert", lpObj->Name,
		(lpTargetObj->GensFamily == 1) ? "Duprian" : "Vanert", lpTargetObj->Name);

	this->AddContribution(aIndex, Kill_WinPoints[DifIndex]);
	this->AddContribution(aTargetIndex, -Kill_WinPoints[DifIndex]);

	return TRUE;
}

void CGensSystem::ExecuteRankUpdate(){

	SYSTEMTIME Time;
	GetLocalTime(&Time);

	if((Time.wHour == 2 && Time.wMinute == 0)
	|| (Time.wHour == 8 && Time.wMinute == 0)
	|| (Time.wHour == 12 && Time.wMinute == 0)
	|| (Time.wHour == 18 && Time.wMinute == 0)
	|| (Time.wHour == 22 && Time.wMinute == 0)
	|| (Time.wHour == 0 && Time.wMinute == 0)){

		LogAddTD("[GensSystem] Duprian and Vernet Ranking Generated");

		DGUpdateGensRanking();

		for(int X=OBJ_STARTUSERINDEX; X < OBJMAX;X++){
			this->GetGensInformation(X);
		}

		GCServerMsgStringSendAll("Atencion, el Ranking de las Familias fue Actualizado!",1);
	}
}

void GensSystem_UpdateTimerRank(void * lpParam){

	while(TRUE){
		GensSystem.ExecuteRankUpdate();
		Sleep(60000);
	}
	
	_endthread();
}