#include "Stdafx.h"
#include "User.h"
#include "SafeMapSystem.h"
#include "LogProc.h"
#include "GameMain.h"
#include "..\include\Readscript.h"

CSafeMapSystem SafeMap;

void CSafeMapSystem::StartProcess() {
	this->Enabled = GetPrivateProfileInt("SafeMapSystem","IsEnabled",1,".\\SafeMapSystem.xtr");
	this->MaxResetDiference = GetPrivateProfileInt("SafeMapSystem","MaxResets_Difference",15,".\\SafeMapSystem.xtr");	

	LogAddTD("[SafeMap] Starting System - Status: %d",this->Enabled);

	char Temp[5];
	for(int X=0; X < MAX_NUMBER_MAP;X++){

		memset(Temp,0,5);
		sprintf(Temp,"Map%02d",X);
		this->SafeMap[X] = GetPrivateProfileInt("SafeMapSystem",Temp,0,".\\SafeMapSystem.xtr");

		if(this->SafeMap[X] == 1){
			LogAddTD("[SafeMap] Map %d is Safe!",X);
		}
	}
}

BOOL CSafeMapSystem::SafeCheck(int aIndex, int tIndex)
{
	if(this->Enabled != 1) return TRUE;
	
	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return TRUE;
	if(!gObjIsConnected(aIndex)) return TRUE;

	LPOBJ lpObj = &gObj[aIndex];

	if(tIndex < (OBJMAX-OBJMAXUSER) || tIndex > (OBJMAX-1)) return TRUE;
	if(!gObjIsConnected(aIndex)) return TRUE;

	LPOBJ lpTargetObj = &gObj[tIndex];

	if((lpObj->Type == 1 && lpTargetObj->Type == 1) && (lpTargetObj->MapNumber == lpObj->MapNumber)){ //Check para ver si es de Usuario a Usuario 
		if(this->SafeMap[gObj->MapNumber] == 1){
			GCServerMsgStringSend("[Safe] No se permite el PvP en este Mapa",aIndex,1);
			return FALSE;
		} 
	}

	return TRUE;
}