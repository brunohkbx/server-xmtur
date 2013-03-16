#include "StdAfx.h"
#include "GMManager.h"
#include "LogProc.h"
#include "GameMain.h"
#include "..\include\Readscript.h"

CGMManager GMManager;

void CGMManager::LoadConfigs()
{
	SMDFile = fopen(".\\GameMasters.xtr","r");

	if(!SMDFile){
		MsgBox("[GMManager] Error Loading GameMaster File");
		return;
	}

	memset(this->GMFeatures,NULL,sizeof(this->GMFeatures));
	this->GMCount = 0;

	int Token;

	while(TRUE){

		Token = GetToken();

		if(Token == 2) break;
		
		else if(Token == 1){
			BYTE Section = (BYTE)TokenNumber;

			while(TRUE)
			{
				GetToken();
				if(!strcmp("end",TokenString)) break;

				if(Section == 0){ // GameMaster Levels

					strcpy(this->GMFeatures[this->GMCount].Name,TokenString);

					GetToken();
					this->GMFeatures[this->GMCount].ConnectMessage = (BYTE)TokenNumber;

					GetToken();
					this->GMFeatures[this->GMCount].Item = (BYTE)TokenNumber;

					GetToken();
					this->GMFeatures[this->GMCount].ChangeZen = (BYTE)TokenNumber;

					GetToken();
					this->GMFeatures[this->GMCount].ChangePKLevel = (BYTE)TokenNumber;

					GetToken();
					this->GMFeatures[this->GMCount].GMoveALL = (BYTE)TokenNumber;

					GetToken();
					this->GMFeatures[this->GMCount].SendGG = (BYTE)TokenNumber;
					
					GetToken();
					this->GMFeatures[this->GMCount].Reloads = (BYTE)TokenNumber;

					GetToken();
					this->GMFeatures[this->GMCount].BanCommands = (BYTE)TokenNumber;

					LogAddTD("[GMManager] Added: %s [%d,%d,%d,%d,%d,%d,%d,%d]",this->GMFeatures[this->GMCount].Name,
					this->GMFeatures[this->GMCount].ConnectMessage,
					this->GMFeatures[this->GMCount].Item,
					this->GMFeatures[this->GMCount].ChangeZen,
					this->GMFeatures[this->GMCount].ChangePKLevel,
					this->GMFeatures[this->GMCount].GMoveALL,
					this->GMFeatures[this->GMCount].SendGG,
					this->GMFeatures[this->GMCount].Reloads,
					this->GMFeatures[this->GMCount].BanCommands);

					this->GMCount++;
				}
			}
		}
	}

	LogAddTD("[GMManager] GameMaster List Loaded - Total: %d",this->GMCount);
}

int CGMManager::GMListNumber(char * Name){

	for(int X=0; X < this->GMCount;X++){
	
		if(_stricmp(Name,this->GMFeatures[X].Name) == 0){
			return X;
		}
	}

	return -1;
}