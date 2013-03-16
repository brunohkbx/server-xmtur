#include "Stdafx.h"
#include "WzMemScript.h"
#include "LogProc.h"
#include "BloodCastleBag.h"
#include "DSProtocol.h"

CBloodCastleBag BloodCastleBag;

void CBloodCastleBag::StartProcess(){

	//Load Configs
	this->IsBloodCastle_ItemManager = GetPrivateProfileInt("BloodCastle","IsEnable",1,".\\Plugin.xtr");
	LogAddTD("[BloodCastleBag] Start Process - Status: %s",this->IsBloodCastle_ItemManager);

	if(this->IsBloodCastle_ItemManager == 1){ //Load Items
		this->LoadItemList();
	}
}

void CBloodCastleBag::LoadItemList()
{
	int Token;

	SMDFile = fopen("..\\Data\\BloodCastle_ItemBag.dat","r");

	if(!SMDFile){
		MsgBox("[BloodCastleBag] Failed Loading File");
		return;
	}

	memset(this->Items,NULL,sizeof(this->Items));
	memset(this->ItemsCount,NULL,sizeof(this->ItemsCount));

	int MapStage, ID = 0;
	BYTE Index, Level, Luck, Skill, Durability, Option, Excellent, Ancient, Count;
				
	while(true){
	    Token = GetToken();

		if(Token == 2) break;
		if(!strcmp("end",(char*)TokenString)) break; 

		if(Token == 2) break;
		
		MapStage = (int)TokenNumber;

		if(MapStage < MAX_BC_LEVEL){

			if(this->ItemsCount[MapStage] == MAX_ITEMS)  break;

			while(true){

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

				if(Count < 1){
					Count = 1;
				}

				//Set Checks!
				if(Level < 0) Level = 0;
				if(Level > 13) Level = 13;
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

				int ItemsCount = this->ItemsCount[MapStage];
				this->Items[MapStage][ItemsCount].Index = Index;
				this->Items[MapStage][ItemsCount].ID = ID;
				this->Items[MapStage][ItemsCount].Level = Level;
				this->Items[MapStage][ItemsCount].Luck = Luck;
				this->Items[MapStage][ItemsCount].Skill = Skill;
				this->Items[MapStage][ItemsCount].Durability = Durability;
				this->Items[MapStage][ItemsCount].Option = Option;
				this->Items[MapStage][ItemsCount].Excellent = Excellent;
				this->Items[MapStage][ItemsCount].Ancient = Ancient;
				this->Items[MapStage][ItemsCount].Count = Count;

				this->ItemsCount[MapStage]++;
			}
		} else {
			break;
		}
	}

	for(int X = 0; X < MAX_BC_LEVEL;X++){
		LogAddTD("[BloodCastleBag] Bridge %d - Items Count: %d",(X+1),this->ItemsCount[X]);
	}
}

void CBloodCastleBag::ItemDrop(LPOBJ gObj){
	
	if((gObj->MapNumber >= 11 && gObj->MapNumber <= 17) || gObj->MapNumber == 52){

		int Map;
		if(gObj->MapNumber == 52){ //Fix BC 8
			Map = 7; 
		} else {
			Map = gObj->MapNumber - 11;
		}

		if(this->ItemsCount[Map] < 0){
			LogAddTD("[BloodCastleBag][%s][%s] No Items Found in Map: %d",
			gObj->AccountID,gObj->Name,gObj->MapNumber);
			return;
		}

		int ItemID = rand() % this->ItemsCount[Map];

		for(int i = 0; i < this->Items[Map][ItemID].Count; i++){
			
			int CordX = gObj->X;
			int CordY = gObj->Y;
			int Excellent = GetExcellentOption();

			ItemSerialCreateSend(gObj->m_Index,gObj->MapNumber,CordX,CordY,
			((this->Items[Map][ItemID].Index*512)+this->Items[Map][ItemID].ID),
			this->Items[Map][ItemID].Level,
			this->Items[Map][ItemID].Durability,
			this->Items[Map][ItemID].Skill,
			this->Items[Map][ItemID].Luck,
			this->Items[Map][ItemID].Option,
			gObj->m_Index,
			Excellent,
			this->Items[Map][ItemID].Ancient);
		}

		LogAddTD("[BloodCastleBag][%s][%s] Prize Drop [%d][%d][%d,%d,%d][%d,%d,%d,%d,%d,%d,%d,%d,%d]",
		gObj->AccountID,gObj->Name,
		this->Items[Map][ItemID].Count,
		ItemID,gObj->MapNumber,gObj->X,gObj->Y,
		this->Items[Map][ItemID].Index,
		this->Items[Map][ItemID].ID,
		this->Items[Map][ItemID].Level,
		this->Items[Map][ItemID].Durability,
		this->Items[Map][ItemID].Skill,
		this->Items[Map][ItemID].Luck,
		this->Items[Map][ItemID].Option,
		this->Items[Map][ItemID].Excellent,
		this->Items[Map][ItemID].Ancient);
		return;

	} else {
		LogAddTD("[BloodCastleBag][%s][%s] User is not in Blood Castle [%d,%d,%d]",
		gObj->AccountID,gObj->Name,gObj->MapNumber,gObj->X,gObj->Y);
		return;
	}
}
