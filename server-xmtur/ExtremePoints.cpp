#include "Stdafx.h"
#include "..\common\WzMemScript.h"
#include "logproc.h"
#include "ExtremePoints.h"
#include "DSProtocol.h"

CExtremePoints ExtremePoints;

CExtremePoints::CExtremePoints(){
}

CExtremePoints::~CExtremePoints(void){
}	

void CExtremePoints::LoadStepsList(){

	int Token, Type;

	SMDFile = fopen("..\\Data\\ExtremePoints\\ExtremePoints.dat","r");

	if(!SMDFile){
		MsgBox("[ExtremePoints] Failed Loading File");
		return;
	}

	//IMPORTANT!
	memset(this->Monsters,-1,sizeof(this->Monsters));
	this->StepsCount = 0;
	memset(this->StartHour,-1,sizeof(this->StartHour));
	memset(this->EndHour,-1,sizeof(this->EndHour));

	while(true){
	    Token = GetToken();

		if(Token == 2) break;
		if(!strcmp("end",(char*)TokenString)) break; 

		Type = (int)TokenNumber;

		if(Type == 0){

			while(true){
				Token = GetToken();

				if(Token == 2) break;
				if(!strcmp("end",(char*)TokenString)) break;

				this->IsEnable = (int)TokenNumber;

				GetToken();
				this->AddPoints_Minutes = (int)TokenNumber;

				GetToken();
				this->DefaultPoints = (int)TokenNumber;

				GetToken();
				this->Type = (int)TokenNumber; //Add Type: 0: Monsters - 1: Per Cycle
			}
		}

		if(Type == 1){

			while(true){
				Token = GetToken();

				
				if(Token == 2) break;
				if(!strcmp("end",(char*)TokenString)) break;

				if(this->Type == 0){ //Step for Monsters
					if(this->StepsCount == 512) break;

					this->Monsters[this->StepsCount] = (int)TokenNumber;
				} else { //Step for Timer
					if(this->StepsCount == 10) break;

					this->StartHour[this->StepsCount] = (int)TokenNumber;

					GetToken();
					this->EndHour[this->StepsCount] = (int)TokenNumber;
				}
		
				GetToken();
				this->Points[this->StepsCount] = (int)TokenNumber;

				this->StepsCount += 1;
			}
		}
	}

	if(this->Type == 0){
		LogAddTD("[ExtremePoints] Total Monsters: %d",this->StepsCount);
	} else {
		LogAddTD("[ExtremePoints] Total Steps: %d",this->StepsCount);
	}

	if(this->IsEnable == 1){
		_beginthread(ExtremePoints_CheckCore,0,NULL);
	}

	fclose(SMDFile);
}
void CExtremePoints::GetExtremePoints(int aIndex){
	
	if(aIndex < OBJ_STARTUSERINDEX  || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;
	LPOBJ gObj = &gObj[aIndex];
	
	DSGetExtremePoints(aIndex);
}
void CExtremePoints::SaveExtremePoints(int aIndex){
	
	if(aIndex < OBJ_STARTUSERINDEX  || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;
	LPOBJ gObj = &gObj[aIndex];

	DSSaveExtremePoints(aIndex);
}

BOOL CExtremePoints::AddPointsPerMonster(int aIndex, int MonsterID){

	if(this->Type != 0) return FALSE;
	
	if(aIndex < OBJ_STARTUSERINDEX  || aIndex > (OBJMAX-1)) return FALSE;
	if(!gObjIsConnected(aIndex)) return FALSE;
	
	char Buff[255];
	int Points = 0;

	for(int X=0; X < this->StepsCount;X++){
		if(this->Monsters[X] > -1){
			if(this->Monsters[X] == MonsterID){
				Points = this->Points[X]; //Add Points
			}
		}
	}

	if(Points > 0){
		wsprintf(Buff,"%d punto(s) Extreme Obtenido(s)",Points);
		GCServerMsgStringSend(Buff,aIndex,1);

		if((gObj[aIndex].ExtremePoints += Points) < 2000000000){
			gObj[aIndex].ExtremePoints += Points;
			LogAddTD("[XtrPoints][%s][%s] %d Points Obtained",gObj[aIndex].AccountID,gObj[aIndex].Name,Points);
		} else {
			gObj[aIndex].ExtremePoints = 2000000000;
		}
		return TRUE;
	}

	return FALSE;
}

int CExtremePoints::AddPointsPerHour(){

	
	SYSTEMTIME Time;
	GetLocalTime(&Time);

	for(int X=0; X < this->StepsCount;X++){
		if(Time.wHour >= this->StartHour[X] 
		&& Time.wHour <= this->EndHour[X]){

			return this->Points[X]; //Add Points
		}
	}

	return this->DefaultPoints;
}


void ExtremePoints_CheckCore(void * lpParam){
	
	char Buff[255];
	int Points;

	while(TRUE){

		for(int Player=OBJ_STARTUSERINDEX ; Player < (OBJMAX-1);Player++){

			if(gObjIsConnected(Player) == TRUE){
				
				if(gObj[Player].ExtremeTickCount > GetTickCount() + (1000 * 60) * ExtremePoints.AddPoints_Minutes){
				
					Points = ExtremePoints.AddPointsPerHour();

					if(Points > 0) continue;
					
					wsprintf(Buff,"[XtrPoints] %d puntos Ganados",Points);
					GCServerMsgStringSend(Buff,Player,1);

					//Add Points
					gObj[Player].ExtremePoints += Points;
					LogAddTD("[XtrPoints][%s][%s] %d Points Obtained",gObj[Player].AccountID,gObj[Player].Name,Points);

					//Set New TickCount
					gObj->ExtremeTickCount = GetTickCount();
				}
			}
		}
		Sleep(100);
	
	}
}