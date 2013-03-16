#include "Stdafx.h"
#include "User.h"
#include "WarpSystem.h"
#include "LogProc.h"
#include "..\include\readscript.h"

CWarpSystem WarpSystem;

void CWarpSystem::LoadMoves(){

	int Token;
	SMDFile = fopen("..\\Data\\WarpSystem.dat","r");

	if(!SMDFile){
		MsgBox("[WarpSystem] Failed Loading File");
		return;
	}

	memset(this->Moves,NULL,sizeof(this->Moves));
	this->Count = 0;
	int Index, Money, GensMap, Gate;
	short Level;
				
	while(true){

		Token = GetToken();
		if(Token == 2) break;
		if(!strcmp("end",(char*)TokenString)) break;

		Index = (BYTE)TokenNumber;

		Token = GetToken();
		strcpy(this->Moves[this->Count].Name,TokenString);
		
		GetToken();
		Money = (int)TokenNumber;

		GetToken();
		Level = (short)TokenNumber;

		GetToken();
		GensMap = (int)TokenNumber;

		GetToken();
		Gate = (int)TokenNumber;

		if(Count < 1){
			Count = 1;
		}

		//Set Checks!
		if(Index < 0) Index = 0;
		if(Index > 50) Index = 50;
		if(GensMap < 0) GensMap = 0;
		if(GensMap > 1) GensMap = 1;
		if(Money < 1) Money = 1;
		if(Money > 2000000000) Money = 2000000000;
		if(Level < 1) GensMap = 1;
		if(Level > 400) GensMap = 400;
		if(Gate < 0) Gate = 1;
		if(Gate > 500) Gate = 500;

		this->Moves[this->Count].Index = Index;
		this->Moves[this->Count].Level = Level;
		this->Moves[this->Count].Money = Money;
		this->Moves[this->Count].GensMap = GensMap;
		this->Moves[this->Count].Gate = Gate;

		this->Count++;
	}

	LogAddTD("[WarpSystem] Warps Count: %d",this->Count);
	fclose(SMDFile);
}

int CWarpSystem::GetWarpIndex(int MoveIndex){

	for(int X=0; X < this->Count;X++){
		if(this->Moves[X].Index == MoveIndex) return X;
	}

	return -1;
}

void CWarpSystem::MovePlayer(int aIndex, int MoveIndex)
{
	
	if(aIndex < OBJ_STARTUSERINDEX || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	int Index = this->GetWarpIndex(MoveIndex);

	if(Index == -1){
		LogAddTD("[WarpSystem][%d][%s] Non-allocate Move Info [Index %d]",
			lpObj->m_Index,lpObj->AccountID,MoveIndex);
		return;
	}

	if(this->Moves[Index].Gate == -1){
		LogAddTD("[WarpSystem][%d][%s] Bad GateInfo (-1) in Move Info [%d]",
			lpObj->m_Index,lpObj->AccountID,Index);
		return;
	}

	if(lpObj->Teleport != 0){
		GCServerMsgStringSend("[Warp] no puedes Moverte Ahora",lpObj->m_Index,1);
		return;
	} 

	if((lpObj->m_IfState.use) != 0){
		if(lpObj->m_IfState.type == 3){
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.use = 0;
			lpObj->m_IfState.type = 0;
		}
	} 
		
	if(lpObj->m_IfState.use > 0){
		GCServerMsgStringSend("[Warp] no puedes Moverte Ahora",lpObj->m_Index,1);
		return;
	} 
		
	if(lpObj->m_PK_Level >= 6){			
		GCServerMsgStringSend("[Warp] Los PKs no pueden Moverse",lpObj->m_Index,1);
		return;
	} 

	
	int Price = this->Moves[Index].Money;
	short Level = this->Moves[Index].Level;

	if(lpObj->Class == 3 || lpObj->Class == 4){
		Price = Price * 2 / 3;
		Level = Level * 2 / 3;
	}

	//Check Level and Money
	if(lpObj->Money < Price || lpObj->Level < Level){
		LogAddTD("[WarpSystem][%s][%s] Failed to Move cause of Low Level or Zen",lpObj->AccountID,lpObj->Name);
		return;
	} 

	
	////Gens System Check
	//if(this->Moves[Index].GensMap == 1 && lpObjEx->GensFamily < 1){	
	//	GCServerMsgStringSend("[Warp] Mapa solo para Gens",lpObj->m_Index,1);
	//	return;
	//}
	
	int Gate = this->Moves[Index].Gate;

	if(gObjMoveGate(aIndex,Gate)){
		lpObj->m_bPShopRedrawAbs = TRUE;
		lpObj->Money -= Price;

		LogAddTD("[WarpSystem][%d][%s][%s] Warped to Gate: %d",
			lpObj->m_Index,lpObj->AccountID,lpObj->Name,Gate);

		GCMoneySend(lpObj->m_Index, lpObj->Money);
		return;
	} else {
		LogAddTD("[WarpSystem][%d][%s][%s] Failed to Warp Gate: %d",
			lpObj->m_Index,lpObj->AccountID,lpObj->Name,Gate);
		return;
	}
}