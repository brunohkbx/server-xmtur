#include "Stdafx.h"
#include "InfernoEvent.h"
#include "LogProc.h"
#include "GameMain.h"
#include "..\include\Readscript.h"
#include "User.h"
#include "DSProtocol.h"

CInfernoEvent InfernoEvent;

void CInfernoEvent::DropItem(int aIndex){

	if(aIndex < (OBJMAX-OBJMAXUSER) || aIndex > (OBJMAX-1)) return;
	if(!gObjIsConnected(aIndex)) return;

	LPOBJ lpObj = &gObj[aIndex];

	if(this->ItemsCount < 0){
		LogAddTD("[InfernoAttack][%s][%s] No Items Found",gObj->AccountID,gObj->Name);
		return;
	}

	int ItemID = rand() % this->ItemsCount;

	
	for(int i=0; i < this->Items[ItemID].Count;i++){
			
		int CordX = (gObj->X-2) + rand()%3;
		int CordY = (gObj->Y-2) + rand()%3;

		int Excellent = GetExcellentOption();

		ItemSerialCreateSend(gObj->m_Index,gObj->MapNumber,CordX,CordY,
		((this->Items[ItemID].Index*512)+this->Items[ItemID].ID),
		this->Items[ItemID].Level,
		this->Items[ItemID].Durability,
		this->Items[ItemID].Skill,
		this->Items[ItemID].Luck,
		this->Items[ItemID].Option,
		gObj->m_Index,
		Excellent,
		this->Items[ItemID].Ancient);
	}

	LogAddTD("[InfernoAttack][%s][%s] Prize Drop [%d][%d][%d,%d,%d][%d,%d,%d,%d,%d,%d,%d,%d,%d]",
	gObj->AccountID,gObj->Name,
	this->Items[ItemID].Count,
	ItemID,gObj->MapNumber,gObj->X,gObj->Y,
	this->Items[ItemID].Index,
	this->Items[ItemID].ID,
	this->Items[ItemID].Level,
	this->Items[ItemID].Durability,
	this->Items[ItemID].Skill,
	this->Items[ItemID].Luck,
	this->Items[ItemID].Option,
	this->Items[ItemID].Excellent,
	this->Items[ItemID].Ancient);
	return;
}

void CInfernoEvent::LoadItemList()
{
	int Token;
	SMDFile = fopen("..\\Data\\ItemBags\\InfernoAttack_ItemBag.dat","r");

	if(!SMDFile){
		MsgBox("[InfernoAttack] Failed Loading File");
		return;
	}

	memset(this->Items,NULL,sizeof(this->Items));
	this->ItemsCount = 0;

	int ID = 0;
	BYTE Index, Level, Luck, Skill, Durability, Option, Excellent, Ancient, Count;
				
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

		int ItemsCount = this->ItemsCount;
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

		this->ItemsCount++;
	}

	LogAddTD("[InfernoAttack] Items Count: %d",this->ItemsCount);
	
	fclose(SMDFile);
}