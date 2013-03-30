#include "Stdafx.h"
#include "User.h"
#include "ImperialGuardian.h"
#include "LogProc.h"
#include "Readscript.h"
#include "DSProtocol.h"
#include "BuffManager.h"
#include "GameMain.h"
#include "BloodCastle.h"
#include "IllusionTempleProcess.h"
#include "Winutil.h"

CImperialGuardian ImperialGuardian;

void ImperialGuardian_CheckCore(void * lpParam){

	while(TRUE){
		ImperialGuardian.SendTimer();
		Sleep(1000);
	}

	_endthread();
}

void CImperialGuardian::StartProcess(BOOL Reload){
	
	this->BattleStatus == IMPERIAL_NONE;

	if(Reload == 0){
		_beginthread(ImperialGuardian_CheckCore,0,NULL);
	}

	this->LoadGaionItemBag();

	this->PieceOfPaperDropRate = GetPrivateProfileInt("ImperialGuardian","PieceOfPaperDropRate",60,CONFIG_FILE);
	this->TrapsDamageBase =  GetPrivateProfileInt("ImperialGuardian","TrapsDamageBase",1000,CONFIG_FILE);
	this->TrapsDamagePlus =  GetPrivateProfileInt("ImperialGuardian","TrapsDamagePlus",500,CONFIG_FILE);

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
	
	
	for(short i=12; i < 76;++i){

		if(Day != 7){
			if(lpObj->pInventory[i].m_Type == ITEMGET(14,102)
			|| (lpObj->pInventory[i].m_Type == ITEMGET(13,126) && lpObj->pInventory[i].m_Durability >= 1.0f)){
				TicketPos = i;
				break;
			}
		} else {
			if(lpObj->pInventory[i].m_Type == ITEMGET(14,109)
			|| (lpObj->pInventory[i].m_Type == ITEMGET(13,127) && lpObj->pInventory[i].m_Durability >= 1.0f)){
				TicketPos = i;
				break;
			}
		}
	}


	if(TicketPos == -1){
		pMsg.Result = 2;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[ImperialGuardian][%s][%s] Ticket not Found",lpObj->AccountID,lpObj->Name);
		return;
	}

	if(this->BattleStatus != IMPERIAL_NONE){
		pMsg.Result = 1;
		pMsg.EntryTime = ((this->Timer / 60) / 1000);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[ImperialGuardian][%s][%s] Battle Already Started",lpObj->AccountID,lpObj->Name);
		return;
	}

	//If not in Party
	if(lpObj->PartyNumber == -1){
		pMsg.Result = 6;
		DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
		LogAddTD("[ImperialGuardian][%s][%s] User is not in Party",lpObj->AccountID,lpObj->Name);
		return;
	}

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

	//Remove Durability
	if(lpObj->pInventory[TicketPos].m_Type == ITEMGET(13,126) 
	|| lpObj->pInventory[TicketPos].m_Type == ITEMGET(13,127) 
	&& lpObj->pInventory[TicketPos].m_Durability > 1.0f){
		lpObj->pInventory[TicketPos].m_Durability -= 1.0f;
		GCItemDurSend2(lpObj->m_Index,TicketPos,lpObj->pInventory[TicketPos].m_Durability,0);
	} else {
		//Delete Ticket
		gObjInventoryDeleteItem(aIndex,TicketPos);
		GCInventoryItemDeleteSend(aIndex,TicketPos,1);
	}

	LogAddTD("[ImperialGuardian][%s][%s] Ticket Deleted %d",lpObj->AccountID,lpObj->Name,TicketPos);

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

void CImperialGuardian::OpenCloseGate(int CordX, int CordY){

	char Buff[256];
	short PacketSize = 0;
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)(Buff);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)(Buff + sizeof(PMSG_SETMAPATTR_COUNT));

	lpMsgBody[0].btX = CordX-5;
	lpMsgBody[0].btY = CordY-5;
	lpMsgBody[1].btX = CordX+5;
	lpMsgBody[1].btY = CordY+5;
	
	PacketSize = (sizeof(PMSG_SETMAPATTR_COUNT)+6*sizeof(PMSG_SETMAPATTR));

	lpMsg->btMapAttr = 16;
	lpMsg->btMapSetType = 1;
	lpMsg->btCount = 1;
	lpMsg->btType = 0;

	lpMsg->h.c = 0xC1;
	lpMsg->h.headcode = 0x46;
	lpMsg->h.size = PacketSize;

	for(int i=OBJ_MAXMONSTER;i<OBJMAX;i++){
		if(IMPERIAL_MAP_RANGE(gObj[i].MapNumber)){
			if(gObj[i].Connected > PLAYER_LOGGED){
				DataSend(i,(LPBYTE)Buff,PacketSize);
			}
		}
	}

	for(int i=(CordX-5);i<=(CordX+5);i++){
		for(int j= (CordY-5);j<=(CordY+5);j++){
			MapC[this->EventMap].m_attrbuf[j * 256 + i] = 0;
		}
	}
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

		if(this->BattleStatus == IMPERIAL_STANDBY){
			this->SetState(IMPERIAL_START); 
		} else {
			LogAddTD("[ImperialGuardian] Time is Over");
			this->SetState(IMPERIAL_FAILED);
			return;
		}
	}

	if(this->PlayersCount < 2){
		LogAddTD("[ImperialGuardian] Not enought Users for Battle");
		this->SetState(IMPERIAL_FAILED);
		return;
	}

	//Check for Traps!
	this->CheckTraps();

	//Check the Party Members, if not in Party Move to Devias and Delete from Event
	this->CheckPartyStatus();

	int PlayerIndex = 0;

	//Check for Players
	for(int X=0; X < 5;X++){
		PlayerIndex = this->Player[X];
		if(PlayerIndex >= OBJ_STARTUSERINDEX && (gObjIsConnected(this->Player[X]) != TRUE 
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

	for(int i=OBJ_MAXMONSTER;i<OBJMAX;i++){
		if(IMPERIAL_MAP_RANGE(gObj[i].MapNumber)){
			if(gObj[i].Connected > PLAYER_LOGGED){
				DataSend(i,(LPBYTE)aRecv,Size);
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
	
	switch(SetState){
		case IMPERIAL_START:	
			this->Timer = 587; //Set Timer en 10 Minutos
			this->SendStage();

			this->BattleStatus = IMPERIAL_START; //Monster Kill
			LogAddTD("[ImperialGuardian] SetState->IMPERIAL_START (%d)",this->Step);
		break;

		case IMPERIAL_STANDBY:
			this->Timer = 30; //Seconds
			this->SendStage(); //Zone 1 - Fist After Kill Gate

			this->BattleStatus = IMPERIAL_STANDBY;
			LogAddTD("[ImperialGuardian] SetState->IMPERIAL_STANDBY (%d)",this->Step);
		
		break;

		case IMPERIAL_GATEKILL:
			//Set Step
			this->Step++;
			//Set Monsters
			this->SetMonsters();

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

	if(Gate < 307 || Gate > 328) return;
	if(this->BattleStatus == IMPERIAL_NONE) return;

	if((this->Step == 3 || this->Step == 6 || this->Step == 9) && this->BattleStatus == IMPERIAL_GATEKILL){
		ImperialGuardian.SetState(IMPERIAL_STANDBY);
	}
}

void CImperialGuardian::CheckGates(){
	//Not in use Anymore!
}

//////////////////////////////////////////////////////////////////
// Imperial Guardian :: Monsters Step

void CImperialGuardian::SetDoors(){

	int Result = -1;
	LPOBJ lpObj = NULL;
	memset(this->Door,0,sizeof(this->Door));
	this->TotalDoors = 0;
	BYTE X, Y;
	WORD wMonsterIndex = 0;

	for(int n=0;n<gMSetBase.m_Count;n++)
	{
		if(gMSetBase.m_Mp[n].m_MapNumber == this->EventMap)
		{
			wMonsterIndex = gMSetBase.m_Mp[n].m_Type;
				
			if(wMonsterIndex == 524 || wMonsterIndex == 525 ||
				wMonsterIndex == 527 || wMonsterIndex == 528){

				BYTE btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
				Result = gObjAddMonster(btMapNumber);

				if(Result >= 0)
				{
					gObj[Result].m_PosNum = -1;
					gObj[Result].X = gMSetBase.m_Mp[n].m_X;
					gObj[Result].Y = gMSetBase.m_Mp[n].m_Y;
					gObj[Result].MapNumber = gMSetBase.m_Mp[n].m_MapNumber;
					gObj[Result].TX = gObj[Result].X;
					gObj[Result].TY = gObj[Result].Y;
					gObj[Result].m_OldX = gObj[Result].X;
					gObj[Result].m_OldY = gObj[Result].Y;
					gObj[Result].StartX = gObj[Result].X;
					gObj[Result].StartY = gObj[Result].Y;
					gObj[Result].Dir = gMSetBase.m_Mp[n].m_Dir;
					gObjSetMonster(Result, wMonsterIndex);
					gObj[Result].DieRegen = FALSE;
					gObj[Result].MaxRegenTime = 0;
					gObj[Result].RegenOk = 0;
					gObj[Result].Dir = gMSetBase.m_Mp[n].m_Dir;

					this->Door[this->TotalDoors] = Result;
					this->TotalDoors++;

					X = gMSetBase.m_Mp[n].m_X;
					Y = gMSetBase.m_Mp[n].m_Y;

					for(int i=(X);i<=(X);i++){
						for(int j= (Y);j<=(Y);j++){
							MapC[this->EventMap].m_attrbuf[j * 256 + i] = 0;
						}
					}
				} else {
					LogAddTD("[ImperialGuardian] Failed to Add Gate %d",n);
				}
			}
		}
	}

	LogAddTD("[ImperialGuardian] Gates Added Successfully");
}

void CImperialGuardian::CreateDoor(int GateType, int X, int Y, int Direction){
}

//Set Monster For Event
void CImperialGuardian::LoadMonsters()
{
}

void CImperialGuardian::SetMonsters(){

	int Result = -1;
	LPOBJ lpObj = NULL;
	if(this->CurrentMonsters > 0) return;
	memset(this->SetedMonsters,0,sizeof(this->SetedMonsters));
	this->TotalMonsters = 0;
	this->CurrentMonsters = 0;
	DWORD wMonsterIndex = 0;

	for(int n=0;n<gMSetBase.m_Count;n++)
	{
		if(gMSetBase.m_Mp[n].m_MapNumber == this->EventMap)
		{
			wMonsterIndex = gMSetBase.m_Mp[n].m_Type;
				
			if(wMonsterIndex == 524 || wMonsterIndex == 525 || wMonsterIndex == 527 || wMonsterIndex == 528) continue;

			if(this->EventDay == 1 && wMonsterIndex == 511) continue;
			if(this->EventDay == 2 && wMonsterIndex == 512) continue;
			if(this->EventDay == 3 && wMonsterIndex == 506) continue;
			if(this->EventDay == 4 && wMonsterIndex == 508) continue;
			if(this->EventDay == 5 && wMonsterIndex == 509) continue;
			if(this->EventDay == 6 && wMonsterIndex == 510) continue;

			if(gMSetBase.m_Mp[n].m_Dir == this->Step){

		
				BYTE btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
				Result = gObjAddMonster(btMapNumber);

				if(Result >= 0)
				{
					gObj[Result].m_PosNum = n;
					gObj[Result].X = gMSetBase.m_Mp[n].m_X;
					gObj[Result].Y = gMSetBase.m_Mp[n].m_Y;
					gObj[Result].MapNumber = gMSetBase.m_Mp[n].m_MapNumber;
					gObj[Result].TX = gObj[Result].X;
					gObj[Result].TY = gObj[Result].Y;
					gObj[Result].m_OldX = gObj[Result].X;
					gObj[Result].m_OldY = gObj[Result].Y;
					gObj[Result].Dir = gMSetBase.m_Mp[n].m_Dir;
					gObj[Result].StartX = gObj[Result].X;
					gObj[Result].StartY = gObj[Result].Y;
					gObjSetMonster(Result, wMonsterIndex);
					gObj[Result].DieRegen = FALSE;
					gObj[Result].MaxRegenTime = 0;
					gObj[Result].RegenOk = 0;
					gObj[Result].Dir = rand() % 8;

					this->SetedMonsters[this->TotalMonsters] = Result;
					this->TotalMonsters++;
					this->CurrentMonsters++;
				} else {
					LogAddTD("[ImperialGuardian] Failed to Add Monster %d",n);
				}
			}
		}
	}
}


void CImperialGuardian::ClearMonsters(){

	for(int X=0; X < OBJ_MAXMONSTER;X++){
		if(IMPERIAL_MAP_RANGE(gObj[X].MapNumber)){
			gObjDel(X);
		}
	}

	memset(this->SetedMonsters,-1,sizeof(this->SetedMonsters));
	memset(this->Door,-1,sizeof(this->Door));
	this->TotalMonsters = 0;
	this->CurrentMonsters = 0;
			
	LogAddTD("[ImperialGuardian][%d] All Monsters Cleared",this->EventMap);
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
}

void CImperialGuardian::LoadGaionItemBag(){
}