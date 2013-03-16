#include "Stdafx.h"
#include "PartySystem.h"
#include "User.h"
#include "LogProc.h"
#include "GameMain.h"
#include "Readscript.h"
#include "PartyClass.h"

CPartySystem PartySystem;

void CPartySystem::StartProcess(){

	this->Enabled = GetPrivateProfileInt("PartySystem","IsEnabled",0,CONFIG_FILE);

	this->TracePartyMinLevel = GetPrivateProfileInt("PartySystem","TracePartyMinLevel",50,CONFIG_FILE);
	this->CreatePartyDiferenceLevel = GetPrivateProfileInt("PartySystem","CreateParty_DifLevel",130,CONFIG_FILE);
	this->CreatePartyDiferenceResets = GetPrivateProfileInt("PartySystem","CreateParty_DifResets",0,CONFIG_FILE);
	this->LoadConfig();
}

void CPartySystem::LoadConfig(){

	int Token;

	if(this->Enabled == 0) return;

	SMDFile = fopen(".\\PartySystem.xtr","r");

	if(!SMDFile){
		MsgBox("[PartySystem] Failed Loading File");
		return;
	}

	while(true){
		
		Token = GetToken();

		if(Token == 2) break;
		if(!strcmp("end",(char*)TokenString)) break;

		if(Token == 1){

			this->MapCount = 0;
			
			while(TRUE){
				
				Token = GetToken();
				if(!strcmp("end",(char*)TokenString)) break;

				this->Maps[this->MapCount].Map = (int)TokenNumber;

				Token = GetToken();
				this->Maps[this->MapCount].LevelMax = (int)TokenNumber;

				Token = GetToken();
				this->Maps[this->MapCount].ResetMax = (int)TokenNumber;

				Token = GetToken();
				this->Maps[this->MapCount].RequiredZen = (int)TokenNumber;

				this->MapCount++;
			}
		}
	}

	LogAddTD("[PartySystem] Config Loaded - Enabled: %d - Maps: %d",this->Enabled,this->MapCount);
}

void CPartySystem::WarpToParty(int aIndex, int PartyNumber){
	
	
	if(PartyNumber == -1) return;

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	int LeaderIndex = gParty.m_PartyS[PartyNumber].Number[0];

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpLeaderObj = &gObj[LeaderIndex];

	if(lpObj->m_IfState.use && lpObj->m_IfState.type != 0) return;

	if(lpObj->m_PK_Level > 4){
		GCServerMsgStringSend("[Party] Los Asesinos no pueden Moverse",aIndex,1);
		return;
	}

	if(lpObj->Level < this->TracePartyMinLevel){
		char Buff[255];
		sprintf(Buff,"[Party] Necesitas ser Nivel %d para Moverte",this->TracePartyMinLevel);
		GCServerMsgStringSend(Buff,aIndex,1);
		return;
	}

	if((lpLeaderObj->MapNumber >= 11 && lpLeaderObj->MapNumber <= 16 || lpLeaderObj->MapNumber == 52) //BloodCastle
		|| (lpLeaderObj->MapNumber == 9 || lpLeaderObj->MapNumber == 32)//DevilSquare 
		|| (lpLeaderObj->MapNumber >= 18 && lpLeaderObj->MapNumber <= 23 || lpLeaderObj->MapNumber == 53) //ChaosCastle
		|| (lpLeaderObj->MapNumber >= 45 && lpLeaderObj->MapNumber <= 50) //Illusion Temple
		|| (lpLeaderObj->MapNumber >= 24 && lpLeaderObj->MapNumber <= 29 || lpLeaderObj->MapNumber == 36) //Kalima
		|| (lpLeaderObj->MapNumber == 30) || (lpLeaderObj->MapNumber == 34)//LorenDeep (CS) y Crywolf
		|| (lpLeaderObj->MapNumber == 64 || lpLeaderObj->MapNumber == 62) //Duel Arena & Santa Town
		|| (lpLeaderObj->MapNumber == 39 || lpLeaderObj->MapNumber == 58)//Kanturu Tower & Raklion
		|| (lpLeaderObj->MapNumber == 41 || lpLeaderObj->MapNumber == 42)){ //Season3 Quest Map
		return;
	}

	int CheckGuild = memcmp(lpLeaderObj->GuildName,lpObj->GuildName,11);
	
	if(lpLeaderObj->MapNumber == 31 && CheckGuild != 0){//Land of Trials
		LogAddTD("[PartySystem][%s][%s] Player is in Land of Trials and Guilds are not the Same",lpObj->AccountID,lpObj->Name);
		return;
	}

	if(lpLeaderObj->MapNumber == 10){
		if(lpObj->pInventory[7].m_Type == -1 && lpObj->pInventory[8].m_Type != 0x1A03){
			GCServerMsgStringSend("[Party] No puedes ir a Icarus sin Alas",aIndex,1);
			return;
		}
	}

	if(lpLeaderObj->MapNumber == 7 && lpObj->pInventory[8].m_Type == 0x1A02){
		GCServerMsgStringSend("[Party] No puedes ir a Atlans montando una Urniria",aIndex,1);
		return;
	}


	int DifLevel = 0, DifReset = 0;
	if(lpObj->Level >= lpLeaderObj->Level){
		DifLevel = lpObj->Level - lpLeaderObj->Level;}
	else if(lpObj->Level < lpLeaderObj->Level){
		DifLevel = lpLeaderObj->Level - lpObj->Level;}
	
	if(lpObj->Resets >= lpLeaderObj->Resets){
		DifReset = lpObj->Resets - lpLeaderObj->Resets;
	} else if(lpObj->Resets < lpLeaderObj->Resets){
		DifReset = lpLeaderObj->Resets - lpObj->Resets;
	}


	int Mapa = -1;
	for(int X = 0; X < this->MapCount; X++){
		if(this->Maps[X].Map == lpLeaderObj->MapNumber){ //si el Mapa esta en la Lista
			Mapa = X; //pone el Map en la Varbiale
		}
	}

	if(Mapa == -1) return;

	if(this->Maps[Mapa].LevelMax < DifLevel && this->Maps[Mapa].LevelMax > 0){
		GCServerMsgStringSend("[Party] la diferencia de Nivel es Mayor a la Permitida",aIndex,1);
		return;
	}

	if(this->Maps[Mapa].ResetMax < DifReset && this->Maps[Mapa].ResetMax > 0){
		GCServerMsgStringSend("[Party] la diferencia de Resets es Mayor a la Permitida",aIndex,1);
		return;
	}

	if(lpObj->Money < this->Maps[Mapa].RequiredZen){
		char Buff[255];
		sprintf(Buff,"[Party] Necesitas %d de Zen para Moverte",this->Maps[Mapa].RequiredZen);
		GCServerMsgStringSend(Buff,aIndex,1);
		return;
	}

	else if(lpObj->Money > this->Maps[Mapa].RequiredZen){

		lpObj->Money -= this->Maps[Mapa].RequiredZen;
		GCMoneySend(lpObj->m_Index,lpObj->Money);

		gObjTeleport(aIndex,lpLeaderObj->MapNumber,lpLeaderObj->X,lpLeaderObj->Y);

		LogAddTD("[PartySystem] Warp %s to %s [%d] (%d,%d)",lpObj->Name,lpLeaderObj->Name,lpLeaderObj->MapNumber,lpLeaderObj->X,lpLeaderObj->Y);
		GCServerMsgStringSend("[Party] Fuiste Movido!",aIndex,1);
		return;		
	}
		
	
}

BOOL CPartySystem::CheckLevelResetsDif(int aIndex, int LeaderIndex){
	
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return FALSE;
	if(!gObjIsConnected(aIndex)) return FALSE;

	LPOBJ lpObj = &gObj[aIndex];

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return FALSE;
	if(!gObjIsConnected(aIndex)) return FALSE;

	LPOBJ lpLeaderObj = &gObj[LeaderIndex];

	int DifLevel, DifReset=0;

	if(lpObj->Level >= lpLeaderObj->Level){
		DifLevel = lpObj->Level - lpLeaderObj->Level;
	} else if(lpObj->Level < lpLeaderObj->Level){
		DifLevel = lpLeaderObj->Level - lpObj->Level;
	}
	
	if(lpObj->Resets >= lpLeaderObj->Resets){
		DifReset = lpObj->Resets - lpLeaderObj->Resets;
	} else if(lpObj->Resets < lpLeaderObj->Resets){
		DifReset = lpLeaderObj->Resets - lpObj->Resets;
	}

	char Buff[255];
		
	if(PartySystem.CreatePartyDiferenceLevel < DifLevel&& PartySystem.CreatePartyDiferenceLevel > 0){
		sprintf(Buff,"[Party] La diferencia de Nivel es Mayor a %d",PartySystem.CreatePartyDiferenceLevel);
		GCServerMsgStringSend(Buff,aIndex,1);
		return TRUE;
	}

	if(PartySystem.CreatePartyDiferenceResets < DifReset && PartySystem.CreatePartyDiferenceResets > 0){
		sprintf(Buff,"[Party] La diferencia de Resets es Mayor a %d",PartySystem.CreatePartyDiferenceResets);
		GCServerMsgStringSend(Buff,aIndex,1);
		return TRUE;
	}

}