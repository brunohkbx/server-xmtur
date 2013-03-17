#include "Stdafx.h"
#include "User.h"
#include "Doppelganger.h"
#include "LogProc.h"
#include "Readscript.h"
#include "DSProtocol.h"
#include "BuffManager.h"
#include "GameMain.h"
#include "TMonsterAIUtil.h"
#include "Winutil.h"

CDoppelganger Doppelganger;

static TMonsterAIUtil MonsterAIUtil;

void CDoppelganger::MoveProc(LPOBJ lpObj)
{
	if(DOPPELGANGER_MAP_RANGE(lpObj->MapNumber) && lpObj->Type == OBJ_MONSTER){
		
		for(int X=0; X < this->MonstersCount;X++){
			
			if(this->CurrentMonster[X] == lpObj->m_Index){
				lpObj->MTX = this->PosX[this->EventMap-65];
				lpObj->MTY = this->PosY[this->EventMap-65];
				MonsterAIUtil.FindPathToMoveMonster(lpObj,lpObj->MTX,lpObj->MTY,10,2);
			}
		}
	}
}


void Doppelganger_CheckCore(void * lpParam);

void CDoppelganger::StartProcess(BOOL Reload)
{
	this->State = NONE; //0: Not Runing, 1: Preparing, 2: Started, 3: StandBy
	this->PlayersCount = 0;
	this->Timer = 599;
	this->MonsterPassed = 0;

	this->EventMap = 64;

	this->OnlyInParty = GetPrivateProfileIntA("Doppelganger","EnterInParty",0,"..\\Data\\Events\\Doppelganger.dat");
	this->SilverChest_DropRate = GetPrivateProfileIntA("Doppelganger","SilverChestDropRate",50,"..\\Data\\Events\\Doppelganger.dat");
	this->StandByTime = GetPrivateProfileIntA("Doppelganger","StandByMinutes",60,"..\\Data\\Events\\Doppelganger.dat");

	this->PieceofSpeculumDropRate = GetPrivateProfileIntA("Doppelganger","PieceOfSpeculumDropRate",60,"..\\Data\\Events\\Doppelganger.dat");

	memset(this->Player,-1,sizeof(this->Player));
	memset(this->MonsterBoss,-1,sizeof(this->MonsterBoss));
	memset(this->Monster,-1,sizeof(this->Monster));
	memset(this->CurrentMonster,-1,sizeof(this->CurrentMonster));

	//Load All Shit
	this->LoadItemBag(); //Silver & Golden Chest

	//Set Monsters Codinates
	this->PosX[0] = 200, this->PosY[0] = 32;
	this->PosX[1] = 140, this->PosY[1] = 70;
	this->PosX[2] = 112, this->PosY[2] = 65;
	this->PosX[3] = 98, this->PosY[3] = 21;

	//Monster Start Coord (Not in Use)
	this->MPosX[0] = 224, this->MPosY[0] = 100;
	this->MPosX[1] = 113, this->MPosY[1] = 180;
	this->MPosX[2] = 110, this->MPosY[2] = 150;
	this->MPosX[3] = 43, this->MPosY[3] = 108;

	if(Reload == 0){
		//Start Cores
		_beginthread(Doppelganger_CheckCore,0,NULL);
	}
}

void CDoppelganger::Lugard(int aIndex)
{
	PMSG_DGOER_EVENT_INFO pMsg;

	pMsg.C = 0xC3;
	pMsg.Size = 0x05;
	pMsg.Headcode = 0x30;
	pMsg.Subcode = 0x23;
	pMsg.Minutes = 0;

	if(this->State == STANDBY){
		pMsg.Minutes = (BYTE)(this->StandbyTimer / 60) + 1;
	}

	DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
}

void CDoppelganger::EnterDoppelganger(int aIndex,int Invitation)
{
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	if(this->State != NONE) return;
	
	//Get Map Number
	this->EventMap += 1;
	if(this->EventMap > 68) this->EventMap = 65;

	int Gate = 264 + this->EventMap;

	if(this->OnlyInParty == 1 && lpObj->PartyNumber == -1){
		GCServerMsgStringSend("[Doppelganger] Event only for Party.",aIndex,1);
		return;
	}

	this->EventLevel = lpObj->Level / 10;

	//Set Player who Give the Ticket as Principal Player
	this->Player[0] = aIndex;
	LogAddTD("[Doppelganger][%d] Add Player: %d %s",this->PlayersCount,lpObj->m_Index,lpObj->Name);
	this->PlayersCount += 1;

	this->PartyNumber = lpObj->PartyNumber;

	if(lpObj->PartyNumber != -1)
	{	
		for(int i=OBJ_STARTUSERINDEX; i < OBJMAX;i++){	
			if(this->PlayersCount < 5 && gObjIsConnected(i)){
				
				LPOBJ gTargetObj = &gObj[i];
	
				if(gTargetObj->PartyNumber == lpObj->PartyNumber && gTargetObj->m_Index != aIndex && gTargetObj->PartyNumber >= 0){	

					this->Player[this->PlayersCount] = gTargetObj->m_Index;
					
					if(this->EventLevel < (gTargetObj->Level / 10)) this->EventLevel = gTargetObj->Level / 10;
	
					LogAddTD("[Doppelganger][%d] Add Player: %d %s",this->PlayersCount,gTargetObj->m_Index,gTargetObj->Name);
					gObjMoveGate(i,Gate);
					this->PlayersCount++;
				}
			}
		}
	}

	
	//Delete Ticket
	gObjInventoryDeleteItem(aIndex,Invitation);
	GCInventoryItemDeleteSend(aIndex,Invitation,1);
	
	gObjMoveGate(aIndex,Gate);

	for(int X=0; X < 5;X++){
		if(this->Player[X] >= OBJ_STARTUSERINDEX){
			this->ChangeMapAttr(TRUE,Doppelganger.Player[X]); //Block Map Entrance
			this->SendTimeInfo(17,Doppelganger.Player[X]); //Send 30 Seconds to Start
		}
	}

	this->dwTickCount = GetTickCount(); //Set Start Time
	this->State = PREPARING;
	
	LogAddTD("[Doppelganger] SetState->PREPARING");
	LogAddTD("[Doppelganger] Players: %d/5, Event Map: %d, Level: %d",this->PlayersCount,this->EventMap,this->EventLevel);
}
/////////////////////////////////////////////////////////
// Doppelganger STATES

void CDoppelganger::State_STANDBY(){

	this->StandbyTimer -= 1;

	if(this->StandbyTimer == ((this->StandByTime*60)-30)){ //30 Seconds to Move when Finish
		for(int X=0; X < 5;X++){
			
			if(this->Player[X] >= OBJ_STARTUSERINDEX && gObjIsConnected(this->Player[X])){

				if(gObj[this->Player[X]].MapNumber == this->EventMap){
					gObjMoveGate(this->Player[X],267);
				}

				this->Player[X] = -1;
			}

		}

		LogAddTD("[Doppelganger] Remove Players from Event Map");

		///Set Variables
		this->Timer = 0;
		this->PlayersCount = 0;
		this->MonsterPassed = 0;
	}

	if(this->StandbyTimer == 0){
		this->State = NONE;
		LogAddTD("[Doppelganger] SetState->NONE");
	}
}

void CDoppelganger::State_PREPARING(){
	
	//Check if Players was in Event
	this->CheckPartyMembers();

	if(GetTickCount() > (this->dwTickCount + 30000)){ //30 Seconds

		//Init Timer
		PMSG_DGOER_TIMER_INIT Timer = { 0xC1,0x05,0xBF,0x10,0x02 };

		for(int X=0; X < 5; X++){
			if(this->Player[X] >= OBJ_STARTUSERINDEX && gObjIsConnected(this->Player[X])){ 
				DataSend(this->Player[X],(LPBYTE)&Timer,Timer.Size);
				this->ChangeMapAttr(FALSE,Doppelganger.Player[X]); //Unblock Map
			}
		}

		//Change State to Running
		this->State = RUNNING;
		LogAddTD("[Doppelganger] SetState->RUNNING");
				
		this->CreateSlaughterer();
		this->Timer = 599;
	}
}

void CDoppelganger::State_RUNNING(){

	//Check if Players was in Event
	this->CheckPartyMembers();

	//Rest Time
	this->Timer -= 1;

	if(this->Timer == 150){
		this->CreateIceWalker();

		for(int X=0; X < 5;X++){
			if(this->Player[X] >= OBJ_STARTUSERINDEX && gObjIsConnected(this->Player[X])){
				if(gObj[this->Player[X]].MapNumber == this->EventMap){
					GCServerMsgStringSendAll("[Doppelganger] El Ice Walker ha Aparecido, Eliminalo antes que el tiempo Termine!",0);
				}
			}
		}
	}


	//30 Seconds to Finish Event
	if(this->Timer == 30 && this->MonsterBoss[2] > 0){
		//Send Finish Packet's
		for(int X=0; X < 5;X++){
			if(this->Player[X] >= OBJ_STARTUSERINDEX && gObjIsConnected(this->Player[X])){
				if(gObj[this->Player[X]].MapNumber == this->EventMap){
					this->SendTimeInfo(19,this->Player[X]); //Send 30 Seconds to Ice Walker Appears
				}
			}
		}
	}
	
	this->MonstersProcess();

	//Check Party Members Status
	this->CheckPartyStatus();

	for(int X=0; X < 5; X++){ //Here send all Information to Party Members
		if(this->Player[X] >= OBJ_STARTUSERINDEX && gObjIsConnected(this->Player[X])){ 
			this->SendTrianglesPosition(this->Player[X]);
		}
	}
}


void CDoppelganger::State_FINISHED()
{
	//Clear Monsters
	this->ClearMonsters();

	//Finish Packet
	PMSG_DGOER_END Finish = { 0xC1,0x05,0xBF,0x13 };
	Finish.Status = this->Status;

	//Send Finish Packet's
	for(int X=0; X < 5;X++){
		if(this->Player[X] >= OBJ_STARTUSERINDEX && gObjIsConnected(this->Player[X])){

			DataSend(this->Player[X],(LPBYTE)&Finish,Finish.Size);

			if(gObj[this->Player[X]].MapNumber == this->EventMap){
				this->SendTimeInfo(19,this->Player[X]); //Send 30 Seconds to Finish
			}
		}
	}

	//Remove TickCount
	this->dwTickCount = 0;

	//Set Standby for 10 Minutes
	this->StandbyTimer = this->StandByTime * 60; //(10 Min * 60 = 600 Seconds)
	this->State = STANDBY;

	LogAddTD("[Doppelganger] SetState->STANDBY");
}


void Doppelganger_CheckCore(void * lpParam)
{
	while(true){	

		switch(Doppelganger.State){

			case NONE:
			break;

			case PREPARING:
				Doppelganger.State_PREPARING();
			break;

			case RUNNING:
				Doppelganger.State_RUNNING();
			break;
			
			case FINISHED:
				Doppelganger.State_FINISHED();
			break;
	
			case STANDBY:
				Doppelganger.State_STANDBY();
			break;
		}
		
		Sleep(1000);
	}

	_endthread();
}


///////////////////////////////////////////////////////
// Doppelganger Checks


void CDoppelganger::CheckMonsterPassed(){

	
	if(this->State != RUNNING) return;

	for(int i=0; i < (OBJ_STARTUSERINDEX-1);i++){

		if(DOPPELGANGER_MAP_RANGE(gObj[i].MapNumber)){

			if((gObj[i].Class >= 533 || gObj[i].Class <= 539) && gObj[i].Live){
				if(gObj[i].X <= this->PosX[this->EventMap-65] && gObj[i].Y <= this->PosY[this->EventMap-65]){
					gObj[i].Life = FALSE;
					gObjDel(i);
					this->MonsterPassed++;
					LogAddTD("[Doppelganger][%d] Monster Passed to Protected Zone",gObj[i].m_Index);
				}
			}
		}
	}
}

void CDoppelganger::CheckPartyMembers(){
	
	for(int X=0; X < 5; X++){ //Here send all Information to Party Members

		if(this->Player[X] >= OBJ_STARTUSERINDEX){ 

			//Check if Principal Player was Online in Party if was Required and in Event Map
			if(gObjIsConnected(this->Player[X]) == FALSE || gObj[this->Player[X]].MapNumber != this->EventMap){

				if(X == 0){ //Player 0, the Party Creator
					this->Status = FAILED; //Failed
					this->State = FINISHED;
					LogAddTD("[Doppelganger] Main Party Member was Out of Event - EVENT FINISH FAILED");
				} else {
					this->Player[X] = -1;
				}
			}
		}
	}
}

void CDoppelganger::CheckPartyStatus(){
	
	for(int X=0; X < 5;X++){
		if(this->Player[X] >= OBJ_STARTUSERINDEX && gObj[this->Player[X]].MapNumber == this->EventMap && gObjIsConnected(this->Player[X])){

			LPOBJ gTargetObj = &gObj[this->Player[X]];
	
			if(gTargetObj->PartyNumber != this->PartyNumber){	
				gObjMoveGate(this->Player[X],267);
				this->Player[X] = -1;

				if(X==0){
					LogAddTD("[Doppelganger] Party Leader has not in Party or Party was Disolved - EVENT FINISH FAILED");
					this->Status = FAILED; //Success
					this->State = FINISHED;
					return;
				}
			}
		}
	}
}


void CDoppelganger::SendTrianglesPosition(int aIndex){

	if(this->State != RUNNING 
	|| gObj[aIndex].MapNumber != this->EventMap 
	|| !gObjIsConnected(aIndex)
	|| aIndex < OBJ_STARTUSERINDEX) return;

	//Double Goer Set Line Position  
	PMSG_DGOER_TRIANG_POS LinePosition = { 0xC1, sizeof(LinePosition), 0xBF, 0x0F };
	LinePosition.Pos = this->GetPosition(aIndex);

	//Update Bar(Triangles)
	PMSG_DGOER_UPDATE_BAR Position;
	Position.C = 0xC1;
	Position.Size = sizeof(Position);
	Position.Headcode = 0xBF;
	Position.Subcode = 0x12;
	Position.Timer = this->Timer;
	Position.Players = this->PlayersCount;

	//Set Triangles Position...
	memset(Position.Player,0,sizeof(Position.Player));

	Position.Player[0].Index = aIndex;
	Position.Player[0].Visible = 0;
	Position.Player[0].Distance = this->GetPosition(aIndex);

	int Count = 1;

	for(int i=0; i < 5;i++){
		
		if(this->Player[i] != -1 && gObj[this->Player[i]].MapNumber == this->EventMap && this->Player[i] != aIndex){
			Position.Player[Count].Index = this->Player[i];
			Position.Player[Count].Visible = 0;
			Position.Player[Count].Distance = this->GetPosition(this->Player[i]);
			Count++;
		}
	}

	//Double Goer Set Line Position
	PMSG_DGOER_RTRIANG_POS IceWalker = { 0xC1, sizeof(IceWalker), 0xBF, 0x11, 0x01, 0xFF };

	if(Doppelganger.MonsterBoss[2] > 0){
		IceWalker.Unk = 0;
		IceWalker.Pos = this->GetPosition(this->MonsterBoss[2]);
	}


	//Set Monster Kill Count
	PMSG_DGOER_MONSTER_COUNT KillCount;
	PHeadSubSetB((LPBYTE)&KillCount, 0xBF, 0x14, sizeof(KillCount));
	KillCount.Total = 3;

	//Finish Event if +3 Monster Pass
	if(this->MonsterPassed >= 3){
		this->Status = FAILED; //Failed
		this->State = FINISHED;
		this->MonsterPassed = 3;
		LogAddTD("[Doppelganger] 3 Monsters Passed the Magic Circles - EVENT FINISH FAILED");
	}

	
	KillCount.Killed = this->MonsterPassed;

	DataSend(aIndex,(LPBYTE)&IceWalker,IceWalker.Size); //Send Triangles	
	DataSend(aIndex,(LPBYTE)&LinePosition,LinePosition.Size); //Send Line			
	DataSend(aIndex,(LPBYTE)&Position,Position.Size); //Send Position
	DataSend(aIndex,(LPBYTE)&KillCount,KillCount.h.size); //Send Kill Count
}



struct PMSG_SET_TIMERINFO
{
	PBMSG_HEAD h;	// C1:92
	BYTE Type;	// 3
};

void CDoppelganger::SendTimeInfo(int Type, int aIndex)
{
	PMSG_SET_TIMERINFO pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0x92;
	pMsg.Type = Type;

	DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
}

struct PMSG_SETMAPATTR_COUNT {
    PBMSG_HEAD h; // C1:46
    BYTE btType; // 3
    BYTE btMapAttr; // 4
    BYTE btMapSetType; // 5
    BYTE btCount; // 6
};

struct PMSG_SETMAPATTR {
    BYTE btX; // 0
    BYTE btY; // 1
};

void CDoppelganger::ChangeMapAttr(BOOL State, int aIndex){

	char MapAttr[256];
	int lOfs = 0;

	PMSG_SETMAPATTR_COUNT pMsg;
	PMSG_SETMAPATTR pAttrMsg;

	lOfs += sizeof(pMsg);
	int MapNumber = this->EventMap - 65;

	pMsg.btType = 0;
	pMsg.btCount = 1;
	pMsg.btMapAttr = 4;
	pMsg.btMapSetType = (State == TRUE) ? 4 : 1;

	pAttrMsg.btX = Doppelganger_Entrance[MapNumber].X1;
	pAttrMsg.btY = Doppelganger_Entrance[MapNumber].Y1;
	memcpy(&MapAttr[lOfs],&pAttrMsg,sizeof(pAttrMsg));
	lOfs += sizeof(pAttrMsg);

	pAttrMsg.btX = Doppelganger_Entrance[MapNumber].X2;
	pAttrMsg.btY = Doppelganger_Entrance[MapNumber].Y2;
	memcpy(&MapAttr[lOfs],&pAttrMsg,sizeof(pAttrMsg));
	lOfs += sizeof(pAttrMsg);

	pMsg.h.c = 0xC1;
	pMsg.h.size = lOfs;
	pMsg.h.headcode = 0x46;
	memcpy(&MapAttr, &pMsg, sizeof(pMsg));
	
	DataSend(aIndex,(LPBYTE)&MapAttr,lOfs);
}

int CDoppelganger::GetPosition(int aIndex)
{
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return 0;
	if(!gObjIsConnected(aIndex)) return 0;

	LPOBJ lpObj = &gObj[aIndex];
	
	if(this->EventMap == 65){
		if(lpObj->Y > 31){
			return (int)22-((lpObj->Y-31)/3.2f);
		}
	} else if(this->EventMap == 66){
		if(lpObj->Y>69){
			return (int)22-(((lpObj->Y -70)*2)/10);
		} 
	} else if(this->EventMap == 67){
		if(lpObj->Y>61){
			return (int)22-((lpObj->Y-61)/4.1f);
		}
	} else if(this->EventMap == 68){
		for(int X=1; X < 23;X++){	
			if(lpObj->MapNumber == 68 && lpObj->Y > 24+((X-1)*4) && lpObj->Y <= 27+((X-1)*4)){
				return X;
			}
		}
	}

	return 22;
}


void CDoppelganger::OpenChest(int aIndex, int X, int Y, int ChestType)
{
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	ChestType -= 541; //0 or 1

	int Index = rand() % this->TotalItems[ChestType];

	if(ChestType == 0){

		memset(this->Larvas,-1,sizeof(this->Larvas));

		gObjDel(this->SiverChestIndex[0]); //Delete Silver 1
		gObjDel(this->SiverChestIndex[1]); //Delete Silver 2

		int Random = rand() % 100;

		if(Random <= this->SilverChest_DropRate)
		{
			int ItemIndex = ITEMGET(this->SilverChest[Index].Type,this->SilverChest[Index].Index);
			
			int Excellent, Option;
			if(this->SilverChest[Index].Excellent == 1){
				Excellent = GetExcellentOption();
			}

			if(this->SilverChest[Index].Option == 1){
				if((rand()%10) <5){ 
					Option=0;
				} else {
					Option=1;
				}
			}
			
			ItemSerialCreateSend(aIndex,this->EventMap,(BYTE)X,(BYTE)Y,ItemIndex,
			this->SilverChest[Index].Level,
			this->SilverChest[Index].Duration,
			this->SilverChest[Index].Skill,
			this->SilverChest[Index].Luck,
			this->SilverChest[Index].Option,aIndex,
			this->SilverChest[Index].Excellent,0);

			LogAddTD("[Doppelganger][%s][%s] Open Silver Chest [%d,%d,%d,%d,%d,%d,%d]",lpObj->AccountID,lpObj->Name,ItemIndex,
			this->SilverChest[Index].Level,Option,this->SilverChest[Index].Luck,
			this->SilverChest[Index].Skill,this->SilverChest[Index].Duration,Excellent);

		} else {
			this->Larvas[0] = gObjAddMonster(this->EventMap);
			this->Larvas[1] = gObjAddMonster(this->EventMap);
			this->Larvas[2] = gObjAddMonster(this->EventMap);
			this->Larvas[3] = gObjAddMonster(this->EventMap);
			this->Larvas[4] = gObjAddMonster(this->EventMap);

			if(this->Larvas[0] >= 0) this->AddMonster(this->Larvas[0],532,X+1,Y-2);
			if(this->Larvas[1] >= 0) this->AddMonster(this->Larvas[1],532,X-1,Y+1);
			if(this->Larvas[2] >= 0) this->AddMonster(this->Larvas[2],532,X-2,Y+1);
			if(this->Larvas[3] >= 0) this->AddMonster(this->Larvas[3],532,X+2,Y+1);
			if(this->Larvas[4] >= 0) this->AddMonster(this->Larvas[4],532,X-1,Y+2);
		}
	} else if(ChestType == 1) {
		int ItemIndex = ITEMGET(this->GoldenChest[Index].Type,this->GoldenChest[Index].Index);

		int Excellent,Option;
		if(this->GoldenChest[Index].Excellent == 1){
			Excellent = GetExcellentOption();
		}

		if(this->GoldenChest[Index].Option == 1){
			if((rand()%10) <5){ 
				Option=0;
			} else {
				Option=1;
			}
		}

		ItemSerialCreateSend(aIndex,this->EventMap,(BYTE)X,(BYTE)Y,ItemIndex,
			this->GoldenChest[Index].Level,
			this->GoldenChest[Index].Duration,
			this->GoldenChest[Index].Skill,
			this->GoldenChest[Index].Luck,
			Option,aIndex,Excellent,0);

		LogAddTD("[Doppelganger][%s][%s] Open Golden Chest [%d,%d,%d,%d,%d,%d,%d]",
			lpObj->AccountID,lpObj->Name,ItemIndex,
			this->GoldenChest[Index].Level,
			Option,
			this->GoldenChest[Index].Luck,
			this->GoldenChest[Index].Skill,
			this->GoldenChest[Index].Duration,
			Excellent);

		gObjDel(this->GoldenChestIndex); //Delete Golden Chest!
		
		this->Status = SUCCESS; //Success
		this->State = FINISHED;
	}
}

void CDoppelganger::InterimRewardChest(int X,int Y)
{
	for(int i=0; i < 2;++i){
		if(this->SiverChestIndex[i] != -1){
			gObjDel(this->SiverChestIndex[i]);
		}
	}

	this->SiverChestIndex[0] = gObjAddMonster(this->EventMap);
	this->SiverChestIndex[1] = gObjAddMonster(this->EventMap);

	if(this->SiverChestIndex[0] >= 0){
		this->AddMonster(this->SiverChestIndex[0],541,X-1,Y-1);
	}

	if(this->SiverChestIndex[1] >= 0){
		this->AddMonster(this->SiverChestIndex[1],541,X+1,Y+1);
	}
}

void CDoppelganger::FinalRewardChest(int X, int Y){

	this->MonsterBoss[2] = -1;
	this->GoldenChestIndex = gObjAddMonster(this->EventMap);

	if(this->GoldenChestIndex >= 0){
		this->AddMonster(this->GoldenChestIndex,542,X,Y);
		return;
	} else {
		LogAddTD("[Doppelganger] Failed to call Golden Chest!");
		return;
	}
}

void CDoppelganger::ClearMonsters()
{
	int Map = this->EventMap - 65;

	for(int X=0; X < this->MonstersCount;X++){
		if(this->CurrentMonster[X] > 0){
			if(gObj[this->CurrentMonster[X]].MapNumber == this->EventMap){
				gObjDel(this->CurrentMonster[X]);
			}
		}
	}
	
	///Clear Larvas
	for(int X=0; X < 5;X++){
		if(this->Larvas[X] > 0){
			if(gObj[this->Larvas[X]].MapNumber == this->EventMap){
				gObjDel(this->Larvas[X]);
			}
		}
	}
		
	//Clear Chest's
	for(int X=0; X < 2;++X){
		if(this->SiverChestIndex[X] != NULL){
			if(gObj[this->SiverChestIndex[X]].MapNumber == this->EventMap){
				gObjDel(this->SiverChestIndex[X]);
			}
		}
	}

	for(int X=0; X < 3;++X){
		if(this->MonsterBoss[X] != NULL){
			if(gObj[this->MonsterBoss[X]].MapNumber == this->EventMap){
				gObjDel(this->MonsterBoss[X]);
			}
		}
	}
	
	memset(this->CurrentMonster,-1,sizeof(this->CurrentMonster));
	memset(this->MonsterBoss,-1,sizeof(this->MonsterBoss));
	memset(this->SiverChestIndex,-1,sizeof(this->SiverChestIndex));
	memset(this->Larvas,-1,sizeof(this->Larvas));
	this->GoldenChestIndex = -1;

	LogAddTD("[Doppelganger] Monster Cleared");
}

void CDoppelganger::MonstersProcess(){

	int MapNumber = this->EventMap - 65;
	int Monster = 533;
	if(this->MonstersCount < 80){

		for(int i=0; i < 80;i++){

			Monster++;
			if(Monster == 540) Monster = 533;

			if(this->CurrentMonster[i] == -1){
				this->CurrentMonster[i] = gObjAddMonster(this->EventMap);

				if(this->CurrentMonster[i] >= 0){
					this->AddMonster(this->CurrentMonster[i],Monster,
					this->MPosX[MapNumber],this->MPosY[MapNumber]);
					this->MonstersCount++;
				}
			}
		}
	}
}

void CDoppelganger::CreateSlaughterer()
{	
	int MapNumber = this->EventMap - 65;

	this->MonsterBoss[0] = gObjAddMonster(this->EventMap);
	if(this->MonsterBoss[0] >= 0){
		this->AddMonster(this->MonsterBoss[0],529,this->MPosX[MapNumber],this->MPosY[MapNumber]);
	}
	
	this->MonsterBoss[1] = gObjAddMonster(this->EventMap);
	if(this->MonsterBoss[1] >= 0){
		this->AddMonster(this->MonsterBoss[1],530,this->MPosX[MapNumber],this->MPosY[MapNumber]);
	}
	
	LogAddTD("[Doppelganger] Slaughterer and Furious Slaughterer Created on Map %d",this->EventMap);
}

void CDoppelganger::CreateIceWalker(){

	this->MonsterBoss[2] = gObjAddMonster(this->EventMap);
	if(this->MonsterBoss[2] >= 0){
		this->AddMonster(this->MonsterBoss[2],531,
			Doppelganger_Boss_Position[this->EventMap-65].Boss3X,
			Doppelganger_Boss_Position[this->EventMap-65].Boss3Y);
	}
	
	LogAddTD("[Doppelganger] Ice Walker Cerated on Map %d",this->EventMap);
}

void CDoppelganger::AddMonster(int aIndex, int Class, int X, int Y){

	LPOBJ lpObj = &gObj[aIndex];
	  
	lpObj->MapNumber = this->EventMap;
	lpObj->X = X;
	lpObj->Y = Y;

	gObjSetMonster(aIndex,Class);

	if(lpObj->Class == 541 || lpObj->Class == 542) lpObj->m_MoveRange = 0;

	float PartyPercent = 0;
	if(this->PlayersCount == 2){
		PartyPercent = 0.2f;
	} else if(this->PlayersCount == 3){
		PartyPercent = 0.5f;
	} else if(this->PlayersCount == 4){
		PartyPercent = 1.0f;
	} else if(this->PlayersCount == 5){
		PartyPercent = 1.7f;
	}
	
	lpObj->Life += lpObj->Life * (this->EventLevel * 0.3f);
	lpObj->MaxLife = lpObj->Life;
	lpObj->m_Defense += (int)(lpObj->m_Defense * (this->EventLevel * 0.06f));
	lpObj->m_AttackDamageMin += (int)(lpObj->m_AttackDamageMin * (this->EventLevel * 0.13f));
	lpObj->m_AttackDamageMax += (int)(lpObj->m_AttackDamageMax * (this->EventLevel * 0.16f));

	if(this->PlayersCount > 1){
		lpObj->Life += lpObj->Life * PartyPercent;
		lpObj->MaxLife = lpObj->Life;
		lpObj->m_Defense += (int)(lpObj->m_Defense * PartyPercent);
		lpObj->m_AttackDamageMin += (int)(lpObj->m_AttackDamageMin * PartyPercent);
		lpObj->m_AttackDamageMax += (int)(lpObj->m_AttackDamageMax * PartyPercent);
	}
}

void CDoppelganger::LoadItemBag(){

	SMDFile = fopen("..\\Data\\ItemBags\\Doppelganger_ItemBag.dat","r");

	if(!SMDFile){
		MsgBox("[Doppelganger] Failed Loading Chest's File");
		return;
	}
	
	int Token,Chest,Type,Index,Level,Option,Luck,Skill,Duration,Excellent,Count;

	memset(this->TotalItems,0,sizeof(this->TotalItems));
	memset(this->SilverChest,0,sizeof(this->SilverChest));
	memset(this->GoldenChest,0,sizeof(this->GoldenChest));
	Count = 0;

	while(true){
		
		Token = GetToken();

		if(Token == 2){ break; }
		if(!strcmp("end",TokenString)){ break; }
	
		Chest = (int)TokenNumber;
	
		while(true){ //0: Silver Chest - 1: Golden Chest

			Token = GetToken();

			if(Token == 2){ break; }
			if(!strcmp("end",TokenString)){ break; }

			Type = (int)TokenNumber;

			Token = GetToken();
			Index = (int)TokenNumber;

			Token = GetToken();
			Level = (int)TokenNumber;

			Token = GetToken();
			Option = (int)TokenNumber;

			Token = GetToken();
			Luck = (int)TokenNumber;

			Token = GetToken();
			Skill = (int)TokenNumber;

			Token = GetToken();
			Duration = (int)TokenNumber;

			Token = GetToken();
			Excellent = (int)TokenNumber;

					
			Count = this->TotalItems[Chest];

			if(Type == 0){
				this->SilverChest[Count].Type = Type;
				this->SilverChest[Count].Index = Index;
				this->SilverChest[Count].Level = Level;
				this->SilverChest[Count].Option = Option;
				this->SilverChest[Count].Luck = Luck;
				this->SilverChest[Count].Skill = Skill;
				this->SilverChest[Count].Duration = Duration;
				this->SilverChest[Count].Excellent = Excellent;
			} else {
				this->GoldenChest[Count].Type = Type;
				this->GoldenChest[Count].Index = Index;
				this->GoldenChest[Count].Level = Level;
				this->GoldenChest[Count].Option = Option;
				this->GoldenChest[Count].Luck = Luck;
				this->GoldenChest[Count].Skill = Skill;
				this->GoldenChest[Count].Duration = Duration;
				this->GoldenChest[Count].Excellent = Excellent;
			}

			this->TotalItems[Chest]++;
		}
	}
	
	LogAddTD("[Doppelganger] Silver and Golden Chest file Loaded");
	fclose(SMDFile);
}