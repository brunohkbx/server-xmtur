#include "Stdafx.h"
#include "User.h"
#include "ImperialGuardian.h"
#include "LogProc.h"
#include "Readscript.h"
#include "DSProtocol.h"
#include "BuffManager.h"
#include "GameMain.h"
#include "BloodCastle.h"

CImperialGuardian ImperialGuardian;

void ImperialGuardian_CheckCore(void * lpParam){

	while(TRUE){
		ImperialGuardian.SendTimer();
		Sleep(1000);
	}

	_endthread();
}

void ImperialGuardian_CheckGates(void * lpParam){

	while(TRUE){
		ImperialGuardian.CheckGates();
		Sleep(100);
	}

	_endthread();
}

void CImperialGuardian::StartProcess(BOOL Reload){
	
	this->BattleStatus == IMPERIAL_NONE;

	if(Reload == 0){
		_beginthread(ImperialGuardian_CheckCore,0,NULL);
		_beginthread(ImperialGuardian_CheckGates,0,NULL);
	}

	this->LoadGaionItemBag();

	this->PieceOfPaperDropRate = GetPrivateProfileInt("ImperialGuardian","PieceOfPaperDropRate",60,"..\\Data\\Events\\ImperialGuardian.dat");
	this->TrapsDamageBase =  GetPrivateProfileInt("ImperialGuardian","TrapsDamageBase",1000,"..\\Data\\Events\\ImperialGuardian.dat");
	this->TrapsDamagePlus =  GetPrivateProfileInt("ImperialGuardian","TrapsDamagePlus",500,"..\\Data\\Events\\ImperialGuardian.dat");

	memset(this->Player,-1,sizeof(this->Player));
	
	this->LoadMonsters();
}

void CImperialGuardian::EnterEvent(int aIndex){
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	SYSTEMTIME Time;
	GetLocalTime(&Time);

	this->EventDay = Time.wDayOfWeek;

	int Day = this->EventDay;
	if(Day == 0) Day = 7;

	PMSG_FORT_ENTER_RESULT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xF7, 0x02, sizeof(pMsg));

	int TicketPos = -1;
	int GateNumber = 17;

	int Item = ITEMGET(14,102);
	if(Day == 7){
		Item = ITEMGET(14,109);
	}
	
	for(short i=12; i < 76;++i){
		if(lpObj->pInventory[i].m_Type == Item){
			TicketPos = i;
			break;
		}
	}

	if(TicketPos == -1){
		pMsg.Result = 2;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[ImperialGuardian][%s][%s] Ticket not Found",lpObj->AccountID,lpObj->Name);
		return;
	}

	if(this->State!= IMPERIAL_NONE){
		pMsg.Result = 1;
		pMsg.EntryTime = ((this->Timer / 60) / 1000);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[ImperialGuardian][%s][%s] Battle Already Started",lpObj->AccountID,lpObj->Name);
		return;
	}

	//If not in Party
	/*if(lpObj->PartyNumber == -1){
		pMsg.Result = 6;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
		LogAddTD("[ImperialGuardian][%s][%s] User is not in Party",lpObj->AccountID,lpObj->Name);
		return;
	}*/

	if(this->EventDay == 0){ //Domingo
		GateNumber = 322;
		this->EventMap = 72;
	} else if(this->EventDay == 1 || this->EventDay == 4){ //Lunes y Jueves
		GateNumber = 307;
		this->EventMap = 69;
	} else if(this->EventDay == 2 || this->EventDay == 5){ //Martes y Viernes
		GateNumber = 312;
		this->EventMap = 70;
	} else if(this->EventDay == 3 || this->EventDay == 6){ //Miercoles y Sabados
		GateNumber = 317;
		this->EventMap = 71;
	}

	this->PlayersCount = 0;

	//Setea al Primer Player dentro de la Estructura
	this->Player[0] = aIndex;
	this->PlayersCount++;

	gObjMoveGate(aIndex,GateNumber);
	LogAddTD("[ImperialGuardian][%s][%s] Player 1 Enter: %d",lpObj->AccountID,lpObj->Name,aIndex);
	/*
	this->PartyNumber = lpObj->PartyNumber;

	if(PartyNumber != -1){	
		for(int i=OBJ_STARTUSERINDEX; i < OBJMAX;i++){	

			if(this->PlayersCount < 5 && gObjIsConnected(i)){

				LPOBJ gTargetObj = &gObj[i];
				if(gTargetObj->PartyNumber == this->PartyNumber && gTargetObj->m_Index != this->Player[0]){	
				
					this->Player[this->PlayersCount] = gTargetObj->m_Index;

					this->PlayersCount++;
					gObjMoveGate(i,GateNumber);
					LogAddTD("[ImperialGuardian][%s][%s] Player %d Enter: %d",lpObj->AccountID,lpObj->Name,this->PlayersCount,gTargetObj->m_Index);
				}
			}
		}
	}

	gObjInventoryDeleteItem(aIndex,TicketPos);
	GCInventoryItemDeleteSend(aIndex,TicketPos,1);
	LogAddTD("[ImperialGuardian][%s][%s] Ticket Deleted %d",lpObj->AccountID,lpObj->Name,TicketPos);
	*/
	//Clear Event Monsters
	this->ClearMonsters();

	//Clear ALL
	this->TotalDoors = 0;
	this->TotalMonsters = 0;
	this->CurrentMonsters = 0;
	this->MoveTickCount = 0;
	this->Zone = 1;
	
	//Monsters and Doors Information
	this->SetDoors();

	//First Step Before Kill first Gate
	this->Step = 0;

	//Set Tick Count for Start
	this->StartTickCount = GetTickCount();
	this->MessageSend("La Batalla comenzara en 10 Segundos",1);

	LogAddTD("[ImperialGuardian] SetState->PREPARING");
	LogAddTD("[ImperialGuardian] Players: %d/5, Map: %d, Party: %d",this->PlayersCount,this->EventMap,PartyNumber);
}

static const struct KatoMapAttr
{
	int StartX;
	int StartY;
	int EndX;
	int EndY;
} g_KatoMapAttr[9] = {
	142, 190, 147, 192,
	115, 191, 120, 193,
	85, 195, 90, 197,
	222, 133, 224, 138,
	222, 159, 224, 164,
	222, 192, 224, 197,
	166, 216, 168, 221
};

void CImperialGuardian::OpenCloseGate(BYTE Status){ //0x02: OPEN - 0x03: CLOSED
	
	/*PMSG_FORT_OPENCLOSE_GATE pMsg;
	pMsg.h.set((LPBYTE)&pMsg,0xBF,0x09,sizeof(pMsg));
	pMsg.Status = 0x01;
	pMsg.Result = Status;

	//Set All Doors Opened!
	for(int X=0; X < 5;X++){
		if(this->Player[X] >= OBJ_STARTUSERINDEX) this->DoorState[X] = DOOR_OPEN;
	}*/

	char Buff[256];
	short PacketSize = 0;
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)(Buff);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)(Buff + sizeof(PMSG_SETMAPATTR_COUNT));

	lpMsgBody[0].btX = 0;
	lpMsgBody[0].btY = 0;
	lpMsgBody[1].btX = 255;
	lpMsgBody[1].btY = 255;

	PacketSize = (sizeof(PMSG_SETMAPATTR_COUNT)+6*sizeof(PMSG_SETMAPATTR));

	lpMsg->btMapAttr = 16;
	lpMsg->btMapSetType = 1;
	lpMsg->btCount = 1;
	lpMsg->btType = 0;

	lpMsg->h.c = 0xC1;
	lpMsg->h.headcode = 0x46;
	lpMsg->h.size = PacketSize;

	this->PDataSend((LPBYTE)&lpMsg,lpMsg->h.size);
}

void CImperialGuardian::SendTimer(){
	
	if(GetTickCount() > (this->StartTickCount + 10000) && this->StartTickCount > 0){ //10 Seconds...
		LogAddTD("[ImperialGuardian] Event Start!");
		this->StartTickCount = 0;
		this->SetState(IMPERIAL_START); 
		return;
	}

	if(GetTickCount() > (this->MoveTickCount + 10000) && this->MoveTickCount > 0){ //10 Seconds...
		LogAddTD("[ImperialGuardian] Warping Players");
		this->MovePlayers();
		this->MoveTickCount = 0;
		return;
	}

	if(this->BattleStatus == IMPERIAL_NONE) return;

	if(this->Timer > 0){
		this->Timer -= 1; //Resta tiempo al Timer
	}


	if(this->Timer == 0){

		if(this->BattleStatus == IMPERIAL_STANDBY ){
			this->SetState(IMPERIAL_START); 
		} else {
			LogAddTD("[ImperialGuardian] Time is Over");
			this->SetState(IMPERIAL_FAILED);
			return;
		}
	}

	/*if(this->PlayersCount < 2){
		LogAddTD("[ImperialGuardian] Not enought Users for Battle");
		this->SetState(IMPERIAL_FAILED);
		return;
	}*/

	//Check for Traps!
	this->CheckTraps();

	//Check the Party Members, if not in Party Move to Devias and Delete from Event
	//this->CheckPartyStatus();

	int PlayerIndex = 0;
	//Check for Players
	for(int X=0; X < 5;X++){
		PlayerIndex = this->Player[X];
		if(PlayerIndex >= OBJ_STARTUSERINDEX 
		&& (gObjIsConnected(this->Player[X]) != TRUE 
		|| gObj[PlayerIndex].MapNumber != this->EventMap)){
		
			LogAddTD("[ImperialGuardian] Delete User Index: %d %d",X,PlayerIndex);
			this->Player[X] = -1;
			if(X == 0){
				LogAddTD("[ImperialGuardian] Party Leader was Out");
				this->SetState(IMPERIAL_FAILED);
				return;
			}
		}
	}
	
	int FinalTime = this->Timer * 4;

	PMSG_FORT_TIMER pMsg;

	pMsg.h.set((LPBYTE)&pMsg,0xF7,0x04,sizeof(pMsg));
	pMsg.Status = 1;
	pMsg.Unk1 = 1;
	pMsg.Unk2 = 1;
	pMsg.Unk3 = 0;
	pMsg.Unk4 = 0;
	pMsg.TimerH = HIBYTE(FinalTime);
	pMsg.TimerL = LOBYTE(FinalTime);
	pMsg.Unk5 = 0;
	pMsg.Monster = 0;

	if(this->BattleStatus != IMPERIAL_FINISHED && this->BattleStatus != IMPERIAL_STANDBY){
		pMsg.Status = 2;
		pMsg.Monster = this->CurrentMonsters;
	}

	this->PDataSend((LPBYTE)&pMsg,pMsg.h.size);
}

void CImperialGuardian::MessageSend(char* Msg, int Type){

	int PlayerIndex = 0;
	for(int X=0; X < 5;X++){
		if(this->Player[X] >= OBJ_STARTUSERINDEX){
			PlayerIndex = this->Player[X];
			LPOBJ lpObj = &gObj[PlayerIndex];

			if(lpObj->MapNumber == this->EventMap){
				GCServerMsgStringSend(Msg,lpObj->m_Index,Type);
			}
		}
	}
}

void CImperialGuardian::PDataSend(LPBYTE aRecv, int Size){

	int PlayerIndex = 0;
	for(int X=0; X < 5;X++){
		if(this->Player[X] >= OBJ_STARTUSERINDEX){
			PlayerIndex = this->Player[X];
			LPOBJ lpObj = &gObj[PlayerIndex];

			if(lpObj->MapNumber == this->EventMap){
				DataSend(lpObj->m_Index,(LPBYTE)&aRecv,Size);
			}
		}
	}
}

void CImperialGuardian::CheckPartyStatus(){

	int PlayerIndex = 0;
	for(int X=0; X < 5;X++){
		if(this->Player[X] >= OBJ_STARTUSERINDEX){

			PlayerIndex = this->Player[X];
			LPOBJ gTargetObj = &gObj[PlayerIndex];

			if(gTargetObj->MapNumber == this->EventMap){
			
				if(gTargetObj->PartyNumber != PartyNumber){	
					gObjMoveGate(this->Player[X],22);
					this->Player[X] = -1;

					if(X==0){
						LogAddTD("[ImperialGuardian] Event Leader has no Party");
						this->SetState(IMPERIAL_FAILED);
						return;
					}
				}
			}
		}
	}
}

void CImperialGuardian::SendStage(){

	PMSG_FORT_ENTER_RESULT pMsg;
	pMsg.h.set((LPBYTE)&pMsg,0xF7,0x02,sizeof(pMsg));

	int Day = this->EventDay;
	if(Day == 0) Day = 7;

	int Zone = 1;

	if(this->Step >= 3 && this->Step <= 5){
		Zone = 2;
	} if(this->Step >= 6 && this->Step <= 8){
		Zone = 3;
	} else if(this->Step >= 9) {
		Zone = 4;
	}

	pMsg.Day = Day;
	pMsg.EntryTime = 0;
	pMsg.Result = 0;
				
	pMsg.State = Zone;
				
	pMsg.Unk = rand()%4; //Weather

	this->PDataSend((LPBYTE)&pMsg,pMsg.h.size);
}

void CImperialGuardian::SetState(int SetState){
	
	this->SetMonsters();

	switch(SetState){
		case IMPERIAL_START:	
			this->Timer = 587; //Set Timer en 10 Minutos
			this->SendStage();

			this->BattleStatus = IMPERIAL_START; //Monster Kill
			LogAddTD("[ImperialGuardian] SetState->IMPERIAL_START (%d)",this->Step);
		break;

		case IMPERIAL_STANDBY:
			this->Timer = 176; //294 = 5 Min
			this->SendStage(); //Zone 1 - Fist After Kill Gate

			this->BattleStatus = IMPERIAL_STANDBY;
			LogAddTD("[ImperialGuardian] SetState->IMPERIAL_STANDBY (%d)",this->Step);
		
		break;

		case IMPERIAL_GATEKILL:

			//Set Step
			this->Step++;

			//Open Gate
			this->OpenCloseGate(0x02);

			//Set Timer and Status
			this->BattleStatus = IMPERIAL_GATEKILL; //Monster Kill
			LogAddTD("[ImperialGuardian] SetState->IMPERIAL_GATEKILL (%d)",this->Step);
		break;

		case IMPERIAL_FINISHED:

			this->SuccessMessage(100000);
			this->Timer = 0;
			this->ClearMonsters();
			this->MessageSend("Fortaleza Liberada, volveras a Devias en 10 Segundos",1);
			this->MoveTickCount = GetTickCount();
		
			this->BattleStatus = IMPERIAL_NONE;
			LogAddTD("[ImperialGuardian] SetState->IMEPRIAL_FINISHED - SUCCESS");
		break;

		case IMPERIAL_FAILED: 

			this->FailMessage();
			this->Timer = 0;
			this->ClearMonsters();
			this->MessageSend("Has Fallado, volveras a Devias en 10 Segundos",1);
			this->MoveTickCount = GetTickCount();

			this->BattleStatus = IMPERIAL_NONE;
			LogAddTD("[ImperialGuardian] SetState->IMEPRIAL_FINISHED - FAILED");
		break;
	}
}

void CImperialGuardian::MovePlayers(){

	int PlayerIndex = 0;
	//Check for Players
	for(int X=0; X < 5;X++){
		if(gObjIsConnected(this->Player[X]) == TRUE && this->Player[X] >= OBJ_STARTUSERINDEX){
			PlayerIndex = this->Player[X];
			LPOBJ lpObj = &gObj[PlayerIndex];

			if(lpObj->MapNumber == this->EventMap){
				gObjMoveGate(lpObj->m_Index,22);
				this->Player[X] = -1;
			}
		}
	}
}

void CImperialGuardian::SuccessMessage(int GainExp)
{
	PMSG_FORT_END_MESSAGE pMsg;
	pMsg.C = 0xC1;
	pMsg.Size = 0x0D;
	pMsg.Headcode = 0xF7;
	pMsg.Subcode = 0x06;
	pMsg.Type = 2;
	pMsg.Experience = GainExp;

	this->PDataSend((LPBYTE)&pMsg,pMsg.Size);
}

void CImperialGuardian::FailMessage()
{
	PMSG_FORT_END_MESSAGE pMsg;
	pMsg.C = 0xC1;
	pMsg.Size = 0x0D;
	pMsg.Headcode = 0xF7;
	pMsg.Subcode = 0x06;
	pMsg.Type = 0;
	pMsg.Experience = 0;
	this->PDataSend((LPBYTE)&pMsg,pMsg.Size);
}

void CImperialGuardian::GiveUserDamage(int aIndex, int iDamage)
{	
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != 3) return;
	if(lpObj->Life <= 0.0 ) return;

	PMSG_ATTACKRESULT pResult;
	
	pResult.h.set((LPBYTE)&pResult,0x11,sizeof(pResult));
	pResult.NumberH = HIBYTE(aIndex);
	pResult.NumberL = LOBYTE(aIndex);
	pResult.DamageH = HIBYTE(iDamage);
	pResult.DamageL = LOBYTE(iDamage);
	pResult.btShieldDamageH = 0;
	pResult.btShieldDamageL = 0;
	pResult.DamageType = 0;

	DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);

	lpObj->Life -= iDamage;

	if(lpObj->Life <= 0.0){
		lpObj->Life = 0;
		lpObj->m_State = 4;
		lpObj->KillerType = 1;
		lpObj->RegenTime = GetTickCount();
		lpObj->DieRegen = 1;
		lpObj->RegenMapNumber = 2;
		lpObj->RegenMapX = 215;
		lpObj->RegenMapY = 38;
		lpObj->PathCount = 0;

		GCDiePlayerSend(lpObj,lpObj->m_Index,0,0);
	}
}

void CImperialGuardian::CheckTraps(){

	if(this->BattleStatus == IMPERIAL_NONE) return;

	int GetDamage = rand() % TrapsDamageBase + TrapsDamagePlus;

	for(int X=0; X < 5;X++){
		if(this->Player[X] >= OBJ_STARTUSERINDEX){

			LPOBJ lpObj = &gObj[this->Player[X]];

			if(lpObj->MapNumber == 69){
				if((lpObj->X >= 218 && lpObj->X <= 234) && (lpObj->Y >= 58 && lpObj->Y <= 85)
				|| (lpObj->X >= 151 && lpObj->X <= 163) && (lpObj->Y >= 23 && lpObj->Y <= 48)){
					this->GiveUserDamage(lpObj->m_Index,GetDamage);
				}
			}

			if(lpObj->MapNumber == 70){
				if((lpObj->X >= 27 && lpObj->X <= 44) && (lpObj->Y >= 62 && lpObj->Y <= 67 )
				|| (lpObj->X >= 38 && lpObj->X <= 56) && (lpObj->Y >= 120 && lpObj->Y <= 148)){
					this->GiveUserDamage(lpObj->m_Index,GetDamage);
				}
			}

			if(lpObj->MapNumber == 71){
				if((lpObj->X >= 91 && lpObj->X <= 118) && (lpObj->Y >= 185 && lpObj->Y <= 205)
				|| (lpObj->X >= 220 && lpObj->X <= 224) && (lpObj->Y >= 162 && lpObj->Y <= 191)){
					this->GiveUserDamage(lpObj->m_Index,GetDamage);
				}
			}

			if(lpObj->MapNumber == 72){
				if((lpObj->X >= 29 && lpObj->X <= 49) && (lpObj->Y >= 66 && lpObj->Y <= 86) //Trap 1
				|| (lpObj->X == 57) && (lpObj->Y >= 189 && lpObj->Y <= 193) //Trap 2
				|| (lpObj->X == 64) && (lpObj->Y >= 189 && lpObj->Y <= 193) //Trap 3
				|| (lpObj->X >= 67 && lpObj->X <= 69) && (lpObj->Y == 181) //Trap 4
				|| (lpObj->X >= 67 && lpObj->X <= 69) && (lpObj->Y == 174) //Trap 5
				|| (lpObj->X >= 204 && lpObj->X <= 226) && (lpObj->Y >= 131 && lpObj->Y <= 155)){ //Tra6
					this->GiveUserDamage(lpObj->m_Index,GetDamage);
				}
			}
		}
	}
}

void CImperialGuardian::CheckPassGates(int aIndex, WORD Gate){

	/*if(Gate < 307 || Gate > 328) return;
	if(this->BattleStatus == IMPERIAL_NONE) return;

	PMSG_FORT_OPENCLOSE_GATE pMsg;
	pMsg.h.set((LPBYTE)&pMsg,0xBF,0x09,sizeof(pMsg));
	pMsg.Status = 0x01;
	pMsg.Result = 0x03;
	
	if((this->Step == 3 || this->Step == 6 || this->Step == 9) && this->BattleStatus == IMPERIAL_GATEKILL){
		ImperialGuardian.SetState(IMPERIAL_STANDBY);
	}
	
	for(int X=0; X < 5;X++){
		if(this->Player[X] == aIndex){
			if(this->DoorState[X] == DOOR_OPEN){
				DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
			}
		}
	}*/
}

void CImperialGuardian::CheckGates(){

	if(this->BattleStatus == IMPERIAL_NONE) return;

	/*PMSG_FORT_OPENCLOSE_GATE pMsg;
	pMsg.h.set((LPBYTE)&pMsg,0xBF,0x09,sizeof(pMsg));
	pMsg.Status = 0x01;
	pMsg.Result = 0x03;
	
	for(int X=0; X < 5;X++){
		if(this->Player[X] >= OBJ_STARTUSERINDEX){
			int Index = X;

			if(this->DoorState[Index] == DOOR_OPEN){

				LPOBJ lpObj = &gObj[Index];

				if(this->EventMap == 69){
					if((lpObj->X >= 220 && lpObj->X <= 243) && (lpObj->Y >= 32 && lpObj->Y <= 48)
					|| (lpObj->X >= 225 && lpObj->X <= 235) && (lpObj->Y >= 58 && lpObj->Y <= 67)
					|| (lpObj->X >= 205 && lpObj->X <= 212) && (lpObj->Y >= 78 && lpObj->Y <= 84)
					|| (lpObj->X >= 180 && lpObj->X <= 190) && (lpObj->Y >= 19 && lpObj->Y <= 38)
					|| (lpObj->X >= 152 && lpObj->X <= 162) && (lpObj->Y >= 23 && lpObj->Y <= 31)
					|| (lpObj->X >= 152 && lpObj->X <= 156) && (lpObj->Y >= 57 && lpObj->Y <= 65)
					|| (lpObj->X >= 172 && lpObj->X <= 187) && (lpObj->Y >= 82 && lpObj->Y <= 102)){ //Lunes y Jueves
						this->DoorState[X] = DOOR_CLOSED;
					}
				}
				
				if(this->EventMap == 70){
					if((lpObj->X >= 57 && lpObj->X <= 71) && (lpObj->Y >= 52 && lpObj->Y <= 75)
					|| (lpObj->X >= 31 && lpObj->X <= 47) && (lpObj->Y >= 62 && lpObj->Y <= 67)
					|| (lpObj->X >= 9 && lpObj->X <= 16) && (lpObj->Y >= 62 && lpObj->Y <= 67)
					|| (lpObj->X >= 26 && lpObj->X <= 49) && (lpObj->Y >= 96 && lpObj->Y <= 110)
					|| (lpObj->X >= 38 && lpObj->X <= 46) && (lpObj->Y >= 122 && lpObj->Y <= 130)
					|| (lpObj->X >= 53 && lpObj->X <= 57) && (lpObj->Y >= 158 && lpObj->Y <= 162)
					|| (lpObj->X >= 95 && lpObj->X <= 102) && (lpObj->Y >= 104 && lpObj->Y <= 118)){ //Martes y Viernes
						this->DoorState[X] = DOOR_CLOSED;
					}
				}

				
				if(this->EventMap == 71){
					if((lpObj->X >= 130 && lpObj->X <= 143) && (lpObj->Y >= 180 && lpObj->Y <= 204)
					|| (lpObj->X >= 108 && lpObj->X <= 116) && (lpObj->Y >= 185 && lpObj->Y <= 205)
					|| (lpObj->X >= 78 && lpObj->X <= 86) && (lpObj->Y >= 193 && lpObj->Y <= 197)
					|| (lpObj->X >= 213 && lpObj->X <= 232) && (lpObj->Y >= 137 && lpObj->Y <= 150)
					|| (lpObj->X >= 220 && lpObj->X <= 224) && (lpObj->Y >= 163 && lpObj->Y <= 273)
					|| (lpObj->X >= 222 && lpObj->X <= 224) && (lpObj->Y >= 196 && lpObj->Y <= 202)
					|| (lpObj->X >= 157 && lpObj->X <= 176) && (lpObj->Y >= 220 && lpObj->Y <= 230)){ //Miercoles y Sabados
						this->DoorState[X] = DOOR_CLOSED;
					}
				}

				
				if(this->EventMap == 72){
					if((lpObj->X >= 60 && lpObj->X <= 78) && (lpObj->Y >= 59 && lpObj->Y <= 78)
					|| (lpObj->X >= 37 && lpObj->X <= 47) && (lpObj->Y >= 66 && lpObj->Y <= 74)
					|| (lpObj->X >= 29 && lpObj->X <= 32) && (lpObj->Y >= 93 && lpObj->Y <= 98)
					|| (lpObj->X >= 20 && lpObj->X <= 47) && (lpObj->Y >= 179 && lpObj->Y <= 199)
					|| (lpObj->X >= 55 && lpObj->X <= 67) && (lpObj->Y >= 188 && lpObj->Y <= 193)
					|| (lpObj->X >= 66 && lpObj->X <= 72) && (lpObj->Y >= 158 && lpObj->Y <= 163)
					|| (lpObj->X >= 159 && lpObj->X <= 175) && (lpObj->Y >= 127 && lpObj->Y <= 139)
					|| (lpObj->X >= 200 && lpObj->X <= 220) && (lpObj->Y >= 131 && lpObj->Y <= 135)
					|| (lpObj->X >= 221 && lpObj->X <= 226) && (lpObj->Y >= 162 && lpObj->Y <= 166)
					|| (lpObj->X >= 195 && lpObj->X <= 211) && (lpObj->Y >= 20 && lpObj->Y <= 32)){ //Domingos
						this->DoorState[X] = DOOR_CLOSED;
					}
				}

				if(this->DoorState[Index] == DOOR_CLOSED){
					DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
					LogAddTD("[ImperialGuardian][%s][%s] Close Gates %d",lpObj->AccountID,lpObj->Name,this->Step);
				}
			}
		}
	}*/
}

//////////////////////////////////////////////////////////////////
// Imperial Guardian :: Monsters Step

void CImperialGuardian::SetDoors(){

	if(this->EventDay == 0){ //Domingo
		this->CreateDoor(525,81,68,3);
		this->CreateDoor(525,50,69,3);
		this->CreateDoor(525,32,90,1);
		this->CreateDoor(524,52,191,3);
		this->CreateDoor(525,34,176,1);
		this->CreateDoor(527,69,166,1);
		this->CreateDoor(528,156,132,3);
		this->CreateDoor(528,197,132,3);
		this->CreateDoor(528,225,159,1);
		this->CreateDoor(528,214,24,3);
	} else if(EventDay == 1 || EventDay == 4){ //Lunes y Jueves
		this->CreateDoor(524,234,29,1);
		this->CreateDoor(524,233,55,1);
		this->CreateDoor(524,215,80,3);
		this->CreateDoor(525,194,25,3);
		this->CreateDoor(524,167,25,3);
		this->CreateDoor(525,154,53,1);
		this->CreateDoor(525,180,79,1);
	} else if(EventDay == 2 || EventDay == 5){ //Martes y Viernes
		this->CreateDoor(524,50,65,3);
		this->CreateDoor(525,75,67,3);
		this->CreateDoor(527,19,65,3);
		this->CreateDoor(527,37,93,1);
		this->CreateDoor(524,41,117,1);
		this->CreateDoor(527,55,154,1);
		this->CreateDoor(525,107,112,3);
	} else if(EventDay == 3 || EventDay == 6){ //Miercoles y Sabados
		this->CreateDoor(525,146,191,3);
		this->CreateDoor(524,119,192,3);
		this->CreateDoor(525,89,195,3);
		this->CreateDoor(525,223,133,1);
		this->CreateDoor(524,223,159,1);
		this->CreateDoor(525,223,192,1);
		this->CreateDoor(525,167,217,1);
	}

	LogAddTD("[ImperialGuardian] Gates Loaded & Added Successfully");
}

void CImperialGuardian::CreateDoor(int GateType, int X, int Y, int Direction){
	
	int DoorIndex = gObjAddMonster(this->EventMap);

	if(DoorIndex >= 0){
		LPOBJ lpObj = &gObj[DoorIndex];

		lpObj->X = X; lpObj->TX = X; lpObj->MTX = X; lpObj->m_OldX = X; lpObj->StartX = X;
		lpObj->Y = Y; lpObj->TY = Y; lpObj->MTY = Y; lpObj->m_OldY = Y; lpObj->StartY = Y;
		lpObj->Level = 50;

		gObjSetMonster(DoorIndex,GateType);

		lpObj->MapNumber = this->EventMap;	
		lpObj->MaxRegenTime = 0;
		lpObj->Dir = Direction;

		this->Door[this->TotalDoors] = DoorIndex;
		this->TotalDoors++;
	} else {
		LogAddTD("[ImperialGuardian] Failed to Add Door %d",this->TotalDoors+1);
	}
}

//Set Monster For Event
void CImperialGuardian::LoadMonsters()
{
	int Token;
	SMDFile = fopen("..\\Data\\Events\\ImperialGuardian_Monsters.dat","r");

	if(!SMDFile){
		MsgBox("[ImperialGuardian] Failed Loading Monsters List");
		return;
	}
		
	memset(this->Monsters,0,sizeof(this->Monsters));
	memset(this->MonstersCount,0,sizeof(this->MonstersCount));

	int MapGroup = 0;
	
	while(true){
		
		Token = GetToken();
		int MapGroup = (int)TokenNumber;
			
		if(Token == 2) break;
		if(!strcmp("end",TokenString)) break; 

		if(Token == 1)
		{
			int Number = 0;

			while(true){
				
				if(MapGroup < 7){

					Token = GetToken();
					if(!strcmp("end",TokenString)) break; 

					if(Number >= 255) break; 

					this->Monsters[MapGroup][Number].ID = (int)TokenNumber;

					Token = GetToken();
					this->Monsters[MapGroup][Number].X = (int)TokenNumber;

					Token = GetToken();
					this->Monsters[MapGroup][Number].Y = (int)TokenNumber;

					Token = GetToken();
					this->Monsters[MapGroup][Number].Stage = (int)TokenNumber;

					Number++;
					this->MonstersCount[MapGroup]++;
				}
			}
		}	
	}

	fclose(SMDFile);

	LogAddTD("[ImperialGuardian] ..\\Data\\Events\\ImperialGuardian_Monsters.dat file Loaded"); 
}

void CImperialGuardian::SetMonsters(){

	int MonsterIndex = -1;

	if(this->CurrentMonsters > 0) return;

	memset(this->SetedMonsters,0,sizeof(this->SetedMonsters));
	this->TotalMonsters = 0;
	this->CurrentMonsters = 0;
	int Day = this->EventDay;

	for(int i=0; i < this->MonstersCount[Day];i++){

		if(this->Monsters[Day][i].Stage == this->Step){

			MonsterIndex = gObjAddMonster(this->EventMap);

			if(MonsterIndex != -1 && MonsterIndex < OBJ_STARTUSERINDEX){
	
				LPOBJ lpObj = &gObj[MonsterIndex];

				lpObj->Level = 70;
				gObjSetMonster(MonsterIndex,this->Monsters[Day][i].ID);

				lpObj->X = this->Monsters[Day][i].X; lpObj->TX = lpObj->X; lpObj->MTX = lpObj->X; lpObj->m_OldX = lpObj->X; lpObj->StartX = lpObj->X;
				lpObj->Y = this->Monsters[Day][i].Y; lpObj->TY = lpObj->Y; lpObj->MTY = lpObj->Y; lpObj->m_OldY = lpObj->Y; lpObj->StartY = lpObj->Y;
				lpObj->MapNumber = this->EventMap;
				lpObj->DieRegen = FALSE;
				lpObj->MaxRegenTime = 0;
				lpObj->RegenOk = 0;
				lpObj->m_MoveRange = 15;
				lpObj->TargetNumber = -1;

				this->SetedMonsters[this->TotalMonsters] = MonsterIndex;
				this->TotalMonsters++;
				this->CurrentMonsters++;
			} else {
				LogAddTD("[ImperialGuardian] Failed to Add Monster %d",i);
			}
		}
	}
}


void CImperialGuardian::ClearMonsters(){

	int DeletedCount = 0;

	for(int X=0; X < this->TotalMonsters;X++){
		if(this->SetedMonsters[X] > 0){
			if(gObj[this->SetedMonsters[X]].MapNumber == this->EventMap){
				gObjDel(this->SetedMonsters[X]);
				DeletedCount++;
			}
		}
	}
	
	for(int X=0; X < 10;X++){
		if(this->Door[X] > 0){
			if(gObj[this->Door[X]].MapNumber == this->EventMap){
				gObjDel(this->Door[X]);
			}
		}
	}

	memset(this->SetedMonsters,-1,sizeof(this->SetedMonsters));
	memset(this->Door,-1,sizeof(this->Door));
	this->TotalMonsters = 0;
	this->CurrentMonsters = 0;
			
	LogAddTD("[ImperialGuardian][%d] All Monsters Cleared (%d)",this->EventMap,DeletedCount);
}

void CImperialGuardian::DropSemicronPiece(int aIndex, int Class, int X, int Y){
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	int Semicron = 0;
	switch(Class){
		case 508:
			Semicron = 103;
		break;
		
		case 509:
			Semicron = 104;
		break;

		case 510:
			Semicron = 105;
		break;

		case 511:
			Semicron = 106;
		break;

		case 507:
			Semicron = 107;
		break;
		
		case 506:
			Semicron = 108;
		break;
	}

	ItemSerialCreateSend(aIndex,this->EventMap,(BYTE)X,(BYTE)Y,ITEMGET(14,Semicron),0,0,0,0,0,aIndex,0,0);
	LogAddTD("[ImperialFort][%s][%s] Drop Piece of Semicron Part %d ",lpObj->AccountID,lpObj->Name,(Semicron-102));
	
	ImperialGuardian.SetState(IMPERIAL_FINISHED);
}


void CImperialGuardian::GaionItemDrop(int aIndex, int CordX, int CordY)
{
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	if(this->GaionItemCount < 1){
		LogAddTD("[ImperialGuardian][%s][%s] Gaion ItemBag has no Items",lpObj->AccountID,lpObj->Name);
		return;
	}

	int MaxItems = rand()%this->MaxItemsDrop;
	int ItemID = 0;
	int Excellent;

	if(rand()%10000 < this->ItemDrop){
	
		for(int I=1; I < this->MaxItemsDrop; I++){

			for(int X = 1; X <= this->Items[ItemID].Count; X++){
			
				ItemID = rand()%this->GaionItemCount;

				if(this->Items[ItemID].Excellent > 0){
					Excellent = GetExcellentOption();
				}

				ItemSerialCreateSend(lpObj->m_Index,lpObj->MapNumber,CordX,CordY,
				((this->Items[ItemID].Index*512)+this->Items[ItemID].ID),
				this->Items[ItemID].Level,
				this->Items[ItemID].Durability,
				this->Items[ItemID].Skill,
				this->Items[ItemID].Luck,
				this->Items[ItemID].Option,
				lpObj->m_Index,
				Excellent,
				this->Items[ItemID].Ancient);

				LogAddTD("[ImperialGuardian][%s][%s] Item Drop [%d][%d][%d,%d,%d][%d,%d,%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID,lpObj->Name,
				this->Items[ItemID].Count,
				ItemID,lpObj->MapNumber,CordX,CordY,
				this->Items[ItemID].Index,
				this->Items[ItemID].ID,
				this->Items[ItemID].Level,
				this->Items[ItemID].Durability,
				this->Items[ItemID].Skill,
				this->Items[ItemID].Luck,
				this->Items[ItemID].Option,
				Excellent,
				this->Items[ItemID].Ancient);
			}
		}
	} else {
		MapC[lpObj->MapNumber].MoneyItemDrop(this->MoneyDrop,CordX,CordY);
		LogAddTD("[ImperialGuardian][%s][%s] Money Drop [%d]",lpObj->AccountID,lpObj->Name,this->MoneyDrop);
	}
}


void CImperialGuardian::LoadGaionItemBag(){
	int Token;

	SMDFile = fopen("..\\Data\\ItemBags\\Gaion_ItemBag.txt","r");

	if(!SMDFile){
		MsgBox("[Gaion ItemBag] Failed Loading File");
		return;
	}

	memset(this->Items,NULL,sizeof(this->Items));
	this->GaionItemCount = 0;

	int Category, ID = 0;
	BYTE Index, Level, Luck, Skill, Durability, Option, Excellent, Ancient, Count;
				
	while(TRUE){
	    Token = GetToken();

		if(Token == 2) break;
		if(!strcmp("end",(char*)TokenString)) break; 

		if(Token == 2) break;
		
		Category = (int)TokenNumber;

		if(Category == 0){ //0 for Configs

			while(TRUE){
				Token = GetToken();
				if(Token == 2) break;
				if(!strcmp("end",(char*)TokenString)) break;

				this->ItemDrop = (int)TokenNumber; //Drop Rate

				GetToken();
				this->MoneyDrop = (int)TokenNumber;

				GetToken();
				this->MaxItemsDrop = (int)TokenNumber;
			}
		}

		if(Category == 1){ //1 for Items

			if(this->GaionItemCount == MAX_ITEMS)  break;

			while(TRUE){

				Token = GetToken();
				if(Token == 2) break;
				if(!strcmp("end",(char*)TokenString)) break;

				Index = (BYTE)TokenNumber;

				GetToken();
				ID = (int)TokenNumber;

				GetToken();
				Level = (BYTE)TokenNumber;

				GetToken();
				Luck = (BYTE)TokenNumber;

				GetToken();
				Skill = (BYTE)TokenNumber;

				GetToken();
				Durability = (BYTE)TokenNumber;

				GetToken();
				Option = (BYTE)TokenNumber;

				GetToken();
				Excellent = (BYTE)TokenNumber;

				GetToken();
				Ancient = (BYTE)TokenNumber;

				GetToken();
				Count = (BYTE)TokenNumber;

				if(Count < 1) Count = 1;		

				//Set Checks!
				if(Level < 0) Level = 0;
				if(Level > 15) Level = 15;
				if(Option < 0) Option = 0;
				if(Option > 1) Option = 1;
				if(Excellent > 1) Excellent = 1;
				if(Excellent < 0) Excellent = 0;
				if(Ancient > 1) Ancient = 1;
				if(Ancient < 0) Ancient = 0;
				if(Luck > 1) Luck = 1;
				if(Luck < 0) Luck = 0;
				if(Skill > 1) Skill = 1;
				if(Skill < 0) Skill = 0;
				if(Index < 0) Index = 0;
				if(Index > 15) Index = 15;
				if(ID < 0) ID = 0;
				if(ID > 512) ID = 512;
				if(Count < 0) Count = 1;
				if(Count > 255) Count = 255;

				int ItemsCount = this->GaionItemCount;
				this->Items[ItemsCount].Index = Index;
				this->Items[ItemsCount].ID = ID;
				this->Items[ItemsCount].Level = Level;
				this->Items[ItemsCount].Luck = Luck;
				this->Items[ItemsCount].Skill = Skill;
				this->Items[ItemsCount].Durability = Durability;
				this->Items[ItemsCount].Option = Option;
				this->Items[ItemsCount].Excellent = Excellent;
				this->Items[ItemsCount].Ancient = Ancient;
				this->Items[ItemsCount].Count = Count;

				this->GaionItemCount++;
			}
		}
	}

	LogAddTD("[ImperialGuardian] Gaion ItemBag file Loaded",this->GaionItemCount);
	fclose(SMDFile);
}