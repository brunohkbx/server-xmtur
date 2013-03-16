#include "Stdafx.h"
#include "User.h"
#include "Commands.h"
#include "BloodCastle.h"
#include "DuelManager.h"
#include "QuestManager.h"
#include "Doppelganger.h"
#include "ImperialGuardian.h"
#include "DSProtocol.h"
#include "LogProc.h"
#include "GameMain.h"
#include "PartySystem.h"
#include "GMManager.h"
#include "MarrySystem.h"
#include "ObjCalCharacter.h"
#include "DBSockMng.h"
#include "..\common\Winutil.h"

CCommands Commands;

void CCommands::SendPost(int aIndex, LPCSTR Message){
	
	LPOBJ lpObj = &gObj[aIndex];
	
	if(Configs.PostEnable != 1) return;

	char Buff[255];
	if(lpObj->Level < Configs.PostLevel){
		sprintf_s(Buff,"Necesitas ser Nivel %d para usar este Comando",Configs.PostLevel);
		GCServerMsgStringSend(Buff,aIndex,1);
		return;
	}

	if(lpObj->Money < Configs.PostCost){
		sprintf_s(Buff,"Necesitas %d de Zen para usar este Comando",Configs.PostCost);
		GCServerMsgStringSend(Buff,aIndex,1);
		return;
	}

	//Create new Msg
	char CMessage[100];

	//Create Packet!
	PMSG_CHATDATA pMsg;

	pMsg.h.set((LPBYTE)&pMsg,0x00,sizeof(pMsg));

	memset(&pMsg.chatid,0,sizeof(pMsg.chatid));
	memset(&pMsg.chatmsg,0,sizeof(pMsg.chatmsg));
	
	sprintf_s(CMessage,"[POST] %s: %s",lpObj->Name,Message); //Normal

	if(Configs.PostColor == 0){ //Golden
		pMsg.h.headcode = 0x02;
	} else if(Configs.PostColor == 1){ //Blue
		sprintf(CMessage,"~[POST] %s: %s",lpObj->Name,Message);
	} else if(Configs.PostColor == 2){ //Green
		sprintf(CMessage,"@[POST] %s: %s",lpObj->Name,Message);
	} else if(Configs.PostColor == 3){ //Yellow
		sprintf(CMessage,"@@[POST] %s: %s",lpObj->Name,Message);
	}
	
	memcpy(&pMsg.chatmsg,CMessage,strlen(CMessage));
	pMsg.h.size = (strlen(CMessage)+ 0x13);

	DataSendAll((LPBYTE)&pMsg,pMsg.h.size);

	lpObj->Money -= Configs.PostCost;
	GCMoneySend(aIndex,lpObj->Money);
}

void CCommands::DropItem(int aIndex, LPCSTR Message){

	LPOBJ lpObj = &gObj[aIndex];

	if(Configs.ItemDropEnable != 1) return;
	if(lpObj->Authority != 32) return;

	int iIndex=0,iID=0,iLevel=0,iLuck=0,iSkill=0,iOpt=0,iDur=0,iExe=0,iAcc=0;
	
	sscanf(Message," %d %d %d %d %d %d %d %d %d",&iIndex,&iID,&iLevel,&iLuck,&iSkill,&iOpt,&iDur,&iExe,&iAcc);
	ItemSerialCreateSend(aIndex,lpObj->MapNumber,lpObj->X,lpObj->Y,((iIndex*512)+iID),iLevel,iDur,iSkill,iLuck,iOpt,aIndex,iExe,iAcc);
	LogAddTD("[GameMaster][%s][%s] Item Drop [%d,%d,%d,%d,%d,%d,%d,%d,%d]",lpObj->AccountID,lpObj->Name,iIndex,iID,iLevel,iLuck,iSkill,iOpt,iDur,iExe,iAcc);
}

void CCommands::GMove(int aIndex, LPCSTR Message)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->Authority != 32) return;
	
	char CharName[11]; 
	int Map=0, CordX=130, CordY=130;

	sscanf(Message,"%s %d %d %d",&CharName,&Map,&CordX,&CordY);
	
	if(Map > 80 || CordX > 255 || CordY > 255){
		GCServerMsgStringSend("Error al Mover, Chequea los Parametros!",aIndex,1);
		return;
	}

	int TargObjGetIndex = gObjGetIndex(CharName);

	if(gObjIsConnected(TargObjGetIndex) == FALSE) return;

	gObjTeleport(TargObjGetIndex,Map,CordX,CordY);

	GCServerMsgStringSend("Player Movido",aIndex,1);
	GCServerMsgStringSend("Un GameMaster te ha Movido",TargObjGetIndex,1);
	LogAddTD("[GameMaster][%s][%s] Use GMove [%d,%s,%d,%d,%d]",lpObj->AccountID,lpObj->Name,TargObjGetIndex,CharName,Map,CordX,CordY);
}

void CCommands::GMoveALL(int aIndex, LPCSTR Message){
	
	LPOBJ lpObj = &gObj[aIndex];
	
	if(aIndex != -1 && lpObj->Authority != 32) return;

	int Map = 0, CordX = 130, CordY = 130;
	sscanf(Message,"%d %d %d",&Map,&CordX,&CordY);

	if(Map > 80 || CordX > 255 || CordY > 255){
		GCServerMsgStringSend("Error al Mover, Chequea los Parametros!",aIndex,1);
		return;
	}

	for(int X = OBJ_STARTUSERINDEX; X < (OBJMAX-1);X++){
		if(gObjIsConnected(X) == 1){
			gObjTeleport(X,Map,CordX,CordY);
			GCServerMsgStringSend("Un GameMaster te ha Movido",X,1);
		}
	}

	GCServerMsgStringSend("Players Moved",aIndex,1);
	LogAddTD("[GameMaster][%s][%s] Use GMove All [%d,%d,%d]",lpObj->AccountID,lpObj->Name,Map,CordX,CordY);
}

void CCommands::ChangePKLevel(int aIndex, LPCSTR Message){
	
	LPOBJ lpObj = &gObj[aIndex];
	char CharName[11]; 
	int PKLevel = 3;
	int TargObjGetIndex;

	if(lpObj->Authority != 32) return;

	sscanf(Message,"%s %d",&CharName,&PKLevel);
	TargObjGetIndex = gObjGetIndex(CharName);
	
	if(gObjIsConnected(TargObjGetIndex) == 1 && TargObjGetIndex != -1){
		gObj[TargObjGetIndex].m_PK_Level = PKLevel;
		GCPkLevelSend(TargObjGetIndex,PKLevel);
	}

	GCServerMsgStringSend("PK Level Cambiado",aIndex,1);
	GCServerMsgStringSend("Tu PK Level fue modificado por un GameMaster",TargObjGetIndex,1);
	LogAddTD("[GameMaster][%s][%s] Change PK Level [%d,%s,%d]",lpObj->AccountID,lpObj->Name,TargObjGetIndex,CharName,PKLevel);
}

void CCommands::ChangeZen(int aIndex, LPCSTR Message){

	LPOBJ lpObj = &gObj[aIndex];
	char CharName[11];
	int Money = 0;

	if(lpObj->Authority != 32) return;
	
	sscanf(Message,"%s %d",&CharName,&Money);

	if(Money > 2000000000 || Money < 0){
		return GCServerMsgStringSend("La Cantidad de Zen es Incorrecta!",aIndex,1);
	}

	int TargObjGetIndex = gObjGetIndex(CharName);

	if(gObjIsConnected(TargObjGetIndex) == 1 && TargObjGetIndex != -1){
		gObj[TargObjGetIndex].Money = Money;
		GCMoneySend(TargObjGetIndex,Money);
	}

	
	GCServerMsgStringSend("Zen Modificado",aIndex,1);
	GCServerMsgStringSend("Tu Zen fue Modificado por un GameMaster",TargObjGetIndex,1);
	LogAddTD("[GameMaster][%s][%s] Change Zen Amount [%d,%s,%d]",lpObj->AccountID,lpObj->Name,TargObjGetIndex,CharName,Money);
}

void CCommands::SendGG(int aIndex, char * Message){

	char Msg[256];
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Authority != 32) return;

	//AntiCrash Prevent
	for(int X=0; X < (int)strlen(Message);X++){
		if(Message[X] == '%') Message[X] = '"';
	}
	
	sprintf(Msg,"[%s] %s",lpObj->Name,Message);
	GCServerMsgStringSendAll(Msg,0);
	LogAddTD("[GameMaster][%s][%s] General Message (%s) ",lpObj->AccountID,lpObj->Name,Message);
}

void CCommands::AddStats(int aIndex, LPCSTR AddPoints, DWORD Type)
{
	char Message[255];

	int Points = atoi(AddPoints);
	if(Points < 1) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	if(Configs.AddStatsEnable == 0) return;
	
	if(Points > Configs.AddStatsMaxPoints){
		sprintf(Message,"Solo puedes agregar %d Punto(s) a la Vez",Configs.AddStatsMaxPoints);
		GCServerMsgStringSend(Message,aIndex,1);
		return;
	}

	if(Configs.AddStatsLevel > lpObj->Level){
		sprintf(Message,"Necesitas ser Nivel %d para usar este Comando",Configs.AddStatsLevel);
		GCServerMsgStringSend(Message,aIndex,1);
		return;
	}

	if(Configs.AddStatsCost > lpObj->Money){
		sprintf(Message,"Necesitas %d de Zen para usar este Comando",Configs.AddStatsCost);
		GCServerMsgStringSend(Message,aIndex,1);
		return;
	}


	if(Type == 4 && lpObj->DbClass != 64 && lpObj->DbClass != 66){
		return;
	}

	if(lpObj->LevelUpPoint < Points){
		GCServerMsgStringSend("No tienes suficientes Puntos",aIndex,1);
		return;
	}
	
	
	int Class = 0;
	switch(lpObj->DbClass){
		case 0: case 1: case 3: Class = 0; break; //Dark Wizard
		case 16: case 17: case 19: Class = 1; break; //Dark Knight
		case 32: case 33: case 35: Class = 2; break; //Fairy Elf
		case 48: case 50: Class = 3; break; //Magic Gladiator
		case 64: case 66: Class = 4; break; //Dark Lord
		case 80: case 81: case 83: Class = 5; break; //Summoner
	}

	switch(Type){
		case 3:
			if((lpObj->Strength + Points) > Configs.MaxStr[Class]){
				sprintf(Message,"Solo puedes agregar %d Puntos en Fuerza",Configs.MaxStr[Class]);
				GCServerMsgStringSend(Message,aIndex,1);
				return;
			}
		break;

		case 2:
			if((lpObj->Dexterity + Points) > Configs.MaxAgi[Class]){
				sprintf(Message,"Solo puedes agregar %d Puntos en Agilidad",Configs.MaxStr[Class]);
				GCServerMsgStringSend(Message,aIndex,1);
				return;
			}
		break;

		case 1:
			if((lpObj->Vitality + Points) > Configs.MaxVit[Class]){
				sprintf(Message,"Solo puedes agregar %d Puntos en Vitalidad",Configs.MaxStr[Class]);
				GCServerMsgStringSend(Message,aIndex,1);
				return;
			}
		break;

		case 0:
			if((lpObj->Energy + Points) > Configs.MaxEne[Class]){
				sprintf(Message,"Solo puedes agregar %d Puntos en Energia",Configs.MaxStr[Class]);
				GCServerMsgStringSend(Message,aIndex,1);
				return;
			}
		break;

		case 4:
			if((lpObj->Leadership + Points) > Configs.MaxCom){
				sprintf(Message,"Solo puedes agregar %d Puntos en Command",Configs.MaxStr[Class]);
				GCServerMsgStringSend(Message,aIndex,1);
				return;
			}
		break;
	}

	lpObj->Money -= Configs.AddStatsCost;
	GCMoneySend(aIndex,lpObj->Money);
	
	switch(Type){
		case 3:
			lpObj->Strength += Points;
		break;
		case 2:
			lpObj->Dexterity += Points;
		break;
		case 1:
			lpObj->Vitality += Points;
			lpObj->MaxLife += lpObj->VitalityToLife * Points;
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);
		break;
		case 0:
			lpObj->Energy += Points;
			lpObj->MaxMana += lpObj->EnergyToMana * Points;
			gObjSetBP(lpObj->m_Index);
			GCManaSend(lpObj->m_Index,lpObj->MaxMana + lpObj->AddMana,0xFE,0,lpObj->MaxBP);
		break;
		case 4:
			lpObj->Leadership += Points;
		break;
	}

	BYTE PMSG_USE_STAT_FRUIT[8] = { 0xC1,0x08,0x2C,0x00,0x01,0x00,0x03,0x00 };
	PMSG_USE_STAT_FRUIT[4] = LOBYTE(Points);
	PMSG_USE_STAT_FRUIT[5] = HIBYTE(Points);
	PMSG_USE_STAT_FRUIT[6] = (BYTE)Type;
	DataSend(aIndex,(unsigned char *)PMSG_USE_STAT_FRUIT,8);

	lpObj->LevelUpPoint -= Points;
	GCLevelUpMsgSend(lpObj->m_Index,1);

	gObjCalCharacter(aIndex);

	sprintf(Message,"%d Puntos Agregados!",Points);
	GCServerMsgStringSend(Message,aIndex,1);
	sprintf(Message,"Te quedan %d Puntos",lpObj->LevelUpPoint);
	GCServerMsgStringSend(Message,aIndex,1);
}

	
void CCommands::PKClear(int aIndex){
	
	LPOBJ lpObj = &gObj[aIndex];

	if(Configs.PKClearEnable != 1) return;
	if(lpObj->m_PK_Level <= 3) return;

	char Buff[255];
	if(lpObj->Level < Configs.PKClearLevel){
		sprintf(Buff,"Necesitas ser Nivel %d para usar este Comando",Configs.PKClearLevel);
		GCServerMsgStringSend(Buff,aIndex,1);
		return;
	}

	
	if(lpObj->Money < Configs.PKClearCost){
		sprintf(Buff,"Necesitas %d de Zen para usar este Comando",Configs.PKClearCost);
		GCServerMsgStringSend(Buff,aIndex,1);
		return;
	}

	//If is not in Map and Map is not -1 And User is not VIP
	if(gObj->PaymentLevel != 1 && Configs.Payment_IsEnable == 1 && Configs.PKClearMap != -1){
		if(lpObj->MapNumber != Configs.PKClearMap){
			GCServerMsgStringSend("No puedes usar este comando en este Mapa",aIndex,1);
			return;
		}
	}

	lpObj->m_PK_Level = 3;
	lpObj->m_PK_Count = 0;
	lpObj->m_PK_Time = 0;
	GCPkLevelSend(aIndex,3);
	
	lpObj->Money -= Configs.PKClearCost;
	GCMoneySend(aIndex,lpObj->Money);

	GCServerMsgStringSend("PK Level Limpiado",aIndex,1);
	LogAddTD("[PKClear][%s][%s] Clear PK Level [%d,3]",lpObj->AccountID,lpObj->Name,aIndex);
}


void CCommands::ServerInfo(int aIndex){
	char Temp[255];
	GCServerMsgStringSend("GameServer Emulator - Season5.4 Version",aIndex,0);
	GCServerMsgStringSend("Developed by Jose & David",aIndex,0);
	wsprintf(Temp,"Thanks to Deathway, eRRoR, HermeX and others Related to Decompilation");
	GCServerMsgStringSend(Temp,aIndex,0);
}

void CCommands::Online(int aIndex){

	char Buff[256];
	int Players = 0;

	for(int X=OBJ_STARTUSERINDEX; X < (OBJMAX-1); X++){
		if(gObjIsConnected(X) == 1) Players++;
	}

	sprintf(Buff,"[Info] Usuarios Online: %d",Players);
	GCServerMsgStringSend(Buff,aIndex,1);
}

void CCommands::BanCharacter(int aIndex, LPCSTR Message){
	
	LPOBJ lpObj = &gObj[aIndex];
	char CharName[11];
	int TargObjGetIndex;

	if(lpObj->Authority != 32) return;

	sscanf(Message,"%s",&CharName);
	TargObjGetIndex = gObjGetIndex(CharName);
	
	if(gObjIsConnected(TargObjGetIndex) == 1 && TargObjGetIndex != -1){
		
		gObj[TargObjGetIndex].Authority = 1;

		GCServerMsgStringSend("Personaje Baneado",aIndex,1);
		GCServerMsgStringSend("Personaje Baneado",TargObjGetIndex,1);
		gObjCloseSet(TargObjGetIndex,0);

		LogAddTD("[GameMaster][%s][%s] Ban Character [%d,%s]",lpObj->AccountID,lpObj->Name,TargObjGetIndex,CharName);
	} else {
		GCServerMsgStringSend("El Personaje no esta Conectado",aIndex,1);
	}
}

void CCommands::UnBanCharacter(int aIndex, LPCSTR Message){
	
	LPOBJ lpObj = &gObj[aIndex];
	char CharName[11];
	int TargObjGetIndex;

	if(lpObj->Authority != 32) return;

	sscanf(Message,"%s",&CharName);

	TargObjGetIndex = gObjGetIndex(CharName);
	
	gObj[TargObjGetIndex].Authority = 0;

	GCServerMsgStringSend("Personaje Desbaneado",aIndex,1);

	LogAddTD("[GameMaster][%s][%s] UnBan Character [%s]",lpObj->AccountID,lpObj->Name,CharName);
}


void CCommands::BanAccount(int aIndex, LPCSTR Message){
	
	LPOBJ lpObj = &gObj[aIndex];
	char Account[11];

	if(lpObj->Authority != 32) return;

	sscanf(Message,"%s",&Account);
	
	/*MuDB.Close();
	MuDB.ExecQuery("UPDATE MEMB_INFO SET bloc_code='1' WHERE memb___id='%s'",Account);
	MuDB.Fetch();
	
	int TargObjGetIndex = gObjGetIndexAccount(Account);*/
	
	/*if(TargObjGetIndex != -1){
		GCServerMsgStringSend("Cuenta Baneada",aIndex,1);
		GCServerMsgStringSend("Cuenta Baneada",TargObjGetIndex,1);
		gObjCloseSet(TargObjGetIndex,0);
	}*/

	LogAddTD("[GameMaster][%s][%s] Ban Account [%s]",lpObj->AccountID,lpObj->Name,Account);
}

void CCommands::UnBanAccount(int aIndex, LPCSTR Message){
	
	LPOBJ lpObj = &gObj[aIndex];
	char Account[11];

	if(lpObj->Authority != 32) return;

	sscanf(Message,"%s",&Account);
	
	/*MuDB.Close();
	MuDB.ExecQuery("UPDATE MEMB_INFO SET bloc_code='0' WHERE memb___id='%s'",Account);
	MuDB.Fetch();*/
	
	GCServerMsgStringSend("Cuenta Desbaneada",aIndex,1);

	LogAddTD("[GameMaster][%s][%s] UnBan Account [%s]",lpObj->AccountID,lpObj->Name,Account);
}

void CCommands::Reload(int aIndex, LPCSTR Type){
	
	switch(atoi(Type)){
		case 1: //Configs
			//Configs.LoadConfigs();
			GCServerMsgStringSend("[Reload] Configs Reloaded",aIndex,1);
		break;

		case 2: //GameMaster List
			//GMManager.LoadConfigs();
			GCServerMsgStringSend("[Reload] GameMasters List Reloaded",aIndex,1);
		break;

		case 3: //Party System
			//PartySystem.LoadConfig();
			GCServerMsgStringSend("[Reload] PartySystem Reloaded",aIndex,1);
		break;

		case 4: //Moss the Gambler
			//MossTheGambler.LoadItemList();
			GCServerMsgStringSend("[Reload] Moss the Gambler Item List Reloaded",aIndex,1);
		break;

		case 5: //Blood Castle Bag
			//BloodCastle.LoadItemList();
			GCServerMsgStringSend("[Reload] Blood Castle Item List Reloaded",aIndex,1);
		break;

		case 6: //Duel Arena
			DuelManager.StartProcess();
			GCServerMsgStringSend("[Reload] Duel Manager Setting Reloaded",aIndex,1);
		break;

		case 7: //SafeMap System
			//SafeMap.StartProcess();
			GCServerMsgStringSend("[Reload] SafeMap Setting Reloaded",aIndex,1);
		break;

		case 8: //Inferno Attack Event
			GCServerMsgStringSend("[Reload] Inferno Attack Reloaded",aIndex,1);
			//InfernoEvent.LoadItemList();
		break;

		case 9: //Medussa Event
			GCServerMsgStringSend("[Reload] Medussa Event Reloaded",aIndex,1);
			//InfernoEvent.LoadItemList();
		break;

		case 10: //Extreme Points
			GCServerMsgStringSend("[Reload] Extreme Points Reloaded",aIndex,1);
			//ExtremePoints.LoadStepsList();
		break;

		case 11:
			GCServerMsgStringSend("[Reload] Quest Manager Reloaded",aIndex,1);
			QuestManager.StartProcess();
		break;

		case 12:
			GCServerMsgStringSend("[Reload] Marry Reloaded",aIndex,1);
			//MarrySystem.StartProcess(1);
		break;

		case 13:
			GCServerMsgStringSend("[Reload] News Reloaded",aIndex,1);
			//NewsSystem.ReloadConfigs();
		break;

		case 14:
			GCServerMsgStringSend("[Reload] Doppelganger Reloaded",aIndex,1);
			Doppelganger.StartProcess(1);
		break;
		
		case 15:
			GCServerMsgStringSend("[Reload] Imperial Guardian Reloaded",aIndex,1);
			ImperialGuardian.StartProcess(1);
		break;

		case 0:
			GCServerMsgStringSend("[Reload] 1: Configs Reloaded",aIndex,1);
			GCServerMsgStringSend("[Reload] 2: GameMasters List",aIndex,1);
			GCServerMsgStringSend("[Reload] 3: PartySystem",aIndex,1);
			GCServerMsgStringSend("[Reload] 4: Moss the Gambler ItemList",aIndex,1);
			GCServerMsgStringSend("[Reload] 5: Blood Castle ItemList",aIndex,1);
			GCServerMsgStringSend("[Reload] 6: Duel Manager Setting ",aIndex,1);
			GCServerMsgStringSend("[Reload] 7: SafeMap Setting",aIndex,1);
			GCServerMsgStringSend("[Reload] 8: Inferno Attack ItemList",aIndex,1);
			GCServerMsgStringSend("[Reload] 9: Medussa ItemList",aIndex,1);
			GCServerMsgStringSend("[Reload] 10: Extreme Points",aIndex,1);
			GCServerMsgStringSend("[Reload] 11: Quest Manager",aIndex,1);
			GCServerMsgStringSend("[Reload] 12: Marry System",aIndex,1);
			GCServerMsgStringSend("[Reload] 13: News System",aIndex,1);
			GCServerMsgStringSend("[Reload] 14: Doppelganger Event",aIndex,1);
			GCServerMsgStringSend("[Reload] 15: ImperialGuardian Event",aIndex,1);
		break;
	}	
}

void CCommands::SendFireworks(int aIndex, LPCSTR Message)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->Authority != 32) return;

	int Map = lpObj->MapNumber, X = lpObj->X, Y = lpObj->Y, Amount = 1;
	sscanf(Message,"%d %d %d %d",&Map,&X,&Y,&Amount);
	
	if(Amount > 15) Amount = 15;
	if(Amount < 0) Amount = 1;

	PMSG_SERVERCMD pMsg;

	PHeadSubSetB((LPBYTE)&pMsg,0xF3,0x40, sizeof(pMsg));
	pMsg.CmdType = 0;

	for(int i=0;i < Amount;i++){
		pMsg.X = X+(rand() % 5)*2 - 4;
		pMsg.Y = Y+(rand() % 5)*2 - 4;
		MsgSendV2(lpObj,(LPBYTE)&pMsg, sizeof(pMsg));
		::DataSend(lpObj->m_Index ,(LPBYTE)&pMsg, sizeof(pMsg));
	}

	LogAddTD("[GameMaster][%s][%s] Send Fireworks! [%d,%d,%d,%d]",lpObj->AccountID,lpObj->Name,Map,X,Y,Amount);
}


void CCommands::GetPlayerInfo(int aIndex, LPCSTR Message)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->Authority != 32) return;

	char Account[11];
	char cBuff[255] = {0};
	sscanf(Message,"%s",&Account);

	//Get Player Index
	int TargObjGetIndex = gObjGetIndex(Account);

	if(gObjIsConnected(TargObjGetIndex) == 0){
		GCServerMsgStringSend("[GetInfo] Player is Offline!",aIndex,1);
		return;
	}
	
	LPOBJ gTargetObj = &gObj[TargObjGetIndex];

	wsprintf(cBuff,"Account: %s, Name: %s",gTargetObj->AccountID,gTargetObj->Name);
	GCServerMsgStringSend(cBuff,aIndex,0);
	wsprintf(cBuff,"Level: %d, MasterLevel: %d",gTargetObj->Level,gTargetObj->MLevel);
	GCServerMsgStringSend(cBuff,aIndex,0);
	wsprintf(cBuff,"Map: %d (X: %d / Y: %d)",gTargetObj->MapNumber,gTargetObj->X,gTargetObj->Y);
	GCServerMsgStringSend(cBuff,aIndex,0);
	wsprintf(cBuff,"PkLevel: %d, PkCount: %d",gTargetObj->m_PK_Level,gTargetObj->m_PK_Count);
	GCServerMsgStringSend(cBuff,aIndex,0);
	wsprintf(cBuff,"Class: %d, Resets: %d, VIP: %d",gTargetObj->DbClass,gTargetObj->Resets,gTargetObj->PaymentLevel);
	GCServerMsgStringSend(cBuff,aIndex,0);
}

void CCommands::Spawn(int aIndex, LPCSTR Message)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Authority != 32) return;
	
	int MobNumber = 0, Map = lpObj->MapNumber, CordX = lpObj->X, CordY = lpObj->Y, Amount = 1;
	char cBuff[255] = {0};
	sscanf(Message,"%d %d %d %d %d",&MobNumber,&Map,&CordX,&CordY,&Amount);

	if(Amount > 15) Amount = 15;
	if(MobNumber > 548) MobNumber = 548;
	int TempIndex = -1;

	for(int X=0; X < Amount;X++){
		int TempIndex = gObjAddMonster(Map);
		if(TempIndex > 0){
			gObj[TempIndex].X = CordX;
			gObj[TempIndex].Y = CordY;
			gObjSetMonster(TempIndex,MobNumber);
		}
	}

	GCServerMsgStringSend("[Spawn] Monster Creado",aIndex,1);
	LogAddTD("[GameMaster][%s][%s] Spawn Monster [%d,%d,%d,%d]",lpObj->AccountID,lpObj->Name,Map,CordX,CordY,Amount);
}


void CCommands::ChangeSkin(int aIndex, LPCSTR Message){

	LPOBJ lpObj = &gObj[aIndex];
	char CharName[11];
	int Skin = 0;

	if(lpObj->Authority != 32) return;
	
	sscanf(Message,"%s %d",&CharName,&Skin);

	int TargObjGetIndex = gObjGetIndex(CharName);

	if(gObjIsConnected(TargObjGetIndex) == 1 && TargObjGetIndex != -1){
		gObj[TargObjGetIndex].m_Change = Skin;
		gObjViewportListProtocolCreate(&gObj[TargObjGetIndex]);
	}
	
	GCServerMsgStringSend("Skin Modificado",aIndex,1);
	GCServerMsgStringSend("Fuiste Transformado por un GameMaster",TargObjGetIndex,1);
	LogAddTD("[GameMaster][%s][%s] Change Skin [%d,%s,%d]",lpObj->AccountID,lpObj->Name,TargObjGetIndex,CharName,Skin);
}

struct PMSG_REQ_CHANGEWAREHOUSE {
	PBMSG_HEAD2 h;	// C1:B9
	int aIndex;	// 4
	char Account[11];	// C
	BYTE InUse;
	BYTE Change;
};

void CCommands::ChangeVault(int aIndex, LPCSTR Message){

	if(strlen(Message) < 1) return;
	
	if(Configs.MultiVaultEnable != 1) return;

	int VaultID = 0, VaultInUse = 0;
	BOOL q_Status;
	sscanf(Message,"%d",&VaultID);

	if(VaultID < 0 || VaultID > (Configs.MultiVaultMax-1)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->WarehouseSave != 0 || (lpObj->m_IfState.use && lpObj->m_IfState.type == 6)){
		GCServerMsgStringSend("[MultiBaul] Debes Cerrar el Baul Primero",aIndex,1);
		return;
	} 

	if(lpObj->MultiVautFloodTime > 0){
		return;
	}


	if(lpObj->VaultInUse == VaultID){
		GCServerMsgStringSend("[MultiBaul] Ya estas Usando este Baul",aIndex,1);
		return;
	}

	lpObj->MultiVautFloodTime = GetTickCount() + (1000 * Configs.MultiVaultFloodTime);

	PMSG_REQ_CHANGEWAREHOUSE pMsg;
	pMsg.h.set((LPBYTE)&pMsg,0xAA,0x01,sizeof(pMsg));
	pMsg.Change = VaultID;
	pMsg.InUse = lpObj->VaultInUse;
	pMsg.aIndex = aIndex;
	memcpy(pMsg.Account,lpObj->AccountID,sizeof(pMsg.Account)-1);
	
	cDBSMng.Send((char*)&pMsg,pMsg.h.size);

	char Buff[255];
	sprintf(Buff,"[MultiBaul] Baul Cambiado de %d a %d",lpObj->VaultInUse,VaultID);
	GCServerMsgStringSend(Buff,aIndex,1);

	LogAddTD("[MultiVault][%s][%s] Warehouse Changed %d -> %d",lpObj->AccountID,lpObj->Name,lpObj->VaultInUse,VaultID);
	lpObj->VaultInUse = VaultID;
}

BOOL CCommands::Manager(int aIndex, PMSG_CHATDATA * lpMsg){

	LPOBJ lpObj = &gObj[aIndex];

	////Guild SQL Injection Fix!
	if(lpMsg->chatmsg[0] == '@' && lpMsg->chatmsg[1] == '>'){

		for(int X=0; X < (int)strlen(lpMsg->chatmsg);X++){
			if(lpMsg->chatmsg[X] == '\''){
				LogAddTD("[ANTI-HACK][%s][%s] Try to do SQL Injection with Guild Notice!",lpObj->AccountID,lpObj->Name);
				lpMsg->chatmsg[X] = '"';
			}
		}
	}


	char String[60];
	char * Msg;

	memset(String,0,sizeof(String));
	strcpy(String, lpMsg->chatmsg);

	if((Msg = strtok(String," ")) == NULL){
		return FALSE;
	}

	int CLen = strlen(Msg);

	int GMNumber = GMManager.GMListNumber(lpObj->Name);

	if(_stricmp("/post",Msg) == 0){
		this->SendPost(aIndex,&lpMsg->chatmsg[CLen+1]);
		return TRUE;
	}

	if(_stricmp("/serverinfo",Msg) == 0){
		this->ServerInfo(aIndex);
		return TRUE;
	}

	if(_stricmp("/item",Msg) == 0){
		if(GMNumber == -1) return FALSE;
		if(GMManager.GMFeatures[GMNumber].Item == 0) return FALSE;
		this->DropItem(aIndex,&lpMsg->chatmsg[CLen+1]);
		return TRUE;
	}

	if(_stricmp("/gg",Msg) == 0){
		if(GMNumber == -1) return FALSE;
		if(GMManager.GMFeatures[GMNumber].SendGG == 0) return FALSE;
		this->SendGG(aIndex,&lpMsg->chatmsg[CLen+1]);
		return TRUE;
	}

	if(_stricmp("/gmove",Msg) == 0){
		this->GMove(aIndex,&lpMsg->chatmsg[CLen+1]);
		return TRUE;
	}

	if(_stricmp("/gmoveall",Msg) == 0){
		if(GMNumber == -1) return FALSE;
		if(GMManager.GMFeatures[GMNumber].GMoveALL == 0) return FALSE;
		this->GMoveALL(aIndex,&lpMsg->chatmsg[CLen+1]);
		return TRUE;
	}

	if(_stricmp("/changezen",Msg) == 0){
		if(GMNumber == -1) return FALSE;
		if(GMManager.GMFeatures[GMNumber].ChangeZen == 0) return FALSE;
		this->ChangeZen(aIndex,&lpMsg->chatmsg[CLen+1]);
		return TRUE;
	}

	if(_stricmp("/changepk",Msg) == 0){
		if(GMNumber == -1) return FALSE;
		if(GMManager.GMFeatures[GMNumber].ChangePKLevel == 0) return FALSE;
		this->ChangePKLevel(aIndex,&lpMsg->chatmsg[CLen+1]);
		return TRUE;
	}

	if(_stricmp("/addstr",Msg) == 0){
		this->AddStats(aIndex,&lpMsg->chatmsg[CLen+1],3);
		return TRUE;
	}

	if(_stricmp("/addagi",Msg) == 0){
		this->AddStats(aIndex,&lpMsg->chatmsg[CLen+1],2);
		return TRUE;
	}

	if(_stricmp("/addvit",Msg) == 0){
		this->AddStats(aIndex,&lpMsg->chatmsg[CLen+1],1);
		return TRUE;
	}

	if(_stricmp("/addene",Msg) == 0){
		this->AddStats(aIndex,&lpMsg->chatmsg[CLen+1],0);
		return TRUE;
	}

	if(_stricmp("/addcmd",Msg) == 0){
		this->AddStats(aIndex,&lpMsg->chatmsg[CLen+1],4);
		return TRUE;
	}

	if(_stricmp("/pkclear",Msg) == 0){
		this->PKClear(aIndex);
		return TRUE;
	}

	if(_stricmp("/online",Msg) == 0){
		this->Online(aIndex);
		return TRUE;
	}

	if(_stricmp("/traceparty",Msg) == 0){
		PartySystem.WarpToParty(aIndex, lpObj->PartyNumber);
	}

	if(_stricmp("/marry",Msg) == 0){
		MarrySystem.ProposeMarry(aIndex,&lpMsg->chatmsg[CLen+1]);
		return TRUE;
	}

	if(_stricmp("/accept",Msg) == 0){
		MarrySystem.AcceptMarry(aIndex);
		return TRUE;
	}

	if(_stricmp("/tracemarry",Msg) == 0){
		MarrySystem.TraceMarry(aIndex);
		return TRUE;
	}
	
	if(_stricmp("/divorce",Msg) == 0){
		MarrySystem.SendDivorce(aIndex);
		return TRUE;
	}
	
	if(_stricmp("/yes",Msg) == 0){
		MarrySystem.DivorceProcess(aIndex,0);
		return TRUE;
	}
	
	if(_stricmp("/forcedivorce",Msg) == 0){
		MarrySystem.DivorceProcess(aIndex,1);
		return TRUE;
	}

	if(_stricmp("/getmarry",Msg) == 0){
		MarrySystem.MarryInfo(aIndex);
		return TRUE;
	}

	if(_stricmp("/baul",Msg) == 0){
		this->ChangeVault(aIndex,&lpMsg->chatmsg[CLen+1]);
		return TRUE;
	}

	if(lpObj->Authority == 32){

		if(_stricmp("/reload",Msg) == 0){
			if(GMNumber == -1) return FALSE;
			if(GMManager.GMFeatures[GMNumber].Reloads == 0) return FALSE;
			this->Reload(aIndex,&lpMsg->chatmsg[CLen+1]);
			return TRUE;
		}

		if(_stricmp("/spawn",Msg) == 0){
			this->Spawn(aIndex,&lpMsg->chatmsg[CLen+1]);
			return TRUE;
		}
		
		if(_stricmp("/skin",Msg) == 0){
			this->ChangeSkin(aIndex,&lpMsg->chatmsg[CLen+1]);
			return TRUE;
		}

		if(_stricmp("/fireworks",Msg) == 0){
			this->SendFireworks(aIndex,&lpMsg->chatmsg[CLen+1]);
			return TRUE;
		}

		if(_stricmp("/info",Msg) == 0){
			this->GetPlayerInfo(aIndex,&lpMsg->chatmsg[CLen+1]);
			return TRUE;
		}

		if(GMNumber == -1) return FALSE;
		if(GMManager.GMFeatures[GMNumber].BanCommands == 0) return FALSE;

		//Ban Commands
		if(_stricmp("/banchar",Msg) == 0){
			this->BanCharacter(aIndex,&lpMsg->chatmsg[CLen+1]);
			return TRUE;
		}

		if(_stricmp("/unbanchar",Msg) == 0){
			this->UnBanCharacter(aIndex,&lpMsg->chatmsg[CLen+1]);
			return TRUE;
		}

		if(_stricmp("/banacc",Msg) == 0){
			this->BanAccount(aIndex,&lpMsg->chatmsg[CLen+1]);
			return TRUE;
		}

		if(_stricmp("/unbanacc",Msg) == 0){
			this->UnBanAccount(aIndex,&lpMsg->chatmsg[CLen+1]);
			return TRUE;
		}
	}

	return FALSE;
}