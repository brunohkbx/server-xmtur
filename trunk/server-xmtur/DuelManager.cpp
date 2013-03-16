#include "Stdafx.h"
#include "DuelManager.h"
#include "User.h"
#include "GameMain.h"
#include "LogProc.h"
#include "DevilSquare.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "IllusionTempleEvent.h"
#include "Doppelganger.h"
#include "ImperialGuardian.h"
#include "BuffManager.h"

CDuelManager DuelManager;

CDuelManager::CDuelManager(){
	//Clean All the Variables
	memset(DuelRoom,0,sizeof(DuelRoom));
	for(int X=0; X < MAX_DUEL_ROOMS;X++){
		this->DuelRoom[X].Free = TRUE;

		for(int I=0; I < MAX_DUEL_SPECS;I++){
			this->DuelRoom[X].Spectator[I] = -1;
		}
	}
}

CDuelManager::~CDuelManager(void){
}

void CDuelManager::StartProcess(){

	this->IsEnable = GetPrivateProfileInt("DuelManager","IsEnable",0,CONFIG_FILE);
	this->DuelCost = GetPrivateProfileInt("DuelManager","DuelArena_Cost",0,CONFIG_FILE);
	this->DuelMinLevel = GetPrivateProfileInt("DuelManager","DuelArena_MinLevel",0,CONFIG_FILE);
	this->EnableBuff = GetPrivateProfileInt("DuelManager","DuelArena_WinnerBuff",0,CONFIG_FILE);
	this->EnablePrize = GetPrivateProfileInt("DuelManager","DuelArena_WinnerPrize",0,CONFIG_FILE);
	
	LogAddTD("[DuelManager] Configuration Loaded");
	LogAddTD("[DuelManager] Cost: %d, Min Level: %d",this->DuelCost,this->DuelMinLevel);
}

void CDuelManager::RequestDuelNPC(int aIndex){

	PMSG_DUEL_STATUS pMsg = { 0xC1,0x00,0xAA,0x06 };
	
	pMsg.Size = sizeof(pMsg);

	for(int X=0; X < MAX_DUEL_ROOMS;X++){

		pMsg.RoomStatus[X].DuelRunning = 0;
		pMsg.RoomStatus[X].DuelOpen = 0;

		if(this->DuelRoom[X].Free == FALSE && 
		(this->DuelRoom[X].Dueler1 != -1 && this->DuelRoom[X].Dueler2 != -1)){
		
			memcpy(pMsg.RoomStatus[X].Name1,gObj[this->DuelRoom[X].Dueler1].Name,10);
			memcpy(pMsg.RoomStatus[X].Name2,gObj[this->DuelRoom[X].Dueler2].Name,10);

			pMsg.RoomStatus[X].DuelRunning = 1;
			pMsg.RoomStatus[X].DuelOpen = 1;
		}
	}

	BYTE Buff[4] = { 0xC3,0x04,0x30,0x21 };
	DataSend(aIndex,Buff,Buff[1]);
	DataSend(aIndex,(PBYTE)&pMsg,pMsg.Size);
}

void CDuelManager::DuelRequest(int aIndex, PMSG_DUEL_QUESTION_START * lpMsg){
	
	if(this->IsEnable != 1){
		GCServerMsgStringSend("Dueling was Disabled",aIndex,1);
		return;
	}

	int iDuelIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	LogAddTD("[DuelManager][%d] Duel Index (%d)",aIndex, iDuelIndex);

	if(iDuelIndex < OBJ_STARTUSERINDEX || iDuelIndex > (OBJMAX-1)){
		LogAddTD("[DuelManager][%d] Duel Index out of Range (%d)",aIndex, iDuelIndex);
		return;
	}

	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)){
		LogAddTD("[DuelManager][%d] Duel Index out of Range (%d)",iDuelIndex, aIndex);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[iDuelIndex];

	if(lpTargetObj->m_iDuelUser != -1 || lpObj->m_iDuelUser != -1){
		LogAddTD("[DuelManager] Some Dueler was Alerady Dueling");
		return;
	}

	PMSG_DUEL_START pAns = {0xC1, sizeof(pAns), 0xAA, 0x01};

	if(lpObj->DuelRoom != -1 || lpTargetObj->DuelRoom != -1){
		pAns.bDuelStart = 0x0F;
		DataSend(aIndex,(LPBYTE)&pAns,sizeof(pAns));
		LogAddTD("[DuelManager] Some Dueler was Alerady on Duel Arena");
		return;
	}

	if(aIndex == iDuelIndex){
		LogAddTD("[DuelManager][%d] Can't Duel with Yourself",aIndex);
		return;
	}

	if(lpObj->m_PK_Level > 3 || lpTargetObj->m_PK_Level > 3){
		GCServerMsgStringSend("Killers can't duel Now",aIndex,1);
		return;
	}

	if(lpObj->MapNumber != lpTargetObj->MapNumber){
		LogAddTD("[DuelManager][%d] Warriors are on Diferent Map (%d)",aIndex,iDuelIndex);
		return;
	}

	if(lpObj->PartyNumber != -1){
		GCServerMsgStringSend("Leave Party before Duel",aIndex,1);
		return;
	}

	if(lpTargetObj->PartyNumber != -1){
		GCServerMsgStringSend("Can't Duel while other User is in Party",aIndex,1);
		return;
	}

	if(lpObj->lpGuild && lpObj->lpGuild->WarState == 1){
		GCServerMsgStringSend("Can't Duel while you're in Guild War",aIndex,1);
		return;
	}

	if(lpTargetObj->lpGuild && lpTargetObj->lpGuild->WarState == 1){
		GCServerMsgStringSend("Can't Duel while other User is in Guild War",aIndex,1);
		return;
	}

	if(DS_MAP_RANGE(aIndex) ||
	BC_MAP_RANGE(aIndex) ||
	CC_MAP_RANGE(aIndex) ||
	IT_MAP_RANGE(aIndex) ||
	IMPERIAL_MAP_RANGE(aIndex) ||
	DOPPELGANGER_MAP_RANGE(aIndex) ||
	lpObj->MapNumber == 34 || lpObj->MapNumber == 31 || lpObj->MapNumber == 30){
		GCServerMsgStringSend("Can't duel from Here",aIndex,1);
		return;
	}

	for(int X= 0;X < 7;X++){
		if(lpObj->SelfDefense[X] >= 0 || lpTargetObj->SelfDefense[X] >= 0){
			return;
		}
	}

	if(lpObj->m_IfState.use > 0 || lpTargetObj->m_IfState.use > 0){
		return;
	}

	if(lpTargetObj->DuelRoom != -1){
		return;
	}

	if(lpTargetObj->CloseCount >= 0) return;
	if(lpTargetObj->Connected != 3) return;
	if(lpTargetObj->Type != OBJ_USER) return;

	if(lpObj->Level < this->DuelMinLevel || lpTargetObj->Level < this->DuelMinLevel){
		pAns.bDuelStart = 0x0C;
		DataSend(aIndex,(LPBYTE)&pAns,sizeof(pAns));
		return;
	}

	if(lpObj->Money < this->DuelCost || lpTargetObj->Money < this->DuelCost){
		pAns.bDuelStart = 0x1E;
		DataSend(aIndex,(LPBYTE)&pAns,sizeof(pAns));
		return;
	}

	char bRoom = this->ReserveDuelRoom(aIndex, iDuelIndex);

	if(bRoom == -1){
		pAns.bDuelStart = 0x10;
		DataSend(aIndex,(LPBYTE)&pAns,sizeof(pAns));
		return;
	}

	lpObj->m_iDuelUserRequested = 1;
	lpObj->m_iDuelUserReserved = iDuelIndex;
	lpObj->DuelRoom = bRoom;

	lpTargetObj->m_iDuelUserReserved = aIndex;
	lpTargetObj->DuelRoom = bRoom;

	LogAddTD("[DuelManager][%d][%s][%s] Duel Request Send (%d)(Room: %d)",
		aIndex, lpObj->AccountID, lpObj->Name, iDuelIndex, bRoom);

	PMSG_DUEL_QUESTION_START pMsg = {0xC1, sizeof(pMsg), 0xAA, 0x02};
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	memcpy(pMsg.szName,lpObj->Name,10);

	DataSend(iDuelIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}

void CDuelManager::DuelResponse(int aIndex,PMSG_DUEL_ANSWER_START* lpMsg){

	int iDuelIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if(iDuelIndex < OBJ_STARTUSERINDEX || iDuelIndex > (OBJMAX-1)){
		LogAddTD("[DuelManager][%d] Duel Index out of Range (%d)",aIndex, iDuelIndex);
		return;
	}

	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)){
		LogAddTD("[DuelManager][%d] Duel Index out of Range (%d)",iDuelIndex, aIndex);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[iDuelIndex];

	PMSG_DUEL_START pMsg;
	pMsg.H = 0xC1;
	pMsg.Headcode = 0xAA;
	pMsg.Subcode = 0x01;
	pMsg.bDuelStart = 0;
	pMsg.Size = sizeof(pMsg);
	
	short Room = lpObj->DuelRoom;

	if(DuelManager.DuelRoom[Room].Waiting == FALSE){
		lpObj->m_iDuelUserRequested = 0;
		lpObj->m_iDuelUserReserved = -1;
		lpObj->DuelRoom = -1;

		lpTargetObj->m_iDuelUserReserved = -1;
		lpTargetObj->DuelRoom = -1;

		pMsg.bDuelStart = 0xF;
		memcpy(pMsg.szName,lpObj->Name,sizeof(pMsg.szName));
		DataSend(iDuelIndex,(LPBYTE)&pMsg,pMsg.Size);

		LogAddTD("[DuelManager][%d][%s][%s] Duel Time Out [%d][%s][%s] ",
			aIndex, lpObj->AccountID, lpObj->Name, iDuelIndex, lpTargetObj->AccountID, lpTargetObj->Name);
		return;
	}


	if(lpMsg->bDuelOK == 1){ //Duel Start
		
		lpObj->Money -= this->DuelCost;
		lpTargetObj->Money -= this->DuelCost;
		GCMoneySend(lpObj->m_Index,lpObj->Money);
		GCMoneySend(iDuelIndex,lpTargetObj->Money);

		lpTargetObj->m_iDuelUserReserved = -1;
		lpTargetObj->m_btDuelScore = 0;
		lpTargetObj->m_iDuelUser = lpObj->m_Index;
		lpTargetObj->m_iDuelTickCount = GetTickCount();

		lpObj->m_iDuelUserRequested = -1;
		lpObj->m_iDuelUserReserved = -1;
		lpObj->m_btDuelScore = 0;
		lpObj->m_iDuelUser = lpTargetObj->m_Index;
		lpObj->m_iDuelTickCount = GetTickCount();

		//Send to Dueler 1
		pMsg.NumberH = SET_NUMBERH(iDuelIndex);
		pMsg.NumberL = SET_NUMBERL(iDuelIndex);

		memcpy(pMsg.szName,lpTargetObj->Name,sizeof(pMsg.szName));
		DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.Size);

		gObjMoveGate(aIndex,(295+(Room*2)));

		//Send to Dueler 2
		pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
		pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

		memcpy(pMsg.szName,lpObj->Name,sizeof(pMsg.szName));
		DataSend(lpTargetObj->m_Index,(LPBYTE)&pMsg,pMsg.Size);

		gObjMoveGate(lpTargetObj->m_Index,(296+(Room*2)));

		//Set DuelRoom End Time
		this->DuelRoom[Room].dwTickCount = GetTickCount() + (30 * 60000);
		this->DuelRoom[Room].Dueler1 = aIndex;
		this->DuelRoom[Room].Points1 = 0;
		this->DuelRoom[Room].Dueler2 = iDuelIndex;
		this->DuelRoom[Room].Points2 = 0;
		this->DuelRoom[Room].Free = FALSE;
		this->DuelRoom[Room].Waiting = FALSE;

		//Close Personal Store
		if(lpObj->m_bPShopOpen == TRUE) CGPShopReqClose(aIndex);
		if(lpTargetObj->m_bPShopOpen == TRUE) CGPShopReqClose(iDuelIndex);
		
		LogAddTD("[DuelManager][%d][%s][%s] Duel Accepted by [%d][%s][%s] ",
			aIndex, lpObj->AccountID, lpObj->Name, iDuelIndex, lpTargetObj->AccountID, lpTargetObj->Name);
		return;
	} else {
		lpObj->m_iDuelUserRequested = 0;
		lpObj->m_iDuelUserReserved = -1;
		lpObj->DuelRoom = -1;

		lpTargetObj->m_iDuelUserReserved = -1;
		lpTargetObj->DuelRoom = -1;

		pMsg.bDuelStart = 0xF;
		memcpy(pMsg.szName,lpObj->Name,sizeof(pMsg.szName));
		DataSend(iDuelIndex,(LPBYTE)&pMsg,pMsg.Size);

		LogAddTD("[DuelManager][%d][%s][%s] Duel Rejected by [%d][%s][%s] ",
			aIndex, lpObj->AccountID, lpObj->Name, iDuelIndex, lpTargetObj->AccountID, lpTargetObj->Name);
		return;
	}

}

void CDuelManager::SendSpectatorList(int aIndex, int Room)
{
	if(Room < 0 || Room >= MAX_DUEL_ROOMS) return;
	
	if(this->DuelRoom[Room].Dueler1 == -1 || this->DuelRoom[Room].Dueler2 == -1){
		return;
	}

	PMSG_DUEL_SPEC_LIST pMsg;
	pMsg.H = 0xC1;
	pMsg.Headcode = 0xAA;
	pMsg.Subcode = 0x0B;
	pMsg.btCount = 0;

	for(int X = 0; X < MAX_DUEL_SPECS; X++){
		if(this->DuelRoom[Room].Spectator[X] != -1){
			memcpy(&pMsg.szName[pMsg.btCount++][0],gObj[this->DuelRoom[Room].Spectator[X]].Name,10);
		}
	}
	
	pMsg.Size = 5 + (pMsg.btCount * 10);
	DataSend(aIndex,(BYTE*)&pMsg,pMsg.Size);
}

void CDuelManager::DuelWatchRequest(int aIndex,PMSG_DUEL_WATCH_REQ* lpMsg){

	int Room = lpMsg->Room;
	if(this->DuelRoom[Room].Free == TRUE) return;

	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)){
		LogAddTD("[DuelManager][%d] Duel Index out of Range",aIndex);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];

	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)){
		LogAddTD("[DuelManager][%d][%s][%s] Spectator Index out of Range",
			aIndex, lpObj->AccountID, lpObj->Name);
		return;
	}

	int SpecPlace = this->ReserveWatchPlace(Room);

	if(SpecPlace == -1){
		GCServerMsgStringSend("Can't enter Now, Room is Full",aIndex,1);
		return;
	}

	this->DuelRoom[Room].Spectator[SpecPlace] = aIndex;

	lpObj->DuelRoom = Room;

	gObjApplyBuffEffectDuration(lpObj, 98, 0, 0, 0, 0, -10); //Add Duel Interface
	gObjApplyBuffEffectDuration(lpObj, 18, 0, 0, 0, 0, -10); //Set Invisibility

	this->DuelIntefaceInit(aIndex,Room);

	gObjMoveGate(aIndex,(303+(1*Room)));
}

void CDuelManager::DuelRemoveRequest(int aIndex){
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)){
		LogAddTD("[DuelManager][%d] Duel Index out of Range", aIndex);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	short Room = lpObj->DuelRoom;
	if(Room == -1) return;

	for(int X=0; X < MAX_DUEL_SPECS;X++){
		if(this->DuelRoom[Room].Spectator[X] == aIndex)

		lpObj->DuelRoom = -1;

		gObjMoveGate(aIndex,17);

		gObjApplyBuffEffectDuration(lpObj, 98, 0, 0, 0, 0, 0); //Remove Duel Interface
		gObjApplyBuffEffectDuration(lpObj, 18, 0, 0, 0, 0, 0); //Set Visibility
	}
}

void CDuelManager::DuelIntefaceStatus(int aIndex, int Room)
{
	if(Room < 0 || Room >= MAX_DUEL_ROOMS) return;

	if(this->DuelRoom[Room].Dueler1 == -1 || this->DuelRoom[Room].Dueler2 == -1) return;
	
	LPOBJ Player1 = &gObj[this->DuelRoom[Room].Dueler1];
	LPOBJ Player2 = &gObj[this->DuelRoom[Room].Dueler2];

	PMSG_DUEL_LIFEBAR_REFILL pMsg;

	pMsg.H = 0xC1;
	pMsg.Size = sizeof(pMsg);
	pMsg.Headcode = 0xAA;
	pMsg.Subcode = 0x05;

	pMsg.Player1H = HIBYTE(Player1->m_Index);
	pMsg.Player1L = LOBYTE(Player1->m_Index);

	pMsg.Player2H = HIBYTE(Player2->m_Index);
	pMsg.Player2L = LOBYTE(Player2->m_Index);

	pMsg.Life1 = (BYTE)(Player1->Life / ((Player1->MaxLife + Player1->AddLife) / 100));
	pMsg.Life2 = (BYTE)(Player2->Life / ((Player2->MaxLife + Player2->AddLife) / 100));

	pMsg.Shield1 = Player1->iShield / ((Player1->iMaxShield + Player1->iAddShield) / 100);
	pMsg.Shield2 = Player2->iShield / ((Player2->iMaxShield + Player2->iAddShield) / 100);

	DataSend(aIndex,(PBYTE)&pMsg,pMsg.Size);
}

void CDuelManager::DuelIntefaceInit(int aIndex, int Room)
{
	if(Room < 0 || Room >= MAX_DUEL_ROOMS) return;

	if(this->DuelRoom[Room].Dueler1 == -1 || this->DuelRoom[Room].Dueler2 == -1) return;

	LPOBJ Player1 = &gObj[this->DuelRoom[Room].Dueler1];
	LPOBJ Player2 = &gObj[this->DuelRoom[Room].Dueler2];

	PMSG_DUEL_LIFEBAR_NAME pMsg;
	pMsg.H = 0xC1;
	pMsg.Size = sizeof(pMsg);
	pMsg.Headcode = 0xAA;
	pMsg.Subcode = 0x07;
	pMsg.Type = 0x00;
	
	memcpy(pMsg.Name1,Player1->Name,10);
	memcpy(pMsg.Name2,Player2->Name,10);

	pMsg.Player1H = HIBYTE(Player1->m_Index);
	pMsg.Player1L = LOBYTE(Player1->m_Index);
	pMsg.Player2H = HIBYTE(Player2->m_Index);
	pMsg.Player2L = LOBYTE(Player2->m_Index);

	DataSend(aIndex,(PBYTE)&pMsg, pMsg.Size);

	PMSG_DUEL_LIFEBAR_INIT pInit;
	pInit.H = 0xC1;
	pInit.Size = sizeof(pMsg);
	pInit.Headcode = 0xAA;
	pInit.Subcode = 0x0D;

	DataSend(aIndex,(PBYTE)&pInit,pInit.Size);
}

///------------------------------------------------------------------
/// Check Functions
char CDuelManager::ReserveDuelRoom(int aIndex, int aTarget){

	for(int X=0; X < MAX_DUEL_ROOMS;X++){

		if(this->DuelRoom[X].Free == TRUE && this->DuelRoom[X].Waiting == FALSE){
			this->DuelRoom[X].Waiting = TRUE;
			this->DuelRoom[X].dwWaitTime = GetTickCount() + 10000;
			return X;
		}
	}

	return -1;
}

char CDuelManager::ReserveWatchPlace(int Room){

	for(int X=0; X < MAX_DUEL_SPECS;X++){
		if(this->DuelRoom[Room].Spectator[X] == -1) return X;
	}

	return -1;
}

void CDuelManager::UpdateDuelScore(int Room) 
{ 
	if(Room < 0 || Room > MAX_DUEL_ROOMS - 1) return;

	if(this->DuelRoom[Room].Free == TRUE) return;
	
	LPOBJ Player1 = &gObj[this->DuelRoom[Room].Dueler1];
	LPOBJ Player2 = &gObj[this->DuelRoom[Room].Dueler2];

	if(Player1->Connected != 3 || Player2->Connected != 3) return;

	this->DuelRoom[Room].Points1 = Player1->m_btDuelScore;
	this->DuelRoom[Room].Points2 = Player2->m_btDuelScore;

	PMSG_DUEL_SCORE pMsg;

	pMsg.H = 0xC1;
	pMsg.Size = sizeof(pMsg);
	pMsg.Headcode = 0xAA;
	pMsg.Subcode = 0x04;
	pMsg.NumberH1 = SET_NUMBERH(Player1->m_Index);
	pMsg.NumberL1 = SET_NUMBERL(Player1->m_Index);
	pMsg.NumberH2 = SET_NUMBERH(Player2->m_Index);
	pMsg.NumberL2 = SET_NUMBERL(Player2->m_Index);
	pMsg.btDuelScore1 = this->DuelRoom[Room].Points1;
	pMsg.btDuelScore2 = this->DuelRoom[Room].Points2;

	DataSend(Player1->m_Index,(PBYTE)&pMsg,pMsg.Size);
	DataSend(Player2->m_Index,(PBYTE)&pMsg,pMsg.Size);


	for(int X=0; X < MAX_DUEL_SPECS;X++){
		if(this->DuelRoom[Room].Spectator[X] != -1){
			DataSend(this->DuelRoom[Room].Spectator[X],(PBYTE)&pMsg,pMsg.Size);
			this->DuelIntefaceStatus(this->DuelRoom[Room].Spectator[X],Room);
			this->SendSpectatorList(this->DuelRoom[Room].Spectator[X],Room);
		}
	}
}

void CDuelManager::CheckRoomStatus(int Room){

	if(this->DuelRoom[Room].Free == FALSE && 
		(this->DuelRoom[Room].Dueler1 != -1 && this->DuelRoom[Room].Dueler2 != -1)){
		
		if(gObjIsConnected(this->DuelRoom[Room].Dueler1) == FALSE 
			|| gObjIsConnected(this->DuelRoom[Room].Dueler2) == FALSE){
			this->MoveDuelers(Room);
		}

		if(gObj[this->DuelRoom[Room].Dueler1].MapNumber != 64 
			|| gObj[this->DuelRoom[Room].Dueler2].MapNumber != 64){
			this->MoveDuelers(Room);
		}

		if(this->DuelRoom[Room].dwTickCount < GetTickCount()){
			this->MoveDuelers(Room);
		}

		if((this->DuelRoom[Room].Points1 >= 10 || this->DuelRoom[Room].Points2 >= 10)
			&& this->DuelRoom[Room].DuelFinish == FALSE){
			this->DuelRoom[Room].DuelFinish = TRUE;
			this->DuelWinner(Room);
		}
	}
}

void CDuelManager::DuelWinner(int Room){

	int WinnerIndex;

	PMSG_DUEL_FINISH pMsg;

	pMsg.H = 0xC1;
	pMsg.Size = sizeof(pMsg);
	pMsg.Headcode = 0xAA;
	pMsg.Subcode = 0x0C;
	
	LPOBJ Player1 = &gObj[this->DuelRoom[Room].Dueler1];
	LPOBJ Player2 = &gObj[this->DuelRoom[Room].Dueler2];

	if(this->DuelRoom[Room].Points1 > this->DuelRoom[Room].Points2){
		memcpy(pMsg.szWinner,gObj[this->DuelRoom[Room].Dueler1].Name,10);
		memcpy(pMsg.szLooser,gObj[this->DuelRoom[Room].Dueler2].Name,10);
		WinnerIndex = this->DuelRoom[Room].Dueler1;
	} else if(this->DuelRoom[Room].Points1 < this->DuelRoom[Room].Points2){
		memcpy(pMsg.szWinner,gObj[this->DuelRoom[Room].Dueler2].Name,10);
		memcpy(pMsg.szLooser,gObj[this->DuelRoom[Room].Dueler1].Name,10);
		WinnerIndex = this->DuelRoom[Room].Dueler2;
	}

	gObjApplyBuffEffectDuration(&gObj[WinnerIndex],103,0,0,0,0,1800);

	DataSend(this->DuelRoom[Room].Dueler1,(PBYTE)&pMsg,pMsg.Size);
	DataSend(this->DuelRoom[Room].Dueler2,(PBYTE)&pMsg,pMsg.Size);

	PMSG_DUEL_CANCEL pMsg2 = { 0xC1,0x05,0xAA,0x03,0x00 };
	DataSend(this->DuelRoom[Room].Dueler1,(PBYTE)&pMsg2,pMsg2.Size);
	DataSend(this->DuelRoom[Room].Dueler2,(PBYTE)&pMsg2,pMsg2.Size);

	this->DuelRoom[Room].dwMoveTime = GetTickCount() + 10000;
}

void CDuelManager::MoveDuelers(int Room){

	int Player1, Player2;

	Player1 = this->DuelRoom[Room].Dueler1;
	Player2 = this->DuelRoom[Room].Dueler1;

	if(Player1 < (OBJMAX-OBJMAXUSER) || Player1 > (OBJMAX-1)) return;
	if(!gObjIsConnected(Player1)) return;

	if(Player2 < (OBJMAX-OBJMAXUSER) || Player2 > (OBJMAX-1)) return;
	if(!gObjIsConnected(Player2)) return;

	LPOBJ lpPlayer1 = &gObj[Player1];
	LPOBJ lpPlayer2 = &gObj[Player2];

	if(gObjIsConnected(lpPlayer1->m_Index) == TRUE 
	&& lpPlayer1->MapNumber == 64){
		gObjMoveGate(lpPlayer1->m_Index,17);
	}

	if(gObjIsConnected(lpPlayer2->m_Index) == TRUE 
	&& lpPlayer2->MapNumber == 64){
		gObjMoveGate(lpPlayer2->m_Index,17);
	}

	LogAddTD("[DuelManager] Players Moved");

	this->FinishDuel(lpPlayer1->m_Index);
	this->FinishDuel(lpPlayer2->m_Index);

	
	LogAddTD("[DuelManager] Finished Duel");

	for(int X=0; X < MAX_DUEL_SPECS;X++){
		if(this->DuelRoom[Room].Spectator[X] != -1){
			this->DuelRemoveRequest(this->DuelRoom[Room].Spectator[X]);
		}
	}
	
	this->RoomReset(Room);
	
	LogAddTD("[DuelManager] Room Reset Request Success");
}

void CDuelManager::RoomReset(int Room){

	this->DuelRoom[Room].Free = TRUE;
	this->DuelRoom[Room].Waiting = FALSE;

	this->DuelRoom[Room].Dueler1 = -1;
	this->DuelRoom[Room].Dueler2 = -1;
	this->DuelRoom[Room].Points1 = 0;
	this->DuelRoom[Room].Points2 = 0;

	this->DuelRoom[Room].dwTickCount = 0;
	this->DuelRoom[Room].dwWaitTime = 0;
	this->DuelRoom[Room].dwMoveTime = 0;

	this->DuelRoom[Room].DuelFinish = FALSE;

	for(int X=0; X < MAX_DUEL_SPECS;X++){
		this->DuelRoom[Room].Spectator[X] = -1;
	}

	
	LogAddTD("[DuelManager] Room Reseted");
}

BOOL CDuelManager::IsSpectator(int Room, int aIndex){
	
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return FALSE;
	if(!gObjIsConnected(aIndex)) return FALSE;

	for(int X=0; X < MAX_DUEL_SPECS;X++){
		if(this->DuelRoom[Room].Spectator[X] == aIndex) return TRUE;
	}

	return FALSE;
}

void CDuelManager::FinishDuel(int aIndex) 
{ 
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	lpObj->m_iDuelUser = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_btDuelScore = 0;
	lpObj->m_iDuelTickCount = 0;

	lpObj->DuelRoom = -1;
	lpObj->DuelUserReserved = -1;

	PMSG_DUEL_CANCEL pMsg = { 0xC1,0x05,0xAA,0x03,0x00 };
	DataSend(aIndex,(PBYTE)&pMsg,pMsg.Size);
}

///------------------------------------------------------------------
/// Threads Check
void CDuelManager::Run(){
	
	for(int n=0;n<OBJMAX;n++){

		if(gObj[n].MapNumber == 64){
			gObjStateSetCreate(n);
			gObjViewportListDestroy(n);
			gObjViewportListCreate(n);
			gObjViewportListProtocol(n);
		}
	}

	//Check for Wait Time
	for(int X=0; X < MAX_DUEL_ROOMS;X++){

		DuelManager.CheckRoomStatus(X);
		DuelManager.UpdateDuelScore(X);

		if(DuelManager.DuelRoom[X].dwMoveTime > 0 &&
			(DuelManager.DuelRoom[X].dwMoveTime < GetTickCount())){
			DuelManager.MoveDuelers(X);
		}

		if(DuelManager.DuelRoom[X].dwWaitTime < GetTickCount()){
			DuelManager.DuelRoom[X].Waiting = FALSE;
		}
	}
}

void CDuelManager::Core(int aIndex, unsigned char* aRecv){

	if(aRecv[3] == 0x01){ //Duel Request
		this->DuelRequest(aIndex,(PMSG_DUEL_QUESTION_START*)aRecv);
	} else if(aRecv[3] == 0x02){ //Duel Answer
		this->DuelResponse(aIndex,(PMSG_DUEL_ANSWER_START*)aRecv);
	} else if(aRecv[3] == 0x07){ //Spectator Request
		this->DuelWatchRequest(aIndex,(PMSG_DUEL_WATCH_REQ*)aRecv);
	} else if(aRecv[3] == 0x09){ //Spectator Quit
		this->DuelRemoveRequest(aIndex);
	}
}