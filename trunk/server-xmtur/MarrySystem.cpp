#include "Stdafx.h"
#include "MarrySystem.h"
#include "User.h"
#include "GameMain.h"
#include "LogProc.h"
#include "Winutil.h"
#include "DSProtocol.h"

CMarrySystem MarrySystem;

void MarrySystem_ProposeTimer(void * lParam){

	while(true){
		Sleep(1000);

		for(int i=OBJ_STARTUSERINDEX; i < (OBJMAX);i++){

			if(gObjIsConnected(i)){

				if(MarrySystem.IsEnable == 1){
					if(gObj[i].MarryProposeTime < GetTickCount() && gObj[i].MarryProposeTime > 0){

						GCServerMsgStringSend("El Tiempo de la Propuesta Termino...",i,1);

						gObj[i].MarryProposeTime = 0;
						gObj[i].MarryTarget = 0;
					}
				}

				if(Configs.MultiVaultEnable == 1){
					if(gObj[i].MultiVautFloodTime < GetTickCount() && gObj[i].MultiVautFloodTime > 0){

						GCServerMsgStringSend("Ya puedes abrir tu Baul...",i,1);

						gObj[i].MultiVautFloodTime = 0;
					}
				}
			}
		}	
	}

	_endthread();
}

void CMarrySystem::StartProcess(int Reload){

	this->IsEnable = GetPrivateProfileInt("MarrySystem","IsEnable",1,MARRYSYSTEM_FILE);
	this->MapNumber = GetPrivateProfileInt("MarrySystem","MapNumber",2,MARRYSYSTEM_FILE);
	this->CordX_1 = GetPrivateProfileInt("MarrySystem","CordX_1",1,MARRYSYSTEM_FILE);
	this->CordY_1 = GetPrivateProfileInt("MarrySystem","CordY_1",1,MARRYSYSTEM_FILE);
	this->CordX_2 = GetPrivateProfileInt("MarrySystem","CordX_2",1,MARRYSYSTEM_FILE);
	this->CordY_2 = GetPrivateProfileInt("MarrySystem","CordY_2",1,MARRYSYSTEM_FILE);

	this->EnableTraceMarry = GetPrivateProfileInt("MarrySystem","IsTraceMarryEnable",1,MARRYSYSTEM_FILE);
	this->MarryPlusExperience = GetPrivateProfileInt("MarrySystem","Marry_PlusExperience",1,MARRYSYSTEM_FILE);
	this->ForceDivorceMoney = GetPrivateProfileInt("MarrySystem","Marry_ForceDivorceMoney",20000000,MARRYSYSTEM_FILE);

	this->MinLevel = GetPrivateProfileInt("MarrySystem","LevelRequired",350,MARRYSYSTEM_FILE);
	this->MinResets = GetPrivateProfileInt("MarrySystem","ResetsRequired",0,MARRYSYSTEM_FILE);

	this->IsShopEnable = GetPrivateProfileInt("MarrySystem","IsShopEnable",1,MARRYSYSTEM_FILE);
	this->Shop_MapNumber = GetPrivateProfileInt("MarrySystem","MarryShop_MapNumber",2,MARRYSYSTEM_FILE);
	this->Shop_CordX = GetPrivateProfileInt("MarrySystem","MarryShop_X",2,MARRYSYSTEM_FILE);
	this->Shop_CordY = GetPrivateProfileInt("MarrySystem","MarryShop_Y",2,MARRYSYSTEM_FILE);

	LogAddTD("[MarrySystem] IsEnable: %d",this->IsEnable);

	if(Reload == 0){ //Start Core...
		_beginthread(MarrySystem_ProposeTimer,0,NULL);
	}
}

void CMarrySystem::ProposeMarry(int aIndex, LPSTR Message){

	if(this->IsEnable == 0) return;

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	char Buff[255];
	char ProposeName[10];
	sscanf(Message,"%s",&ProposeName);

	int aTargetIndex = gObjGetIndex(ProposeName);
	
	if(aTargetIndex < (OBJMAX-OBJMAXUSER) || aTargetIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aTargetIndex)) return;

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->IsMarried == TRUE
		|| lpTargetObj->IsMarried == TRUE){
		LogAddTD("[MarrySystem][%s][%s] Cant Marry because Some of the two Players was Already Married...",lpObj->AccountID,lpObj->Name);
		return;
	}

	sprintf(Buff,"El Nivel Minimo para Casarse es %d",this->MinLevel);

	if(this->MinLevel > lpObj->Level){
		GCServerMsgStringSend(Buff,aIndex,1);
		LogAddTD("[MarrySystem][%s][%s] Cant Marry because MinLevel is (%d)...",lpObj->AccountID,lpObj->Name,this->MinLevel);
		return;
	}

	if(this->MinLevel > lpTargetObj->Level){
		GCServerMsgStringSend(Buff,aTargetIndex,1);
		LogAddTD("[MarrySystem][%s][%s] Cant Marry because MinLevel is (%d)...",lpTargetObj->AccountID,lpTargetObj->Name,this->MinLevel);
		return;
	}

	sprintf(Buff,"Los Resets Minimos para Casarse son %d",this->MinResets);

	if(this->MinResets > lpObj->Resets){
		GCServerMsgStringSend(Buff,aIndex,1);
		LogAddTD("[MarrySystem][%s][%s] Cant Marry because MinResets is (%d)...",lpObj->AccountID,lpObj->Name,this->MinResets);
		return;
	}

	if(this->MinResets > lpTargetObj->Resets){
		GCServerMsgStringSend(Buff,aTargetIndex,1);	
		LogAddTD("[MarrySystem][%s][%s] Cant Marry because MinResets is (%d)...",lpTargetObj->AccountID,lpTargetObj->Name,this->MinResets);
		return;
	}
	
	if(lpObj->DbClass == 32 || lpObj->DbClass == 33 || lpObj->DbClass == 35
	|| lpObj->DbClass == 80 || lpObj->DbClass == 81 || lpObj->DbClass == 83){
		LogAddTD("[MarrySystem][%s][%s] Cant Marry because Proposer is a Woman...",lpObj->AccountID,lpObj->Name,ProposeName);
		return;
	}

	if(lpTargetObj->DbClass != 32 && lpTargetObj->DbClass != 33 && lpTargetObj->DbClass != 35
		&& lpTargetObj->DbClass != 80 && lpTargetObj->DbClass != 81 && lpTargetObj->DbClass != 83){
		LogAddTD("[MarrySystem][%s][%s] Cant Marry because Proposed (%s) is not a Woman...",lpObj->AccountID,lpObj->Name,ProposeName);
		return;
	}

	if(lpTargetObj->MapNumber != this->MapNumber || lpObj->MapNumber != this->MapNumber){
		GCServerMsgStringSend("No puedes Casarte Aqui",aIndex,1);
		GCServerMsgStringSend("No puedes Casarte Aqui",aTargetIndex,1);	
		LogAddTD("[MarrySystem][%s][%s] - (%s)(%s) Cant Marry on that Map.",lpObj->AccountID,lpObj->Name,lpTargetObj->AccountID,lpTargetObj->Name);
		return;
	}

	if((lpObj->X < this->CordX_1 || lpObj->X > this->CordX_2) || (lpObj->Y < this->CordY_1 || lpObj->Y > this->CordY_2)){
		GCServerMsgStringSend("No puedes Casarte Aqui",aIndex,1);
		LogAddTD("[MarrySystem][%s][%s] Cant Marry on that Cords.",lpObj->AccountID,lpObj->Name);
		return;
	}

	if((lpTargetObj->X < this->CordX_1 || lpTargetObj->X > this->CordX_2) || (lpTargetObj->Y < this->CordY_1 || lpTargetObj->Y > this->CordY_2)){
		GCServerMsgStringSend("No puedes Casarte Aqui",aIndex,1);
		LogAddTD("[MarrySystem][%s][%s] Cant Marry on that Cords.",lpTargetObj->AccountID,lpTargetObj->Name);
		return;
	}

	//Set Marry Information...
	lpTargetObj->MarryTarget = aIndex;
	lpTargetObj->MarryProposeTime = GetTickCount() + 5000; 

	lpObj->MarryTarget = aIndex;
	lpObj->MarryProposeTime = GetTickCount() + 5000;

	sprintf(Buff,"Has propuesto Casamiento a %s",lpTargetObj->Name);
	GCServerMsgStringSend(Buff,aIndex,1);
	sprintf(Buff,"%s te ha propuesto Casamiento, Aceptas?",lpObj->Name);
	GCServerMsgStringSend(Buff,aTargetIndex,0);

	GCServerMsgStringSend("Tienes 5 Segundo(s) para Responder",aTargetIndex,1);
}

void CMarrySystem::AcceptMarry(int aIndex){ //If say Yes!

	if(this->IsEnable == 0) return;

	if(aIndex < (OBJ_STARTUSERINDEX) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MarryTarget == 0){
		LogAddTD("[MarrySystem][%s][%s] Target was Incorrect.",lpObj->AccountID,lpObj->Name);
		return;
	}

	int aTargetIndex = lpObj->MarryTarget;

	if(aTargetIndex < (OBJ_STARTUSERINDEX) || aTargetIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aTargetIndex)) return;

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->MarryProposeTime < GetTickCount() && lpObj->MarryTarget != 0){
		GCServerMsgStringSend("El Tiempo de la Propuesta Termino...",aIndex,1);	
		return;
	}

	//Set Marry Information...
	lpObj->IsMarried = TRUE;
	memcpy(lpObj->HusbandWifeName,lpTargetObj->Name,sizeof(lpTargetObj->Name));

	lpTargetObj->IsMarried = TRUE;
	memcpy(lpTargetObj->HusbandWifeName,lpObj->Name,sizeof(lpObj->Name));


	DSSetMarryInfo(lpObj->m_Index);

	char Buff[255];

	sprintf(Buff,"Felicitacines! Ahora estas Casada con %s",lpTargetObj->Name);
	GCServerMsgStringSend(Buff,aIndex,0);
	sprintf(Buff,"Felicitacines! Ahora estas Casado con %s",lpObj->Name);
	GCServerMsgStringSend(Buff,aTargetIndex,0);	

	sprintf(Buff,"Felicitacines! Ahora %s y %s estan Casados",lpTargetObj->Name,lpObj->Name);
	GCServerMsgStringSendAll(Buff,0);

	//Clean Structure
	lpObj->MarryTarget = 0;
	lpTargetObj->MarryTarget = 0;
	lpObj->MarryProposeTime = 0;
	lpTargetObj->MarryProposeTime = 0;

	//Send Fireworks
	PMSG_SERVERCMD pMsg;

	PHeadSubSetB((LPBYTE)&pMsg,0xF3,0x40, sizeof(pMsg));
	pMsg.CmdType = 0;

	for(int i=0;i < 5;i++){
		pMsg.X = lpObj->X+(rand()%5)*2-4;
		pMsg.Y = lpObj->Y+(rand()%5)*2-4;
		MsgSendV2(lpObj,(LPBYTE)&pMsg, sizeof(pMsg));
		::DataSend(lpObj->m_Index,(LPBYTE)&pMsg,sizeof(pMsg));
	}
}

struct PMSG_ANS_PSHOP_TEXT_CHANGED
{
	BYTE H; // C1:3F:10
	BYTE Size;
	BYTE Head;
	BYTE SubHead;
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE btPShopText[36];	// 6
	BYTE btName[10];	// 2A
};

void CMarrySystem::SendMarryText(int aIndex){

	if(aIndex < (OBJ_STARTUSERINDEX) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	char Buff[255];
	wsprintf(Buff,"[Marry] %s",lpObj->HusbandWifeName);
	memcpy(lpObj->m_szPShopText,Buff,sizeof(Buff));
			
	PMSG_ANS_PSHOP_TEXT_CHANGED pMsg;

	pMsg.H = 0xC1;
	pMsg.Head = 0x3F;
	pMsg.SubHead = 0x10;
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	memcpy(pMsg.btPShopText,lpObj->m_szPShopText,sizeof(lpObj->m_szPShopText));
	memcpy(pMsg.btName,lpObj->Name,sizeof(lpObj->Name));
	pMsg.Size = sizeof(pMsg);

	DataSend(aIndex,(LPBYTE)&pMsg,pMsg.Size);
}

void CMarrySystem::SendDivorce(int aIndex){

	if(this->IsEnable == 0) return;

	if(aIndex < (OBJ_STARTUSERINDEX) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];
	
	int aTargetIndex = gObjGetIndex(lpObj->HusbandWifeName);
	
	if(aTargetIndex < (OBJ_STARTUSERINDEX) || aTargetIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aTargetIndex)) return;

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->IsMarried == FALSE || lpTargetObj->IsMarried == FALSE){
		LogAddTD("[MarrySystem][%s][%s] Cant Marry because Players was not Married...",lpObj->AccountID,lpObj->Name);
		return;
	}

	//Set Marry Information...
	lpTargetObj->MarryProposeTime = GetTickCount() + 5000; 
	lpObj->MarryProposeTime = GetTickCount() + 5000;

	char Buff[255];
	sprintf(Buff,"Le has pedido el Divorcio a %s",lpTargetObj->Name);
	GCServerMsgStringSend(Buff,aIndex,1);
	sprintf(Buff,"%s te ha Pedido el Divorcio!",lpObj->Name);

	GCServerMsgStringSend("Tienes 5 Segundo(s) para Responder",aTargetIndex,1);
}

void CMarrySystem::DivorceProcess(int aIndex, int Force){
	
	if(aIndex < (OBJ_STARTUSERINDEX) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	int aTargetIndex = gObjGetIndex(lpObj->HusbandWifeName);

	if(aTargetIndex < (OBJ_STARTUSERINDEX) || aTargetIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aTargetIndex)) return;

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(Force == 0){
		if(lpObj->MarryProposeTime < 1){
			GCServerMsgStringSend("El Tiempo de Divorcio se Termino...",aIndex,1);	
			return;
		}
	} else {
		if(lpObj->Money < this->ForceDivorceMoney){
			GCServerMsgStringSend("No tienes suficiente Zen para Divorciarte",aIndex,1);	
			LogAddTD("[MarrySystem][%s][%s] Cant Marry because Player is Short of Zen",lpObj->AccountID,lpObj->Name);
			return;
		}

		lpObj->Money -= this->ForceDivorceMoney;
		GCMoneySend(aIndex,lpObj->Money);
	}

	
	DSDeleteMarryInfo(lpObj->m_Index);

	char Buff[255];
	sprintf(Buff,"Te has Divorciado de %s...",lpObj->HusbandWifeName);
	GCServerMsgStringSend(Buff,aIndex,1);

	lpObj->MarryTarget = 0;
	lpObj->MarryProposeTime = 0;
	lpObj->IsMarried = FALSE;
	memset(lpObj->HusbandWifeName,0,sizeof(lpObj->HusbandWifeName));
		

	if(Force == 0){
		sprintf(Buff,"Te has Divorciado de %s...",lpObj->Name);
		GCServerMsgStringSend(Buff,aTargetIndex,1);	
		lpTargetObj->MarryTarget = 0;
		lpTargetObj->MarryProposeTime = 0;
		lpTargetObj->IsMarried = FALSE;
		memset(lpTargetObj->HusbandWifeName,NULL,11);
	} else {
		sprintf(Buff,"Costo: %d",this->ForceDivorceMoney);
		GCServerMsgStringSend(Buff,aIndex,1);
	}
}

BOOL CMarrySystem::IsInEventMap(int aIndex){

	if(aIndex < (OBJ_STARTUSERINDEX) || aIndex > (OBJMAX-1)) return TRUE;
	if(!gObjIsConnected(aIndex)) return TRUE;

	LPOBJ lpObj = &gObj[aIndex];

	if((lpObj->MapNumber >= 11 && lpObj->MapNumber <= 16 || lpObj->MapNumber == 52) //BloodCastle
		|| (lpObj->MapNumber == 9 || lpObj->MapNumber == 32)//DevilSquare 
		|| (lpObj->MapNumber >= 18 && lpObj->MapNumber <= 23 || lpObj->MapNumber == 53) //ChaosCastle
		|| (lpObj->MapNumber >= 45 && lpObj->MapNumber <= 50) //Illusion Temple
		|| (lpObj->MapNumber >= 24 && lpObj->MapNumber <= 29 || lpObj->MapNumber == 36) //Kalima
		|| (lpObj->MapNumber == 30) || (lpObj->MapNumber == 34) //Loren Deep (CS) y Crywolf
		|| (lpObj->MapNumber == 64 || lpObj->MapNumber == 62) //Duel Arena & Santa Town
		|| (lpObj->MapNumber == 39 || lpObj->MapNumber == 58) //Kanturu Tower & Raklion
		|| (lpObj->MapNumber == 41 || lpObj->MapNumber == 42) //Season3 Quest Map
		|| (lpObj->MapNumber >= 65 && lpObj->MapNumber <= 68) //Double Goer Event
		|| (lpObj->MapNumber >= 69 && lpObj->MapNumber <= 72)){ //Imperial Guardian
		return TRUE;
	}

	return FALSE;
}

void CMarrySystem::TraceMarry(int aIndex){

	if(this->EnableTraceMarry == 0) return;

	if(aIndex < (OBJ_STARTUSERINDEX) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_IfState.use && lpObj->m_IfState.type != 0) return;

	//Check if was Married :)
	if(lpObj->IsMarried != TRUE) return;

	int aTargetIndex = gObjGetIndex(lpObj->HusbandWifeName);

	if(aTargetIndex < (OBJ_STARTUSERINDEX) || aTargetIndex > (OBJMAX-1)) return;

	if(!gObjIsConnected(aTargetIndex)){
		GCServerMsgStringSend("El Jugador esta Desconectado",aIndex,1);	
		LogAddTD("[MarrySystem][%s][%s] Husband/Wife is Offline, cant Trace",lpObj->AccountID,lpObj->Name);
		return;
	}
	
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->m_PK_Level > 4){
		GCServerMsgStringSend("No puedes Moverte siendo PK",aIndex,1);
		return;
	}

	if(lpTargetObj->MapNumber == 10){
		if(lpObj->pInventory[7].m_Type == -1 && lpObj->pInventory[8].m_Type != 0x1A03){
			GCServerMsgStringSend("[MarrySystem] Can't enter WO Wings",aIndex,1);
			return;
		}
	}

	if(lpTargetObj->MapNumber == 7 && lpObj->pInventory[8].m_Type == 0x1A02){
		GCServerMsgStringSend("[MarrySystem] Can't enter with Uniria",aIndex,1);
		return;
	}
	
	int CheckGuild = memcmp(lpObj->GuildName,lpTargetObj->GuildName,11);
	
	if((lpTargetObj->MapNumber == 31 && CheckGuild != 0)
	|| (lpObj->MapNumber == 31 && CheckGuild != 0)){//Land of Trials
		LogAddTD("[MarrySystem][%s][%s] Player is in Land of Trials, and Guilds are not the Same",lpObj->AccountID,lpObj->Name);
		return;
	}

	if(this->IsInEventMap(aIndex) == TRUE){ 
		GCServerMsgStringSend("No puedes Moverte a un Evento",aIndex,1);	
		LogAddTD("[MarrySystem][%s][%s] Player is in Event Map, cant Trace",lpObj->AccountID,lpObj->Name);
		return;
	}

	if(this->IsInEventMap(aTargetIndex) == TRUE){ 
		GCServerMsgStringSend("No puedes Moverte a un Evento",aIndex,1);
		LogAddTD("[MarrySystem][%s][%s] Husband/Wife is in Event Map, cant Trace",lpObj->AccountID,lpObj->Name);
		return;
	}

	gObjTeleport(aIndex,lpTargetObj->MapNumber,lpTargetObj->X,lpTargetObj->Y);
	LogAddTD("[MarrySystem][%s][%s] Teleporting (%d,%d,%d)",lpObj->AccountID,lpObj->Name,lpTargetObj->MapNumber,lpTargetObj->X,lpTargetObj->Y);
	return;
}

void CMarrySystem::GetMarryInfo(int aIndex){
	
	if(aIndex < (OBJ_STARTUSERINDEX) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	
	DSGetMarryInfo(lpObj->m_Index);
}

void CMarrySystem::MarryInfo(int aIndex){

	if(aIndex < (OBJ_STARTUSERINDEX) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->IsMarried == FALSE){
		GCServerMsgStringSend("No estas Casado",aIndex,1);	
		return;
	}

	char Buff[255];

	if(lpObj->DbClass == 32 || lpObj->DbClass == 33 || lpObj->DbClass == 35
	|| lpObj->DbClass == 80 || lpObj->DbClass == 81 || lpObj->DbClass == 83){
		sprintf(Buff,"Estas casada con %s",lpObj->HusbandWifeName);
	} else {
		sprintf(Buff,"Estas casado con %s",lpObj->HusbandWifeName);
	}

	GCServerMsgStringSend(Buff,aIndex,1);	
	return;	
}

BOOL CMarrySystem::CheckMarryShop(int aIndex, int ShopIndex){

	if(aIndex < (OBJ_STARTUSERINDEX) || aIndex > (OBJMAX-1)) return FALSE;
	if(!gObjIsConnected(aIndex)) return FALSE;

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ ShopTarget = &gObj[ShopIndex];

	if(ShopTarget->MapNumber != MarrySystem.Shop_MapNumber) return FALSE;
	if(ShopTarget->X != MarrySystem.Shop_CordX && ShopTarget->Y != MarrySystem.Shop_CordY) return FALSE;
	if(ShopTarget->Class != 379) return FALSE;
	if(lpObj->IsMarried != TRUE) return FALSE;
	
	return TRUE;
}